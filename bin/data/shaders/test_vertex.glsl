#version 330 core
layout(location = 0) in vec3 coord3d;
layout(location = 1) in vec2 uv2d;
layout(location = 2) in vec3 normal3d;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec3 fragpos;
out vec3 normal;

void main(void)
{
	mat4 mvp    = proj * view * model;
	gl_Position = mvp * vec4(coord3d, 1.0);
	fragpos     = vec3(model * vec4(coord3d, 1.0));;
	normal      = normal3d;
}
