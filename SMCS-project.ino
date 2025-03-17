#include <Arduino_JSON.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

int plotA_sensor_reading, plotB_sensor_reading;
const int plotA_sensor_analog_in = 32;
const int plotB_sensor_analog_in = 33;
const float dryLimit = 600;
const float wetLimit = 300;
const int maxWaterDisposalTime = 9000;

const int green_ledPin = 25;

bool isAutoWaterDisposalOn = true;
bool is_plotA_water_On = false;
bool is_plotB_water_On = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

//for serial communication with uno
const int RESET_SERVOS = 1;
const int OPEN_VALVE_A = 2;
const int OPEN_VALVE_B = 3;
const int CLOSE_VALVE_A = 4;
const int CLOSE_VALVE_B = 5;

const String VALVE_A_ON = "A1";
const String VALVE_A_OFF = "A0";
const String VALVE_B_ON = "B1";
const String VALVE_B_OFF = "B0";

const String SSID = "SLT-4G_NTS";
const String PASS = "boadingwifi";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/feed");

// Json Variable to Hold Sensor Readings
JSONVar readings;
JSONVar plotA;
JSONVar plotB;

void notifyClients() {
  plotA["reading"] = plotA_sensor_reading;
  plotB["reading"] = plotB_sensor_reading;
  plotA["valve"] = is_plotA_water_On;
  plotB["valve"] = is_plotB_water_On;
  readings["A"] = plotA;
  readings["B"] = plotB;
  ws.textAll(JSON.stringify(readings));
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char *)data;
    // Check if the message is "getReadings"
    if (message == "getReadings") {
      notifyClients();
    } else if (strcmp((char *)data, "auto_dispense_on") == 0) {
      isAutoWaterDisposalOn = true;
    } else if (strcmp((char *)data, "auto_dispense_off") == 0) {
      isAutoWaterDisposalOn = false;
    } else if (strcmp((char *)data, "valveA") == 0) {
      if (!isAutoWaterDisposalOn) {
        Serial2.print(OPEN_VALVE_A);
        saveDispenseData("A","manual",plotA_sensor_reading);
      }
    } else if (strcmp((char *)data, "valveB") == 0) {
      if (!isAutoWaterDisposalOn) {
        Serial2.print(OPEN_VALVE_B);
        saveDispenseData("B","manual",plotB_sensor_reading);
      }
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void startServer() {
  server.begin();
  lcd.setCursor(0, 1);
  lcd.print("Server Online...");
  Serial.print("Server running on: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  //for the communication with uno

  pinMode(green_ledPin, OUTPUT);

  initializeLCD();

  connectWifi();
  initWebSocket();
  startServer();

  configureLCD();

  digitalWrite(green_ledPin, HIGH);  //indicate the system is on
  resetServos();
  checkAutoDispenseOn();
}

void loop() {
  plotA_sensor_reading = readMoistureSensor(plotA_sensor_analog_in);
  plotB_sensor_reading = readMoistureSensor(plotB_sensor_analog_in);
  notifyClients();
  printMostureDataOnLCD();

  if (isAutoWaterDisposalOn) {
    supplyWaterIfNeeded();
  }

  delay(2000);
  ws.cleanupClients();
}


void checkAutoDispenseOn() {
  HTTPClient request = HTTPClient();
  request.begin("http://192.168.1.189:8080/SMCS_Server/CheckDispenseMode");
  int status = request.GET();
  if (status > 0) {
    if (status == HTTP_CODE_OK) {
      //get the auto water disposal on from db
      String response = request.getString();
      if (response == "on") {
        isAutoWaterDisposalOn = true;
      } else if (response == "off") {
        isAutoWaterDisposalOn = false;
      }
    }
  } else {
    //error
  }
  request.end();
  delay(1000);
}

void connectWifi() {
  WiFi.begin(SSID, PASS);
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Searching WiFi");
  lcd.setCursor(0, 1);
  lcd.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected.");
  delay(500);
}

void configureLCD() {
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Showing sensor");
  lcd.setCursor(0, 1);
  lcd.print("data.");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Plot A:");
  lcd.setCursor(0, 1);
  lcd.print("Plot B:");
}

void resetServos() {
  Serial2.print(RESET_SERVOS);
  delay(1000);
}

void supplyWaterIfNeeded() {
  if (plotA_sensor_reading > dryLimit) {
    Serial2.print(OPEN_VALVE_A);
    is_plotA_water_On = true;
    notifyClients();
    saveDispenseData("A","auto",plotA_sensor_reading);
    delay(maxWaterDisposalTime);
    is_plotA_water_On = false;
    notifyClients();
  }
  if (plotB_sensor_reading > dryLimit) {
    Serial2.print(OPEN_VALVE_B);
    is_plotB_water_On = true;
    notifyClients();
    saveDispenseData("B","auto",plotB_sensor_reading);
    delay(maxWaterDisposalTime);
    is_plotB_water_On = false;
    notifyClients();
  }
}

void saveDispenseData(String plot,String mode,int value) {
  HTTPClient request = HTTPClient();
  request.begin(String("http://192.168.1.189:8080/SMCS_Server/UpdateDIspenseHistory?plot=")+plot+"&mode="+mode+"&reading="+value);
  request.GET();  
  request.end();
  delay(1000);
}

void printMostureDataOnLCD() {
  lcd.setCursor(8, 0);
  lcd.print(plotA_sensor_reading);

  lcd.setCursor(11, 0);
  if (plotA_sensor_reading < wetLimit) {
    lcd.print(" DAMP");
  } else if (plotA_sensor_reading < dryLimit) {
    lcd.print(" NORM");
  } else {
    lcd.print("  DRY");
  }

  lcd.setCursor(8, 1);
  lcd.print(plotB_sensor_reading);

  lcd.setCursor(11, 1);
  if (plotB_sensor_reading < wetLimit) {
    lcd.print(" DAMP");
  } else if (plotB_sensor_reading < dryLimit) {
    lcd.print(" NORM");
  } else {
    lcd.print("  DRY");
  }
}

void initializeLCD() {
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO SMCS!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("System stating");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(1000);
}

int readMoistureSensor(int analogPin) {
  return -map(analogRead(analogPin), 0, 1023, 255, 0);  // map the 10-bit data to 8-bit data
}
