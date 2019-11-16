#include <fstream>
#include "shader.h"

Shader::Shader(const string &path)
{

	string vertex_shader_src = read_file(path+".vert");

	string fragment_shader_src = read_file(path+".frag");

	_vertex_shader = compile(GL_VERTEX_SHADER, vertex_shader_src);
	_fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_shader_src);

	link();
}

string Shader::read_file(const string &file)
{
	std::ifstream ifile(file.c_str());
	return string(std::istreambuf_iterator<char>(ifile),std::istreambuf_iterator<char>());
}

GLuint Shader::compile(GLenum type, const string &source)
{
	GLuint shader = glCreateShader(type);	//erstelle ein shader objekt vom typ type, referenz kommt als int zurueck

		if (shader == 0) {
			cout << "Could not create shader object." << endl;
		return 0;
	}

	const char* src = source.data();	//character pointer auf diesen string

	int size = source.size();

	glShaderSource(shader,		//fuelle das angegebene shader objekt mit diesem sourcecode
		1,						
		&src,					//hier muss ein doppel-pointer her
		&size);

	glCompileShader(shader);	//kompiliere code im vertex/fragment prozessor

	int status;	//status check

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		cout << "Shader compilation failed: "<<status << endl;

		GLint blen = 1000;	
		GLsizei slen = 0;

		char compiler_log[1000];
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)
	    glfwGetProcAddress("glGetInfoLogARB");
	
		glGetInfoLogARB(shader, blen, &slen, compiler_log);
		cout << "compiler_log: " << compiler_log;

	}

	return shader;

}


void Shader::link()
{
	_program = glCreateProgram();	//erstelle program objekt. Shader wird hier dran gelinkt

	glAttachShader(_program, _vertex_shader);
	glAttachShader(_program, _fragment_shader);

	glLinkProgram(_program);

    GLint success;
    glGetProgramiv (_program, GL_LINK_STATUS, & success);

	if(success != GL_TRUE)
	{
		cout<<"Shader linking did not return true: "<<success<<endl;
	}
}

