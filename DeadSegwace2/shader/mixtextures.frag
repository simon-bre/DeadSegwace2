
#version 150 core


in	vec3	ex_color;
in	vec2	ex_texcoords;

out	vec4	out_color;
uniform sampler2D tex;
uniform sampler2D tex2;

void main(void)
{
	vec4 texd = texture(tex,ex_texcoords);

	vec4 texd2 = texture(tex2,ex_texcoords);

	out_color = (vec4(texd.x,texd.y,texd.z,1)+(0.70*(vec4(texd2.x,texd2.y,texd2.z,1)))) * vec4(ex_color,1.0);
}
