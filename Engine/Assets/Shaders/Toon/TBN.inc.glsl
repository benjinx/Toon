#ifndef TOON_TBN_INC_GLSL
#define TOON_TBN_INC_GLSL

mat3 CalculateTBN(mat4 model, vec4 normal, vec4 tangent)
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * tangent.xyz);
    vec3 N = normalize(normalMatrix * normal.xyz);

    // Gram-Schmidt process to re-orthogonalize the TBN vectors
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T) * tangent.w;

    return mat3(T, B, N);
}

#endif // TOON_TBN_INC_GLSL