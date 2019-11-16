#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "config.h"

class texture
{

public:
	
	texture();

	void generatecheckerboard();
	void generateemptytexture();

	GLuint loadtexture();
	ILuint loadimagefile(string filename);
	static void initil()
	{
		ilInit();
	}

private:
	string path;
	
	static const int size = 256;
	GLubyte image[size][size][4];
	ILubyte * teximage;
	bool generated;
	ILenum destformat;
	ILenum desttype;

	int width;
	int height;
};


#endif