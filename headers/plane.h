
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

        point = position;
        const double length = bill::vector::norm(normal);
        normal_unit_vec = normal / length;
    }

    bool handleCollision(std::shared_ptr<bill::BillMaterialPoint> &mp) override {
        const bill::vector p = mp->x() - point;
        const double signed_distance = normal_unit_vec * p;

        if (signed_distance <= 0.0) {

            bill::vector new_pos;
            new_pos = mp->x() - 1.01 * signed_distance * normal_unit_vec;
            mp->set_future_position(new_pos);

            bill::vector v_n;
            bill::vector v_s;
            const double dot_product = normal_unit_vec * mp->v();
            v_n = dot_product * normal_unit_vec;
            v_s = mp->v() - v_n;

            v_n = (1.0 - m_attenuation) * v_n;
            v_s = (1.0 - m_friction) * v_s;
            bill::vector new_velocity = v_s - v_n;
            mp->set_velocity(new_velocity);

            auto f = mp->Force();
            mp->CalculateMove(f);

            return true;
        }

        return false;
    }


    void draw() const {

    }

private:
    bill::vector normal_unit_vec;
    bill::vector point;

    const double m_attenuation;
    const double m_friction;
};


#endif //MESH_PLANE_H
