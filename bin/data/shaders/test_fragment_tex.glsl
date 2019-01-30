#version 330 core

in vec2 uv;
in vec3 fragpos;
in vec3 normal;

uniform sampler2D texture0;

out vec3 color;

void main(void)
{
	vec3 lightpos = vec3(4.0, 4.0, -4.0);
	vec3 norm     = normalize(normal);
	vec3 lightDir = normalize(lightpos - fragpos);
	float diff    = max(dot(norm, lightDir), 0.0);
	vec3 diffuse  = diff * vec3(1.0, 1.0, 1.0);
	
	color = texture(texture0, uv).rgb * diffuse + vec3(0.05, 0.05, 0.2);;
}
