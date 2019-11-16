#ifndef SEGWAYMAN_H
#define SEGWAYMAN_H

#include "common.h"
#include "objectgroup.h"
#include "camera.h"
#include "light.h"
#include "config.h"
#include <irrklang.h>
using namespace irrklang;
#include <ext.hpp>

class segwayman : public objectgroup
{
public:
	segwayman(void);
	~segwayman(void);

	void setCamera(camera *_cam);
	void setSun(light *_light);
	void setTorch(light *_light);
	void setElapsed(float _elapsed);
	void setFloorAndObstacles(objectgroup *_floor, objectgroup *_obstacles);
	
	bool isDead();

	void accelerate();
	void slowDown();
	void stopBreakSound();
	void stopSounds();
	void turnLeft();
	void turnRight();
	void twist();
	void explode();
	
	void update();

	ISound* createSound(const string &name, bool loop, bool startPaused);
	ISound *music;

	void reset();

	const float getspeed();

private:
	/*
	objects:
	0: wheels
	1: tray
	2: man
	3: skybox
	4,5: numberwheels 1/100 second
	6,7: numberwheels second
	8,9: numberwheels minute
	10: explosion
	*/
	
	
	camera *cam;
	light *sun;
	light *torch;
	objectgroup *obstacles;
	objectgroup *floor;

	float realElapsed;
	float elapsed;		//realElapsed*63.291; ein zyklus dauert bei mir meistens so 0.0158 sekunden. 1/0.0158=~63.291
	float speed;
	float cameraAngle;
	float turnAngle;
	float wheelSpeed;
	float wheelAngle;
	float bendAngle;
	float explosionTimer;

	int twistAngle;
	int twistSpeed;

	bool twisting;
	bool ceiling;
	bool dead;
	bool falling;
	bool obstacleCollision;
	bool floorCollision;
	
	int *numberStates;

	ISoundEngine* soundengine;
	ISound *engineSound;
	ISound *breakSound;
	

};

#endif