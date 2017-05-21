#include "../headers/billwindow.h"

bill::Window::Window(int argc, char **argv){
	window_position.x = 0;
	window_position.y = 0;
	window_resolution.x=800;
	window_resolution.y=600;
	window_title = "Title";

	renderScene=static_cast<void(*)()>([](){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();

	glutSwapBuffers();
});

	changeSize=static_cast<void(*)(int,int)>([](int w, int h){
  if(h == 0)
    h = 1;
  float ratio = 1.0* w / h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(0,ratio,0.1,10000.0);
  glMatrixMode(GL_MODELVIEW);

});
	processNormalKeys=static_cast<void(*)(unsigned char,int,int)>([](unsigned char key, int x, int y){});
	processNormalKeysUp=static_cast<void(*)(unsigned char,int,int)>([](unsigned char key, int x, int y){});
	pressKey=static_cast<void(*)(int,int,int)>([](int key, int x, int y){});
	releaseKey=static_cast<void(*)(int,int,int)>([](int key, int x, int y){});
	mouseButton=static_cast<void(*)(int,int,int,int)>([](int button, int state, int x, int y){});
	mouseActiveMove=static_cast<void(*)(int,int)>([](int x, int y){});
	mousePassiveMove=static_cast<void(*)(int,int)>([](int x, int y){});
	mainLoop=renderScene;

	this->argc=argc;
	this->argv=argv;	
}

void bill::Window::initiate(){
  glutInit(&argc,argv);
  glutInitWindowPosition(window_position.x,window_position.y);
  glutInitWindowSize(window_resolution.x,window_resolution.y);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(window_title.c_str());
  glutWarpPointer(window_resolution.x/2,window_resolution.y/2);

  // register callbacks
  glutDisplayFunc(renderScene);

  // Here is our new entry in the main function
  glutReshapeFunc(changeSize);

  // Animation
  glutIdleFunc(mainLoop);

  // key processing
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(processNormalKeys);
  glutKeyboardUpFunc(processNormalKeysUp);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);

  // here are the two new functions
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseActiveMove);
  glutPassiveMotionFunc(mousePassiveMove);

  // OpenGL init
  glEnable(GL_DEPTH_TEST);

  // Transparency
  glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // light  
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_COLOR_MATERIAL); // so that there will be color

  // enter GLUT event processing cycle
  glutMainLoop();

}

void bill::Window::set_renderScene(void(*f)(void)){
  renderScene=f;
}
void bill::Window::set_changeSize(void(*f)(int,int)){
  changeSize=f;
}
void bill::Window::set_processNormalKeys(void(*f)(unsigned char,int,int)){
  processNormalKeys=f;
}
void bill::Window::set_processNormalKeysUp(void(*f)(unsigned char,int,int)){
  processNormalKeysUp=f;
}
void bill::Window::set_pressKey(void(*f)(int,int,int)){
  pressKey=f;
}
void bill::Window::set_releaseKey(void(*f)(int,int,int)){
  releaseKey=f;
}
void bill::Window::set_mouseButton(void(*f)(int,int,int,int)){
  mouseButton=f;
}
void bill::Window::set_mouseActiveMove(void(*f)(int,int)){
  mouseActiveMove=f;
}
void bill::Window::set_mousePassiveMove(void(*f)(int,int)){
  mousePassiveMove=f;
}
void bill::Window::set_mainLoop(void(*f)(void)){
  mainLoop=f;
}
