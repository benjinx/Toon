/*
	Draw Color
	By Benji Campbell
	Fragment shader that displays color received from previous pipeline stage.
*/

#version 330 core

// varyings
in vec4 passColor;
in vec2 passTexCoord;
in mat4 passVM;

// uniforms
uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D bumpTex;
uniform float sel;

// ****
// target
layout (location = 0) out vec4 fragColor;

// shader entry point: function executes once per-fragment
void main()
{
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//fragColor = texture(ambient, passTexCoord);
	//fragColor = texture(diffuse, passTexCoord);
	//fragColor = mix(texture(ambientTex, passTexCoord), texture(diffuseTex, passTexCoord), 0.5);

	//vec4 mixedTextureAndColor = texture(diffuseTex, passTexCoord);

	fragColor = texture(diffuseTex, passTexCoord);

	//if (sel == 0)
	//	fragColor = texture(ambientTex, passTexCoord);
	//else if (sel == 1)
	//	fragColor = texture(diffuseTex, passTexCoord);
	//else if (sel == 2)
	//	fragColor = texture(specularTex, passTexCoord);
	//else if (sel == 3)
	//	fragColor = texture(bumpTex, passTexCoord);
	//else if (sel == 4)
	//	fragColor = mix(mixedTextureAndColor, vec4(1.0, 0.0, 1.0, 1.0), 0.5);

}
