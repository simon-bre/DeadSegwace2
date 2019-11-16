
#version 150 core


in	vec3	ex_color;
in	vec2	ex_texcoords;
in	vec2	uv;

out	vec4	out_color;
uniform sampler2D tex;
uniform	float	speed;



const float sampleDist = 0.5;		//1
const float sampleStrength = 3.0;	//2.2

void main(void)
{
	vec4 texd = texture(tex,ex_texcoords);

	float samples[10] = float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);

	vec2 dir = vec2(0.5) - uv; 
	
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
	dir = dir/dist; //normalisieren

	vec4 rad = texd;

	for(int i=0;i<10;i++)
	{						//entlang der richtung zur mitte 11 samples nehmen, um const faktor und samplefaktor verringern
		rad+= texture(tex, uv+dir*samples[i]*sampleDist);
	}

	rad = rad/11.0;

	float t = 1*dist * sampleStrength;	//0.98
	t = clamp(t,0.0,1.0);
	t = t * 2.6 * speed;
	t = clamp(t,0.0,1.0);
	t = t*t;

	out_color = mix(vec4(texd.x,texd.y,texd.z,1), rad, t) * vec4(ex_color,1.0);
}