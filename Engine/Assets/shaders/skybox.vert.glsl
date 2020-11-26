#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	TexCoords = position;
	vec4 pos = projMat * viewMat * vec4(position, 1.0);

     // An optimization:
     // By forcing the z to be 1 via w
     // We can trick it into thinking it's always in the back on the depth test.
     gl_Position = pos.xyww;
}