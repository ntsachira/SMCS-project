package controller;

import com.google.gson.Gson;
import entity.SensorData;
import java.io.IOException;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
import model.HibernateUtil;
import org.hibernate.Criteria;
import org.hibernate.criterion.Order;

@ServerEndpoint("/HistorySocket")
public class HistorySocket {

    @OnOpen
    public void open(Session session) throws IOException {
        session.getBasicRemote().sendText(loadHistory());
    }

    @OnClose
    public void close(Session session) {
        System.out.println("socket close");
    }

    @OnError
    public void onError(Throwable error) {
        System.out.println("socket error");
    }

    @OnMessage
    public void handleMessage(String message, Session session) throws IOException {
        session.getBasicRemote().sendText(loadHistory());
    }

    private String loadHistory() {
        org.hibernate.Session session = HibernateUtil.getSessionFactory().openSession();
        Criteria criteria = session.createCriteria(SensorData.class);
        criteria.setMaxResults(30);
        criteria.addOrder(Order.desc("supplied_time"));
        return new Gson().toJson(criteria.list());
    }
}
