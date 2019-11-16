#include "framebuffer.h"

framebuffer::framebuffer()
{
	_scene = new scene();
	playerspeed = 0;
}

void framebuffer::loadScene(scene &scene)
{
	_scene = &scene;
	for (int i=0; i<scene.getsceneobjects()->size(); i++)
	{
		GLuint VAO_ID;
		GLuint locVBO_ID;
		GLuint colVBO_ID;
		GLuint norVBO_ID;
		GLuint ligVBO_ID;
		GLuint texVBO_ID;

		PFNGLBINDVERTEXARRAYPROC my_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
		PFNGLGENVERTEXARRAYSPROC my_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");


		//ein geometrie-objekt mit Quelldatei fuer den Konstruktor

		//testGeom->testImport(); //gibt eine LIste mit Dreiecken und dazugehoerigen Daten + importierte materialen im Standartoutput aus
		GLfloat *positions = scene.getsceneobjects()->at(i)->getGeometry()->getPositions();

		my_glGenVertexArrays(1, &VAO_ID);
		my_glBindVertexArray(VAO_ID);

		glGenBuffers(1, &locVBO_ID);
		glGenBuffers(1, &colVBO_ID);
		glGenBuffers(1, &norVBO_ID);
		glGenBuffers(1, &ligVBO_ID);
		glGenBuffers(1, &texVBO_ID);

		glBindBuffer(GL_ARRAY_BUFFER, colVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, scene.getsceneobjects()->at(i)->getGeometry()->getVertexCount()*3*sizeof(GLfloat), 
			scene.getsceneobjects()->at(i)->getGeometry()->getDiffuse(), GL_STATIC_DRAW);

		GLint color_loc = _shader->getAttrib("in_color");
		glEnableVertexAttribArray(color_loc);					//die werte hier drinnen werden werden beim zeichnen verwendet
		glVertexAttribPointer(	color_loc,						//gleiches, wie bei den bisherigen versuchen schon: gibt an, wie die gebufferten daten
			3,								//zu interpretieren sind. (in diesem fall, speichere sie in das vorher enable'te ding)
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);		// = null pointer

		glBindBuffer(GL_ARRAY_BUFFER, norVBO_ID);		//normalen
		glBufferData(GL_ARRAY_BUFFER, scene.getsceneobjects()->at(i)->getGeometry()->getVertexCount()*3*sizeof(GLfloat), 
			scene.getsceneobjects()->at(i)->getGeometry()->getNormals(), GL_STATIC_DRAW);

		GLint normal_loc = _shader->getAttrib("in_normal");
		glEnableVertexAttribArray(normal_loc);				
		glVertexAttribPointer(	normal_loc,							
			3,								
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);


		glBindBuffer(GL_ARRAY_BUFFER, locVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, scene.getsceneobjects()->at(i)->getGeometry()->getVertexCount()*4*sizeof(GLfloat), 
			scene.getsceneobjects()->at(i)->getGeometry()->getPositions(), GL_STATIC_DRAW);


		GLint vertex_loc = _shader->getAttrib("in_position");
		glEnableVertexAttribArray(vertex_loc);				
		glVertexAttribPointer(	vertex_loc,							
			4,								
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

		glBindBuffer(GL_ARRAY_BUFFER, texVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, scene.getsceneobjects()->at(i)->getGeometry()->getVertexCount()*2*sizeof(GLfloat), 
			scene.getsceneobjects()->at(i)->getGeometry()->getTexCoords(), GL_STATIC_DRAW);


		GLint tex_loc = _shader->getAttrib("in_texcoords");
		glEnableVertexAttribArray(tex_loc);				
		glVertexAttribPointer(tex_loc,							
			2,								
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

		scene.getsceneobjects()->at(i)->setvao(VAO_ID);



		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D,0);
		my_glBindVertexArray(0);

		

	}


}

void framebuffer::init(int &width, int &height)
{
	_width = width; _height = height;
	if (!glfwInit()) cout << "Error initializing GLFW!" << endl;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//fenster erstellen
	window = glfwCreateWindow( _width,  _height,  "Dead Segwace 2",  0,  0);

	glfwMakeContextCurrent(window);
	
	//openGL Version angeben (wir haben seltsamerweise Version 3.2 und das laesst sich auch nicht aendern)
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// check if we have a core-profile (wir haben ein compatibility-profile; brauchen aber core-profile)
	GLint profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (profile == GL_CONTEXT_CORE_PROFILE_BIT) {
		std::cout << "Having a core-profile" << std::endl;
	} else {
		std::cout << "Having a compatibility-profile" << std::endl;
	}

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		cout << "GLEW INIT ERROR " << glewGetErrorString(err);
	}

	bloomshader = new Shader("shader/bloom");
	bloomshader->bindFragData("out_color");

	glEnable(GL_BLEND);		//transparenz
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)glfwGetProcAddress("glGenFramebuffersEXT");PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)glfwGetProcAddress("glBindFramebufferEXT");PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)glfwGetProcAddress("glGenRenderbuffersEXT");PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)glfwGetProcAddress("glBindRenderbufferEXT");PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glfwGetProcAddress("glRenderbufferStorageEXT");
	//PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glfwGetProcAddress("glFramebufferRenderbufferEXT");
	//PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glfwGetProcAddress("glCheckFramebufferStatusEXT");
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)glfwGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)glfwGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)glfwGetProcAddress("glFramebufferTexture2DEXT");
	glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)glfwGetProcAddress("glGenRenderbuffersEXT");
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)glfwGetProcAddress("glBindRenderbufferEXT");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glfwGetProcAddress("glRenderbufferStorageEXT");
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glfwGetProcAddress("glCheckFramebufferStatusEXT");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glfwGetProcAddress("glFramebufferRenderbufferEXT");
	glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)glfwGetProcAddress("glGenerateMipmapEXT");

	//FBO-Textur erstellen und einrichten
	glGenTextures(1,&bloomtexid);
	glBindTexture(GL_TEXTURE_2D, bloomtexid);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&bloomtexid_actual);
	glBindTexture(GL_TEXTURE_2D, bloomtexid_actual);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&bloomfiltertex_actual);
	glBindTexture(GL_TEXTURE_2D, bloomfiltertex_actual);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1,&bloomfiltertex_actual2);
	glBindTexture(GL_TEXTURE_2D, bloomfiltertex_actual2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	glBindTexture(GL_TEXTURE_2D, 0);

		//============================================================== BULLSHIT

			//FBO-Textur erstellen und einrichten
	glGenTextures(1,&bloomtexid2);
	glBindTexture(GL_TEXTURE_2D, bloomtexid2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	/*
		// create a renderbuffer object to store depth info
	glGenRenderbuffersEXT(1, &rboId2);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId2);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	
	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId2);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId2);
	
	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, bloomtexid2, 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboId2);
		
	GLint status2 = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status2 != GL_FRAMEBUFFER_COMPLETE_EXT) cout << "Error creating FBOid2: "<< status2 <<endl;*/


	//============================================================ BULLSHIT ENDE

	//================== generate stuff for shadow mapping
	glGenTextures(1,&shadowmapping_depthtex);
	glBindTexture(GL_TEXTURE_2D, shadowmapping_depthtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glGenFramebuffersEXT(1, &shadowmapping_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shadowmapping_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
	GL_TEXTURE_2D, shadowmapping_depthtex, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
	GL_TEXTURE_2D, bloomtexid2, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) cout << "Error creating shadowmapping FBO: "<< status <<endl;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


	//================== stuff for bloom and radial blur
	// create a renderbuffer object to store depth info
	glGenRenderbuffersEXT(1, &rboId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, bloomtexid, 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboId);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) cout << "Error creating FBO: "<< status <<endl;



	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glGetError(); 
	PFNGLBINDVERTEXARRAYPROC my_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
	PFNGLGENVERTEXARRAYSPROC my_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");

	bloomshader = new Shader("shader/bloom");
	bloomshader->bindFragData("out_color");
	bloomshader_vert = new Shader("shader/bloom_vert");
	bloomshader_vert->bindFragData("out_color");
	bloomfiltershader = new Shader("shader/bloomfilter");
	bloomfiltershader->bindFragData("out_color");
	mixtextures = new Shader("shader/mixtextures");
	mixtextures->bindFragData("out_color");
	radialblur = new Shader("shader/radialblur");
	radialblur->bindFragData("out_color");
	primitive = new Shader("shader/primitiveshader");
	//primitive->bindFragData("out_color");

	//create second fbo to draw the bloom texture
	glGenFramebuffersEXT(1, &bloomfbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bloomfbo);
	glGenTextures(1,&bloomfiltertex);
	glBindTexture(GL_TEXTURE_2D, bloomfiltertex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, bloomfiltertex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboid2;
	glGenRenderbuffersEXT(1, &rboid2);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboid2);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboid2);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) cout << "Error creating bloom FBO: "<< status <<endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);




	//draw quad erstelelen
	GLuint locVBO_ID;
	GLuint colVBO_ID;
	GLuint texVBO_ID;

	my_glGenVertexArrays(1, &bloomvao);
	my_glBindVertexArray(bloomvao);

	glGenBuffers(1, &locVBO_ID);
	glGenBuffers(1, &colVBO_ID);
	glGenBuffers(1, &texVBO_ID);

	GLfloat cols[] = {1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, colVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), 
		cols, GL_STATIC_DRAW);

	GLint color_loc = bloomshader->getAttrib("in_color");
	glEnableVertexAttribArray(color_loc);					
	glVertexAttribPointer(	color_loc,						
		3,								
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);	

	GLfloat pos[] = {1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, locVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, 6*4*sizeof(GLfloat), 
		pos, GL_STATIC_DRAW);

	GLint vertex_loc = bloomshader->getAttrib("in_position");
	glEnableVertexAttribArray(vertex_loc);				
	glVertexAttribPointer(	vertex_loc,							
		4,								
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);


	GLfloat texc[] = {1.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, texVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(GLfloat), 
		texc, GL_STATIC_DRAW);

	GLint tex_loc = bloomshader->getAttrib("in_texcoords");
	glEnableVertexAttribArray(tex_loc);				
	glVertexAttribPointer(tex_loc,							
		2,								
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	my_glBindVertexArray(0);

}

