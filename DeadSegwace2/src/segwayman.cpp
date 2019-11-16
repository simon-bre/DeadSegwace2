#include "segwayman.h"


segwayman::segwayman(void)
{
	elapsed=0;
	realElapsed=0;
	speed=0;
	turnAngle=0;
	cameraAngle=0;

	wheelSpeed=0;
	wheelAngle=0;
	bendAngle=0;
	explosionTimer=0;
	twistAngle=0;
	twistSpeed=5;

	dead=false;
	falling=false;
	twisting=false;
	ceiling=false;
	obstacleCollision=false;




	numberStates = new int[6];
	for(int i=0; i<6; i++){
		numberStates[i]=0;
	}


	//sounds
	soundengine = createIrrKlangDevice();
	if(!soundengine){
		printf("Could not startup eninge\n");
		return;
	}

	engineSound = createSound("engine.ogg", true, true);
	engineSound->setVolume(0);
	breakSound = createSound("break.ogg", true, true);
	breakSound->setVolume(0.5);

	music = createSound("never_ever_land.ogg", true, false);
	if(config::music) music->setVolume(0.4);
	else music->setVolume(0);
}

void segwayman::setCamera(camera *_cam){
	cam = _cam;
}
void segwayman::setSun(light *_light){
	sun = _light;
}
void segwayman::setTorch(light *_light){
	torch = _light;
}
void segwayman::setElapsed(float _elapsed){
	realElapsed = _elapsed;
	elapsed = _elapsed*63.291;
}
void segwayman::setFloorAndObstacles(objectgroup *_floor, objectgroup *_obstacles){
	floor = _floor;
	obstacles = _obstacles;
}


bool segwayman::isDead(){
	return dead;
}



void segwayman::accelerate(){
	if(floor->collide(objects->at(0))){
		if(speed<1){
			speed+=0.001*elapsed;
			wheelSpeed=(180*speed)/(0.1395*M_PI);	//0.1395=radius
			if(engineSound->getIsPaused()){
				engineSound->setIsPaused(false);
			}
			engineSound->setPlaybackSpeed(speed*3+1);
			engineSound->setVolume(speed+0.2);
		}
	}
}
void segwayman::slowDown(){
	if(floor->collide(objects->at(0))){
		if(speed>0){
			speed-=0.004*elapsed;
			wheelSpeed=(180*speed)/(0.1395*M_PI);
			engineSound->setPlaybackSpeed(speed*3+1);
			engineSound->setVolume(speed+0.2);
			if(breakSound->getIsPaused()) breakSound->setIsPaused(false);
		}
		else{
			speed=0;
			wheelSpeed=0;
			if(!breakSound->getIsPaused()) breakSound->setIsPaused(true);
		}
	}
}

void segwayman::stopBreakSound(){
	if(!breakSound->getIsPaused()) breakSound->setIsPaused(true);
}

void segwayman::turnLeft(){	//wie stark die lenkung anspricht ist abhängig von der geschwindigkeit
	if(!twisting){
		objects->at(0)->rotate(wheelAngle, 1, 0, 0);
		objects->at(0)->rotate(elapsed/(speed+1),0,1,0);		
		objects->at(0)->rotate(-wheelAngle, 1, 0, 0);


		objects->at(1)->rotate(elapsed/(speed+1),0,1,0);

		if(bendAngle < 20){
			objects->at(2)->rotate(elapsed*1, 0, 0, 1);
			bendAngle+=elapsed*1;
		}
		objects->at(2)->rotate(-bendAngle, 0, 0, 1);
		objects->at(2)->rotate(elapsed/(speed+1),0,1,0);
		objects->at(2)->rotate(bendAngle, 0, 0, 1);

		turnAngle+=elapsed/(speed+1);
	}
}
void segwayman::turnRight(){
	if(!twisting){
		objects->at(0)->rotate(wheelAngle, 1, 0, 0);
		objects->at(0)->rotate(-elapsed/(speed+1),0,1,0);
		objects->at(0)->rotate(-wheelAngle, 1, 0, 0);

		objects->at(1)->rotate(-elapsed/(speed+1),0,1,0);

		if(bendAngle > -20){
			objects->at(2)->rotate(-elapsed*1, 0, 0, 1);
			bendAngle-=elapsed*1;
		}
		objects->at(2)->rotate(-bendAngle, 0, 0, 1);
		objects->at(2)->rotate(-elapsed/(speed+1),0,1,0);
		objects->at(2)->rotate(bendAngle, 0, 0, 1);

		turnAngle-=elapsed/(speed+1);
	}
}
void segwayman::twist(){

	if(!twisting && floor->collide(objects->at(0))){ 

		createSound("swoosh.ogg", false, false)->setVolume(0.6);

		twisting = true;

		if(cameraAngle!=turnAngle){		//macht man das nicht, verdreht sich die kamera bei der twist-animation
			cam->translate(0,0,-2);
			cam->rotate(cameraAngle-turnAngle,0,1,0);
			cam->translate(0,0,2);
			cameraAngle=turnAngle;
		}

	}
}




