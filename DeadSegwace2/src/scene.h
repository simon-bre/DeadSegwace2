#ifndef SCENE_H
#define SCENE_H

#include "common.h"
#include "sceneobject.h"
#include "camera.h"
#include "light.h"
#include "objectgroup.h"
#include "segwayman.h"

class scene
{

public:
	scene();
	~scene(void);

	void addSceneObject(sceneobject &obj, string type);
	void addInvisibleObject(sceneobject &obj, string type);
	void setCamera(camera &cam);
	void addlight(light &lig);

	vector<sceneobject*> * getsceneobjects();
	objectgroup *getGroup(string type);
	camera * getcamera();
	vector<light*> * getlights();
	int getlightcount();


private:
	camera *_camera;
	vector<sceneobject*> *_objects;
	map<string, objectgroup*> groups;
	vector<light*> * _lights;
	float _ambientcoeff;
};


#endif