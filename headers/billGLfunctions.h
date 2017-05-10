#ifndef BILLGLFUNCTIONS_H
#define BILLGLFUNCTIONS_H

#include <cmath>
#include <iostream>
#include "billvector.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


namespace bill{
namespace GLaux{

extern float colorR;
extern float colorB;
extern float colorG;

extern bill::vector eye;
extern bill::vector center;
extern bill::vector versor_up;

// angle of rotation for the camera direction
extern float phi;
extern float theta;

extern int moveParallel, movePerpendicular;
extern int rotateParallel, rotatePerpendicular;

extern double dPhi;
extern bill::quaternion q1;
extern bill::quaternion q2;

extern float dws, dad;

extern bool IS_CLICKED;
extern int mouseTolerance;

void computePos();

// drawing functions
void drawBall(bill::vector position, bill::vector color=bill::vector({1.0,0.0,0.0}), float radius=0.1, float alpha=1.);
void drawVector(bill::vector vec, bill::vector position=bill::vector({0.0,0.0,0.0}), bill::vector color=bill::vector({1.0,1.0,1.0}));
void drawSnowMan();

void processNormalKeys(unsigned char key, int x, int y);
void processNormalKeysUp(unsigned char key, int x, int y);
void pressKey(int key, int xx, int yy);
void releaseKey(int key, int x, int y);
double FDdistribution(double e,double mu,double beta);
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);


} //end of namespace GLaux
} //end of namespace bill
#endif
