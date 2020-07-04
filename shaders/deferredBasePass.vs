#version 410 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;


void main()
{
    vFragPos = vec3(uModel * vec4(inPosition, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * normalize(inNormal);
    vNormal = normalize(vNormal);

    gl_Position = uProjection * uView * vec4(vFragPos, 1.0);
}