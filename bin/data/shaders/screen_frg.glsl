#version 330 core

in vec2 uv;
in vec2 pos;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 color;

const float offset = 1.0 / 600.0;  

void main(void)
{
/*
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
	float kernel[9] = float[](
         1/16.0f,  2/16.0f,  1/16.0f,
         2/16.0f,  4/16.0f,  2/16.0f,
         1/16.0f,  2/16.0f,  1/16.0f
    );
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture0, uv + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
*/
	vec3 col = texture(texture0, uv).rgb;
	color.a = 1.0;
	
	color.rgb = pow(col, vec3(2.2, 2.2, 2.2));
}
