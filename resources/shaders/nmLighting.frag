#version 330 core
// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 lightDir;
	vec4 eyeDir;
} pass;

// Uniforms
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform float shininessAmount;

uniform bool hasAmbient;
uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormal;

uniform vec3 ambientAmount;
uniform vec3 diffuseAmount;
uniform vec3 specularAmount;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(diffuseTex, pass.texCoords).rgb;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 N;
	if (hasNormal)
		N = normalize(texture(normalTex, pass.texCoords).rgb * 2.0 - 1.0);
	else
		N = normalize(pass.normal);

	vec3 L = normalize(pass.lightDir.xyz);
	vec3 V = normalize(pass.eyeDir.xyz);

	// ambient
    float ambientStrength = 0.1;
	vec3 ambient;
	if (hasAmbient)
		ambient = ambientStrength * objectColor;
	else
		ambient = ambientStrength * ambientAmount;
  	
    // diffuse 
    float diff = dot(L, N);
    
    // specular
	vec3 R = (diff + diff) * N - L;

	float shininess = shininessAmount;
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
		diffuse = diff * diffuseAmount;

	vec3 specular;
	if (hasSpecular)
		specular = texture(specularTex, pass.texCoords).rgb * spec * lightColor;
	else
		specular = spec * specularAmount;
        
    vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0);
}