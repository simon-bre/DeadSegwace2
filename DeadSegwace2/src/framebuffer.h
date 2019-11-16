#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "common.h"
#include "config.h"
#include "shader.h"
#include "scene.h"
#include "gamelogic.h"
#include "texture.h"

class framebuffer
{
	GLFWwindow * window;
	
	int _width, _height;
	scene * _scene;
	Shader * _shader;
	gamelogic * _gamelogic;
	Shader * bloomshader;
	Shader * bloomshader_vert;
	Shader * bloomfiltershader;
	Shader * mixtextures;
	Shader * radialblur;
	Shader * primitive;
	float playerspeed;

public:

	void init(int &width, int &height);
	void setshader(Shader &shader);
	framebuffer();
	void loadScene(scene &scene);
	void drawScene();
	void setgamelogic(gamelogic &gamelogic);
	void updategamelogic();
	int getwidth();
	int getheight();
	GLFWwindow * getWindow(){ return window; }
	GLuint testtexid;
	GLuint bloomfiltertex;
	GLuint bloomtexid;
	GLuint bloomtexid2;
	GLuint bloomtexid_actual;
	GLuint bloomfiltertex_actual;
	GLuint bloomfiltertex_actual2;
	GLuint fboId;
	GLuint fboId2;
	GLuint bloomfbo;
	GLuint rboId;
	GLuint rboId2;
	GLuint bloomvao;
	GLuint shadowmapping_fbo;
	GLuint shadowmapping_depthtex;
};

#endif