#version 410 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform vec3 uViewPosition;

struct PointLight {
    float influence;
    vec3 position;
    vec3 color;
};

#define NR_POINT_LIGHTS 3  
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 _calculateLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec4 albedoSpec = texture(gAlbedo, UV);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), albedoSpec.a);
    
    // Attenuation
    float dist = length(light.position - fragPos);
    //float attenuation = dist > light.influence ? 0.0 : 1.0 / dist * dist;
    float attenuation = 1.0 / dist * dist;
    
    // Combine
    vec3 diffuse = diff * light.color * albedoSpec.rgb;
    vec3 specular = spec * light.color;
    
    return attenuation * (diffuse + specular);
}


void main()
{
    // Read GBuffer
    vec3 position = texture(gPosition, UV).xyz;

    vec3 normal = texture(gNormal, UV).xyz;
    normal = normalize(normal);

    vec3 diffuseColor = texture(gAlbedo, UV).rgb;    

    // Calculate camera component
    vec3 viewDir = normalize(uViewPosition - position);

    // Calculate lighting
    vec3 radiance = vec3(0.0);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        radiance += _calculateLighting(pointLights[i], normal, position, viewDir);    
    
    FragColor = vec4(radiance, 1.0);
}