void framebuffer::setshader(Shader &shader)
{
	_shader = &shader;
}

void framebuffer::drawScene()
{
	PFNGLBINDVERTEXARRAYPROC my_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");

	camera *playercam = _scene->getcamera();
	int lightcount = _scene->getlightcount();
	light *mainlight	= _scene->getlights()->at(0);

	glm::mat4 * lightproj = new glm::mat4(glm::gtc::matrix_transform::perspective(60.0f, (float)_width / (float)_height, 0.1f, 100.0f));
	camera *lightcam = new camera(*lightproj);
	lightcam->translate(0,10,1);
	lightcam->rotate(80,1,0,0);

	if(config::shadow)
	{
	//===== shadow map
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,shadowmapping_fbo);

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PFNGLBINDVERTEXARRAYPROC my_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");

	glDisable(GL_BLEND);

	camera *playercam = _scene->getcamera();
	int lightcount = _scene->getlightcount();
	light *mainlight	= _scene->getlights()->at(0);

	glm::mat4 * proj = new glm::mat4(glm::gtc::matrix_transform::perspective(60.0f, (float)_width / (float)_height, 0.1f, 100.0f));
	//glm::mat4 * proj = new glm::mat4();
	camera *lightcam = new camera(*proj);
	lightcam->translate(0,10,1);
	lightcam->rotate(80,1,0,0);

	for (int i=0; i<_scene->getsceneobjects()->size(); i++)
	{
		_shader->bind();

		sceneobject *sceneobject = _scene->getsceneobjects()->at(i);

		my_glBindVertexArray(sceneobject->getvao());

		GLint mat_loc = _shader->getUniform("transform_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*sceneobject->getMatrix()));

		mat_loc = _shader->getUniform("view_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(glm::inverse(*lightcam->getposmatrix())));

		mat_loc = _shader->getUniform("transp_inv_transform_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(glm::transpose(glm::inverse(*sceneobject->getMatrix()))));

		mat_loc = _shader->getUniform("projection_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*lightcam->getprojmatrix()));

		mat_loc = _shader->getUniform("inverse_view_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*lightcam->getposmatrix()));

		GLint vec_loc = _shader->getUniform("in_lightcount");
		glUniform1f(vec_loc, lightcount);

		for(int i=0; i<lightcount; i++)
		{
			std::stringstream ss;
			ss<<i;
			vec_loc = (*_shader).getUniform("in_lightpos["+ss.str()+"]");
			glm::vec3 lightpos = glm::vec3(_scene->getlights()->at(i)->getposition()->x,
				_scene->getlights()->at(i)->getposition()->y,
				_scene->getlights()->at(i)->getposition()->z);
			glUniform3fv(vec_loc, 1, glm::value_ptr(lightpos));

			vec_loc = _shader->getUniform("in_lightintensity["+ss.str()+"]");
			glUniform1f(vec_loc, _scene->getlights()->at(i)->getintensity());

		}

		vec_loc = _shader->getUniform("in_lightintensity");
		glUniform1f(vec_loc, mainlight->getintensity());

		vec_loc = _shader->getUniform("in_ambientcoefficient");
		glUniform1f(vec_loc, sceneobject->getambientcoefficient());

		vec_loc = _shader->getUniform("in_diffusecoefficient");
		glUniform1f(vec_loc, sceneobject->getdiffusecoefficient());

		vec_loc = _shader->getUniform("in_specularcoefficient");
		glUniform1f(vec_loc, sceneobject->getspecularcoefficient());

		vec_loc = _shader->getUniform("in_shininess");
		glUniform1f(vec_loc, sceneobject->getshininess());

		vec_loc = _shader->getUniform("in_tex");
		glActiveTexture(GL_TEXTURE0+sceneobject->gettex());
		glBindTexture(GL_TEXTURE_2D,sceneobject->gettex());
		glUniform1i(vec_loc, sceneobject->gettex());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		vec_loc = _shader->getUniform("in_bump");
		glActiveTexture(GL_TEXTURE0+sceneobject->getbump());
		glBindTexture(GL_TEXTURE_2D,sceneobject->getbump());
		glUniform1i(vec_loc, sceneobject->getbump());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		int dong = _scene->getsceneobjects()->at(i)->getGeometry()->getVertexCount()*4;

		glDrawArrays(GL_TRIANGLES, 0, dong);

		my_glBindVertexArray(0);

		_shader->unbind();
	}
	}

	//===== normal rendering
	// set rendering destination to FBO (if bloom is activated)
	if(config::bloom){
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	} else {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	if(config::transparency) glEnable(GL_BLEND);

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i=0; i<_scene->getsceneobjects()->size(); i++)
	{
		_shader->bind();

		sceneobject *sceneobject = _scene->getsceneobjects()->at(i);

		my_glBindVertexArray(sceneobject->getvao());

		GLint mat_loc = _shader->getUniform("transform_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*sceneobject->getMatrix()));

		mat_loc = _shader->getUniform("view_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(glm::inverse(*playercam->getposmatrix())));

		mat_loc = _shader->getUniform("transp_inv_transform_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(glm::transpose(glm::inverse(*sceneobject->getMatrix()))));

		mat_loc = _shader->getUniform("projection_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*playercam->getprojmatrix()));

		glm::mat4 m_t = glm::mat4(1,0,0,0,
								  0,1,0,0,
								  0,0,1,0,
								  1/2,1/2,1/2,1);
		glm::mat4 m_s = glm::mat4(1/2,0,0,0,
								  0,1/2,0,0,
								  0,0,1/2,0,
								  0,0,0,1);
		//glm::mat4 lightviewmat = glm::mul(m_t,glm::mul(m_s,*lightcam->getposmatrix()));
		glm::mat4 lightviewmat = *lightcam->getposmatrix();

		mat_loc = _shader->getUniform("lightmatrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(glm::inverse(*lightcam->getposmatrix())));

		mat_loc = _shader->getUniform("lightprojmatrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*lightcam->getprojmatrix()));

		mat_loc = _shader->getUniform("inverse_view_matrix");
		glUniformMatrix4fv(mat_loc, 
			1,
			GL_FALSE,
			glm::value_ptr(*playercam->getposmatrix()));

		GLint vec_loc = _shader->getUniform("in_lightcount");
		glUniform1f(vec_loc, lightcount);

		for(int i=0; i<lightcount; i++)
		{
			std::stringstream ss;
			ss<<i;
			vec_loc = (*_shader).getUniform("in_lightpos["+ss.str()+"]");
			glm::vec3 lightpos = glm::vec3(_scene->getlights()->at(i)->getposition()->x,
				_scene->getlights()->at(i)->getposition()->y,
				_scene->getlights()->at(i)->getposition()->z);
			glUniform3fv(vec_loc, 1, glm::value_ptr(lightpos));

			vec_loc = _shader->getUniform("in_lightintensity["+ss.str()+"]");
			glUniform1f(vec_loc, _scene->getlights()->at(i)->getintensity());

		}

		vec_loc = _shader->getUniform("in_lightintensity");
		glUniform1f(vec_loc, mainlight->getintensity());

		vec_loc = _shader->getUniform("in_ambientcoefficient");
		glUniform1f(vec_loc, sceneobject->getambientcoefficient());

		vec_loc = _shader->getUniform("in_diffusecoefficient");
		glUniform1f(vec_loc, sceneobject->getdiffusecoefficient());

		vec_loc = _shader->getUniform("in_specularcoefficient");
		glUniform1f(vec_loc, sceneobject->getspecularcoefficient());

		vec_loc = _shader->getUniform("in_shininess");
		glUniform1f(vec_loc, sceneobject->getshininess());

		vec_loc = _shader->getUniform("in_tex");
		glActiveTexture(GL_TEXTURE0+sceneobject->gettex());
		glBindTexture(GL_TEXTURE_2D,sceneobject->gettex());
		if(config::mipmode == MIPMODE_NONE)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		}
		else if(config::mipmode == MIPMODE_NEAREST)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		}
		else if(config::mipmode == MIPMODE_LINEAR)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		}
		if(config::texfilter)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		}
		else
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		glUniform1i(vec_loc, sceneobject->gettex());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		vec_loc = _shader->getUniform("in_bump");
		glActiveTexture(GL_TEXTURE0+sceneobject->getbump());
		glBindTexture(GL_TEXTURE_2D,sceneobject->getbump());
		if(config::mipmode == MIPMODE_NONE)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		}
		else if(config::mipmode == MIPMODE_NEAREST)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		}
		else if(config::mipmode == MIPMODE_LINEAR)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		}
		if(config::texfilter)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		}
		else
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		glUniform1i(vec_loc, sceneobject->getbump());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		if(config::shadow) {
		vec_loc = _shader->getUniform("in_shadowmap");
		glActiveTexture(GL_TEXTURE0+shadowmapping_depthtex);
		glBindTexture(GL_TEXTURE_2D,shadowmapping_depthtex);
		glUniform1i(vec_loc, shadowmapping_depthtex);
		}
		else
		{
		vec_loc = _shader->getUniform("in_shadowmap");
		glActiveTexture(GL_TEXTURE0+0);
		glBindTexture(GL_TEXTURE_2D,0);
		glUniform1i(vec_loc, 0);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		int dong = _scene->getsceneobjects()->at(i)->getGeometry()->getVertexCount()*4;

		glDrawArrays(GL_TRIANGLES, 0, dong);

		my_glBindVertexArray(0);

		_shader->unbind();
	}

	//======= post processing
	if(config::bloom){
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D,0);

		glBindTexture(GL_TEXTURE_2D, bloomtexid);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D,bloomtexid_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//set rendering destination to bloom filter fbo, generating the bloom highlight texture
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bloomfbo);
		glDisable(GL_DEPTH_TEST);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//pass 1 -> high brightness filter
		bloomfiltershader->bind();
		my_glBindVertexArray(bloomvao);

		GLint vec_loc = bloomfiltershader->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomtexid);
		glBindTexture(GL_TEXTURE_2D,bloomtexid);
		glUniform1i(vec_loc, bloomtexid);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D,0);
		glEnable(GL_DEPTH_TEST);
		my_glBindVertexArray(0);
		bloomfiltershader->unbind();

		glBindTexture(GL_TEXTURE_2D, bloomfiltertex);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//pass 2 -> horizontal blur
		bloomshader->bind();
		my_glBindVertexArray(bloomvao);

		vec_loc = bloomshader->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex);
		glUniform1i(vec_loc, bloomfiltertex);

		vec_loc = bloomshader->getUniform("tex2");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glUniform1i(vec_loc, bloomfiltertex_actual);									// jetzt kennt sich keiner mehr aus

		//blur image (set low mipmap)
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 2);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Reset mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);

		my_glBindVertexArray(0);
		bloomshader->unbind();

		glBindTexture(GL_TEXTURE_2D, bloomfiltertex);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual2);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//pass 3 -> vertical blur
		bloomshader_vert->bind();
		my_glBindVertexArray(bloomvao);

		vec_loc = bloomshader_vert->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glUniform1i(vec_loc, bloomfiltertex_actual);

		vec_loc = bloomshader_vert->getUniform("tex2");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual2);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual2);
		glUniform1i(vec_loc, bloomfiltertex_actual2);

		//blur image (set low mipmap)
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 2);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Reset mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);

		my_glBindVertexArray(0);
		bloomshader_vert->unbind();

		glBindTexture(GL_TEXTURE_2D, bloomfiltertex);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//pass 4 - put it all together
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mixtextures->bind();
		my_glBindVertexArray(bloomvao);

		vec_loc = mixtextures->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomtexid);
		glBindTexture(GL_TEXTURE_2D,bloomtexid);
		glUniform1i(vec_loc, bloomtexid);

		vec_loc = mixtextures->getUniform("tex2");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glUniform1i(vec_loc, bloomfiltertex_actual);

		//blur image (set low mipmap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Reset mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);

		my_glBindVertexArray(0);
		mixtextures->unbind();

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		//---->>>> set rendering destination back to window system FBO and render to a quad covering the screen
		//an diesem punkt:
		//fboID = urspruengliches bild
		//bloomtexid = ^ color attachment
		//bloomfbo = bloom textur
		//bloomfiltertex = ^ color attachment
		//bloomfiltertex_actual/2 = ^ kopien
		//mixtextures = shader zum texturen mixen
		//bloomshader/_vert = shader zum (horiz./vertikalen) verwischen von textur
		//bloomfiltershader = shader fuer high brightness pass einer textur
		//bloomvao = quad ueber ganzen bildschirm, weiss, mit tex.coords

		//pass 5 - radial blur drueberhauen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		radialblur->bind();
		my_glBindVertexArray(bloomvao);

		vec_loc = radialblur->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glUniform1i(vec_loc, bloomfiltertex_actual);

		vec_loc = radialblur->getUniform("speed");
		glUniform1f(vec_loc, playerspeed);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		my_glBindVertexArray(0);
		radialblur->unbind();

		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//abschluss - zeichnen in system fbo
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mixtextures->bind();
		my_glBindVertexArray(bloomvao);

		vec_loc = mixtextures->getUniform("tex");
		glActiveTexture(GL_TEXTURE0+bloomfiltertex_actual);
		glBindTexture(GL_TEXTURE_2D,bloomfiltertex_actual);
		glUniform1i(vec_loc, bloomfiltertex_actual);

		vec_loc = mixtextures->getUniform("tex2");
		glActiveTexture(GL_TEXTURE0+0);
		glBindTexture(GL_TEXTURE_2D,0);
		glUniform1i(vec_loc, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		my_glBindVertexArray(0);
		mixtextures->unbind();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void framebuffer::setgamelogic(gamelogic &gamelogic)
{
	_gamelogic = &gamelogic;
	_gamelogic->setWindow(window);
}

void framebuffer::updategamelogic()
{
	(*_gamelogic).update(_scene, _width, _height);
	playerspeed = _gamelogic->getspeed();
}
