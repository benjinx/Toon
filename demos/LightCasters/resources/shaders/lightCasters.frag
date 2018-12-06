#version 330 core

struct Material {
	sampler2D diffuse;
};

struct Light {
	vec4 direction;
	vec3 position;

	//vec3 ambient;
	//vec3 diffuse;
	//vec3 specular;

	// Point Lighting
	float constant;
	float linear;
	float quadratic;

	// Spotlight Lighting
	float cutoff;
	float outerCutoff;
};

// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} pass;

// Uniforms
uniform vec3 lightColor;
uniform vec4 eyePos;

uniform Material material;
uniform Light light;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuse, pass.texCoords).rgb;

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(pass.normal);
	vec3 lightDir;
	if (light.direction.w == 0.0) // do directional lighting
		lightDir = normalize(-light.direction.xyz);
	else if (light.direction.w == 1.0) // do normal lighting
		lightDir = normalize(light.position.xyz - pass.fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// attenuation calculations
	float dist = length(light.position - pass.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist +
						light.quadratic * (dist * dist));
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
        
	// Apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// Spotlight
	float theta = dot(lightDir, normalize(-light.direction.xyz));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	if (theta > light.cutoff)
	{
		vec3 result = (ambient + diffuse + specular) * objectColor;
		fragColor = vec4(result, 1.0);
	}
	else // use ambient light so scene isn't completely dark outside of spotlight.
	{
		diffuse /= 8.0;
		vec3 result = (ambient + diffuse) * objectColor;
		fragColor = vec4(result, 1.0);
	}
}
