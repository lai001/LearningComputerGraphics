#version 330 core

layout(location = 0) in vec4 position;
layout (location = 1) in vec2 TexCoord;


out vec2 out_textureCoordinate;

void main()
{
	out_textureCoordinate = TexCoord;

	gl_Position = position;
}