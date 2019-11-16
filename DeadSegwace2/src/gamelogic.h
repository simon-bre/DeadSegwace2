#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "common.h"
#include "scene.h"
#include <irrKlang.h>
#include <ext.hpp>
#include <fstream>
using std::ofstream;

using namespace irrklang;

class gamelogic
{

public:
	gamelogic();
	~gamelogic();
	void update(scene * scene, int &width, int &height);
	segwayman * player;
	const float getspeed();
	void setWindow(GLFWwindow *window){ _window = window; }

private:
	int i;

	bool newLap;
	bool finish;
	bool newRecord;
	bool framerateOutput;

	bool rPressed;
	bool f2Pressed;
	bool f3Pressed;
	bool f4Pressed;
	bool f5Pressed;
	bool f8Pressed;
	bool f9Pressed;
	bool f10Pressed;
	bool f11Pressed;

	int lapCounter;
	
	float timeBuffer;
	float elapsed;
	float displayTimer;

	GLFWwindow * _window;

};

#endif