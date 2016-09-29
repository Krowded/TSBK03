#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
	
	sum += 1.0*textureOffset(texUnit, outTexCoord, ivec2(-2, 0));
	sum += 2.0*textureOffset(texUnit, outTexCoord, ivec2(0, 0));
	sum += 1.0*textureOffset(texUnit, outTexCoord, ivec2(2, 0));

	sum /= 4.0;

    out_Color = sum;
}
