#version 330 core
in vec2 TexCoords;
in vec2 pos;
out vec4 color;

uniform float totaltime;
uniform vec3 tintcolor;
uniform int onlycolor;
uniform sampler2D texture0;

const float PI = 3.1415;
const float RAINBOW_SCALE = 32.0f;
const float RAINBOW_SPEED = 0.1f;

void main()
{    
	if (onlycolor)
	{
		vec4 sampled = texture(texture0, TexCoords);
		color        = vec4(tintcolor, 1.0) * sampled;
		color.a = sampled.r;
	}
	else
	{
		float movex = pos.x - totaltime * RAINBOW_SPEED;
		vec2 posn = vec2(movex, pos.y);
		
		float r = abs(sin(posn.x*RAINBOW_SCALE + 0));
		float g = abs(sin(posn.x*RAINBOW_SCALE + PI/4));
		float b = abs(sin(posn.x*RAINBOW_SCALE + 2*PI/4.0));
		vec4 textColor = vec4(r, g, b, 1.0);
		
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, TexCoords).r);
		
		color = textColor * sampled;
	}
} 
