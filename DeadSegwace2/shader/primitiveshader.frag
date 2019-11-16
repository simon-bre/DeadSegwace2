

#version 150 core

#define MAXLIGHTS 2

in	vec3	ex_color;
in	vec3	ex_normal;
in	vec4	ex_position;
in	vec4	ex_camerapos;

in	vec3	ex_lightpos[MAXLIGHTS];
in	float	ex_lightintensity[MAXLIGHTS];
in	int		ex_lightcount;

in	float	ex_ambientcoefficient;
in	float	ex_diffusecoefficient;
in	float	ex_specularcoefficient;
uniform float	in_shininess;

in	vec2	ex_texcoords;
uniform	sampler2D in_tex;
uniform sampler2D in_bump;

out	vec4	out_color;


void main(void)
{
	vec4 tex = (texture(in_tex,ex_texcoords));
	vec3 texcolor = vec3(tex.x,tex.y,tex.z);
	
	if(texcolor.x==0 && texcolor.y==0 && texcolor.z==0){	//transparenz bei schwarzer textur
		out_color=vec4(0,0,0,0);
		return;
	}

	vec3 temp_color = vec3(0.0,0.0,0.0);
	vec4 normal = texture(in_bump,ex_texcoords);
	vec3 nnormal = (vec3(normal.x,normal.y,normal.z)-0.5)/2;
	
	vec4 v = normalize(ex_camerapos - ex_position);
	vec3 vv = vec3(v.x,v.y,v.z);
	int i=0;
	vec4 lighti = vec4(ex_lightpos[i],1.0)-ex_position;
	vec3 lighth = vec3(lighti.x,lighti.y,lighti.z);
	vec3 r = normalize(normalize(lighth)+vv);
	float cosn = pow(dot(r,normalize(nnormal+ex_normal)),in_shininess);
	
	
	temp_color = ex_ambientcoefficient*texcolor*ex_color;
	if(dot(normalize(nnormal+ex_normal), normalize(ex_lightpos[i]-vec3(ex_position.x,ex_position.y,ex_position.z)))>0.0)
	{
		temp_color = temp_color+ ex_color*texcolor *((ex_lightintensity[i] * ex_diffusecoefficient * dot(normalize(nnormal+ex_normal), normalize(ex_lightpos[i]-vec3(ex_position.x,ex_position.y,ex_position.z)))));
		if(cosn>0.0)
		{
			temp_color = temp_color + ex_lightintensity[i]*ex_specularcoefficient*vec3(1.0,1.0,1.0)*cosn;
		}
	}
	/*i++;

	//light 2
	lighti = vec4(ex_lightpos[i],1.0)-ex_position;
	lighth = vec3(lighti.x,lighti.y,lighti.z);
	r = normalize(normalize(lighth)+vv);
	cosn = pow(dot(r,normalize(nnormal+ex_normal)),in_shininess);
		if(dot(normalize(nnormal+ex_normal), normalize(ex_lightpos[i]-vec3(ex_position.x,ex_position.y,ex_position.z)))>0.0)
	{
		temp_color += vec3(1, 1, 1)*ex_color*texcolor *((ex_lightintensity[i] * ex_diffusecoefficient * dot(normalize(nnormal+ex_normal), normalize(ex_lightpos[i]-vec3(ex_position.x,ex_position.y,ex_position.z)))));
		//if(cosn>0.0)
		//{
		//	temp_color = temp_color + ex_lightintensity[i]*ex_specularcoefficient*vec3(1.0,1.0,1.0)*cosn;
		//}
	}
	*/


	out_color = vec4(temp_color,1);
}