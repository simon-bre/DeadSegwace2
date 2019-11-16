#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include "sceneobject.h"

class objectgroup
{
public:
	objectgroup(void);
	~objectgroup(void);

	void addObject(sceneobject *object);

	vector<sceneobject*> *getObjects();
	int getObjectCount();

	bool collide(sceneobject *other);
	bool collide(objectgroup *other);

	void translateAll(float x, float y, float z);
	void translateAllGlobal(float x, float y, float z);
	void rotateAll(float angle, float x, float y, float z);
	void scaleAll(float x, float y, float z);

	void reset();

protected:
	vector<sceneobject*> *objects;
	glm::vec4 *origin;
};

#endif