#version 410 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform sampler2D uShadowMap;
uniform mat4 uLigthMVP;

uniform vec3 uViewPosition;

struct SpotLight {
    float influence;
    vec3 position;
    vec3 color;

    vec3 direction;
    float cutOffValue;
};

uniform SpotLight spotLight;

float _calculateShadow(vec4 lightSpacePosition, vec3 normal, vec3 lightDir)
{
    vec3 shadowUV = lightSpacePosition.xyz / lightSpacePosition.w;
    shadowUV = shadowUV * 0.5 + 0.5;

    // Get the current depth from light's perspective
    float currentDepth = shadowUV.z;

    // Bias based on slope
    float cosTheta = clamp(dot(normal, -lightDir), 0.0, 1.0);
    float bias = 0.005 * tan(acos(cosTheta));
    bias = clamp(bias, 0.0, 0.00001);

    // Simplest PCF: this could be improved sampling with a better kernel
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(uShadowMap, shadowUV.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
} 

vec3 _calculateLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 radiance = vec3(0.0);

    vec3 lightDir = normalize(light.position - fragPos);
    float angle = dot(-lightDir, normalize(light.direction));

    if (angle > light.cutOffValue) {
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
        
        // Shadow
        vec4 posInLightSpace = uLigthMVP * vec4(fragPos, 1.0);
        float shadow = _calculateShadow(posInLightSpace, normal, lightDir);

        // Combine
        vec3 diffuse  = diff * light.color * albedoSpec.rgb;
        vec3 specular = spec * light.color;
        
        radiance = attenuation * vec3(1 - shadow) * (diffuse + specular);
    } else {
        // Hard coded ambient outside light cone
        radiance = vec3(0.05);
    }

    return radiance;
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
    vec3 radiance = _calculateLighting(spotLight, normal, position, viewDir);    
    
    //FragColor = vec4(vec3(pow(texture(uShadowMap, UV).r, 100.0)), 1.0);
    FragColor = vec4(radiance, 1.0);
}