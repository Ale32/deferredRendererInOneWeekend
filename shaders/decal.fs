#version 410 core
layout (location = 0) out vec4 gDiffuseSpec;

in vec2 UV;

uniform sampler2D gPosition;
uniform sampler2D uDecalDepthMap;

uniform mat4 uDecalViewMatrix;
uniform mat4 uDecalOrthoMatrix;

uniform sampler2D uDecalMap;
uniform float uDecalSpec;

void main()
{
    vec3 fragPos = texture(gPosition, UV).rgb;
    vec4 fragPosInDecalSpace = uDecalOrthoMatrix * uDecalViewMatrix * vec4(fragPos, 1.0);

    vec3 fragPosInDecalSpaceProj = fragPosInDecalSpace.xyz / fragPosInDecalSpace.w;
    fragPosInDecalSpaceProj = (fragPosInDecalSpaceProj + vec3(1.0)) / 2.0;

    float decalDepth = texture(uDecalDepthMap, fragPosInDecalSpaceProj.xy).r;

    if (fragPosInDecalSpaceProj.x > 0.0 && fragPosInDecalSpaceProj.x < 1.0
        && fragPosInDecalSpaceProj.y > 0.0 && fragPosInDecalSpaceProj.y < 1.0
        && fragPosInDecalSpaceProj.z > 0.0 && fragPosInDecalSpaceProj.z < 1.0
        && fragPosInDecalSpaceProj.z - 0.0005 < decalDepth ) {
            vec2 UV = vec2(fragPosInDecalSpaceProj.x, 1 - fragPosInDecalSpaceProj.y);
            gDiffuseSpec = vec4(texture(uDecalMap, UV).xyz, uDecalSpec);
    } else {
        discard;
    }
}