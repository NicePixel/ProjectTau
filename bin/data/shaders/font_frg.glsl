#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture0;

void main()
{    
	vec4 textColor = vec4(0.0, 0.0, 1.0-TexCoords.x, 1.0);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, TexCoords).r);
    color = textColor * sampled;
} 
