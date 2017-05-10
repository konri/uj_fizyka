
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
#include <icosahedron.h>


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


        const int n = 5;
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n - 1; i++) {
                bill::vector x0 = points[i + n * j]->position();
                bill::vector xm = points[i + 1 + n * j]->position();

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

        for (int j = 0; j < n - 1; j++) {
            for (int i = 0; i < n; i++) {
                bill::vector x0 = points[i + n * j]->position();
                bill::vector xm = points[i + n * (j + 1)]->position();

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

int main(int argc, char **argv) {
    bill::GLaux::eye = bill::vector({-6.12583, 2.69013, -1.54564});
    bill::GLaux::center = bill::vector({0, 0, 0});

    std::vector<std::shared_ptr<oscillator>> O;
    const double step = 0.05; // integration step

    std::vector<std::shared_ptr<bill::Obstacle>> obstacles;

    obstacles.emplace_back(new Plane({0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}));
    mesh.AddObstacle(obstacles.back());

//    obstacles.emplace_back(new Triangle({-0.1, 0.5, -0.1},
//                                        {8.0, 0.5, -0.1},
//                                        {8.0, 0.5, 8.0}));
//    mesh.AddObstacle(obstacles.back());


    bill::vector icosahedronPosition = {1.5, 1, 1.5};
    obstacles.emplace_back(new Icosahedron(icosahedronPosition, 0.5, false));
    mesh.AddObstacle(obstacles.back());

    bill::vector green({0.55294118, 0.74117647, 0.04705882});
    const double k = 120.;

    const int N = 5;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            O.push_back(std::shared_ptr<oscillator>(
                    new oscillator(bill::Verlet, k, 0.5, bill::vector({0.5 * i, 2.0, 0.5 * j}),
                                   bill::vector({0.0, 0.0, 0.0}),
                                   1.123, green, step)));
            mesh.AddPoint(O.back());
        }
    }

    for (int j = 1; j < N - 1; j++) {
        for (int i = 1; i < N - 1; i++) {
            O[i + N * j]->set_left(O[i - 1 + N * j]);
            O[i + N * j]->set_right(O[i + 1 + N * j]);

            O[i + N * j]->set_bottom(O[i + N * (j - 1)]);
            O[i + N * j]->set_top(O[i + N * (j + 1)]);
        }
    }

    for (int j = 0; j < N; j++) {
        O[0 + N * j]->set_right(O[1 + N * j]);
        O[N - 1 + N * j]->set_left(O[N - 2 + N * j]);
    }

    for (int i = 0; i < N; i++) {
        O[i + N * (N - 1)]->set_bottom(O[i + N * (N - 2)]);
        O[i + N * 0]->set_top(O[i + N * 1]);
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
