#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

//layout (location = 3) in vec3 tangent;
//layout (location = 4) in vec2 bitangent;

layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

const int MAX_BONES = 100;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 OutNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];

void main()
{
    mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform     += bones[BoneIDs[1]] * Weights[1];
    BoneTransform     += bones[BoneIDs[2]] * Weights[2];
    BoneTransform     += bones[BoneIDs[3]] * Weights[3];

    TexCoord = TexCoords;
	FragPos = vec3(model * BoneTransform * vec4(Position, 1.0));

	OutNormal =  mat3(transpose(inverse(model * BoneTransform))) * Normal;

    gl_Position = projection * view  * model * BoneTransform * vec4(Position, 1.0);
}