#version 410 core
layout (location = 0) out vec4 gDiffuseSpec;
layout (location = 1) out vec3 gPosition;
layout (location = 2) out vec3 gNormal;

in vec3 vFragPos;
in vec3 vNormal;

uniform vec3 uColor;
uniform float uSpecular;

void main()
{
    gNormal = normalize(vNormal);
    gPosition = vFragPos;
    gDiffuseSpec.rgb = uColor;
    gDiffuseSpec.a = uSpecular;
}  