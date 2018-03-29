#version 330 core
// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	mat4 modelMat;
	vec3 lightPos;
	vec3 eyePos;
} pass;

// Uniforms
uniform vec4 lightPos;
uniform vec4 eyePos;
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(diffuseTex, pass.texCoords).rgb;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 norm = normalize(texture(bumpTex, pass.texCoords).rgb * 2.0 - 1.0);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;
  	
    // diffuse 
    //vec3 norm = normalize(pass.normal);
    vec3 lightDir = normalize(lightPos.xyz - pass.fragPos);
    float diff = dot(lightDir, pass.normal);
    
    // specular
    vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 R = reflect(-lightDir, pass.normal);
    float spec = max(dot(viewDir, R), 0.0);
	spec *= spec;
	spec *= spec;
	spec *= spec;

	diff = max(diff, 0.0);
	vec3 diffuse = diff * objectColor;

    vec3 specular = spec * lightColor;
        
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