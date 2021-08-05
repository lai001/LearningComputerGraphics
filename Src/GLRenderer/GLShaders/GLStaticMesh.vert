#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out vec2 TexCoord;
// out vec3 FragPos;
out vec3 OutNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = TexCoords;
	OutNormal = Normal;
    gl_Position = projection * view  * model * vec4(Position, 1.0);
}