/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

uniform vec3 colors[20];
uniform int numberOfLightSources;
uniform sampler2D inTexture;
	
in vec3 fragNormal;
in vec3 directionalLight[20];

in vec2 fragTexCoord;
in vec3 reflection[20];

out vec4 out_Color;



void main(void)
{
	vec3 normalizedNormal = normalize(fragNormal);

	float reflectionConstant = 0.5;
	float diffuseConstant = 0.5;

	vec3 diffuseSum = vec3(0,0,0);
	vec3 reflectionSum = vec3(0,0,0);
	vec3 ambient = vec3(0.1, 0.1, 0.1);


	for(int i = 0; i < numberOfLightSources; ++i)
	{
		diffuseSum += diffuseConstant * colors[i] * max(dot(normalize(directionalLight[i]), normalizedNormal), 0);
		reflectionSum += reflectionConstant * colors[i] * max(dot(normalize(reflection[i]), vec3(0,0,1)), 0);
	}

	
	vec3 colorSum = ambient + diffuseSum + reflectionSum;
	float maxColor = max(colorSum.r, max(colorSum.g, colorSum.b));

	out_Color = texture(inTexture, fragTexCoord);
	out_Color = out_Color/maxColor * vec4(colorSum, 1.0);
}


