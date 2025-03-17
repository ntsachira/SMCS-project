
package controller;

import entity.AutoDispense;
import entity.DispenseMode;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import model.HibernateUtil;
import org.hibernate.Criteria;
import org.hibernate.Session;


@WebServlet(name = "CheckDispenseMode", urlPatterns = {"/CheckDispenseMode"})
public class CheckDispenseMode extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Session session = HibernateUtil.getSessionFactory().openSession();
        Criteria criteria = session.createCriteria(AutoDispense.class);
        List result = criteria.list();
        if(result.isEmpty()){
            response.getWriter().write("off");
        }else{
            response.getWriter().write(((AutoDispense)result.get(0)).getStatus());
        }
        session.close();
    }


}
