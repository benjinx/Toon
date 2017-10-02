#version 330 core

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
uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D bumpTex;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 lightColor = texture(diffuseTex, pass.texCoords).rgb;
	//vec3 norm = normalize(pass.normal.xyz);
	vec3 norm = normalize(mat3(transpose(inverse(pass.modelMat))) * (texture(bumpTex, pass.texCoords).rgb * 2.0 - 1.0));
	vec3 lightDir = normalize(pass.lightPos.xyz);
	vec3 viewDir = normalize(pass.eyePos.xyz);

	// Ambient
	vec3 ambient = 0.1 * lightColor;
	//vec4 ambient = texture(ambientTex, pass.texCoords) * lightColor;

	// Diffuse
	float diff = dot(lightDir, norm);

	// Specular
	//vec3 R = (diff + diff) * norm - lightDir;
	vec3 R = reflect(-lightDir, norm);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = max(dot(viewDir, R), 0.0);
	spec *= spec;
	spec *= spec;
	spec *= spec;

	diff = max(diff, 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 specular = texture(specularTex, pass.texCoords).rgb * spec;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}