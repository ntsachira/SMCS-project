
package controller;

import entity.AutoDispense;
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

/**
 *
 * @author Dell
 */
@WebServlet(name = "UpdateDispenseMode", urlPatterns = {"/UpdateDispenseMode"})
public class UpdateDispenseMode extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Session session = HibernateUtil.getSessionFactory().openSession();
        Criteria criteria = session.createCriteria(AutoDispense.class);
         List result = criteria.list();
        if(!result.isEmpty()){
            AutoDispense dispenseMode = (AutoDispense) result.get(0);
            dispenseMode.setStatus(request.getParameter("mode")); 
            session.update(dispenseMode);
            session.beginTransaction().commit();
        }
        session.close();
    }


}
