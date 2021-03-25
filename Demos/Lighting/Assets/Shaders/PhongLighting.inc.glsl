#ifndef TOON_PHONG_LIGHTING_INC_GLSL
#define TOON_PHONG_LIGHTING_INC_GLSL

vec4 PhongLighting(
    vec3 LightColor,
    vec3 LightPosition,
    vec3 CameraPosition,
    vec3 Position,
    vec3 Normal
)
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColor;
      
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPosition - Position);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;

    // Specular
    vec3 specular = vec3(0.0, 0.0, 0.0);

    // We dot the normal and lightDir to make sure it won't 'leak' through the model.
    if (dot(norm, lightDir) >= 0.0f) {
	    // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(CameraPosition - Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        specular = specularStrength * spec * LightColor;
    }

    return vec4(ambient.rgb + diffuse.rgb + specular.rgb, 1.0);
}

#endif // TOON_PHONG_LIGHTING_INC_GLSL