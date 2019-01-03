#version 330 core

struct Material {
	vec3 ambientVal;
	vec3 diffuseVal;
	vec3 specularVal;
	float shininess;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
};

// Uniforms
uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormal;

uniform Material material;


uniform vec3 lightColor;

// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec3 lightDir;
	vec3 eyeDir;
} pass;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuse, pass.texCoords).rgb;

	vec3 L = pass.lightDir;
	vec3 V = pass.eyeDir;

	// ambient
    float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * objectColor;
  	
    // diffuse 
	vec3 N;
	if (hasNormal)
		N = normalize(texture(material.normal, pass.texCoords).rgb * 2.0 - 1.0);
	//else
	//	N = normalize(pass.normal);

    float diff = max(dot(N, L), 0.0);

    vec3 diffuse;
	//if (hasDiffuse)
		diffuse = diff * objectColor;
	//else
	//	diffuse = diff * material.diffuseVal * lightColor;

    // specular
	float shininess = material.shininess;
	//if (shininess <= 1.0)
		shininess = 32.0;

	vec3 halfwayDir = normalize(L + V);
    float spec = pow(max(dot(N, halfwayDir), 0.0), shininess);

	vec3 specular;
	if (hasSpecular)
		specular = texture(material.specular, pass.texCoords).rgb * spec;
	//else
	//	specular = spec * material.specularVal * lightColor;
        
    vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0);
}