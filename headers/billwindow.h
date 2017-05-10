#ifndef BILLWINDOW_H
#define BILLWINDOW_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "billvector.h"

namespace bill{

template<typename T>
struct coord{
	T x;
	T y;
};

class Window{
private:
	coord<unsigned int> window_position;
	coord<int> window_resolution;
	std::string window_title;
	void(*mainLoop)(void);
	void(*renderScene)(void);
	void(*changeSize)(int,int);
	void(*processNormalKeys)(unsigned char,int,int);
	void(*processNormalKeysUp)(unsigned char,int,int);
	void(*pressKey)(int,int,int);
	void(*releaseKey)(int,int,int);
	void(*mouseButton)(int,int,int,int);
	void(*mouseActiveMove)(int,int);
	void(*mousePassiveMove)(int,int);
	int argc;
	char **argv;

public:
	Window(int argc, char **argv);
	void initiate();
	
	void set_renderScene(void(*)(void));
	void set_changeSize(void(*)(int,int));
	void set_processNormalKeys(void(*)(unsigned char,int,int));
	void set_processNormalKeysUp(void(*)(unsigned char,int,int));
	void set_pressKey(void(*)(int,int,int));
	void set_releaseKey(void(*)(int,int,int));
	void set_mouseButton(void(*)(int,int,int,int));
	void set_mouseActiveMove(void(*)(int,int));
	void set_mousePassiveMove(void(*)(int,int));
	void set_mainLoop(void(*)(void));
}; //end of class Window
} //end of namespace bill

#endif //end of file
