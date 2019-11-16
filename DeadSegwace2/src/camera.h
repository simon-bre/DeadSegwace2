#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class camera
{
public:

	camera(glm::mat4 &projmatrix);
	~camera(void);
	
	void multiplymatrix(glm::mat4 &matrix);
	glm::mat4 * getprojmatrix();
	glm::mat4 * getposmatrix();	
	
	void translate(float x, float y, float z);
	void translateGlobal(float x, float y, float z);
	void rotate(float angle, float x, float y, float z);

	void defineInitialMatrix();		//legt fest, zu welcher matrix bei betaetigung des restart-buttons zurueckgekehrt werden soll
	void reset();	//setzt die positionsmatrix auf genau jene matrix zurueck

protected:

	glm::mat4 * _projmatrix;
	glm::mat4 * _posmatrix;
	glm::mat4 * _initialMatrix;

};

#endif