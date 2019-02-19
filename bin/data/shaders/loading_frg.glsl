#version 330 core

in vec2 uv;
in vec2 pos;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 color;

void main(void)
{
	color.rgba = texture(texture0, vec2(uv.x, -uv.y)).rgba;
}
