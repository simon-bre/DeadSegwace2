#version 150 core


in	vec3	ex_color;
in	vec2	ex_texcoords;

out	vec4	out_color;
uniform sampler2D tex;

void main(void)
{
	vec4 texd = texture(tex,ex_texcoords);

	vec4 col = (vec4(texd.x,texd.y,texd.z,1)) * vec4(ex_color,1.0);

	float check = (col.x + col.y + col.z)/3;
	float border = 0.22;
	if(check < border)
	{
		col = vec4(0,0,0,0);
	}
	else
	{
		col = col - border;
	}

	out_color = col;
}