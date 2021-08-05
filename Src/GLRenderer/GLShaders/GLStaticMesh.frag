#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    float shininess;
};

in vec2 TexCoord;
uniform Material material;

void main()
{
	vec4 textureColor = texture(material.diffuseTexture, TexCoord);
    FragColor = textureColor;
}