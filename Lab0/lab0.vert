/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;

uniform int numberOfLightSources;
uniform vec3 lightSources[20];
uniform bool isDirectional[20];

out vec3 fragNormal;
out vec3 directionalLight[20];
out vec2 fragTexCoord;
out vec3 reflection[20];

void main(void)
{
	fragTexCoord = in_TexCoord;

	mat3 rotationMatrix = mat3(worldMatrix);
	vec4 worldPosition = worldMatrix*vec4(in_Position, 1.0);

	fragNormal = mat3(worldMatrix)*in_Normal;
	gl_Position=projectionMatrix*viewMatrix*worldMatrix*vec4(in_Position, 1.0);

	for(int i = 0; i < numberOfLightSources; ++i) 
	{
		if(isDirectional[i])
		{
			directionalLight[i] = lightSources[i];
		}
		else
		{
			directionalLight[i] = lightSources[i] - vec3(worldPosition);
		}
		reflection[i] = directionalLight[i] - 2*dot(directionalLight[i], fragNormal)*fragNormal;
	}
}