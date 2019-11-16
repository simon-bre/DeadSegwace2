

#version 150 core

in	vec3	ex_color;
in	vec3	ex_normal;
in	vec4	ex_position;
in	vec3	ex_lightpos;

out	vec4	out_color;

void main(void)
{

	vec3 lightDirection = normalize(ex_lightpos-vec3(ex_position.x,ex_position.y,ex_position.z));

	float cos = dot(ex_normal, lightDirection);
	if(cos>0.0)
	{
		
		vec3 ambient = 0.1*ex_color;
		vec3 diffuse = ex_color *(0.9* 0.8* cos);
		
		vec3 camerapos = vec3(0,0,-1);
		vec3 vievVec = normalize(camerapos-vec3(ex_position.x,ex_position.y,ex_position.z));
		
		
		float cos2 = dot(ex_normal, normalize(lightDirection+vievVec));

		if(cos2>0.0){
			vec3 specular = vec3(1,1,1)*pow(cos2,10);
			out_color = vec4(ambient+diffuse+specular, 1.0);
		}
		else{
			out_color = vec4(ambient+diffuse, 1.0);
		}
	}
	else
	{
		out_color = vec4(0.1*ex_color, 1.0);
	}
}