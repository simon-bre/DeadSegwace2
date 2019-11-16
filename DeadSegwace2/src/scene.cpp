#include "scene.h"
#include <typeinfo>

scene::scene()
{
	_objects = new vector<sceneobject*>();
	_lights = new vector<light*>();
}


void scene::addSceneObject(sceneobject &obj, string type){
	if(groups[type]==NULL){
		if(type == "segwayman") groups[type] = new segwayman();
		else groups[type] = new objectgroup();

	}
	groups[type]->addObject(&obj);
	_objects->push_back(&obj);
}

void scene::addInvisibleObject(sceneobject &obj, string type){
	if(groups[type]==NULL) groups[type] = new objectgroup();
	groups[type]->addObject(&obj);
}


void scene::addlight(light &lig)
{
	_lights->push_back(&lig);
}

void scene::setCamera(camera &cam){
	_camera = &cam;
}

vector<sceneobject*> * scene::getsceneobjects()
{
	return _objects;
}

objectgroup *scene::getGroup(string type){
	return groups[type];
}


camera * scene::getcamera()
{
	return _camera;
}

vector<light*> * scene::getlights()
{
	return _lights;
}

scene::~scene(){
}

int scene::getlightcount()
{
	return _lights->size();
}