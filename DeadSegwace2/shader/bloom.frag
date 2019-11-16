#version 150 core


in	vec3	ex_color;
in	vec2	ex_texcoords;

out	vec4	out_color;
uniform sampler2D tex;
uniform sampler2D tex2;

void main(void)
{
	vec4 texd = texture(tex,ex_texcoords);

	vec4 texd2 = vec4(0.0);
	float blurSize = 1.0/256.0;

	texd2 += texture(tex2, vec2(ex_texcoords.x - 5.0*blurSize, ex_texcoords.y))*0.03;
	texd2 += texture(tex2, vec2(ex_texcoords.x - 4.0*blurSize, ex_texcoords.y))*0.05;
	texd2 += texture(tex2, vec2(ex_texcoords.x - 3.0*blurSize, ex_texcoords.y))*0.09;
	texd2 += texture(tex2, vec2(ex_texcoords.x - 2.0*blurSize, ex_texcoords.y))*0.12;
	texd2 += texture(tex2, vec2(ex_texcoords.x - blurSize, ex_texcoords.y))*0.15;
	texd2 += texture(tex2,ex_texcoords)*0.16;
	texd2 += texture(tex2, vec2(ex_texcoords.x + blurSize, ex_texcoords.y))*0.15;
	texd2 += texture(tex2, vec2(ex_texcoords.x + 2.0*blurSize, ex_texcoords.y))*0.12;
	texd2 += texture(tex2, vec2(ex_texcoords.x + 3.0*blurSize, ex_texcoords.y))*0.09;
	texd2 += texture(tex2, vec2(ex_texcoords.x + 4.0*blurSize, ex_texcoords.y))*0.05;
	texd2 += texture(tex2, vec2(ex_texcoords.x + 5.0*blurSize, ex_texcoords.y))*0.03;

	out_color = (vec4(texd.x,texd.y,texd.z,1)+(0.62*vec4(texd2.x,texd2.y,texd2.z,1))) * vec4(ex_color,1.0);
}