//
// Created by Konrad Hopek on 28.05.2017.
//

#ifndef BOX_AABB_H
#define BOX_AABB_H

#include "box.h"
#include <limits>

class AABB {
protected:
    int dimension = 3;
    bill::vector vecMin;
    bill::vector vecMax;
    box *rigidBox;
    bill::vector versAABB[8];

    void calculateVersAABB() {
        versAABB[0] = {vecMax[0], vecMax[1], vecMin[2]};
        versAABB[1] = {vecMin[0], vecMax[1], vecMin[2]};
        versAABB[2] = {vecMin[0], vecMin[1], vecMin[2]};
        versAABB[3] = {vecMax[0], vecMin[1], vecMin[2]};
        versAABB[4] = {vecMax[0], vecMin[1], vecMax[2]};
        versAABB[5] = {vecMax[0], vecMax[1], vecMax[2]};
        versAABB[6] = {vecMin[0], vecMax[1], vecMax[2]};
        versAABB[7] = {vecMin[0], vecMin[1], vecMax[2]};
    }

public:
    AABB(box *rigidBox) : rigidBox(rigidBox) {
    }

    void setDefaultValues(bill::vector &point) {
        for (int i = 0; i < dimension; i++) {
            vecMin[i] = vecMax[i] = point[i];
        }
    }

    void calculateVectorsMinMax() {
        std::vector<bill::vector> points;
        rigidBox->get_vertices(points);

        for (int i = 0; i < dimension; i++) {
            vecMin[i] = std::numeric_limits<double>::max();
            vecMax[i] = std::numeric_limits<double>::min();
        }

        setDefaultValues(points[0]);

        for (int j = 1, len = points.size(); j < len; j++) {
            bill::vector &verticeTmp = points[j];
            for (int i = 0; i < dimension; i++) {
                if (verticeTmp[i] < vecMin[i]) {
                    vecMin[i] = verticeTmp[i];
                }

                if (verticeTmp[i] > vecMax[i]) {
                    vecMax[i] = verticeTmp[i];
                }
            }
        }
        calculateVersAABB();
    }

    void draw() const {
        glLineWidth(1.1f);
        glColor3f(0.5f, 0.5f, 0.5f);

        glBegin(GL_LINE_LOOP);
        glVertex3f(versAABB[0][0], versAABB[0][1], versAABB[0][2]);
        glVertex3f(versAABB[1][0], versAABB[1][1], versAABB[1][2]);
        glVertex3f(versAABB[2][0], versAABB[2][1], versAABB[2][2]);
        glVertex3f(versAABB[3][0], versAABB[3][1], versAABB[3][2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(versAABB[4][0], versAABB[4][1], versAABB[4][2]);
        glVertex3f(versAABB[5][0], versAABB[5][1], versAABB[5][2]);
        glVertex3f(versAABB[6][0], versAABB[6][1], versAABB[6][2]);
        glVertex3f(versAABB[7][0], versAABB[7][1], versAABB[7][2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(versAABB[0][0], versAABB[0][1], versAABB[0][2]);
        glVertex3f(versAABB[5][0], versAABB[5][1], versAABB[5][2]);
        glVertex3f(versAABB[6][0], versAABB[6][1], versAABB[6][2]);
        glVertex3f(versAABB[1][0], versAABB[1][1], versAABB[1][2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(versAABB[4][0], versAABB[4][1], versAABB[4][2]);
        glVertex3f(versAABB[7][0], versAABB[7][1], versAABB[7][2]);
        glVertex3f(versAABB[2][0], versAABB[2][1], versAABB[2][2]);
        glVertex3f(versAABB[3][0], versAABB[3][1], versAABB[3][2]);
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
