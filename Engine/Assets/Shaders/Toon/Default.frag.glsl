#version 450 core

#include <Toon/ColorSpace.inc.glsl>
#include <Toon/Globals.inc.glsl>
#include <Toon/Material.inc.glsl>
#include <Toon/Math.inc.glsl>

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

const vec3 c_LightRadiance = vec3(1.0);
const vec3 c_EyePosition = vec3(2.0, 3.0, 2.0);
const vec3 c_LightDirection = -c_EyePosition;

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = M_PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float cosTheta, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float denom = cosTheta * (1.0 - k) + k;

    return cosTheta / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{
    vec3 N = (2.0 * texture(u_NormalMap, v_TexCoord).rgb - 1.0) * u_NormalScale;
    N = normalize(v_TBN * N);

    vec4 albedo = SRGBToLinear(texture(u_BaseColorMap, v_TexCoord)) * u_BaseColorFactor;
    vec3 metallicRoughness = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
    float roughness = metallicRoughness.g * u_RoughnessFactor;
    float metallic = metallicRoughness.b * u_MetallicFactor;

    roughness = clamp(roughness, 0.04, 1.0);
    metallic = clamp(metallic, 0.0, 1.0);

    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    vec3 V = (c_EyePosition - v_Position);

    // Calc relevant per-light variables
    vec3 DirectLighting = vec3(0.0);
    for (int i = 0; i < LightCount; ++i)
    {
        // Calculate per-light radiance
        vec3 L = normalize(Lights[i].Position - v_Position);
        vec3 H = normalize(V + L);
        float distance = length(Lights[i].Position - v_Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = vec3(1.0);//Lights[i].Color.rgb * attenuation;

        // vec3 L = -c_LightDirection;
        // vec3 H = normalize(V + L);

        // Cook-torrance brdf
        vec3 F  = fresnelSchlick(F0, max(dot(H, V), 0.0));
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);

        //vec3 kD = vec3(1.0) - F;
        //kD *= 1.0 - metallic;
        vec3 kD = mix(vec3(1.0) - F, vec3(0.0), metallic);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        // Add to outgoing radiance directLighting
        float NdotL = max(dot(N, L), 0.0);
        vec3 diffuse = kD * albedo.rgb / M_PI;
        DirectLighting += (diffuse + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo.rgb;
    vec3 color = ambient + DirectLighting;

    // Ambient Occlusion
    float AO = texture(u_OcclusionMap, v_TexCoord).r;
    color = mix(color, color * AO, u_OcclusionStrength);

    // Emissive
    vec3 emissiveLighting = SRGBToLinear(texture(u_EmissiveMap, v_TexCoord)).rgb * u_EmissiveFactor;
    color += emissiveLighting;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    o_Color = vec4(color, albedo.a);
}
