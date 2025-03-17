
package entity;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;

@Entity
@Table(name = "sensor_data")
public class SensorData implements Serializable {
    
    @Id
    @Column(name = "id")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    
    @Column(name = "supplied_time",nullable = false)
    private Date supplied_time;
    
    @Column(name = "sensor_reading",nullable = false)
    private int sensor_reading;
    
    @Column(name = "plot",nullable = false,length = 5)
    private String plot;
    
    @ManyToOne
    @JoinColumn(name = "dispense_mode_id")
    private DispenseMode dispenseMode;

    public SensorData() {
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Date getSupplied_time() {
        return supplied_time;
    }

    public void setSupplied_time(Date supplied_time) {
        this.supplied_time = supplied_time;
    }

    public int getSensor_reading() {
        return sensor_reading;
    }

    public void setSensor_reading(int sensor_reading) {
        this.sensor_reading = sensor_reading;
    }

    public String getPlot() {
        return plot;
    }

    public void setPlot(String plot) {
        this.plot = plot;
    }

    public DispenseMode getDispenseMode() {
        return dispenseMode;
    }

    public void setDispenseMode(DispenseMode dispenseMode) {
        this.dispenseMode = dispenseMode;
    }
    
}
