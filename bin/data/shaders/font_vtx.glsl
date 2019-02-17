#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

out vec2 pos;

uniform mat4 proj;
uniform mat4 model;

void main()
{
	mat4 mvp    = proj * model;
	gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
	pos = gl_Position.xz;
}  
