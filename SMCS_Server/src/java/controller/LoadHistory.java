
package controller;

import com.google.gson.Gson;
import entity.SensorData;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import model.HibernateUtil;
import org.hibernate.Criteria;
import org.hibernate.criterion.Order;


@WebServlet(name = "LoadHistory", urlPatterns = {"/LoadHistory"})
public class LoadHistory extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {        
        org.hibernate.Session session = HibernateUtil.getSessionFactory().openSession();        
        Criteria criteria = session.createCriteria(SensorData.class);
        criteria.setMaxResults(30);
        criteria.addOrder(Order.desc("supplied_time"));
        response.getWriter().write(new Gson().toJson(criteria.list()));
        session.close();
    }


}
