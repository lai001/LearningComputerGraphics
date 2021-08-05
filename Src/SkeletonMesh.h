#pragma once
#include <map>
#include <vector>

#include "ThirdParty/assimp.h"
#include "TextureDescription.h"
#include "SkeletionSubMesh.h"
#include "BoneInfo.h"
#include "LightingSystem/LightingSystem.h"

class FSkeletonMesh
{
public:
	FSkeletonMesh();
	FSkeletonMesh(std::string ModelFilePath);
	~FSkeletonMesh();
	bool HasAnimations();
	const aiAnimation* GetCurrentAnimation();

protected:
	std::string Directory;
	const aiScene* Scene = nullptr;
	Assimp::Importer* Importer = nullptr;
	glm::mat4 GlobalInverseTransform;
	std::vector<const aiAnimation*> Animations;
	int SelectedAniamtionIndex = -1;

	void LoadModel();
	FVertex CreateVertex(aiMesh * Mesh, int Index);
	void ProcessNode(aiNode * Node);
	FSkeletionSubMesh* CreateMesh(aiMesh * Mesh);
	std::vector<FTextureDescription*> CreateTexture(aiMaterial * Material, aiTextureType Type);

private:
	std::vector<FBoneInfo> BoneInfos;
	std::vector<glm::mat4> Transforms;
	void LoadBoneInfos(aiMesh * Mesh, FSkeletionSubMesh * SubMesh);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode * AiNode, const glm::mat4 & ParentTransform);
	glm::mat4 CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim * NodeAnim);
	glm::mat4 CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim * NodeAnim);
	glm::mat4 CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim * NodeAnim);
	int FindPosition(float AnimationTime, const aiNodeAnim * NodeAnim);
	int FindRotation(float AnimationTime, const aiNodeAnim * NodeAnim);
	int FindScaling(float AnimationTime, const aiNodeAnim * NodeAnim);

public:
	std::vector<FSkeletionSubMesh*> SubMeshs;

	std::string	VertexShaderFilePath;
	std::string FragmentShaderFilePath;
	std::string ModelFilePath;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;

	float RunningTime;
	bool bIsEnableBindingPost;
	float Speed;

	FLightingSystem* LightingSystem;

	void UpdateBoneTransform();
	const std::vector<glm::mat4> GetTransforms() const;
	const glm::mat4 GetGlobalInverseTransform() const;
};