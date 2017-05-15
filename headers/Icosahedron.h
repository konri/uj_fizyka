
#ifndef MESH_ICOSAHEDRON_H
#define MESH_ICOSAHEDRON_H

#include <cmath>
#include <memory>
#include <stdexcept>
#include "triangle.h"


class Icosahedron : public bill::Obstacle {
public:
    Icosahedron(bill::vector &position, double scale, bool is_irregular = false) :
            m_point(position), m_scale(scale), m_is_irregular(is_irregular) { //scale???
        m_faces.reserve(20);

        createFaces();
    }

    bool handleCollision(std::shared_ptr<bill::BillMaterialPoint> &mp) override {
        if (is_outside_sphere(mp)) {
           // return false;
        }

        for (auto &face : m_faces) {
            if (face->handleCollision(mp)) {
                return true;
            }
        }
        return false;
    }

    void draw() const {
        for (auto &face : m_faces) {
            face->draw();
        }
    }

private:
    const bill::vector m_point;
    const double m_scale;
    const bool m_is_irregular;
    double m_radius;
    std::vector<std::unique_ptr<Triangle> > m_faces;

    void createFaces() {
        const size_t N = 12;
        const double t = (1.0 + ::sqrt(5.0)) / 2.0;

        bill::vector v[N] = {
                {t,  1,  0},
                {-t, 1,  0},
                {t,  -1, 0},
                {-t, -1, 0},
                {1,  0,  t},
                {1,  0,  -t},
                {-1, 0,  t},
                {-1, 0,  -t},
                {0,  t,  1},
                {0,  -t, 1},
                {0,  t,  -1},
                {0,  -t, -1}
        };

        m_radius = 0.0;
        double length;
        for (size_t j = 0; j < N; j++) {
            v[j] = v[j] * m_scale / t;

            if (m_is_irregular) {
                for (size_t i = 0; i < 3; i++) {
                    v[j][i] = v[j][i] + 0.05 * ((i + j) % 6);
                }
            }

            length = bill::vector::norm(v[j]);
            if (length > m_radius) {
                m_radius = length;
            }
        }

        for (size_t j = 0; j < N; j++) {
            v[j] = v[j] + m_point;
        }

        m_faces.emplace_back(new Triangle(v[0], v[8], v[4])); //  0
        m_faces.emplace_back(new Triangle(v[0], v[5], v[10])); //  1
        m_faces.emplace_back(new Triangle(v[2], v[4], v[9])); //  2
        m_faces.emplace_back(new Triangle(v[2], v[11], v[5])); //  3
        m_faces.emplace_back(new Triangle(v[1], v[6], v[8])); //  4
        m_faces.emplace_back(new Triangle(v[1], v[10], v[7])); //  5
        m_faces.emplace_back(new Triangle(v[3], v[9], v[6])); //  6
        m_faces.emplace_back(new Triangle(v[3], v[7], v[11])); //  7
        m_faces.emplace_back(new Triangle(v[0], v[10], v[8])); //  8
        m_faces.emplace_back(new Triangle(v[1], v[8], v[10])); //  9
        m_faces.emplace_back(new Triangle(v[2], v[9], v[11])); // 10
        m_faces.emplace_back(new Triangle(v[3], v[9], v[11])); // 11
        m_faces.emplace_back(new Triangle(v[4], v[2], v[0])); // 12
        m_faces.emplace_back(new Triangle(v[5], v[0], v[2])); // 13
        m_faces.emplace_back(new Triangle(v[6], v[1], v[3])); // 14
        m_faces.emplace_back(new Triangle(v[7], v[3], v[1])); // 15
        m_faces.emplace_back(new Triangle(v[8], v[6], v[4])); // 16
        m_faces.emplace_back(new Triangle(v[9], v[4], v[6])); // 17
        m_faces.emplace_back(new Triangle(v[10], v[5], v[7])); // 18
        m_faces.emplace_back(new Triangle(v[11], v[7], v[5])); // 19
    }

    bool is_outside_sphere(std::shared_ptr<bill::BillMaterialPoint> &mp) {
        const bill::vector sub = mp->position() - m_point;
        const double d = bill::vector::norm(sub);
        return (d - m_radius) > 0.0;
    }
};

#endif //MESH_ICOSAHEDRON_H
