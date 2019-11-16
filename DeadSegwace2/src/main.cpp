
#include "common.h"
#include "framebuffer.h"		//inkludiert shader und geometry und kamera und light und texture



void MainLoop(framebuffer &framebuffer)
{
	while(!glfwWindowShouldClose( framebuffer.getWindow() ))
	{
		framebuffer.drawScene();
		framebuffer.updategamelogic();
		//glfwSleep(0.02);
	}
}

void cleanup(){
}

int main()
{

	config::readConfig();

	int windowwidth = config::windowwidth;
	int windowheight = config::windowheight;

	//create framebuffer
	framebuffer * mainbuffer = new framebuffer();

	//open window
	(*mainbuffer).init(windowwidth,windowheight);

	//set shader
	Shader phongshader("shader/phong");
	phongshader.bindFragData("out_color");
	(*mainbuffer).setshader(phongshader);

	
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<TEXTURES>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	texture::initil();				//initialize devIL once
	
	

	//no texture
	texture * white = new texture();
	white->generateemptytexture();
	GLuint nothing = white->loadtexture();
	
	//checkerboard
	texture *_checkerboard = new texture();
	_checkerboard->generatecheckerboard();
	GLuint checkerboard = _checkerboard->loadtexture();


	//skybox
	texture * sky = new texture();
	sky->loadimagefile("duo-space.jpg");
	GLuint sky_tex = sky->loadtexture();


	//floor
	texture * floorGerade = new texture();
	floorGerade->loadimagefile("floor_gerade.jpg");
	GLuint floor_gerade = floorGerade->loadtexture();
	
	texture * floorGeradeN = new texture();
	floorGeradeN->loadimagefile("floor_gerade_n.jpg");
	GLuint floor_gerade_n = floorGeradeN->loadtexture();


	texture * floorKurveklein = new texture();
	floorKurveklein->loadimagefile("floor_kurveklein.jpg");
	GLuint floor_kurveklein = floorKurveklein->loadtexture();
	
	texture * floorKurvekleinN = new texture();
	floorKurvekleinN->loadimagefile("floor_kurveklein_n.jpg");
	GLuint floor_kurveklein_n = floorKurvekleinN->loadtexture();


	texture * floorKurvegross = new texture();
	floorKurvegross->loadimagefile("floor_kurvegross.jpg");
	GLuint floor_kurvegross = floorKurvegross->loadtexture();
	
	texture * floorKurvegrossN = new texture();
	floorKurvegrossN->loadimagefile("floor_kurvegross_n.jpg");
	GLuint floor_kurvegross_n = floorKurvegrossN->loadtexture();


	texture * floorUebergang = new texture();
	floorUebergang->loadimagefile("floor_uebergang.jpg");
	GLuint floor_uebergang = floorUebergang->loadtexture();
	
	texture * floorUebergangN = new texture();
	floorUebergangN->loadimagefile("floor_uebergang_n.jpg");
	GLuint floor_uebergang_n = floorUebergangN->loadtexture();


	//segwayman
	texture * tyre = new texture();
	tyre->loadimagefile("tire.jpg");
	GLuint tire = tyre->loadtexture();

	texture * metall = new texture();
	metall->loadimagefile("metal.jpg");
	GLuint metal = metall->loadtexture();


	//numberwheels
	texture *_numbers = new texture();
	_numbers->loadimagefile("numbers.jpg");
	GLuint numbers = _numbers->loadtexture();


	//displays
	texture *_lap = new texture();
	_lap->loadimagefile("lap2.jpg");
	GLuint lap = _lap->loadtexture(); 

	texture *_record = new texture();
	_record->loadimagefile("new-record.jpg");
	GLuint record = _record->loadtexture(); 


	//explosion
	texture *_explo = new texture();
	_explo->loadimagefile("explosion.png");
	GLuint explo = _explo->loadtexture(); 

	//sun
	texture *_suntex = new texture();
	_suntex->loadimagefile("sun2.png");
	GLuint suntex = _suntex->loadtexture();


	//BOXES

	//gray container
	texture * container = new texture();
	container->loadimagefile("container04b.jpg");
	GLuint container_tex = (*container).loadtexture();

	//with transparency
	texture *_transboxc = new texture();
	_transboxc->loadimagefile("vent11.jpg");
	GLuint transboxc = _transboxc->loadtexture(); 

	texture *_transboxn = new texture();
	_transboxn->loadimagefile("vent11n.jpg");
	GLuint transboxn = _transboxn->loadtexture(); 

	//massive rusty
	texture *_rustyc = new texture();
	_rustyc->loadimagefile("rusty.jpg");
	GLuint rustyc = _rustyc->loadtexture(); 

	texture *_rustyn = new texture();
	_rustyn->loadimagefile("rustyn.jpg");
	GLuint rustyn = _rustyn->loadtexture();

	//metalcrate, label "explosive"
	texture *_explosivec = new texture();
	_explosivec->loadimagefile("explosive.jpg");
	GLuint explosivec = _explosivec->loadtexture(); 

	texture *_explosiven = new texture();
	_explosiven->loadimagefile("explosiven.jpg");
	GLuint explosiven = _explosiven->loadtexture();

	//alien artefact and space ships
	texture *_alienc = new texture();
	_alienc->loadimagefile("hatch12.jpg");
	GLuint alienc = _alienc->loadtexture(); 

	texture *_alienn = new texture();
	_alienn->loadimagefile("hatch12n.jpg");
	GLuint alienn = _alienn->loadtexture();

	texture *_babylon = new texture();
	_babylon->loadimagefile("babylonstation_JPG.jpg");
	GLuint babylon = _babylon->loadtexture();

	texture *_ghoul = new texture();
	_ghoul->loadimagefile("ghoul_map_jpg.jpg");
	GLuint ghoul = _ghoul->loadtexture();


	//<<<<<<<<<<<<<<<<<<<<CREATE SCENE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	scene * _scene = new scene();

	glm::mat4 * proj = new glm::mat4(glm::gtc::matrix_transform::perspective(60.0f, (float)windowwidth / (float)windowheight, 0.1f, 100.0f));
	camera playercam(*proj);
	playercam.translate(0,-1,-0.5);
	playercam.defineInitialMatrix();
	(*_scene).setCamera(playercam);

	glm::vec4 * lightpos1 = new glm::vec4(33, 1, 68, 1);
	GLfloat lightintensity1 = 0.7f;
	light light1(lightpos1, lightintensity1);
	(*_scene).addlight(light1);

	glm::vec4 * lightpos2 = new glm::vec4(0, 0, 1, 1);
	GLfloat lightintensity2 = 0.9f;
	light light2(lightpos2, lightintensity2);
	(*_scene).addlight(light2);
	
	


	//<<<<<<<<<<<<<<<<<<<<<<<<<CREATE GAMEPLAY OBJECTS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//	DISPLAYS
	sceneobject *lap2 = new sceneobject((*new geometry("lap2.obj", false, false)));
	lap2->settex(lap);
	lap2->setbump(nothing);
	lap2->setMatrix(*(playercam.getposmatrix()));
	lap2->translate(0,0,0.5);
	_scene->addSceneObject(*lap2, "display");

	sceneobject *new_record = new sceneobject((*new geometry("record.obj", false, true)));
	new_record->settex(record);
	new_record->setbump(nothing);
	new_record->setMatrix(*(playercam.getposmatrix()));
	new_record->translate(0,0,0.5);
	_scene->addSceneObject(*new_record, "display");



	//	SEGWAYMAN + SKYBOX + gui + explosion
	
	//gesamtdurchmesser der reifen: 0.279 --> r=0.1395
	sceneobject *wheels = new sceneobject((*new geometry("wheels.obj", true, false)));
	wheels->settex(nothing);
	wheels->setbump(tire);
	wheels->setdiffusecoefficient(0.2);
	wheels->translate(0,-1.87,-2.5);
	wheels->defineInitialMatrix();
	_scene->addSceneObject(*wheels, "segwayman");

	sceneobject *tray = new sceneobject((*new geometry("kasten.obj", true, false)));
	tray->settex(metal);
	tray->setbump(nothing);
	tray->translate(0,-1.87,-2.5);
	tray->defineInitialMatrix();
	_scene->addSceneObject(*tray, "segwayman");
	
	sceneobject *man = new sceneobject((*new geometry("man.obj", true, false)));
	man->settex(metal);
	man->setbump(nothing);
	man->translate(0,-1.87,-2.5);
	man->defineInitialMatrix();
	_scene->addSceneObject(*man, "segwayman");

	sceneobject *skybox = new sceneobject(*new geometry("skybox.obj", false, false));
	skybox->settex(sky_tex);
	skybox->setbump(nothing);
	skybox->translate(0,0,-2.5);
	skybox->defineInitialMatrix();
	skybox->setambientcoefficient(0.9);
	skybox->setdiffusecoefficient(0);
	skybox->setspecularcoefficient(0);
	_scene->addSceneObject(*skybox, "segwayman");
		
	//numberwheels
	geometry *numberwheel = new geometry("numberWheel.obj", false, false);
	for(int i=0; i<6; i++){
		sceneobject *nw = new sceneobject(*numberwheel);
		nw->settex(numbers);
		nw->setbump(nothing);
		nw->translate(0,-1.87,-2.5);
		nw->translate(0.038*pow(-1.0,i), 0.42+(i/2)*0.077, 0.236-(i/2)*0.004);
		nw->defineInitialMatrix();
		_scene->addSceneObject(*nw, "segwayman");
	}


	((segwayman*)(_scene->getGroup("segwayman")))->setCamera(&playercam);
	((segwayman*)(_scene->getGroup("segwayman")))->setSun(&light1);
	((segwayman*)(_scene->getGroup("segwayman")))->setTorch(&light2);






	//	FLOOR

	sceneobject *gerade = new sceneobject((*new geometry("boden_gerade.obj", true, false)));
	gerade->settex(floor_gerade);
	gerade->setbump(floor_gerade_n);
	_scene->addSceneObject(*gerade, "floor");

	sceneobject *kurveklein = new sceneobject((*new geometry("boden_kurve_klein.obj", true, false)));
	kurveklein->settex(floor_kurveklein);
	kurveklein->setbump(floor_kurveklein_n);
	_scene->addSceneObject(*kurveklein, "floor");

	sceneobject *kurvegross = new sceneobject((*new geometry("boden_kurve_gross.obj", true, false)));
	kurvegross->settex(floor_kurvegross);
	kurvegross->setbump(floor_kurvegross_n);
	_scene->addSceneObject(*kurvegross, "floor");

	sceneobject *uebergang = new sceneobject((*new geometry("boden_uebergang.obj", true, false)));
	uebergang->settex(floor_uebergang);
	uebergang->setbump(floor_uebergang_n);
	_scene->addSceneObject(*uebergang, "floor");

	_scene->getGroup("floor")->translateAll(0, -2.2, 0);

	//	OBSTACLES
	/**
	sceneobject *obstacle1 = new sceneobject(*new geometry("smallCube.obj", true, false));
	obstacle1->settex(transboxc);
	obstacle1->setbump(transboxn);
	obstacle1->translate(0,-1.5,-10);
	_scene->addSceneObject(*obstacle1, "obstacle");

	sceneobject *obstacle2 = new sceneobject(*new geometry("smallCube.obj", true, false));
	obstacle2->settex(rustyc);
	obstacle2->setbump(rustyn);
	obstacle2->translate(0,-1.5,-20);
	_scene->addSceneObject(*obstacle2, "obstacle");
	**/
	sceneobject *obstacle3 = new sceneobject(*new geometry("boom.obj", true, false));
	obstacle3->settex(explosivec);
	obstacle3->setbump(explosiven);
	obstacle3->translate(0,-2.2,0);
	_scene->addSceneObject(*obstacle3, "obstacle");
	/*
	sceneobject *obstacle4 = new sceneobject(*new geometry("smallCube.obj", true, false));
	obstacle4->settex(alienc);
	obstacle4->setbump(alienn);
	obstacle4->translate(0,-1.5,-40);
	_scene->addSceneObject(*obstacle4, "obstacle");
	*/
	sceneobject *obstacle5 = new sceneobject(*new geometry("babylon.obj", true, false));
	obstacle5->settex(babylon);
	obstacle5->setbump(nothing);
	obstacle5->translate(0,-2.2,0);
	_scene->addSceneObject(*obstacle5, "obstacle");
	
	sceneobject *obstacle6 = new sceneobject(*new geometry("ghoul.obj", true, false));
	obstacle6->settex(ghoul);
	obstacle6->setbump(nothing);
	obstacle6->translate(0,-2.2,0);
	_scene->addSceneObject(*obstacle6, "obstacle");
	
	//give floor and obstacles to player (needs it for collision detection)
	((segwayman*)(_scene->getGroup("segwayman")))->setFloorAndObstacles(_scene->getGroup("floor"), _scene->getGroup("obstacle"));


	//	FINISH
	sceneobject *finish = new sceneobject((*new geometry("finish.obj", true, false)));
	finish->settex(checkerboard);
	finish->setbump(nothing);
	finish->translate(0,-2.25,0.01);
	finish->scale(0.999, 1, 1);
	_scene->addSceneObject(*finish, "finish");


	// explosion	(ACHTUNG: gehoert zu segwayman, wird nur al letztes hinzugefuegt, wegen transparenz)
	sceneobject *explosion = new sceneobject((*new geometry("square.obj", false, false)));
	explosion->settex(explo);
	explosion->setbump(nothing);
	explosion->setambientcoefficient(1);
	explosion->setdiffusecoefficient(0);
	explosion->setspecularcoefficient(0);
	explosion->translate(0,-1.37,-0.4);	//translation im verhaeltnis zum restlichen segwayman: (0, 0.5, 0.1)
	explosion->scale(0.1, 0.1, 1);
	explosion->defineInitialMatrix();
	_scene->addSceneObject(*explosion, "segwayman");



	//load scene into framebuffer
	(*mainbuffer).loadScene(*_scene);

	//make a game logic and give it to framebuffer
	gamelogic * maingamelogic = new gamelogic();
	(*mainbuffer).setgamelogic(*maingamelogic);


	cout << "---NEW RACE---" << endl;

	//right before starting the mainloop, set time to 0 (damit im ersten frame keine komischen dinge passieren)
	glfwSetTime(0);

	//give framebuffer to mainloop
	MainLoop(*mainbuffer);

	cleanup();

	return 0;
}