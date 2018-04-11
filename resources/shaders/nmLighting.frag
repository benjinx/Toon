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

// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 lightDir;
	vec4 eyeDir;
} pass;


// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuse, pass.texCoords).rgb;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 N;
	if (hasNormal)
		N = normalize(texture(material.normal, pass.texCoords).rgb * 2.0 - 1.0);
	else
		N = normalize(pass.normal);

	vec3 L = normalize(pass.lightDir.xyz);
	vec3 V = normalize(pass.eyeDir.xyz);

	// ambient
    float ambientStrength = 0.1;
	vec3 ambient;
	//if (hasAmbient)
		ambient = ambientStrength * objectColor;
	//else
		//ambient = ambientStrength * ambientAmount;
  	
    // diffuse 
    float diff = dot(L, N);
    
    // specular
	vec3 R = (diff + diff) * N - L;

	float shininess = material.shininess;
	if (shininess <= 1.0)
		shininess = 32.0;

    float spec = pow(max(dot(V, R), 0.0), shininess);
	//spec *= spec;
	//spec *= spec;
	//spec *= spec;

	diff = max(diff, 0.0);
	vec3 diffuse;
	if (hasDiffuse)
		diffuse = diff * objectColor;
	else
		diffuse = diff * material.diffuseVal;

	vec3 specular;
	if (hasSpecular)
		specular = texture(material.specular, pass.texCoords).rgb * spec * lightColor;
	else
		specular = spec * material.specularVal;
        
    vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0);
}