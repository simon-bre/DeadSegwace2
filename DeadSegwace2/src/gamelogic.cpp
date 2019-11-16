#include "gamelogic.h"


bool mousecaptured = true;

gamelogic::gamelogic(){
	newLap=false;
	finish=false;
	newRecord=false;
	framerateOutput=false;
	lapCounter=0;
	displayTimer=0;
	timeBuffer = glfwGetTime();
	elapsed=0;
	i = 0;

	rPressed = false;
	f2Pressed = false;
	f3Pressed = false;
	f9Pressed = false;
	f8Pressed = false;
	f10Pressed = false;
	f11Pressed = false;
}

float a=0;
float j=0;

void gamelogic::update(scene * scene, int &width, int &height)
{
	elapsed = (glfwGetTime()-timeBuffer);



	//blinkelicht
	/*a=a+0.05;
	float b = 0.6+(glm::sin(a))*0.4;
	scene->getlights()->at(0)->setintensity(b);
	*/

	

	if(glfwGetKey(_window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		mousecaptured = false;
	}

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		mousecaptured=true;
	}

	//TEST MODE
	if(config::freeCamera){

		//translate camera
		if (glfwGetKey(_window, 'A') == GLFW_PRESS)
		{
			(*scene).getcamera()->translate(-0.08,0,0);
		}
		if (glfwGetKey(_window, 'D') == GLFW_PRESS)
		{
			(*scene).getcamera()->translate(0.08,0,0);
		}
		if (glfwGetKey(_window, 'W') == GLFW_PRESS)
		{
			(*scene).getcamera()->translate(0,0,-0.08);
		}
		if (glfwGetKey(_window, 'S') == GLFW_PRESS)
		{
			(*scene).getcamera()->translate(0,0,0.08);
		}


		if(mousecaptured)
		{
			//mausbewegung
			double px,py;

			glfwGetCursorPos(_window, &px, &py);

			GLfloat diffX = (float)(px)/width;
			GLfloat diffY = (float)(py)/height;

			diffX = diffX - 0.5;
			diffY = 0.499999 - diffY;

			(*(*scene).getcamera()).multiplymatrix(glm::gtx::transform::rotate(30.0f*((glm::abs(diffX)+glm::abs(diffY))), -diffY, diffX, 0.0f));

			glfwSetCursorPos(_window, width / 2, height / 2);
		}


	}

	//GAMEPLAY MODE


	else{

		player = (segwayman*)(scene->getGroup("segwayman"));

		timeBuffer=glfwGetTime();
		player->setElapsed(elapsed);




		//------normale bewegung----------
		if(!player->isDead() && !finish){
			//lenken
			if (glfwGetKey(_window, 'A') == GLFW_PRESS)
			{
				player->turnLeft();
			}
			if (glfwGetKey(_window, 'D') == GLFW_PRESS)
			{
				player->turnRight();
			}


			//beschleunigen/bremsen
			if (glfwGetKey(_window, 'W') == GLFW_PRESS)
			{
				player->accelerate();
			}
			if (glfwGetKey(_window, 'S') == GLFW_PRESS)
			{
				player->slowDown();
			} else player->stopBreakSound();




			if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				player->twist();
			}


			player->update();
			
			//ziellinie
			if(scene->getGroup("finish")->collide(player) && displayTimer==0){

				if(lapCounter<1){
					float lapTime = glfwGetTime();
					int m = (int)(lapTime/60);
					int s = ((int)lapTime)%60;
					int h = ((int)(lapTime*100))%100;

					cout << "Lap Time: " << m << ":" << s << ":" << h <<endl;
					newLap=true;
					lapCounter++;	
				} else{
					//finish
					float newTime = glfwGetTime();

					int mNew = (int)(newTime/60);
					int sNew = ((int)newTime)%60;
					int hNew = ((int)(newTime*100))%100;

					cout << "Total time: " << mNew << ":" << sNew << ":" << hNew <<endl;
					
					finish=true;
					player->stopSounds();
					player->createSound("echo.ogg", false, false);

					//highscore
					FILE *highscoreFile;
					char buffer[64];


					highscoreFile = fopen("record.txt", "r");

					if(!highscoreFile){
						cout << "Error opening file record.txt" << endl;
						return;
					}

					int m=0, s=0, h=0;
					fscanf(highscoreFile, "%d:%d:%d",&m, &s, &h);
					fclose(highscoreFile);
					
					float oldTime = m*60 + s + (float)h/100;
					
					if(oldTime > 0)	cout << "old record: " << m << ":" << s << ":" << h <<endl;

					
					if(newTime < oldTime || oldTime==0){
						ofstream record;
						record.open("record.txt");
						
						record << mNew << ":" << sNew << ":" << hNew;
						record.close();

						newRecord=true;
						cout << "NEW RECORD! Time has been saved to 'record.txt'";
					}
					
				}
			}

			scene->getsceneobjects()->at(0)->setMatrix(glm::transpose(*(scene->getcamera()->getposmatrix())));
			if(newLap){
				scene->getsceneobjects()->at(0)->translate(0, 4, -10);
				displayTimer+=elapsed;
				if(displayTimer>3){
					newLap=false;
					displayTimer=0;
				}
			}
			else scene->getsceneobjects()->at(0)->translate(0, 0, 3);

			scene->getsceneobjects()->at(1)->setMatrix(glm::transpose(*(scene->getcamera()->getposmatrix())));
			scene->getsceneobjects()->at(1)->translate(0, 0, 3);

		}
		else if(finish){
			scene->getcamera()->translate(0,0,-2);
			scene->getcamera()->rotate(1, 0, 1, 0);
			scene->getcamera()->translate(0, 0, 2);

			scene->getsceneobjects()->at(0)->setMatrix(glm::transpose(*(scene->getcamera()->getposmatrix())));
			scene->getsceneobjects()->at(0)->translate(0, 0, 3);

			scene->getsceneobjects()->at(1)->setMatrix(glm::transpose(*(scene->getcamera()->getposmatrix())));
			if(newRecord) scene->getsceneobjects()->at(1)->translate(0, 4, -10);
			else scene->getsceneobjects()->at(1)->translate(0, 0, 3);
		} else{
			player->explode();
		}



		if (glfwGetKey(_window, 'R') == GLFW_PRESS)
		{
			rPressed = true;
		}
		if (glfwGetKey(_window, 'R') == GLFW_RELEASE && rPressed)
		{
			rPressed = false;
			cout << endl << "---NEW RACE---" << endl;
			player->reset();
			glfwSetTime(0);
			timeBuffer=0;
			finish=false;
			newLap=false;
			newRecord=false;
			lapCounter=0;
			displayTimer=0;

			f2Pressed=false;
			f3Pressed=false;
			f9Pressed=false;
			f8Pressed = false;
			f10Pressed=false;
			f11Pressed=false;
		}


	}

	//-----SPECIAL COMMANDS-----
	

	//framerate output
	if (glfwGetKey(_window, GLFW_KEY_F2) == GLFW_PRESS){
		f2Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F2) == GLFW_RELEASE && f2Pressed){
		f2Pressed = false;
		framerateOutput=!framerateOutput;
	}

	//F3 --> wireframe mode
	if (glfwGetKey(_window, GLFW_KEY_F3) == GLFW_PRESS){
		f3Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F3) == GLFW_RELEASE && f3Pressed){
		f3Pressed = false;
		config::wireframe = !config::wireframe;
		if(config::wireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			config::bloom = false;
			cout << "wireframe mode on; bloom and blur switched off" << endl;
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			cout << "wireframe mode off" << endl;
		}
	}

	//F4 --> texture filtering
	if (glfwGetKey(_window, GLFW_KEY_F4) == GLFW_PRESS){
		f4Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F4) == GLFW_RELEASE && f4Pressed){
		f4Pressed = false;
		config::texfilter = !config::texfilter;
		if(config::texfilter){
			cout << "texture filtering mode: nearest neighbor" << endl;
		}
		else{
			cout << "texture filtering mode: linear" << endl;
		}
	}

	//F5 --> mipmapping quality
	if (glfwGetKey(_window, GLFW_KEY_F5) == GLFW_PRESS){
		f5Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F5) == GLFW_RELEASE && f5Pressed){
		f5Pressed = false;
		config::mipmode = config::mipmode+1;
		if(config::mipmode == 4)
		{ config::mipmode=MIPMODE_LINEAR;
		}
		if(config::mipmode == MIPMODE_NONE){
			cout << "Mipmapping quality set to: none" << endl;
		}
		else if(config::mipmode == MIPMODE_NEAREST){
			cout << "Mipmapping quality set to: nearest neighbor" << endl;
		}
		else if(config::mipmode == MIPMODE_LINEAR){
			cout << "Mipmapping quality set to: linear" << endl;
		}
	}

	//F8 --> free camera mode
	if (glfwGetKey(_window, GLFW_KEY_F8) == GLFW_PRESS) {
		f8Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F8) == GLFW_RELEASE && f8Pressed) {
		f8Pressed = false;
		config::freeCamera = !config::freeCamera;


		if (config::freeCamera) {
			cout << "free camera mode on" << endl;
		}
		else {
			//reset
			cout << endl << "---NEW RACE---" << endl;
			((segwayman*)(scene->getGroup("segwayman")))->reset();
			glfwSetTime(0);
			timeBuffer = 0;
			finish = false;
			newLap = false;
			newRecord = false;
			lapCounter = 0;
			displayTimer = 0;

			cout << "free camera mode off" << endl;
		}
	}
	
	//F9 --> transparency
	if (glfwGetKey(_window, GLFW_KEY_F9) == GLFW_PRESS){
		f9Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F9) == GLFW_RELEASE && f9Pressed){
		f9Pressed = false;
		config::transparency = !config::transparency;
		if(config::transparency){
			glEnable(GL_BLEND);
			cout << "transparency on" << endl;
		}
		else{
			glDisable(GL_BLEND);
			cout << "transparency off" << endl;
		}
	}

	
	//F10 --> bloom
	if (glfwGetKey(_window, GLFW_KEY_F10) == GLFW_PRESS){
		f10Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F10) == GLFW_RELEASE && f10Pressed){
		f10Pressed = false;
		config::bloom = !config::bloom;
		if(config::bloom) cout << "bloom on" << endl;
		else cout << "bloom off" << endl;
	}

	//F11 --> music
	if (glfwGetKey(_window, GLFW_KEY_F11) == GLFW_PRESS){
		f11Pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_F11) == GLFW_RELEASE && f11Pressed){
		f11Pressed = false;
		if(config::music){
			((segwayman*)(scene->getGroup("segwayman")))->music->setVolume(0);
			config::music=false;
		} else{
			((segwayman*)(scene->getGroup("segwayman")))->music->setVolume(0.4);
			config::music=true;
		}
	}

	if(framerateOutput) cout << "fps: " << 1/elapsed << endl;
}

const float gamelogic::getspeed()
{
	if(i<1000)
	{
		return player->getspeed();
	}
	else
	{
		i++;
		return 1;
	}
}

gamelogic::~gamelogic(){
}