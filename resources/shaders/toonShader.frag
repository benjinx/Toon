#version 410

// Varyings
in vertexData
{
	vec4 fragPos;
	vec4 normal;
	vec4 lightPos;
	vec4 eyePos;
	vec2 texCoords;
} pass;

// Uniforms
uniform sampler2D diffuseTex;

uniform vec3 lightCol;
uniform vec3 objCol;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec4 lightColor = vec4(lightCol, 1.0);
	vec4 objColor = vec4(objCol, 1.0);
	vec4 diffTex = texture(diffuseTex, pass.texCoords);

	float amb = 0.1;
	vec4 ambient = amb * lightColor;

	vec4 lightDir = normalize(pass.lightPos - pass.fragPos);

	float intensity = dot(pass.normal, lightDir);
	intensity = clamp(intensity, 0.1f, 1.0f);

	vec4 diffuse;

	if (intensity > 0.95)
		diffuse = objColor * vec4(0.95, 0.95, 0.95, 1.0);
	else if (intensity > 0.5)
		diffuse = objColor * vec4(0.5, 0.5, 0.5, 1.0);
	else if (intensity > 0.25)
		diffuse = objColor * vec4(0.25, 0.25, 0.25, 1.0);
	else
		diffuse = objColor * vec4(0.01, 0.01, 0.01, 1.0);

	fragColor = vec4((ambient.xyz + diffuse.xyz), 1.0f);
}