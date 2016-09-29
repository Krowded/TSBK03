#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec3 Vs;
in vec3 Vt;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 outTexCoord;
out mat3 Mvt;
out vec3 pixPos;  // Needed for specular reflections
out vec3 viewDirection;

void main(void)
{
    outTexCoord = in_TexCoord;
    pixPos = vec3(viewMatrix * vec4(in_Position, 1.0));

    vec3 Ps = normalize( mat3(viewMatrix) * Vs );
    vec3 Pt = normalize( mat3(viewMatrix) * Vt );
    vec3 out_Normal = normalize(mat3(viewMatrix) * in_Normal);
    Mvt = transpose(mat3(Ps,Pt,out_Normal));

    viewDirection = vec3(viewMatrix * vec4(in_Position, 1.0));

    gl_Position = projMatrix * viewMatrix * vec4(in_Position, 1.0);
}
