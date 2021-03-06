#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

uniform mat4 viewMatrix;

in vec2 outTexCoord;
in vec3 Ps;
in vec3 Pt;
in vec3 out_Normal;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
	
	float derivativeS = (texture(texUnit, outTexCoord+vec2(offset, 0)) - texture(texUnit, outTexCoord)).x;
	float derivativeT = (texture(texUnit, outTexCoord+vec2(0, offset)) - texture(texUnit, outTexCoord)).x;
	vec3 normal = normalize( normalize(out_Normal) + derivativeS*normalize(Ps) + derivativeT*normalize(Pt) );

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4(dot(normal, light));
}
