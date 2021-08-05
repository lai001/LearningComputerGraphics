#version 330 core
out vec4 FragColor;

void main()
{
	vec3 Color = vec3(0.2, 0.3, 0.5);
    FragColor = vec4(Color, 1.0);
}