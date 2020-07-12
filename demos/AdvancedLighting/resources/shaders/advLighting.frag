#version 330 core

struct Material {
    sampler2D diffuseMap;
};

// Varyings
in vertexData
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} pass;

// Uniforms
uniform vec3 lightColor;
uniform vec4 lightVec;
uniform vec4 eyePos;

uniform Material material;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 objectColor = texture(material.diffuseMap, pass.texCoords).rgb;

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
      
    // diffuse 
    vec3 norm = normalize(pass.normal);
    vec3 lightDir = normalize(lightVec.xyz - pass.fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 specular = vec3(0.0, 0.0, 0.0);

    // We dot the normal and lightDir to make sure it won't 'leak'
    if (dot(norm, lightDir) >= 0.0f) {
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
        specular = specularStrength * spec * lightColor;
    }
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}
