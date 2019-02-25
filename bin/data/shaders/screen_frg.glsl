#version 330 core

in vec2 uv;
in vec2 pos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform int istext;
uniform vec3 tintcolor = vec3(1.0f, 0.0f, 1.0f);

out vec4 color;

const float offset = 1.0 / 600.0;  
const float gamma  = 1.5;

vec3 postprocessing(void)
{
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
	
    vec3 output_color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        output_color += sampleTex[i] * kernel[i];
		
	return output_color;
}

void main(void)
{
	vec4 col = texture(texture0, uv);
	if (istext > 0)
	{
		color.a = col.r; // Font uses only R channel.
		col.rgb = tintcolor;
	}
	else
	{
		color.a = 1.0;
	}
	
	color.rgb = pow(col.rgb, vec3(gamma, gamma, gamma));
}
