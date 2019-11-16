#include "sceneobject.h"


sceneobject::sceneobject(geometry &_geom)
{
	geom = &_geom;
	modelMatrix = new glm::mat4();
	ambientcoefficient = 0.05;
	diffusecoefficient = 1.0f;
	specularcoefficient = 0.9f;
	shininess = 80.0f;
	TEX=0;
	initialMatrix=NULL;

	collisionMatrix = new float[16];
	colmod = geom->colmod;
}

glm::mat4 * sceneobject::getMatrix()
{
	return modelMatrix;
}

geometry * sceneobject::getGeometry()
{
	return geom;
}


void sceneobject::setMatrix(glm::mat4 _matrix){
	*modelMatrix = _matrix;
}

void sceneobject::multiplymatrix(glm::mat4 &matrix)
{
	(*modelMatrix) *= matrix;
	refreshCollisionMatrix();
}

GLuint sceneobject::getvao()
{
	return VAO;
}

void sceneobject::setvao(GLuint &_VAO)
{
	VAO = _VAO;
}

GLuint sceneobject::gettex()
{
	return TEX;
}

void sceneobject::settex(GLuint &_TEX)
{
	TEX = _TEX;
}

GLuint sceneobject::getbump()
{
	return BUMP;
}

void sceneobject::setbump(GLuint &_BUMP)
{
	BUMP = _BUMP;
}

void sceneobject::translate(float x, float y, float z){
	(*modelMatrix) *= (glm::gtx::transform::translate(x, y, z));
	refreshCollisionMatrix();
}

void sceneobject::translateGlobal(float x, float y, float z){
	(*modelMatrix) = glm::gtx::transform::translate(x, y, z) * (*modelMatrix);
	refreshCollisionMatrix();
}

void sceneobject::rotate(float angle, float x, float y, float z){
	(*modelMatrix) *= (glm::gtx::transform::rotate(angle, x, y, z));
	refreshCollisionMatrix();
}

void sceneobject::scale(float x, float y, float z){
	(*modelMatrix) *= (glm::gtx::transform::scale(x, y, z));

	refreshCollisionMatrix();
}


void sceneobject::setambientcoefficient(float _coeff)
{
	ambientcoefficient = _coeff;
}

float sceneobject::getambientcoefficient()
{
	return ambientcoefficient;
}


void sceneobject::setdiffusecoefficient(float _coeff)
{
	diffusecoefficient = _coeff;
}

float sceneobject::getdiffusecoefficient()
{
	return diffusecoefficient;
}

void sceneobject::setspecularcoefficient(float _coeff)
{
	specularcoefficient = _coeff;
}
float sceneobject::getspecularcoefficient()
{
	return specularcoefficient;
}
void sceneobject::setshininess(float _coeff)
{
	shininess = _coeff;
}
float sceneobject::getshininess()
{
	return shininess;
}



glm::vec4 sceneobject::getOrigin(){
	return (*modelMatrix) * glm::vec4(0,0,0,1);
}


void sceneobject::defineInitialMatrix(){
	initialMatrix = new glm::mat4((*modelMatrix));
}

void sceneobject::reset(){
	if(initialMatrix != NULL){
		*modelMatrix = *initialMatrix;
		refreshCollisionMatrix();
	}
	//else lass das ding einfach wo es is, tu also nix
}



bool sceneobject::collide(sceneobject *other){
	if(this->colmod==NULL || other->colmod==NULL) return false;
	if(this->colmod->collision(other->colmod)) return true;
	else return false;
}

void sceneobject::refreshCollisionMatrix(){
	
	if(colmod == NULL) return;
	
	for(int i=0; i<4; i++){
		for(int j=0; j< 4; j++){
			collisionMatrix[i*4+j] = (float)(*modelMatrix)[i][j];
		}
	}
	geom->colmod->setTransform(collisionMatrix);
}