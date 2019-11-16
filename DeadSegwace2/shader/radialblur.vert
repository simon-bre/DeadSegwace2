#version 150 core

in	vec4	in_position;
in	vec3	in_color;
in	vec2	in_texcoords;

out	vec2	uv;
out	vec3	ex_color;
out	vec2	ex_texcoords;

void main(void)
{
	gl_Position = in_position;

	ex_color = in_color;
	ex_texcoords = in_texcoords;
	uv = (vec2( gl_Position.x, gl_Position.y ) + vec2( 1.0 ) ) / vec2( 2.0 );
}