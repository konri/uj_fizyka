
#ifndef MESH_PLANE_H
#define MESH_PLANE_H

#include <cmath>
#include <stdexcept>
#include <algorithm>

#include <billmaterialpoint.h>
#include <billvector.h>

#define DRAW_NORMAL

class Plane : public bill::Obstacle {
public:
    Plane(const bill::vector &normal, const bill::vector &position) :
            m_attenuation(0.6),
            m_friction(0.3) {

        m_point = position;
        const double length = bill::vector::norm(normal);
        m_normal_vector = normal / length;
    }

    bool handleCollision(std::shared_ptr<bill::BillMaterialPoint> &mp) override {
        const bill::vector r = mp->x() - m_point;
        const double signed_distance = m_normal_vector * r;

        if (signed_distance <= 0.0) {
            bill::vector v_n;
            bill::vector v_s;
            const double dot_product_velocity = m_normal_vector * mp->v();
            v_n = dot_product_velocity * m_normal_vector; //wektor prostopadly
            v_s = mp->v() - v_n; //wektor rownolegly

            v_n = (1.0 - m_attenuation) * v_n;
            v_s = (1.0 - m_friction) * v_s;

            const bill::vector new_velocity = v_s - v_n;
            mp->set_velocity(new_velocity);

            auto f = mp->Force();
            const double signed_force = m_normal_vector * f;
            if (signed_force < 0) {
                bill::vector f_n = -m_normal_vector * f * m_normal_vector;
                bill::vector f_s = f - f_n;
//                u = 1 – zderzenie elastyczne
//                u = 0 – zderzenie maksymalnie nieelastyczne
                const double u = 0.0001;
                f_n = f_n - u * bill::vector::norm(f_s) * v_n / bill::vector::norm(v_n);
                f_s = bill::vector({0.0, 0.0, 0.0});
                f = f_n + f_s;
            }
            mp->CalculateMove(f);

            return true;
        }
        return false;
    }


    void draw() const {

    }

private:
    bill::vector m_normal_vector;
    bill::vector m_point;

    const double m_attenuation;
    const double m_friction;
};


#endif //MESH_PLANE_H
