#version 450 core

#include <Toon/Globals.inc.glsl>

#include <PhongLighting.inc.glsl>
#include <BlinnPhongLighting.inc.glsl>

layout (binding = 0) uniform sampler2D baseColor; // glTexture0

layout (location = 0) in vec4 v_Position;
layout (location = 1) in vec4 v_Normal;
layout (location = 5) in vec2 v_TexCoords;

// Targets
layout (location = 0) out vec4 o_Color;

void main()
{
    vec4 baseColorTexture = texture(baseColor, v_TexCoords);

    vec4 outputColor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < LightCount; i++)
    {
        // outputColor += PhongLighting(
        //     Lights[i].Color,
        //     Lights[i].Position,
        //     CameraPosition,
        //     v_Position,
        //     v_Normal
        // );

        outputColor += BlinnPhongLighting(
            Lights[i].Color,
            Lights[i].Position,
            CameraPosition,
            v_Position,
            v_Normal
        );
    }

    outputColor /= LightCount;

    // Show our results
    o_Color = vec4(outputColor.rgb * baseColorTexture.rgb, 1.0);
}
