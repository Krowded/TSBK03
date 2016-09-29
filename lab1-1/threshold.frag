#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec4 pixel = 3*texture(texUnit, outTexCoord);

	out_Color = max(pixel-1.0, 0.0);
}
