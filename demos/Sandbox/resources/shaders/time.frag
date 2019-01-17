#version 330 core

// Varyings
in vertexData
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} pass;

uniform float glfwtime;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 col = 0.5 + 0.5 * cos(glfwtime + pass.fragPos);

    fragColor = vec4(col, 1.0);
}