void segwayman::update(){

	//-------------------normale bewegung--------------------

	//verzoegertes nachdrehen
	if(cameraAngle!=turnAngle){
		cam->translate(0,0,-2);
		cam->rotate(elapsed*(cameraAngle-turnAngle)/20,0,1,0);
		cam->translate(0,0,2);
		cameraAngle-=elapsed*(cameraAngle-turnAngle)/20;
	}

	//fahren
	objects->at(0)->rotate(-wheelSpeed, 1, 0, 0);
	wheelAngle+=wheelSpeed;
	if(wheelAngle>=360) wheelAngle-=360;
	objects->at(0)->rotate(wheelAngle, 1, 0, 0);
	objects->at(0)->translate(0,0,-speed*elapsed);
	objects->at(0)->rotate(-wheelAngle, 1, 0, 0);

	objects->at(1)->translate(0,0,-speed*elapsed);
	objects->at(2)->translate(0,0,-speed*elapsed);

	if(!ceiling){
		objects->at(3)->translateGlobal(sin(M_PI*(float)turnAngle/180)*(-speed*elapsed), 0, cos(M_PI*(float)turnAngle/180)*(-speed*elapsed));
	}
	else{
		objects->at(3)->translateGlobal(-sin(M_PI*(float)turnAngle/180)*(-speed*elapsed), 0, cos(M_PI*(float)turnAngle/180)*(-speed*elapsed));
	}



	//aufstellen 
	if(bendAngle > 0){
		objects->at(2)->rotate(-0.5*elapsed, 0, 0, 1);
		bendAngle-=0.5*elapsed;
	}
	if(bendAngle < 0){
		objects->at(2)->rotate(0.5*elapsed, 0, 0, 1);
		bendAngle+=0.5*elapsed;
	}


	//kamera-fahrt
	if(!ceiling){
		cam->translateGlobal(sin(M_PI*(float)turnAngle/180)*(-speed*elapsed), 0, cos(M_PI*(float)turnAngle/180)*(-speed*elapsed));
	}
	else{
		cam->translateGlobal(-sin(M_PI*(float)turnAngle/180)*(-speed*elapsed), 0, cos(M_PI*(float)turnAngle/180)*(-speed*elapsed));
	}




	//---------kollision mit hindernis-------------
	if(this->collide(obstacles)){
		if(!dead){
			createSound("explosion.ogg",false,false);
			breakSound->setIsPaused(true);
			engineSound->setIsPaused(true);
			obstacleCollision=true;
			dead=true;
		}
	}


	//------------fallen etc------------------
	floorCollision = floor->collide(objects->at(0));

	if(!floorCollision && !twisting){	

		objects->at(0)->rotate(wheelAngle, 1, 0, 0);		
		objects->at(0)->translate(0,-0.2*elapsed,0);
		objects->at(0)->rotate(-wheelAngle, 1, 0, 0);

		objects->at(1)->translate(0,-0.2*elapsed,0);

		objects->at(2)->rotate(-bendAngle, 0, 0, 1);
		objects->at(2)->translate(0,-0.2*elapsed,0);
		objects->at(2)->rotate(bendAngle, 0, 0, 1);

		if(!ceiling){
			objects->at(3)->translateGlobal(0, -0.2, 0);
		}
		else{
			objects->at(3)->translateGlobal(0, 0.2, 0);
		}

		cam->translate(0,-0.2*elapsed,0);
		falling=true;
	}

	if(floorCollision && falling){
		//zuruecksetzen des spielers an die kollisionsposition!!! (versinkt sonst in der strecke --> scheisse)
		int count = 0;

		objects->at(0)->rotate(wheelAngle, 1, 0, 0);
		while(floor->collide(objects->at(0))){
			objects->at(0)->translate(0,0.01,0);
			count++;
		}
		objects->at(0)->translate(0,-0.01,0);
		objects->at(0)->rotate(-wheelAngle, 1, 0, 0);
		count--;

		objects->at(1)->translate(0,0.01*count,0);

		objects->at(2)->rotate(-bendAngle, 0, 0, 1);
		objects->at(2)->translate(0,0.01*count,0);
		objects->at(2)->rotate(bendAngle, 0, 0, 1);

		cam->translate(0,0.01*count,0);

		falling=false;
	}

	if((objects->at(0)->getOrigin().y>=40 || objects->at(0)->getOrigin().y<=-40) && !dead){
		createSound("WilhelmScream.ogg",false,false);
		breakSound->setIsPaused(true);
		engineSound->setIsPaused(true);
		dead=true;
	}



	//----------twist-gravity-animation------------
	if(twisting){

		twistSpeed=(int)(elapsed*5);

		objects->at(0)->rotate(wheelAngle, 1, 0, 0);
		objects->at(0)->rotate(twistSpeed,0,0,1);
		objects->at(0)->rotate(-wheelAngle, 1, 0, 0);
		objects->at(1)->rotate(twistSpeed,0,0,1);
		objects->at(2)->rotate(-bendAngle, 0, 0, 1);
		objects->at(2)->rotate(twistSpeed,0,0,1);
		objects->at(2)->rotate(bendAngle, 0, 0, 1);
		cam->rotate(twistSpeed,0,0,1);
		if(!ceiling){
			objects->at(0)->translateGlobal(0, 3.74/(180/((float)twistSpeed)), 0);
			objects->at(1)->translateGlobal(0, 3.74/(180/((float)twistSpeed)), 0);
			objects->at(2)->translateGlobal(0, 3.74/(180/((float)twistSpeed)), 0);
			cam->translateGlobal(0, 2/(180/((float)twistSpeed)), 0);
			torch->translate(0, 2.74/(180/((float)twistSpeed)), 0);
		}
		else {
			objects->at(0)->translateGlobal(0, -3.74/(180/((float)twistSpeed)), 0);
			objects->at(1)->translateGlobal(0, -3.74/(180/((float)twistSpeed)), 0);
			objects->at(2)->translateGlobal(0, -3.74/(180/((float)twistSpeed)), 0);
			cam->translateGlobal(0, -2/(180/((float)twistSpeed)), 0);
			torch->translate(0, -2.74/(180/((float)twistSpeed)), 0);
		}

		twistAngle+=twistSpeed;

		if(twistAngle >= 180){

			if(twistAngle > 180){
				float diff = 180-twistAngle;

				objects->at(0)->rotate(wheelAngle, 1, 0, 0);
				objects->at(0)->rotate(diff,0,0,1);
				objects->at(0)->rotate(-wheelAngle, 1, 0, 0);
				objects->at(1)->rotate(diff,0,0,1);
				objects->at(2)->rotate(-bendAngle, 0, 0, 1);
				objects->at(2)->rotate(diff,0,0,1);
				objects->at(2)->rotate(bendAngle, 0, 0, 1);
				cam->rotate(diff,0,0,1);

				if(!ceiling){
					objects->at(0)->translateGlobal(0, 3.74/(180/diff), 0);
					objects->at(1)->translateGlobal(0, 3.74/(180/diff), 0);
					objects->at(2)->translateGlobal(0, 3.74/(180/diff), 0);
					cam->translateGlobal(0, 2/(180/diff), 0);
					torch->translate(0, 2.74/(180/diff), 0);
				}
				else {
					objects->at(0)->translateGlobal(0, -3.74/(180/diff), 0);
					objects->at(1)->translateGlobal(0, -3.74/(180/diff), 0);
					objects->at(2)->translateGlobal(0, -3.74/(180/diff), 0);
					cam->translateGlobal(0, -2/(180/diff), 0);
					torch->translate(0, -2.74/(180/diff), 0);
				}
				/*int count = 0;

				objects->at(0)->rotate(wheelAngle, 1, 0, 0);
				while(floor->collide(objects->at(0))){
				objects->at(0)->translate(0,0.01,0);
				count++;
				}
				objects->at(0)->translate(0,-0.01,0);
				objects->at(0)->rotate(-wheelAngle, 1, 0, 0);
				count--;

				objects->at(1)->translate(0,0.01*count,0);

				objects->at(2)->rotate(-bendAngle, 0, 0, 1);
				objects->at(2)->translate(0,0.01*count,0);
				objects->at(2)->rotate(bendAngle, 0, 0, 1);

				cam->translate(0,0.01*count,0);*/

			}

			twistAngle=0;
			twisting=false;
			ceiling=!ceiling;
			turnAngle*=-1;
			cameraAngle*=-1;
		}

	}

	//timer aktualisieren
	float time = glfwGetTime();
	numberStates[0]=((int)(time*100))%10;
	numberStates[1]=((int)(time*10))%10;
	numberStates[2]=((int)(time))%10;
	numberStates[3]=((int)(time/10))%6;
	numberStates[4]=((int)(time/60))%10;
	numberStates[5]=((int)(time/60/10))%10;

	for(int i=0; i<6; i++){
		objects->at(4+i)->setMatrix(*(objects->at(2)->getMatrix()));
		objects->at(4+i)->translate(0.038*pow(-1.0,i), 0.42+(i/2)*0.077, 0.236-(i/2)*0.004);
		objects->at(4+i)->rotate(36*numberStates[i], -1, 0, 0);
	}

	//explosion mitschleifen
	objects->at(10)->setMatrix(*(objects->at(2)->getMatrix()));
	objects->at(10)->translate(0,0.5,0.1);
	objects->at(10)->scale(0.1, 0.1, 0);

	//lichter mitschleifen
	sun->setposition((*(objects->at(1)->getMatrix()) * glm::vec4(0, 0, 0, 1)));
	sun->translate(33, 1, 68);
	torch->setposition((*(objects->at(1)->getMatrix()) * glm::vec4(0, 0, 0, 1)));
	if(!twisting){
		if(!ceiling) torch->translate(-sin(M_PI*(float)turnAngle/180)*2, 0.5, -cos(M_PI*(float)turnAngle/180)*2);
		else torch->translate(sin(M_PI*(float)turnAngle/180)*2, -0.5, -cos(M_PI*(float)turnAngle/180)*2);
	}
	else torch->translate(-sin(M_PI*(float)turnAngle/180)*2, 0, -cos(M_PI*(float)turnAngle/180)*2);
}


