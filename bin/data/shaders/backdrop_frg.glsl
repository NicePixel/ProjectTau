#version 330 core
in vec2 TexCoords;
varying vec2 position;
out vec4 color;

uniform sampler2D texture0;

void main()
{    
	vec4 color_ceiling = vec4(6.3/100.0, 6.9/100.0, 18.4/100.0, 1.0);
	vec4 color_ground  = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (position.y > 0.0)
	{
		color = color_ceiling;
	}
	else
	{
		color = color_ground;
	}
	float fog_c = 1.0/abs(pow(position.y, 0.4));
	color.rgb = vec3(color.r / fog_c, color.g / fog_c, color.b / fog_c);
} 
