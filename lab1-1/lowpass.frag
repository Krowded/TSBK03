#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec4 sum = vec4(0,0,0,0);


	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(-2, -2));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-1, -2));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(0, -2));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(1, -2));
	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(2, -2));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-2, -1));
	sum += 16*textureOffset(texUnit, outTexCoord, ivec2(-1, -1));
	sum += 24*textureOffset(texUnit, outTexCoord, ivec2(0, -1));
	sum += 16*textureOffset(texUnit, outTexCoord, ivec2(1, -1));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(2, -1));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(-2, 0));
	sum += 24*textureOffset(texUnit, outTexCoord, ivec2(-1, 0));
	sum += 36*textureOffset(texUnit, outTexCoord, ivec2(0, 0));
	sum += 24*textureOffset(texUnit, outTexCoord, ivec2(1, 0));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(2, 0));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-2, 1));
	sum += 16*textureOffset(texUnit, outTexCoord, ivec2(-1, 1));
	sum += 24*textureOffset(texUnit, outTexCoord, ivec2(0, 1));
	sum += 16*textureOffset(texUnit, outTexCoord, ivec2(1, 1));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(2, 1));
	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(-2, 2));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(-1, 2));
	sum += 6*textureOffset(texUnit, outTexCoord, ivec2(0, 2));
	sum += 4*textureOffset(texUnit, outTexCoord, ivec2(1, 2));
	sum += 1*textureOffset(texUnit, outTexCoord, ivec2(2, 2));

	sum /= 256;

    out_Color = sum;
}
