#version 330 core

in vec2 uv;
in vec3 fragpos;
in vec3 normal;

uniform sampler2D texture0;
uniform float uvscale = 1.0f;
uniform vec3 lightpos = vec3(0.0, 4.0, 0.0);
uniform int onlycolor = 0;
uniform vec3 tintcolor = vec3(1.0, 1.0, 1.0);

out vec4 color;

void main(void)
{
	int pointLight = 0;
	vec3 norm     = normalize(normal);
	vec3 lightDir, diffuse;
	vec3 ambient = vec3(0.4, 0.4, 0.4);
	vec3 lightColor = vec3(0.6, 0.6, 0.6);
	float diff;
	
	if (pointLight)
		lightDir = normalize(lightpos - fragpos);
	else 
		lightDir = normalize(lightpos);
	
	diff    = max(dot(norm, lightDir.xyz), 0.0);
	diffuse = (diff * lightColor) + ambient;
	
	if (onlycolor)
	{
		color.rgb = tintcolor * diffuse;
	}
	else
	{
		vec4 colortexture = texture(texture0, vec2(uv.x * uvscale, -uv.y * uvscale));
		color.rgb = colortexture.rgb * diffuse;
	}
	color.a = 1.0;
}
