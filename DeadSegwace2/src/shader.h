#ifndef SHADER_H
#define SHADER_H


#include "common.h"

class Shader
{
	GLuint _program;
	GLuint _vertex_shader;
	GLuint _fragment_shader;

	GLuint compile(GLenum type, const string &source);
	string read_file(const string &name);

	void link();

public:

	Shader(const string &path);
	

	void bind()
	{
		glUseProgram(_program);
	}
	
	void unbind()
	{
		glUseProgram(0);
	}

	void bindFragData(const string &name)	//in der variable mit dem namen "name" steht der farbwert pro pixel
	{
		glBindFragDataLocation(_program,0,name.c_str());
		link();
	}

	GLint getAttrib(const string &name)		//referenz fuer einen variablennamen im shader rausholen
	{
		return glGetAttribLocation(_program, name.c_str());
	}

	GLint getUniform(const string &name)	//gleiches fuer uniform variable
	{
		return glGetUniformLocation(_program, name.c_str());
	}


};


#endif