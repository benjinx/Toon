#version 330 core

struct Material {
    sampler2D diffuseMap;
};

struct DirLight {
    vec4 direction;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec4 direction;

    float cutoff;
    float outerCutoff;
};

struct LightCheck {
    bool Directional;
    bool Point;
    bool Spot;
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
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotlight;
uniform LightCheck lightCheck;

// Targets
layout (location = 0) out vec4 fragColor;

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(-light.direction.xyz);
    float diff = max(dot(pass.normal, lightDir), 0.0);

    // specular
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // combine results
    vec3 diffuse = diff * lightColor;
    vec3 specular = specularStrength * spec * lightColor;
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(light.position - pass.fragPos);
    float diff = max(dot(pass.normal, lightDir), 0.0);

    // specular
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // attenuation calculations
    float dist = length(light.position - pass.fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
                        light.quadratic * (dist * dist));

    // combine results
    vec3 diffuse = diff * lightColor;
    vec3 specular = specularStrength * spec * lightColor;
            
    // Apply attenuation
    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotlight(SpotLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 lightDir = normalize(light.position.xyz - pass.fragPos);
    float diff = max(dot(pass.normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;


    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    vec3 result;

    if (theta > light.cutoff)
    {
        result = (ambient + diffuse + specular) * lightColor;
    }
    else // use ambient light so scene isn't completely dark outside of spotlight.
    {
        //diffuse /= 8.0;
        result = (ambient + diffuse) * lightColor;
    }

    return result;
}

void main()
{

    // Setup
    vec3 normal = normalize(pass.normal);
    vec3 viewDir = normalize(eyePos.xyz - pass.fragPos);
    vec3 objectColor = texture(material.diffuseMap, pass.texCoords).rgb;

    // Directional Light
    vec3 result;
    if (lightCheck.Directional)
        result += CalculateDirectionalLight(dirLight, normal, viewDir);

    // Point Light
    if (lightCheck.Point)
        result += CalculatePointLight(pointLight, normal, viewDir);

    // Spot Light
    if (lightCheck.Spot)
        result += CalculateSpotlight(spotlight, normal, viewDir);

    // Apply the textures
    result *= objectColor;

    // Limit the amount because it's bright
    //result *= 0.75;

    fragColor = vec4(result, 1.0);
}
