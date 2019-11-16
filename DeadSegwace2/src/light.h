#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"

class light
{
	

public:

	light(glm::vec4 * &_position, GLfloat &_intensity);
	
	void setposition(glm::vec4 _position);
	glm::vec4 * getposition();
	
	void setintensity(GLfloat &_intensity);
	GLfloat getintensity();
	
	void changeposition(glm::mat4 * &_matrix);
	void changeviewmatrix(glm::mat4 * &_matrix);

	void translate(float x, float y, float z);

private:
	glm::vec4 * position;
	GLfloat intensity;
};

#endif