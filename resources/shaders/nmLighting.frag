#version 410

// Varyings
in vertexData
{
	vec4 fragPos;
	vec4 normal;
	vec4 lightPos;
	vec4 eyePos;
	vec2 texCoords;
	mat4 modelMat;
} pass;

// uniforms
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D specularTex;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
	float specAmount = shininess;
	if (specAmount <= 0)
		specAmount = 32.0f;

	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 norm = pass.modelMat * (texture(bumpTex, pass.texCoords) * 2.0 - 1.0);

	float amb = 0.1;
	//vec4 ambientF = vec4(ambient, 1.0f) * lightColor;
	vec4 ambientF = amb * lightColor;

	vec4 lightDir = normalize(pass.lightPos - pass.fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuseF = texture(diffuseTex, pass.texCoords) * diff * lightColor;

	vec4 viewDir = normalize(pass.eyePos - pass.fragPos);
	vec4 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), specAmount);
	vec4 specularF = texture(specularTex, pass.texCoords) * spec * lightColor;

	fragColor = vec4(ambientF.xyz + diffuseF.xyz + specularF.xyz, 1.0f);
}