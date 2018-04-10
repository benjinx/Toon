#version 330 core
// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	mat4 modelMat;
	vec4 lightDir;
	vec4 eyeDir;
} pass;

// Uniforms
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform float shininess;

uniform bool hasAmbient;
uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormal;

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
		ambient = ambientStrength * lightColor;
  	
    // diffuse 
    float diff = dot(L, N);
    
    // specular
	vec3 R = (diff + diff) * N - L;
    float spec = /*pow(*/max(dot(V, R), 0.0);//, shininess);
	spec *= spec;
	spec *= spec;
	spec *= spec;

	diff = max(diff, 0.0);
	vec3 diffuse;
	if (hasDiffuse)
		diffuse = diff * objectColor;
	else
		diffuse = diff * lightColor;

	vec3 specular;
	if (hasSpecular)
		specular = texture(specularTex, pass.texCoords).rgb * spec * lightColor;
	else
		specular = spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0);
}


//#version 410

//// Varyings
//in vertexData
//{
//	vec4 fragPos;
//	vec4 normal;
//	vec4 lightPos;
//	vec4 eyePos;
//	vec2 texCoords;
//	mat4 modelMat;
//} pass;

//// uniforms
//uniform sampler2D diffuseTex;
//uniform sampler2D bumpTex;
//uniform sampler2D specularTex;

//uniform vec3 ambient;
//uniform vec3 diffuse;
//uniform vec3 specular;
//uniform float shininess;

//// target
//layout (location = 0) out vec4 fragColor;

//void main()
//{
//	float specAmount = shininess;
//	if (specAmount <= 0)
//		specAmount = 32.0f;

//	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
//	vec4 norm = pass.modelMat * (texture(bumpTex, pass.texCoords) * 2.0 - 1.0);

//	float amb = 0.1;
//	//vec4 ambientF = vec4(ambient, 1.0f) * lightColor;
//	vec4 ambientF = amb * lightColor;

//	vec4 lightDir = normalize(pass.lightPos - pass.fragPos);
//	float diff = max(dot(norm, lightDir), 0.0);
//	vec4 diffuseF = texture(diffuseTex, pass.texCoords) * diff * lightColor;

//	vec4 viewDir = normalize(pass.eyePos - pass.fragPos);
//	vec4 halfwayDir = normalize(lightDir + viewDir);
//	float spec = pow(max(dot(norm, halfwayDir), 0.0), specAmount);
//	vec4 specularF = texture(specularTex, pass.texCoords) * spec * lightColor;

//	fragColor = vec4(ambientF.xyz + diffuseF.xyz + specularF.xyz, 1.0f);
//}