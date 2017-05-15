
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <billwindow.h>
#include <billGLfunctions.h>
#include <billmaterialpoint.h>
#include <billengine.h>
#include <billintegrators.h>
#include <oscillator.h>
#include <plane.h>
#include <triangle.h>
#include <Icosahedron.h>



static const int N = 5;

void renderScene(void);
void mainLoop(void);


class Mesh : public bill::BillSetOfPoints {
protected:
    float ground_level;
public:

    Mesh() : bill::BillSetOfPoints() {
        ground_level = 0.0f;
    }

    virtual void Draw() {
        bill::BillSetOfPoints::Draw();

        glColor4f(0.28627451, 0.15686275, 0.12156863, 0.8);
        glBegin(GL_QUADS);
        const GLfloat size_ground = 10;
        glVertex3f(-size_ground, ground_level, -size_ground);
        glVertex3f(-size_ground, ground_level, size_ground);
        glVertex3f(size_ground, ground_level, size_ground);
        glVertex3f(size_ground, ground_level, -size_ground);
        glEnd();
        glPopMatrix();


        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N - 1; i++) {
                bill::vector x0 = points[i + N * j]->position();
                bill::vector xm = points[i + 1 + N * j]->position();

                glPushMatrix();
                glLineWidth(2.5);
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINES);
                glVertex3f(x0[0], x0[1], x0[2]);
                glVertex3f(xm[0], xm[1], xm[2]);
                glEnd();
                glPopMatrix();
            }
        }

        for (int j = 0; j < N - 1; j++) {
            for (int i = 0; i < N; i++) {
                bill::vector x0 = points[i + N * j]->position();
                bill::vector xm = points[i + N * (j + 1)]->position();

                glPushMatrix();
                glLineWidth(2.5);
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINES);
                glVertex3f(x0[0], x0[1], x0[2]);
                glVertex3f(xm[0], xm[1], xm[2]);
                glEnd();
                glPopMatrix();
            }
        }

    }
} mesh;

bill::BillEngine engine;

std::vector<std::shared_ptr<oscillator>> O;

std::shared_ptr<oscillator> getO(size_t i, size_t j) {
    if (i < 0 || j < 0 || i >= 5 || j >= 5) {
        return nullptr;
    } else {
        return O[i + N * j];
    }
}

int main(int argc, char **argv) {
    bill::GLaux::eye = bill::vector({-6.12583, 2.69013, -1.54564});
    bill::GLaux::center = bill::vector({0, 0, 0});


    const double step = 0.04; // integration step

    std::vector<std::shared_ptr<bill::Obstacle>> obstacles;

    obstacles.emplace_back(new Plane({0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}));
    mesh.AddObstacle(obstacles.back());

    obstacles.emplace_back(new Triangle({-0.1, 0.5, -0.1},
                                        {8.0, 0.5, -0.1},
                                        {8.0, 0.5, 8.0}));
    mesh.AddObstacle(obstacles.back());

//    bill::vector icosahedronPosition = {0.7, 0.5, 0.75};
//    obstacles.emplace_back(new Icosahedron(icosahedronPosition, 0.5, true));
//    mesh.AddObstacle(obstacles.back());

    bill::vector green({0.55294118, 0.74117647, 0.04705882});
    const double k = 5.5;

    const double y = 1.5;
    const double l = 0.45;

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            O.push_back(std::shared_ptr<oscillator>(
                    new oscillator(bill::Euler, k, l,
                                   bill::vector({l * i, y, l * j}),
                                   bill::vector({0.0, 0.0, 0.0}),
                                   0.25, green, step)));
            mesh.AddPoint(O.back());
        }
    }

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            std::shared_ptr<oscillator> &o = O[i + N * j];

            o->set_left(getO(i - 1, j));
            o->set_right(getO(i + 1, j));

            o->set_bottom(getO(i, j - 1));
            o->set_top(getO(i, j + 1));

            o->set_top_left(getO(i - 1, j + 1));
            o->set_top_right(getO(i + 1,j + 1));

            o->set_bottom_left(getO(i - 1, j - 1));
            o->set_bottom_right(getO(i + 1, j - 1));
        }
    }


    engine = bill::BillEngine(mesh);

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

//    std::cout<<bill::GLaux::eye<<std::endl;
    gluLookAt(bill::GLaux::eye[0], bill::GLaux::eye[1], bill::GLaux::eye[2],
              bill::GLaux::center[0], bill::GLaux::center[1], bill::GLaux::center[2],
              bill::GLaux::versor_up[0], bill::GLaux::versor_up[1], bill::GLaux::versor_up[2]);

    mesh.Draw();

    glutSwapBuffers();
}
