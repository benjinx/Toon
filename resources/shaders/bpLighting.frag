#version 330 core
// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} pass;

// Uniforms
uniform vec4 lightPos;
uniform vec4 eyePos;
uniform sampler2D diffuseTex;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(diffuseTex, pass.texCoords).rgb;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;
  	
    // diffuse 
    vec3 norm = normalize(pass.normal);
    vec3 lightDir = normalize(lightPos.xyz - pass.fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0);
}
