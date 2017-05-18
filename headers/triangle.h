
#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H

#include <cmath>
#include <stdexcept>
#include <billmaterialpoint.h>

#define WIRED
//#define DRAW_NORMAL

class Triangle : public bill::Obstacle {
public:
    Triangle(const bill::vector &r1, const bill::vector &r2, const bill::vector &r3) :
            m_r1(r1), m_r2(r2), m_r3(r3),
            m_attenuation(0.5) {

        const bill::vector a = r2 - r1;
        const bill::vector b = r3 - r1;
        // wektor normalny to po prostu iloczyn wektorowy a i b :-)
        m_normal_vector = b ^ a;

        m_area = 0.5 * bill::vector::norm(m_normal_vector);
        m_normal_vector.normalize();

        if (m_area <= 1e-6) {
            throw std::runtime_error("Area equals zero, wrong triangle! ");
        }

//#ifdef DRAW_NORMAL
//        m_normal_at_mid_point = m_normal_vector * 0.05;
//        for (int i = 0; i < 3; i++) {
//            m_mid_point[i] = (m_r1[i] + m_r2[i] + m_r3[i]) / 3.0;
//        }
//        m_normal_at_mid_point = m_normal_at_mid_point + m_mid_point;
//#endif
    }

    bool handleCollision(std::shared_ptr<bill::BillMaterialPoint> &mp) override {

        bill::vector r = mp->position() - m_r1;
        const double signed_distance = m_normal_vector * r;

        if (signed_distance > 0.0) {
            return false;
        }

        const bill::vector p_n = m_normal_vector * signed_distance;
        const bill::vector point_proj = mp->position() - p_n;

        double alpha, beta, gamma;
        barycentric(point_proj, alpha, beta, gamma);

        if (is_inside_triangle(alpha, beta, gamma)) {
            bill::vector v_n;
            bill::vector v_s;
            const double dot_product = m_normal_vector * mp->velocity();
            v_n = m_normal_vector * dot_product;
            v_s = mp->v() - v_n;

            v_n = (1.0 - m_attenuation) * v_n;
            const bill::vector new_velocity = v_s - v_n;
            mp->set_velocity(new_velocity);

            auto f = mp->Force();
            const double signed_force = m_normal_vector * f;
            if ( signed_force < 0) {
                bill::vector f_n = m_normal_vector * f * m_normal_vector;
                bill::vector f_s = f - f_n;

//                u = 1 – zderzenie elastyczne
//                u = 0 – zderzenie maksymalnie nieelastyczne
                const double u = 0.3;
                f_n = f_n - u * bill::vector::norm(f_s) * v_n / bill::vector::norm(v_n);
                f = f_n + f_s;
            }

            mp->CalculateMove(f);

            return true;
        }

        return false;
    }


    void draw() const {
        glLineWidth(1.1f);
        glColor3f(0.5f, 0.5f, 0.5f);

#ifdef WIRED
        glBegin(GL_LINES);
        glVertex3d(m_r1[0], m_r1[1], m_r1[2]);
        glVertex3d(m_r2[0], m_r2[1], m_r2[2]);

        glVertex3d(m_r2[0], m_r2[1], m_r2[2]);
        glVertex3d(m_r3[0], m_r3[1], m_r3[2]);

        glVertex3d(m_r3[0], m_r3[1], m_r3[2]);
        glVertex3d(m_r1[0], m_r1[1], m_r1[2]);
        glEnd();
#else
        glBegin(GL_POLYGON);
        glVertex3d(m_r1[0], m_r1[1], m_r1[2]);
        glVertex3d(m_r2[0], m_r2[1], m_r2[2]);
        glVertex3d(m_r3[0], m_r3[1], m_r3[2]);
        glEnd();
#endif

#ifdef DRAW_NORMAL
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3d(m_mid_point[0], m_mid_point[1], m_mid_point[2]);
        glColor3f(1.0f, 1.0f, 0.8f);
        glVertex3d(m_normal_at_mid_point[0], m_normal_at_mid_point[1], m_normal_at_mid_point[2]);
        glEnd();
#endif
    }


private:
    bill::vector m_r1;
    bill::vector m_r2;
    bill::vector m_r3;
    bill::vector m_normal_vector;
    bill::vector m_mid_point;
    bill::vector m_normal_at_mid_point;
    const double m_attenuation;
    double m_area;

    double area(const bill::vector &point, const bill::vector &r1, const bill::vector &r2) {
        bill::vector v1 = r1 - point;
        bill::vector v2 = r2 - point;
        bill::vector v1_cross_v2 = v1 ^v2;

        return 0.5 * bill::vector::norm(v1_cross_v2);
    }

    void barycentric(const bill::vector &point, double &alpha, double &beta, double &gamma) {
        alpha = area(point, m_r2, m_r3) / m_area;
        beta = area(point, m_r3, m_r1) / m_area;
        gamma = area(point, m_r2, m_r1) / m_area;
    }

    bool is_inside_triangle(double alpha, double beta, double gamma) {
        return (0.0 <= alpha && alpha <= 1.0 &&
                0.0 <= beta && beta <= 1.0 &&
                0.0 <= gamma && gamma <= 1.0 &&
                alpha + beta + gamma <= 1.0);
    }
};


#endif //MESH_TRIANGLE_H
