#include "light.h"

light::light(glm::vec4 * &_position, GLfloat &_intensity)
{
	position = _position;
	intensity = _intensity;
}

void light::setposition(glm::vec4 _position)
{
	*position = _position;
}

glm::vec4 * light::getposition()
{
	return position;
}

void light::setintensity(GLfloat &_intensity)
{
	intensity = _intensity;
}

GLfloat light::getintensity()
{
	return intensity;
}

void light::changeviewmatrix(glm::mat4 * &_matrix)
{
	*position = *position * *_matrix;
}

void light::changeposition(glm::mat4 * &_matrix)
{
	*position = *_matrix **position;
}

void light::translate(float x, float y, float z){
	*position = glm::translate(x, y, z) * *position;
}