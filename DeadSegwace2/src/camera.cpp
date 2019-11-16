#include "camera.h"

camera::camera(glm::mat4 &projmatrix)
{
		_projmatrix = &projmatrix;
		_posmatrix = new glm::mat4();
}

void camera::multiplymatrix(glm::mat4 &matrix)
{
	(*_posmatrix) =  matrix *(*_posmatrix);
}

glm::mat4 * camera::getprojmatrix()
{
	return _projmatrix;
}

glm::mat4 * camera::getposmatrix()
{
	return _posmatrix;
}

void camera::translate(float x, float y, float z){
	(*_posmatrix) = glm::transpose(glm::gtx::transform::translate(x, y, z)) * (*_posmatrix);
}
void camera::translateGlobal(float x, float y, float z){
	(*_posmatrix) *= glm::transpose(glm::gtx::transform::translate(x, y, z));
}

void camera::rotate(float angle, float x, float y, float z){
	(*_posmatrix) = (glm::gtx::transform::rotate(angle, x, y, z)) * (*_posmatrix);
}


void camera::defineInitialMatrix(){
	_initialMatrix = new glm::mat4((*_posmatrix));
}

void camera::reset(){
	if(_initialMatrix != NULL){
		*_posmatrix = *_initialMatrix;
	} else {
		*_posmatrix = glm::mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);
	}
}


camera::~camera(){

}