#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in int g_poly[3];
uniform mat4 matrix;
uniform mat4 matrices[10];


out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{
	vec3 position = vec3(0);
	for(int i = 0; i < 2; ++i )
	{	
		int bone = g_poly[i];
		float weight = in_TexCoord[bone];
		vec4 baseVert = vec4(in_Position, 1.0);
		vec4 transformedPoint = matrices[bone] * baseVert;
		vec3 contribution =  weight * vec3(transformedPoint);
		position += contribution;
	}




	// transformera resultatet med ModelView- och Projection-matriserna
	gl_Position = matrix * vec4(position, 1.0);

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

