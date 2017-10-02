#version 330 core
// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
} pass;

// Uniforms
uniform vec4 lightPos;
uniform vec4 eyePos;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 objectColor = vec3(0.0, 1.0, 0.0);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(pass.normal);
    vec3 lightDir = normalize(lightPos.xyz - pass.fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
}
