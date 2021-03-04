#ifndef TOON_BLINN_PHONG_LIGHTING_INC_GLSL
#define TOON_BLINN_PHONG_LIGHTING_INC_GLSL

vec4 BlinnPhongLighting(
    vec4 LightColor,
    vec4 LightPosition,
    vec4 CameraPosition,
    vec4 Position,
    vec4 Normal
)
{
    // Ambient
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * LightColor;
      
    // Diffuse 
    vec4 norm = normalize(Normal);
    vec4 lightDir = normalize(LightPosition - Position);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * LightColor;

    // Specular
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

    // We dot the normal and lightDir to make sure it won't 'leak' through the model.
    if (dot(norm, lightDir) >= 0.0f) {
	    // specular
        float specularStrength = 0.5;
        vec4 viewDir = normalize(CameraPosition - Position);
        vec4 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
        specular = specularStrength * spec * LightColor;
    }

    return vec4(ambient.rgb + diffuse.rgb + specular.rgb, 1.0);
}

#endif // TOON_BLINN_PHONG_LIGHTING_INC_GLSL