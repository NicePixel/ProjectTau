#version 330 core
layout(location = 0) in vec4 coord_uv;

out vec2 uv;
out vec2 pos;

uniform vec2 pos2d;
uniform vec2 scale2d;
uniform vec2 uvscale;

void main(void)
{
	gl_Position = vec4(coord_uv.x * scale2d.x + pos2d.x, coord_uv.y * scale2d.y + pos2d.y, 0.0, 1.0);
	uv          = coord_uv.zw * uvscale;
	pos = vec2(coord_uv.x, coord_uv.y);
}
