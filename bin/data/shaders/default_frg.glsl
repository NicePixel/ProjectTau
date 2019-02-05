#version 330 core

in vec2 uv;
in vec3 fragpos;
in vec3 normal;

uniform sampler2D texture0;
uniform float uvscale = 1.0f;
uniform int onlycolor = 0;
uniform vec3 tintcolor = vec3(1.0, 1.0, 1.0);

out vec4 color;

void main(void)
{
	if (onlycolor)
	{
		color.rgb = tintcolor;
	}
	else
	{
		vec4 colortexture = texture(texture0, vec2(uv.x * uvscale, -uv.y * uvscale));
		color.rgb = colortexture.rgb;
	}
	color.a = 1.0;
}
