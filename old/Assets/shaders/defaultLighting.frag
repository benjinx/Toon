#version 330 core

struct Material {
    vec3 diffuse,
         emissive;

    float metallic,
          roughness,
          normalScale,
          occlusionStrength;
};

// Varyings
in vertexData
{
    vec3 fragPos;
    vec3 normal;
} pass;

// Uniforms
uniform vec4 eyePos;
uniform vec4 lightVec;
uniform vec3 lightColor;

uniform Material material;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{

    vec3 lc = vec3(1.0, 1.0, 1.0);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
      
    // diffuse 
    vec3 norm = normalize(pass.normal);
    vec3 lightDir = normalize(lightVec.xyz - pass.fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse;
    
    // specular
    vec3 specular = vec3(0.0, 0.0, 0.0);

    // We dot the normal and lightDir to make sure it won't 'leak'
    if (dot(norm, lightDir) >= 0.0f) {
        float specularStrength = 0.5;
        vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        specular = specularStrength * spec * lightColor;
    }

    vec3 result = (ambient + diffuse + specular) * material.diffuse;
    fragColor = vec4(result, 1.0);
}
