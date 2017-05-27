/*
	Phong (FS)
	By Dan Buckstein
	Fragment shader that computes Phong shading.
	
	Modified by: ______________________________________________________________
*/

// version
#version 330 core


// ****
// varyings


in vertexData
{
	vec2 uv;
	vec3 lightDir;
	vec3 viewDir;
} pass;

// ****
// uniforms
uniform sampler2D tex_dm;
uniform sampler2D tex_nm;
uniform sampler2D tex_sm;
uniform sampler2D tex_hm;
//uniform float height_scale;

// ****
// target
layout (location = 0) out vec4 fragColor;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = 1.0 + texture(tex_hm, texCoords).r; // need to inverse to use depthMap
	return texCoords - viewDir.xy / viewDir.z * (height * 0.1); //height_scale
}

// shader function
void main()
{

	vec3 L = normalize(pass.lightDir);
	vec3 V = normalize(pass.viewDir);

	vec2 texCoords = parallaxMapping(pass.uv, V);
	//if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
	//	discard;

	vec3 N = texture(tex_nm, texCoords).rgb * 2.0 - 1.0;

	vec3 color = texture(tex_dm, texCoords).rgb;

	// Ambient
	vec3 ambient = 0.1 * color;
	
	// Diffuse
	float diff = dot(L, N);

	// Specular
	vec3 R = (diff + diff) * N - L;
//	vec3 reflectDir = reflect(-L, N);
//	vec3 halfwayDir = normalize(L + V);
	float spec = max(dot(V, R), 0.0);
	spec *= spec;
	spec *= spec;
	spec *= spec;

	diff = max(diff, 0.0);
	vec3 diffuse = diff * color;
	vec3 specular = texture(tex_sm, texCoords).rgb * spec;
//	vec3 specular = vec3(0.2) * spec;
	
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}