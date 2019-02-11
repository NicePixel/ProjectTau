#version 330 core
layout(location = 0) in vec4 coord_uv;

out vec2 uv;
out vec2 pos;

void main(void)
{
	gl_Position = vec4(coord_uv.x/9.0f, coord_uv.y/8.0f, 0.0, 1.0);
	uv          = coord_uv.zw;
	pos = vec2(coord_uv.x, coord_uv.y);
}
