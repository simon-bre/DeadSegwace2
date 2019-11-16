

#version 150 core

#define MAXLIGHTS 2

uniform	mat4	transform_matrix;
uniform mat4	view_matrix;
uniform	mat4	projection_matrix;
uniform	mat4	transp_inv_transform_matrix;
uniform mat4	inverse_view_matrix;

uniform int		in_lightcount;
uniform	vec3	in_lightpos[MAXLIGHTS];
uniform	float	in_lightintensity[MAXLIGHTS];

uniform float 	in_ambientcoefficient;
uniform	float	in_diffusecoefficient;
uniform float	in_specularcoefficient;

in	vec4	in_position;
in	vec3	in_color;
in	vec3	in_normal;
in	vec2	in_texcoords;

out	vec3	ex_color;
out	vec4	ex_position;
out	vec3	ex_normal;
out	vec3	ex_lightpos[MAXLIGHTS];
out	float	ex_lightintensity[MAXLIGHTS];
out	float	ex_ambientcoefficient;
out	float	ex_diffusecoefficient;
out	float	ex_specularcoefficient;
out	int		ex_lightcount;
out	float	ex_shininess;
out	vec4	ex_camerapos;
out	vec2	ex_texcoords;

void main(void)
{
	vec4 pos =  transform_matrix * in_position * view_matrix;
	vec4 nor = transp_inv_transform_matrix * vec4(in_normal,1.0f);
	vec3 norr = normalize(vec3(nor.x,nor.y,nor.z));
	gl_Position = projection_matrix*pos;

	ex_color = in_color;
	ex_position = transform_matrix * in_position;
	ex_lightpos = in_lightpos;
	ex_normal = norr;
	ex_camerapos = vec4(0.0,0.0,0.0,1.0)*inverse_view_matrix;
	ex_lightintensity = in_lightintensity;
	ex_ambientcoefficient = in_ambientcoefficient;
	ex_diffusecoefficient = in_diffusecoefficient;
	ex_specularcoefficient = in_specularcoefficient;
	ex_lightcount = in_lightcount;
	ex_texcoords = in_texcoords;
}