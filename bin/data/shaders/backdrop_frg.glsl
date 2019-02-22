#version 330 core
in vec2 TexCoords;
varying vec2 position;
out vec4 color;

uniform vec3 colour_ceiling;
uniform vec3 colour_ground;
uniform sampler2D texture0;

void main()
{	
	if (position.y > 0.0)
	{
		color.rgb = colour_ceiling;
	}
	else
	{
		color.rgb = colour_ground;
	}
	float fog_c = 1.0/abs(pow(position.y, 0.9));
	color.rgba = vec4(color.r / fog_c, color.g / fog_c, color.b / fog_c, 1.0);
} 
