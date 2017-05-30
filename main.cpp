#include <iostream>
#include <functional>
#include <AABB.h>
#include "../headers/billwindow.h"
#include "../headers/billGLfunctions.h"
#include "../headers/billmaterialpoint.h"
#include "../headers/billrigidbody.h"
#include "../headers/billRBengine.h"
#include "box.h"

void renderScene(void);

void mainLoop(void);

bill::BillRBIntegrator HollyWood = [](
        std::tuple<bill::vector, bill::vector, bill::quaternion, bill::vector> PhasePoint0,
        std::tuple<bill::vector, bill::vector, bill::quaternion, bill::vector> PhasePointM, bill::vector Force,
        bill::vector torque, double step) {

    bill::vector x = std::get<0>(PhasePoint0);
    bill::vector v = std::get<1>(PhasePoint0);
    bill::quaternion q = std::get<2>(PhasePoint0);
    bill::vector w = std::get<3>(PhasePoint0);
    bill::quaternion qw(0., w);

    q = q + step * 0.5 * (qw * q);
    x += step * v;

    return std::tuple<bill::vector, bill::vector, bill::quaternion, bill::vector>(x, v, q, w);

};

class Boxes : public bill::BillSetOfRigidBodies {
protected:
    std::vector<AABB> bodiesAABB;

public:
    Boxes() : bill::BillSetOfRigidBodies() {

    }

    void setAABBToRigids() {
        for (std::shared_ptr<bill::BillRigidBody> rigid : bodies) {
            bodiesAABB.push_back(AABB((box *) rigid.get()));
        }
    }

    virtual void Act() {

        for (unsigned int i = 0; i < bodiesAABB.size(); ++i) {
            bodiesAABB[i].calculateVectorsMinMax();
            for (unsigned int j = 0; j < i; ++j) {
                if (AABB::isCollision(bodiesAABB[i], bodiesAABB[j])) {
                    bodies[i]->disable();
                    bodies[j]->disable();
                }
            }
            bodiesAABB[i].draw();
        }

    };

} SetOfBodies;

bill::BillRBEngine engine;

int main(int argc, char **argv) {

    bill::GLaux::eye = bill::vector({-1, 0, 0});
    bill::GLaux::center = bill::vector({0, 0, 0});

    SetOfBodies.AddBody(new box(HollyWood, 0.1, 0.2, 0.3, bill::vector({0., 0.0, 0.3})));
    SetOfBodies.AddBody(new box(HollyWood, 0.1, 0.1, 0.1, bill::vector({0., 0.0, -0.3}), bill::vector({0.0, 0.0, 0.02}),
                                bill::quaternion({0., 0., 1., 0.}), bill::vector({0.2, 0., 0.})));
    SetOfBodies.setAABBToRigids();
    engine = bill::BillRBEngine(SetOfBodies);

    bill::Window window(argc, argv);
    window.set_processNormalKeys(bill::GLaux::processNormalKeys);
    window.set_processNormalKeysUp(bill::GLaux::processNormalKeysUp);
    window.set_renderScene(renderScene);
    window.set_mainLoop(mainLoop);

    window.initiate();
    return 0;
}

void mainLoop(void) {
    engine.step();
    renderScene();
}


void renderScene(void) {
    if (bill::GLaux::moveParallel | bill::GLaux::movePerpendicular | bill::GLaux::rotateParallel |
        bill::GLaux::rotatePerpendicular)
        bill::GLaux::computePos();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 1000000000.0);
    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();
    gluLookAt(bill::GLaux::eye[0], bill::GLaux::eye[1], bill::GLaux::eye[2],
              bill::GLaux::center[0], bill::GLaux::center[1], bill::GLaux::center[2],
              bill::GLaux::versor_up[0], bill::GLaux::versor_up[1], bill::GLaux::versor_up[2]);


    SetOfBodies.Draw();

    glutSwapBuffers();
}

