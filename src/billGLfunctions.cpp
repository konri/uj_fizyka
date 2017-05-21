#include "../headers/billGLfunctions.h"

float bill::GLaux::colorR=0.0;
float bill::GLaux::colorB=0.0;
float bill::GLaux::colorG=0.0;

bill::vector bill::GLaux::eye=bill::vector({1.0,1.0,0.0});
bill::vector bill::GLaux::center=bill::vector({0.0,1.0,0.0});
bill::vector bill::GLaux::versor_up=bill::vector({0.0,1.0,0.0});

float bill::GLaux::phi = 0.0f;
float bill::GLaux::theta = 0.0f;

int bill::GLaux::moveParallel=0;
int bill::GLaux::movePerpendicular=0;
int bill::GLaux::rotateParallel=0;
int bill::GLaux::rotatePerpendicular=0;

double bill::GLaux::dPhi = 0.01;
bill::quaternion bill::GLaux::q1=bill::quaternion(versor_up,dPhi);
bill::quaternion bill::GLaux::q2=bill::quaternion(((center - eye).versor())^versor_up,dPhi);

float bill::GLaux::dws=0.01f;
float bill::GLaux::dad=0.01f;

bool bill::GLaux::IS_CLICKED = false;
int bill::GLaux::mouseTolerance=0;

void bill::GLaux::computePos() {
  
  //std::cout<<"center-eye: "<<center-eye<<" "<<bill::vector::norm(center-eye)<<" up: "<<versor_up<<" "<<bill::vector::norm(versor_up)<<std::endl;
  
  if(moveParallel|movePerpendicular){
  bill::vector deltaEyeCen = (center - eye).versor();
  if(moveParallel){
    eye += deltaEyeCen*(moveParallel*dws);
    center += deltaEyeCen*(moveParallel*dws);
  }
  if(movePerpendicular){
    bill::vector perp=deltaEyeCen^versor_up;
    eye+= perp*(movePerpendicular*dad);
    center+= perp*(movePerpendicular*dad);
  }
  }
  
 if(rotatePerpendicular|rotateParallel){ 
  if(rotatePerpendicular)
    center = eye + (rotatePerpendicular>0 ? (*q2).rotate(center-eye) : (q2).rotate(center-eye));
   
  if(rotateParallel)
    center = eye + (rotateParallel>0 ? (*q1).rotate(center-eye) : (q1).rotate(center-eye));
  
  q2.updateU(((center - eye).versor())^versor_up);
}
  
}


void bill::GLaux::drawBall(bill::vector position, bill::vector color, float radius, float alpha){
  glPushMatrix();

  if(fabs(alpha-1.)<1e-15)
    glColor3f(color[0],color[1],color[2]);
  else 
    glColor4f(color[0],color[1],color[2],alpha);

  glTranslatef(position[0],position[1],position[2]);

  if(radius<0.05)
    glutSolidSphere(radius,5,5);
  else if(radius>0.5)
    glutSolidSphere(radius,50,50);
  else{
    unsigned int res = static_cast<unsigned int>(100*radius);
    glutSolidSphere(radius,res,res);
  }

  glPopMatrix();
}

void bill::GLaux::drawBox(std::vector<bill::vector> vertecies, std::vector<bill::vector> colors, float alpha){
  std::vector<bill::vector> normals;

  normals.push_back((vertecies[0]-vertecies[4]).versor());
  normals.push_back((vertecies[4]-vertecies[0]).versor());
  normals.push_back((vertecies[0]-vertecies[1]).versor());
  normals.push_back((vertecies[1]-vertecies[0]).versor());
  normals.push_back((vertecies[0]-vertecies[3]).versor());
  normals.push_back((vertecies[3]-vertecies[0]).versor());

  glPushMatrix();

  glBegin(GL_QUADS);
	// top
        glColor4f(colors[0][0],colors[0][1],colors[0][2],alpha);
	glNormal3f(normals[0][0],normals[0][1],normals[0][2]);
	glVertex3f(vertecies[0][0],vertecies[0][1],vertecies[0][2]);
	glVertex3f(vertecies[1][0],vertecies[1][1],vertecies[1][2]);
	glVertex3f(vertecies[2][0],vertecies[2][1],vertecies[2][2]);
	glVertex3f(vertecies[3][0],vertecies[3][1],vertecies[3][2]);
	// bottom
        glColor4f(colors[1][0],colors[1][1],colors[1][2],alpha);
	glNormal3f(normals[1][0],normals[1][1],normals[1][2]);
	glVertex3f(vertecies[4][0],vertecies[4][1],vertecies[4][2]);
	glVertex3f(vertecies[5][0],vertecies[5][1],vertecies[5][2]);
	glVertex3f(vertecies[6][0],vertecies[6][1],vertecies[6][2]);
	glVertex3f(vertecies[7][0],vertecies[7][1],vertecies[7][2]);
	// left
        glColor4f(colors[2][0],colors[2][1],colors[2][2],alpha);
	glNormal3f(normals[2][0],normals[2][1],normals[2][2]);
	glVertex3f(vertecies[0][0],vertecies[0][1],vertecies[0][2]);
	glVertex3f(vertecies[3][0],vertecies[3][1],vertecies[3][2]);
	glVertex3f(vertecies[7][0],vertecies[7][1],vertecies[7][2]);
	glVertex3f(vertecies[4][0],vertecies[4][1],vertecies[4][2]);
	// right
        glColor4f(colors[3][0],colors[3][1],colors[3][2],alpha);
	glNormal3f(normals[3][0],normals[3][1],normals[3][2]);
	glVertex3f(vertecies[1][0],vertecies[1][1],vertecies[1][2]);
	glVertex3f(vertecies[2][0],vertecies[2][1],vertecies[2][2]);
	glVertex3f(vertecies[6][0],vertecies[6][1],vertecies[6][2]);
	glVertex3f(vertecies[5][0],vertecies[5][1],vertecies[5][2]);
	// front
        glColor4f(colors[4][0],colors[4][1],colors[4][2],alpha);
	glNormal3f(normals[4][0],normals[4][1],normals[4][2]);
	glVertex3f(vertecies[0][0],vertecies[0][1],vertecies[0][2]);
	glVertex3f(vertecies[1][0],vertecies[1][1],vertecies[1][2]);
	glVertex3f(vertecies[5][0],vertecies[5][1],vertecies[5][2]);
	glVertex3f(vertecies[4][0],vertecies[4][1],vertecies[4][2]);
	// back
        glColor4f(colors[5][0],colors[5][1],colors[5][2],alpha);
	glNormal3f(normals[5][0],normals[5][1],normals[5][2]);
	glVertex3f(vertecies[3][0],vertecies[3][1],vertecies[3][2]);
	glVertex3f(vertecies[2][0],vertecies[2][1],vertecies[2][2]);
	glVertex3f(vertecies[6][0],vertecies[6][1],vertecies[6][2]);
	glVertex3f(vertecies[7][0],vertecies[7][1],vertecies[7][2]);
  glEnd();

  glPopMatrix();
}


