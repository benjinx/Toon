#version 330 core

// Varyings
in vertexData
{
	vec4 fragPos;
	vec4 normal;
	vec4 lightPos;
	vec4 eyePos;
	vec2 texCoords;
} pass;

// Uniforms
uniform sampler2D diffuseTex;

uniform float gammaAmount;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffTex = texture(diffuseTex, pass.texCoords);

	float amb = 0.1;
	vec4 ambient = amb * lightColor;

	vec4 lightDir = normalize(pass.lightPos - pass.fragPos);
	float diff = max(dot(pass.normal, lightDir), 0.0);
	vec4 diffuse = diff * diffTex * lightColor;

	vec4 viewDir = normalize(pass.eyePos - pass.fragPos);
	vec4 reflectDir = reflect(-lightDir, pass.normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec4 specular = spec * lightColor;

	float gamma = gammaAmount;
	fragColor = vec4(pow((ambient.xyz + diffuse.xyz + specular.xyz), vec3(gamma)), 1.0f);
}