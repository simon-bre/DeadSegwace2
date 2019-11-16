#include "objectgroup.h"


objectgroup::objectgroup(void)
{
	objects = new vector<sceneobject*>();
	origin = new glm::vec4(0,0,0,1);
}




void objectgroup::addObject(sceneobject *object){
	objects->push_back(object);
}

vector<sceneobject*> *objectgroup::getObjects(){
	return objects;
}
int objectgroup::getObjectCount(){
	return objects->size();
}



bool objectgroup::collide(sceneobject *other){
	for(int i=0; i<objects->size(); i++){
		if(objects->at(i)->collide(other)) return true;
	}
	return false;
}
bool objectgroup::collide(objectgroup *other){
	for(int i=0; i<objects->size(); i++){
		for(int j=0; j<other->getObjects()->size(); j++){
			if(objects->at(i)->collide(other->getObjects()->at(j))) return true;
		}
	}
	return false;
}


void objectgroup::translateAll(float x, float y, float z){
	for(int i=0; i<objects->size(); i++){
		objects->at(i)->translate(x, y, z);
	}
}

void objectgroup::translateAllGlobal(float x, float y, float z){
	for(int i=0; i<objects->size(); i++){
		objects->at(i)->translateGlobal(x, y, z);
	}
}
	
void objectgroup::rotateAll(float angle, float x, float y, float z){
	for(int i=0; i<objects->size(); i++){
		objects->at(i)->rotate(angle, x, y, z);
	}
}

void objectgroup::scaleAll(float x, float y, float z){
	for(int i=0; i<objects->size(); i++){
		objects->at(i)->scale(x, y, z);
	}
}

void objectgroup::reset(){
	for(int i=0; i<objects->size(); i++){
		objects->at(i)->reset();
	}
}


objectgroup::~objectgroup(void)
{
	delete objects;
	delete origin;
}
