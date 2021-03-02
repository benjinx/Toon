#version 450 core

#include <Toon/Globals.inc.glsl>

layout (location = 0) in vec4 v_Position;
layout (location = 1) in vec4 v_Normal;

// uniform vec4 eyePos;
// uniform vec4 lightPos;
// uniform vec3 objectColor;
// uniform vec3 lightColor;

// Targets
layout (location = 0) out vec4 o_Color;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * Lights[0].Color;
      
    // Diffuse 
    vec4 norm = normalize(v_Normal);
    vec4 lightDir = normalize(Lights[0].Position - v_Position);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * Lights[0].Color;

    // Specular
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

    // We dot the normal and lightDir to make sure it won't 'leak'
    if (dot(norm, lightDir) >= 0.0f) {
	    // specular
        float specularStrength = 0.5;
        vec4 viewDir = normalize(CameraPosition - v_Position);
        vec4 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        specular = specularStrength * spec * Lights[0].Color;
    }

    // Show our results
    o_Color = vec4(ambient.rgb + diffuse.rgb + specular.rgb, 1.0); // * objectColor;
}