ISound* segwayman::createSound(const string &name, bool loop, bool startPaused){	
	string pfad = config::soundPath+name;
	return soundengine->play2D(pfad.c_str(), loop, startPaused, true);	//letzter parameter: trackable
}

void segwayman::explode(){
	if(obstacleCollision && explosionTimer<100){	//<2
		//explosionTimer+=realElapsed;
		//float factor=1+(1.0/pow(explosionTimer*25,2));
		explosionTimer+=0.46;
		float factor=1+(1.0/pow(explosionTimer,2));
		objects->at(10)->scale(factor, factor, 0);
	}
}


void segwayman::reset(){
	speed=0;
	turnAngle=0;
	cameraAngle=0;
	dead=false;
	falling=false;
	twisting=false;
	twistAngle=0;
	ceiling=false;
	obstacleCollision=false;
	wheelSpeed=0;
	wheelAngle=0;
	bendAngle=0;
	explosionTimer=0;
	elapsed=0;
	realElapsed=0;

	for(int i=0; i<6; i++){
		numberStates[i]=0;
	}

	engineSound->setVolume(0.1);
	engineSound->setPlaybackSpeed(1);
	engineSound->setIsPaused(true);
	breakSound->setIsPaused(true);

	cam->reset();

	for(int i=0; i<objects->size(); i++){
		objects->at(i)->reset();
	}
}

void segwayman::stopSounds(){
	engineSound->setIsPaused(true);
	breakSound->setIsPaused(true);
}

const float segwayman::getspeed()
{
	return speed;
}


segwayman::~segwayman(void)
{
	soundengine->drop();
}
