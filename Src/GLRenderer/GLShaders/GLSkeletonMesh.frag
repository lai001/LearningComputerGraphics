#version 330 core

#include "Library/GLPhongLighting.glsl"

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 OutNormal;

uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;
uniform int spotLightEnable;

void main()
{    
	vec4 textureColor = texture(material.diffuseTexture, TexCoord);
	vec4 lightColor = phongCompositionLightColor(directionalLight, 
												pointLight,
												spotLight, 
												material, 
												TexCoord, 
												OutNormal,
												viewPos, 
												FragPos,
												spotLightEnable);
	FragColor = textureColor * lightColor;
}