void bill::GLaux::drawVector(bill::vector vec, bill::vector pos, bill::vector color){
  double length = bill::vector::norm(vec);

  bill::vector end = pos + vec; // end of the vector

  bill::vector vp1; // vectors perpendicular to vec
  bill::vector vp2;

  if(fabs(vec[0]) > 1e-5){
    vp1[0]=-vec[1];
    vp1[1]= vec[0];
    vp1[2]= 0.;
  }
  else{
    vp1[0]= 0.;
    vp1[1]=-vec[2];
    vp1[2]= vec[1];
  }

  vp2 = vp1^vec;

  vp1.normalize();
  vp2.normalize();

  bill::vector s1p = end + 0.05*length*vp1 - 0.05*vec;
  bill::vector s1m = end - 0.05*length*vp1 - 0.05*vec;
  bill::vector s2p = end + 0.05*length*vp2 - 0.05*vec;
  bill::vector s2m = end - 0.05*length*vp2 - 0.05*vec;


  // drawing
  glPushMatrix();
  glLineWidth(2.5);
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_LINES);
  glVertex3f(pos[0],pos[1],pos[2]);
  glVertex3f(end[0],end[1],end[2]); // main line
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_TRIANGLES);         // head
  glVertex3f(end[0],end[1],end[2]);
  glVertex3f(s1p[0],s1p[1],s1p[2]);
  glVertex3f(s2p[0],s2p[1],s2p[2]);
  glVertex3f(end[0],end[1],end[2]);
  glVertex3f(s2p[0],s2p[1],s2p[2]);
  glVertex3f(s1m[0],s1m[1],s1m[2]);
  glVertex3f(end[0],end[1],end[2]);
  glVertex3f(s1m[0],s1m[1],s1m[2]);
  glVertex3f(s2m[0],s2m[1],s2m[2]);
  glVertex3f(end[0],end[1],end[2]);
  glVertex3f(s2m[0],s2m[1],s2m[2]);
  glVertex3f(s1p[0],s1p[1],s1p[2]);
  glEnd();
  glPopMatrix();

}

void bill::GLaux::drawSnowMan() {
  
  glColor3f(1.0f, 1.0f, 1.0f);
  
  // Draw Body
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);
  
  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);
  
  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();
  
  // Draw Nose
  glColor3f(1.0f, 0.5f , 0.5f);
  glRotatef(0.0f,1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f,0.5f,10,2);
}

void bill::GLaux::processNormalKeys(unsigned char key, int x, int y) {
  
  switch (key) {
    case 'x' : exit(0); break;
    case 'w' : 
      moveParallel=1;  
      break;
    case 's' : 
      moveParallel=-1;
      break;
    case 'a' : 
      movePerpendicular=-1;
      break;
    case 'd' : 
      movePerpendicular=1;
      break;
    case 'e' : 
      rotateParallel=1;
      break;
    case 'q' : 
      rotateParallel=-1;
      break;
  }
}

void bill::GLaux::processNormalKeysUp(unsigned char key, int x, int y) {
  
  switch (key) {
    case 'w' : 
      moveParallel=0; 
      break;
    case 's' : 
      moveParallel=0;
      break;
    case 'a' : 
      movePerpendicular=0;
      break;
    case 'd' : 
      movePerpendicular=0;
      break;
    case 'e' : 
      rotateParallel=0;
      break;
    case 'q' : 
      rotateParallel=0;
      break;
  }
}

void bill::GLaux::pressKey(int key, int xx, int yy) {
  
  switch (key) {
    case GLUT_KEY_UP : break;
    case GLUT_KEY_DOWN : break;
  }
} 

void bill::GLaux::releaseKey(int key, int x, int y) { 	
  
  switch (key) {
    case GLUT_KEY_UP : break;
    case GLUT_KEY_DOWN : break;
  }
} 
double bill::GLaux::FDdistribution(double e,double mu,double beta){
  double exponent=exp((e-mu)*beta);
  return exponent/(exponent+1.0);
}

void bill::GLaux::mouseMove(int x, int y) { 	
}

void bill::GLaux::mouseButton(int button, int state, int x, int y) {
  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {
    
    // when the button is released
    if (state == GLUT_UP) {
      IS_CLICKED = false;
      rotateParallel=rotatePerpendicular=0;
    }
    else  {// state = GLUT_DOWN
      IS_CLICKED = true;
    }
  }
}

