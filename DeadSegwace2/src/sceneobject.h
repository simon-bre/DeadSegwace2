#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "common.h"
#include "geometry.h"


class sceneobject
{	
public:
	
	sceneobject(geometry &_geometry);	//lies eine geometrie ein, setzte mvmatrix auf unit
	glm::mat4 * getMatrix();
	geometry * getGeometry();
	sceneobject();
	GLuint getvao();
	void setvao(GLuint &VAO);
	void setambientcoefficient(float _coeff);
	float getambientcoefficient();
	void setdiffusecoefficient(float _coeff);
	float getdiffusecoefficient();
	void setspecularcoefficient(float _coeff);
	float getspecularcoefficient();
	void setshininess(float _coeff);
	float getshininess();
	glm::vec4 getOrigin();
	GLuint gettex();
	void settex(GLuint &TEX);
	GLuint getbump();
	void setbump(GLuint &_BUMP);

	void setMatrix(glm::mat4 _matrix);
	void multiplymatrix(glm::mat4 &matrix);	//multipliziere eine matrix auf die aktuelle mvmatrix drauf	
	void translate(float x, float y, float z);
	void translateGlobal(float x, float y, float z);
	void rotate(float angle, float x, float y, float z);
	void scale(float x, float y, float z);

	void defineInitialMatrix();	//legt fest, zu welcher matrix bei betaetigung des restart-buttons zurueckgekehrt werden soll
	void reset();	//setzt die positionsmatrix auf genau jene matrix zurueck

	CollisionModel3D *colmod;	//ACHTUNG, kann auch NULL sein!
	bool collide(sceneobject *other);

protected:
	glm::mat4 * modelMatrix;
	glm::mat4 * initialMatrix;
	geometry * geom;
	GLuint VAO;
	GLuint TEX;
	GLuint BUMP;
	float ambientcoefficient;
	float diffusecoefficient;	//default = 1.0 --v
	float specularcoefficient;
	float shininess;	//default = 80
	
	float *collisionMatrix;	//coldet braucht die model matrix als float-array.. drum hier nochmal.
	void refreshCollisionMatrix();
};

#endif