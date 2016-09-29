#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 pixPos;  // Needed for specular reflections
in mat3 Mvt;
in vec3 viewDirection;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec3 viewDirectionTex = normalize(Mvt * viewDirection);

    vec3 light = Mvt*vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
	float bst = (texture(texUnit, outTexCoord+vec2(offset,offset)) - texture(texUnit, outTexCoord)).x;
	
	vec2 t0 = outTexCoord;
	vec2 tn = t0 - bst * vec2(viewDirectionTex.x, viewDirectionTex.y);///viewDirectionTex.z;

	float bs = (texture(texUnit, tn+vec2(offset,0)) - texture(texUnit, tn)).x;
	float bt = (texture(texUnit, tn+vec2(0,offset)) - texture(texUnit, tn)).x;

	vec3 normal = normalize(vec3(bs, bt, 1.0));

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4(dot(normal, light));
}
