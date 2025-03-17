package controller;

import entity.DispenseMode;
import entity.SensorData;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import model.HibernateUtil;
import org.hibernate.Criteria;
import org.hibernate.Session;
import org.hibernate.criterion.Restrictions;

@WebServlet(name = "UpdateDIspenseHistory", urlPatterns = {"/UpdateDIspenseHistory"})
public class UpdateDIspenseHistory extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if (request.getParameter("plot") != null && request.getParameter("mode") != null && request.getParameter("reading") != null) {
            Session session = HibernateUtil.getSessionFactory().openSession();
            Criteria criteria = session.createCriteria(DispenseMode.class);
            criteria.add(Restrictions.eq("name", request.getParameter("mode")));
            List result = criteria.list();
            if (!result.isEmpty()) {
                DispenseMode dispenseMode = (DispenseMode) result.get(0);
                SensorData sensorData = new SensorData();
                sensorData.setDispenseMode(dispenseMode);
                sensorData.setPlot(request.getParameter("plot"));
                sensorData.setSensor_reading(Integer.parseInt(request.getParameter("reading")));
                sensorData.setSupplied_time(new Date());

                session.save(sensorData);
                session.beginTransaction().commit();
            }
            session.close();
        }
    }

}
