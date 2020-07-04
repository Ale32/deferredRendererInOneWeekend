#version 410 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

in vec2 UV;


void main() {
    vec3 diffuseColor = texture(gAlbedo, UV).rgb;

    vec3 normal = texture(gNormal, UV).xyz;
    vec3 position = texture(gPosition, UV).xyz;
    
    FragColor = vec4(diffuseColor, 1.0);
}
