#version 330 core

struct Material {
	vec3 diffuse;
	sampler2D diffuseMap;
	sampler2D normalMap;
	bool hasDiffuseMap;
	bool hasNormalMap;
};

// Uniforms
uniform Material material;
uniform vec3 lightColor;

// Varyings
in vertexData
{
	vec3 fragPos;
	vec2 texCoords;
	vec3 lightDir;
	vec3 eyeDir;
	vec3 normal;
} pass;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuseMap, pass.texCoords).rgb;

	// ambient
    float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * objectColor;
  	
    // diffuse
	vec3 N;
	if (material.hasNormalMap)
		N = normalize(texture(material.normalMap, pass.texCoords).rgb * 2.0 - 1.0);
	else
		N = pass.normal;

	vec3 L = pass.lightDir;
    float diff = max(dot(N, L), 0.0);
	vec3 diffuse;
	if (material.hasDiffuseMap)
		diffuse = diff * objectColor;
	else
		diffuse = diff * lightColor;

    // specular
	//float specularStrength = 0.5;
	vec3 V = pass.eyeDir;
	vec3 halfwayDir = normalize(L + V);
    float spec = pow(max(dot(N, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;
        
    vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}