

#version 150 core

uniform	mat4	transform_matrix;
uniform	vec3	in_lightpos;
uniform	mat4	projection_matrix;

in	vec4	in_position;
in	vec3	in_color;
in	vec3	in_normal;

out	vec3	ex_color;
out vec3	ex_normal;
out vec4	ex_position;
out vec3	ex_lightpos;

void main(void)
{
	vec4 pos = transform_matrix * in_position;
	vec4 nor = transform_matrix * vec4(in_normal,1.0f);
	vec3 norr = vec3(nor.x,nor.y,nor.z);
	gl_Position = projection_matrix*pos;
	
	ex_color=in_color;
	ex_normal=norr;
	ex_position=pos;
	ex_lightpos=in_lightpos;

}