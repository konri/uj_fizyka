#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billengine.h"
#include "../../headers/billintegrators.h"
#include "oscillator.h"

void renderScene(void);
void mainLoop(void);

class Grass: public bill::BillSetOfPoints{
protected:
  float ground_level;
public:

  Grass():bill::BillSetOfPoints(){
    ground_level=0.f;
  }

  void set_level(float level){
    ground_level=level;
  }

  virtual void Draw(){
    bill::BillSetOfPoints::Draw();

    glColor4f(0.28627451, 0.15686275, 0.12156863,0.8);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, ground_level-0.2, -5.0f);
    glVertex3f(-5.0f, ground_level-0.2,  5.0f);
    glVertex3f( 5.0f, ground_level,  5.0f);
    glVertex3f( 5.0f, ground_level, -5.0f);
    glEnd();
    glPopMatrix();

    for(unsigned int i =1; i<points.size(); ++i){
      bill::vector x0 =   points[i]->position();
      bill::vector xm = points[i-1]->position();

      glPushMatrix();
      glLineWidth(2.5);
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_LINES);
      glVertex3f(x0[0],x0[1],x0[2]);
      glVertex3f(xm[0],xm[1],xm[2]);
      glEnd();
      glPopMatrix();
    }
  }
} grass;

bill::BillEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-5,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  std::vector<std::shared_ptr<oscillator>> O;
  double step = 0.1; // integration step

  int above = 10;
  int below = 3;

  //Tworzymy Oscylatory Sprzężone
  for(int i=-below; i<=above; ++i){
    bill::vector color({0.97254902, 0.92156863, 0.83529412}); // ecru
    bill::vector green({0.55294118, 0.74117647, 0.04705882});
    double alpha=0.2;
    double k=100.;
    if(i>0){//nie jest korzeniem
      alpha = static_cast<double>(i)/static_cast<double>(0.7*above);
      if(alpha>1.) alpha=1.;
      color=(1.-alpha)*color+alpha*green;
      k-=90.*alpha;
    }
    O.push_back(std::shared_ptr<oscillator>(new oscillator(bill::Verlet,k,0.15,bill::vector({0.0,0.15*i,0.}),bill::vector({0.0,0.0,0.0}),2.4-2*alpha,color,step)));
    grass.AddPoint(O.back());
  }

  //Tworzymy sprężynki do prawego
  for(int i=0; i<above+below; ++i){
    O[i]->set_right(O[i+1]);
  }
  //Tworzymy sprężynki do lewego
  for(int i=1; i<above+below+1; ++i){
    O[i]->set_left(O[i-1]);
  }

  //Tworzymy sprężynki do 2go prawego
  for(int i=0; i<above+below-1; ++i){
    O[i]->set_2nd_right(O[i+2]);
  }
  //Tworzymy sprężynki do 2go lewego
  for(int i=2; i<above+below+1; ++i){
    O[i]->set_2nd_left(O[i-2]);
  }

  //Tworzymy sprężynki do 3go prawego
  for(int i=0; i<above+below-2; ++i){
    O[i]->set_3rd_right(O[i+3]);
  }
  //Tworzymy sprężynki do 3go lewego
  for(int i=3; i<above+below+1; ++i){
    O[i]->set_3rd_left(O[i-3]);
  }

  for(int i=0; i<=below; ++i)
    O[i]->disable();

  O[below+above]->set_position(O[below+above]->x()+bill::vector({0.,0.,.01}));

  grass.set_level(O[below]->x()[1]);

  engine = bill::BillEngine(grass);

  bill::Window window(argc,argv);
  window.set_processNormalKeys(bill::GLaux::processNormalKeys);
  window.set_processNormalKeysUp(bill::GLaux::processNormalKeysUp);
  window.set_renderScene(renderScene);
  window.set_mainLoop(mainLoop);

  window.initiate();
  return 0;
}

void mainLoop(void){
  engine.step();
  renderScene();
}


void renderScene(void) {
  if(bill::GLaux::moveParallel|bill::GLaux::movePerpendicular|bill::GLaux::rotateParallel|bill::GLaux::rotatePerpendicular)
    bill::GLaux::computePos();
 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,800.0/600.0,0.1,1000000000.0);
  glMatrixMode(GL_MODELVIEW);
  
  
  glLoadIdentity();
  gluLookAt(bill::GLaux::eye[0],	bill::GLaux::eye[1],		bill::GLaux::eye[2],
	    bill::GLaux::center[0],	bill::GLaux::center[1],		bill::GLaux::center[2],
	    bill::GLaux::versor_up[0],	bill::GLaux::versor_up[1],	bill::GLaux::versor_up[2]);


  grass.Draw();
  
  glutSwapBuffers();
}

