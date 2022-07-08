#pragma once
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <Foundation/Foundation.hpp>
#include "BridgingModels/SkeletonVertex.hpp"
#include "TextureDescription.hpp"
#include "BoneInfo.hpp"

class FSkeletionSubMesh
{
public:
	FSkeletionSubMesh(std::vector<FSkeletonVertex> Vertices,
		std::vector<unsigned int> Indices,
		std::vector<FTextureDescription*> Textures)
		:Vertices(Vertices), Indices(Indices), Textures(Textures)
	{
	}

	std::vector<FSkeletonVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;
};

class FSkeletonMesh: public ks::noncopyable
{
public:
	static FSkeletonMesh* New(const std::string& ModelFilePath);
	~FSkeletonMesh();
	bool HasAnimations();
	const aiAnimation* GetCurrentAnimation();

protected:
	std::string ModelFilePath;
	std::string Directory;
	const aiScene* Scene = nullptr;
	Assimp::Importer* Importer = nullptr;
	glm::mat4 GlobalInverseTransform;
	std::vector<const aiAnimation*> Animations;
	int SelectedAniamtionIndex = -1;
	std::vector<FSkeletionSubMesh*> SubMeshs;
	bool IsFlipVertically = true;

	void LoadModel();
	FSkeletonVertex CreateVertex(aiMesh * Mesh, int Index);
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
	std::string Name = "";

	glm::vec3 Position = glm::vec3(0.0);
	glm::vec3 Rotation = glm::vec3(0.0);
	glm::vec3 Scale = glm::vec3(1.0);

	float RunningTime = 0.0f;
	bool bIsEnableBindingPost = false;
	float Speed = 1.0f;

	//bool bIsUnlit = false;
	bool bIsVisible = true;

	void UpdateBoneTransform();
	const std::vector<glm::mat4> GetTransforms() const;
	const glm::mat4 GetGlobalInverseTransform() const;
	glm::mat4 GetModelMatrix() const;
	std::vector<const FSkeletionSubMesh*> GetSubMeshs() const;
};
