#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec4 sum = vec4(0,0,0,0);

	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(-32, -32));
	sum += 2*textureOffset(texUnit, outTexCoord, ivec2(-16, -16));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-8, -8));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(-4, -4));
	sum += 8*textureOffset(texUnit, outTexCoord, ivec2(-2, -2));
	sum += 12*textureOffset(texUnit, outTexCoord, ivec2(0, 0));
	sum += 8*textureOffset(texUnit, outTexCoord, ivec2(2, 2));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(4, 4));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(8, 8));
	sum += 2*textureOffset(texUnit, outTexCoord, ivec2(16, 16));
	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(32, 32));

	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(-32, 32));
	sum += 2*textureOffset(texUnit, outTexCoord, ivec2(-16, 16));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-8, 8));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(-4, 4));
	sum += 8*textureOffset(texUnit, outTexCoord, ivec2(-2, 2));

	sum += 8*textureOffset(texUnit, outTexCoord, ivec2(2, -2));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(4, -4));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(8, -8));
	sum += 2*textureOffset(texUnit, outTexCoord, ivec2(16, -16));
	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(32, -32));

	
	sum /= 96;

	out_Color = sum;
}