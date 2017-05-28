//
// Created by Konrad Hopek on 28.05.2017.
//

#ifndef BOX_AABB_H
#define BOX_AABB_H

#include "box.h"

class AABB {
protected:
    int dimension = 3;
    bill::vector vecMin;
    bill::vector vecMax;
    box *rigidBox;
    std::vector<bill::vector> versAABB;
    std::vector<bill::vector> colors;

    void calculateVersAABB() {
        versAABB.clear();

        versAABB.push_back({vecMin[0], vecMax[1], vecMin[2]});
        versAABB.push_back({vecMin[0], vecMax[1], vecMax[2]});
        versAABB.push_back({vecMax[0], vecMax[1], vecMin[2]});
        versAABB.push_back({vecMax[0], vecMax[1], vecMax[2]});

        versAABB.push_back({vecMin[0], vecMin[1], vecMin[2]});
        versAABB.push_back({vecMin[0], vecMin[1], vecMax[2]});
        versAABB.push_back({vecMax[0], vecMin[1], vecMin[2]});
        versAABB.push_back({vecMax[0], vecMin[1], vecMax[2]});

    }

public:
    AABB(box *rigidBox) : rigidBox(rigidBox) {
        colors.push_back(bill::vector({0.95686275, 0.34117647, 0.03921569}));
        colors.push_back(bill::vector({0.95686275, 0.34117647, 0.03921569}));
        colors.push_back(bill::vector({0.56470588, 0.6627451, 0.78039216}));
        colors.push_back(bill::vector({0.56470588, 0.6627451, 0.78039216}));
        colors.push_back(bill::vector({0.95294118, 0.75294118, 0.78431373}));
        colors.push_back(bill::vector({0.95294118, 0.75294118, 0.78431373}));

    }

    void calculateVectorsMinMax() {
        std::vector<bill::vector> points;
        rigidBox->get_vertices(points);

        for (bill::vector ver : points) {
            for (int i = 0; i < dimension; i++) {
                if (ver[i] < vecMin[i]) {
                    vecMin[i] = ver[i];
                }

                if (ver[i] > vecMax[i]) {
                    vecMax[i] = ver[i];
                }
            }
        }

        std::cout << "vecMax: " << vecMax << std::endl;
        std::cout << "vecMin :" << vecMin << std::endl;

        calculateVersAABB();
    }

    void draw() {
//        bill::GLaux::drawBox(versAABB, colors, 1.0);

        glLineWidth(1.1f);
        glColor3f(0.5f, 0.5f, 0.5f);

        glBegin(GL_LINES);

        glVertex3d(versAABB[0][0], versAABB[0][1], versAABB[0][2]);
        glVertex3d(versAABB[3][0], versAABB[3][1], versAABB[3][2]);

        glVertex3d(versAABB[3][0], versAABB[3][1], versAABB[3][2]);
        glVertex3d(versAABB[7][0], versAABB[7][1], versAABB[7][2]);

        glVertex3d(versAABB[7][0], versAABB[7][1], versAABB[7][2]);
        glVertex3d(versAABB[5][0], versAABB[5][1], versAABB[5][2]);

        glVertex3d(versAABB[5][0], versAABB[5][1], versAABB[5][2]);
        glVertex3d(versAABB[0][0], versAABB[0][1], versAABB[0][2]);

        glEnd();

    }

    bool static isCollision(const AABB &box1, const AABB &box2) {
        for (int i = 0; i < 3; i++) {
            if (box1.vecMin[i] > box2.vecMax[i])
                return false;
            if (box1.vecMax[i] < box2.vecMin[i])
                return false;
        }

        return true;
    }
};

#endif //BOX_AABB_H
