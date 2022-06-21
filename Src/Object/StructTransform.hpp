#pragma once
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

static glm::mat4 CovertMat3(const aiMatrix3x3* AssimpMatrix)
{
	glm::mat4 Mat(1.0f);
	aiMatrix3x3 AssimpMatrixDeref = *AssimpMatrix;
	Mat[0][0] = AssimpMatrixDeref.a1; Mat[0][1] = AssimpMatrixDeref.a2; Mat[0][2] = AssimpMatrixDeref.a3; Mat[0][3] = 0.0f;
	Mat[1][0] = AssimpMatrixDeref.b1; Mat[1][1] = AssimpMatrixDeref.b2; Mat[1][2] = AssimpMatrixDeref.b3; Mat[1][3] = 0.0f;
	Mat[2][0] = AssimpMatrixDeref.c1; Mat[2][1] = AssimpMatrixDeref.c2; Mat[2][2] = AssimpMatrixDeref.c3; Mat[2][3] = 0.0f;
	Mat[3][0] = 0.0f; Mat[3][1] = 0.0f; Mat[3][2] = 0.0f; Mat[3][3] = 1.0f;
	return Mat;
}

static glm::vec3 ConvertVec3(const aiVector3D* AssimpVec3)
{
	aiVector3D Vec3 = *AssimpVec3;
	return glm::vec3(Vec3.x, Vec3.y, Vec3.z);
}

static glm::vec2 ConvertVec2(const aiVector2D* AssimpVec2)
{
	aiVector2D Vec2 = *AssimpVec2;
	return glm::vec2(Vec2.x, Vec2.y);
}

static glm::quat ConvertQuat(const aiQuaternion* AssimpQuat)
{
	aiQuaternion RotationQ = *AssimpQuat;
	return glm::quat(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
}

static glm::mat4 ConvertMat4(const aiMatrix4x4 AssimpMat)
{
	glm::mat4 Mat(1.0f);
	Mat[0][0] = AssimpMat[0][0];
	Mat[0][1] = AssimpMat[0][1];
	Mat[0][2] = AssimpMat[0][2];
	Mat[0][3] = AssimpMat[0][3];

	Mat[1][0] = AssimpMat[1][0];
	Mat[1][1] = AssimpMat[1][1];
	Mat[1][2] = AssimpMat[1][2];
	Mat[1][3] = AssimpMat[1][3];

	Mat[2][0] = AssimpMat[2][0];
	Mat[2][1] = AssimpMat[2][1];
	Mat[2][2] = AssimpMat[2][2];
	Mat[2][3] = AssimpMat[2][3];

	Mat[3][0] = AssimpMat[3][0];
	Mat[3][1] = AssimpMat[3][1];
	Mat[3][2] = AssimpMat[3][2];
	Mat[3][3] = AssimpMat[3][3];
	return Mat;
}