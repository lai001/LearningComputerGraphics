#include "SkeletonMesh.h"

#include <iostream>
#include <stdio.h>

#include "ThirdParty/glm.h"
#include "ThirdParty/spdlog.h"

#include "StructTransform.h"
#include "FileManager/FileManager.h"

FSkeletonMesh::FSkeletonMesh()
{
}

FSkeletonMesh::FSkeletonMesh(std::string ModelFilePath)
	:ModelFilePath(ModelFilePath)
{
	Directory = FFileManager::GetDir(ModelFilePath);
	LoadModel();

	if (HasAnimations())
	{
		SelectedAniamtionIndex = 0;
	}

	for (int i = 0; i < Scene->mNumAnimations; i++)
	{
		aiAnimation* Animation = Scene->mAnimations[i];
		Animations.push_back(Animation);
	}

	ProcessNode(Scene->mRootNode);
}

FSkeletonMesh::~FSkeletonMesh()
{
	for (int i = 0; i < SubMeshs.size(); i++)
	{
		for (int j = 0; j < SubMeshs[i]->Textures.size(); j++)
		{
			delete SubMeshs[i]->Textures[j];
		}
		delete SubMeshs[i];
	}
	delete Importer;
}

void FSkeletonMesh::LoadModel()
{
	int ASSIMP_LOAD_FLAGS = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	Importer = new Assimp::Importer();
	Scene = Importer->ReadFile(ModelFilePath, ASSIMP_LOAD_FLAGS);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		spdlog::error("Import model failed: {}", Importer->GetErrorString());
		__debugbreak();
	}

	aiMatrix4x4 MTransformation = Scene->mRootNode->mTransformation;
	GlobalInverseTransform = ConvertMat4(MTransformation);
	GlobalInverseTransform = glm::inverse(GlobalInverseTransform);
}

void FSkeletonMesh::ProcessNode(aiNode* Node)
{
	for (int i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* Aimesh = Scene->mMeshes[Node->mMeshes[i]];
		FSkeletionSubMesh* Mesh = CreateMesh(Aimesh);
		SubMeshs.push_back(Mesh);
	}

	for (int i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i]);
	}
}

FSkeletionSubMesh* FSkeletonMesh::CreateMesh(aiMesh* Mesh)
{
	std::vector<FVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;

	for (int i = 0; i < Mesh->mNumVertices; i++)
	{
		FVertex Vertex = CreateVertex(Mesh, i);
		Vertices.push_back(Vertex);
	}

	for (int i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace Face = Mesh->mFaces[i];
		for (int j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}

	aiMaterial* Material = Scene->mMaterials[Mesh->mMaterialIndex];

	std::vector<FTextureDescription*> DiffuseTexturesGroup = CreateTexture(Material, aiTextureType::aiTextureType_DIFFUSE);
	Textures.insert(Textures.end(), DiffuseTexturesGroup.begin(), DiffuseTexturesGroup.end());
	std::vector<FTextureDescription*> SpecularTexturesGroup = CreateTexture(Material, aiTextureType::aiTextureType_SPECULAR);
	Textures.insert(Textures.end(), SpecularTexturesGroup.begin(), SpecularTexturesGroup.end());

	FSkeletionSubMesh* SubMesh = new FSkeletionSubMesh(Vertices, Indices, Textures);
	LoadBoneInfos(Mesh, SubMesh);

	return SubMesh;;
}

std::vector<FTextureDescription*> FSkeletonMesh::CreateTexture(aiMaterial * Material, aiTextureType Type)
{
	std::vector<FTextureDescription*> Textures;
	for (int i = 0; i < Material->GetTextureCount(Type); i++)
	{
		aiString TextureFilepath;
		Material->GetTexture(Type, i, &TextureFilepath);
		
		FTextureDescription* Texture = new FTextureDescription(FFileManager::Join({ Directory , TextureFilepath.C_Str() }), Type);
		Textures.push_back(Texture);
	}
	return Textures;
}

FVertex FSkeletonMesh::CreateVertex(aiMesh* Mesh, int Index)
{
	FVertex Vertex;
	Vertex.Position = ConvertVec3(&Mesh->mVertices[Index]);
	Vertex.Normal = ConvertVec3(&Mesh->mNormals[Index]);
	glm::vec3 TextureCoords = ConvertVec3(&Mesh->mTextureCoords[0][Index]);
	Vertex.TexCoords = glm::vec2(TextureCoords.x, TextureCoords.y);
	return Vertex;
}

bool FSkeletonMesh::HasAnimations()
{
	if (Scene)
	{
		return Scene->HasAnimations();
	}
	else
	{
		return false;
	}
}

const aiAnimation * FSkeletonMesh::GetCurrentAnimation()
{
	if (SelectedAniamtionIndex >= 0 && SelectedAniamtionIndex < Animations.size())
	{
		return Animations[SelectedAniamtionIndex];
	}
	else
	{
		return nullptr;
	}
}

void FSkeletonMesh::UpdateBoneTransform()
{
	if (bIsEnableBindingPost)
	{
		for (int i = 0; i < BoneInfos.size(); i++)
		{
			Transforms[i] = glm::mat4(1.0);
		}
	}
	else
	{
		if (Speed > 0.0)
		{
			const aiAnimation* Animation = GetCurrentAnimation();
			glm::mat4 Identity = glm::mat4(1.0f);
			float TicksPerSecond = (float)(Animation->mTicksPerSecond != 0 ? Animation->mTicksPerSecond : 25.0f);
			float TimeInTicks = RunningTime * Speed;
			float AnimationTime = fmod(TimeInTicks, (float)Animation->mDuration);
			ReadNodeHeirarchy(AnimationTime, Scene->mRootNode, Identity);

			for (int i = 0; i < BoneInfos.size(); i++)
			{
				Transforms[i] = BoneInfos[i].FinalTransformation;
			}
		}
	}
}

void FSkeletonMesh::LoadBoneInfos(aiMesh * Mesh, FSkeletionSubMesh* SubMesh)
{
	for (int BoneID = 0; BoneID < Mesh->mNumBones; BoneID++)
	{
		aiBone* Bone = Mesh->mBones[BoneID];

		for (int i = 0; i < Bone->mNumWeights; i++)
		{
			int VertexID = Bone->mWeights[i].mVertexId;
			float Weight = Bone->mWeights[i].mWeight;
			SubMesh->Vertices[VertexID].AddBoneData(BoneInfos.size(), Weight);
		}

		glm::mat4 offset = ConvertMat4(Bone->mOffsetMatrix);
		FBoneInfo BoneInfo;
		BoneInfo.BoneName = Bone->mName.data;
		BoneInfo.BoneOffset = offset;
		BoneInfos.push_back(BoneInfo);

		Transforms.push_back(glm::mat4(1.0f));
	}

	const aiAnimation* Animation = GetCurrentAnimation();
	for (int ChannelIndex = 0; ChannelIndex < Animation->mNumChannels; ChannelIndex++)
	{
		aiNodeAnim* NodeAnimation = Animation->mChannels[ChannelIndex];

		for (int i = 0; i < BoneInfos.size(); i++)
		{
			if (BoneInfos[i].BoneName == std::string(NodeAnimation->mNodeName.data))
			{
				BoneInfos[i].NodeAnimation = NodeAnimation;
			}
		}
	}
}

void FSkeletonMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode * AiNode, const glm::mat4 & ParentTransform)
{
	std::string NodeName(AiNode->mName.data);

	glm::mat4 NodeTransformation = ConvertMat4(AiNode->mTransformation);

	FBoneInfo* BoneInfo = nullptr;

	for (int i = 0; i < BoneInfos.size(); i++)
	{
		if (BoneInfos[i].BoneName == NodeName)
		{
			BoneInfo = &BoneInfos[i];
		}
	}

	if (BoneInfo)
	{
		glm::mat4 TranslationMatrix(1.0f);
		glm::mat4 RotationMatrix(1.0f);
		glm::mat4 ScaleMatrix(1.0f);
		TranslationMatrix = CalcInterpolatedPosition(AnimationTime, BoneInfo->NodeAnimation);
		RotationMatrix = CalcInterpolatedRotation(AnimationTime, BoneInfo->NodeAnimation);
		ScaleMatrix = CalcInterpolatedScaling(AnimationTime, BoneInfo->NodeAnimation);
		NodeTransformation = RotationMatrix * TranslationMatrix * ScaleMatrix;
	}

	NodeTransformation = NodeTransformation * ParentTransform;

	if (BoneInfo)
	{
		glm::mat4 FinalTransformation = BoneInfo->BoneOffset * NodeTransformation * GlobalInverseTransform;
		FinalTransformation = glm::transpose(FinalTransformation);
		BoneInfo->FinalTransformation = FinalTransformation;
	}

	for (int i = 0; i < AiNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, AiNode->mChildren[i], NodeTransformation);
	}
}

glm::mat4 FSkeletonMesh::CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	glm::mat4 Identity(1.0f);
	if (NodeAnim->mNumPositionKeys == 1)
	{
		return glm::translate(Identity, ConvertVec3(&NodeAnim->mPositionKeys[0].mValue));
	}

	int PositionIndex = FindPosition(AnimationTime, NodeAnim);
	int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < NodeAnim->mNumPositionKeys);

	float DeltaTime = NodeAnim->mPositionKeys[NextPositionIndex].mTime - NodeAnim->mPositionKeys[PositionIndex].mTime;
	float Factor = (AnimationTime - NodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	glm::vec3 Start = ConvertVec3(&NodeAnim->mPositionKeys[PositionIndex].mValue);
	glm::vec3 End = ConvertVec3(&NodeAnim->mPositionKeys[NextPositionIndex].mValue);
	glm::vec3 Transform = End - Start;

	return glm::transpose(glm::translate(Identity, Start + Transform * Factor));
}

glm::mat4 FSkeletonMesh::CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	glm::mat4 Identity(1.0f);

	if (NodeAnim->mNumRotationKeys == 1)
	{
		return glm::mat4_cast(ConvertQuat(&NodeAnim->mRotationKeys[0].mValue));
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, NodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < NodeAnim->mNumRotationKeys);

	float DeltaTime = NodeAnim->mRotationKeys[NextRotationIndex].mTime - NodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (AnimationTime - NodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	glm::quat StartRotationQ = ConvertQuat(&NodeAnim->mRotationKeys[RotationIndex].mValue);
	glm::quat EndRotationQ = ConvertQuat(&NodeAnim->mRotationKeys[NextRotationIndex].mValue);
	return glm::transpose(glm::mat4_cast(glm::slerp(StartRotationQ, EndRotationQ, Factor)));
	glm::mat4 Result = glm::interpolate(glm::mat4_cast(StartRotationQ), glm::mat4_cast(EndRotationQ), Factor);
	return 	glm::transpose(Result);
}

glm::mat4 FSkeletonMesh::CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	glm::mat4 Identity(1.0f);
	if (NodeAnim->mNumScalingKeys == 1)
	{
		return glm::scale(Identity, ConvertVec3(&NodeAnim->mScalingKeys[0].mValue));
	}

	int ScalingIndex = FindScaling(AnimationTime, NodeAnim);
	int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < NodeAnim->mNumScalingKeys);

	float DeltaTime = NodeAnim->mScalingKeys[NextScalingIndex].mTime - NodeAnim->mScalingKeys[ScalingIndex].mTime;
	float Factor = (AnimationTime - NodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	glm::vec3 Start = ConvertVec3(&NodeAnim->mScalingKeys[ScalingIndex].mValue);
	glm::vec3 End = ConvertVec3(&NodeAnim->mScalingKeys[NextScalingIndex].mValue);
	glm::vec3 Delta = End - Start;

	return glm::transpose(glm::scale(Identity, Start + Factor * Delta));
}

int FSkeletonMesh::FindPosition(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	for (unsigned int i = 0; i < NodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)NodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

int FSkeletonMesh::FindRotation(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	assert(NodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < NodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)NodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

int FSkeletonMesh::FindScaling(float AnimationTime, const aiNodeAnim * NodeAnim)
{
	assert(NodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < NodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)NodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

const std::vector<glm::mat4> FSkeletonMesh::GetTransforms() const
{
	return Transforms;
}

const glm::mat4 FSkeletonMesh::GetGlobalInverseTransform() const
{
	return GlobalInverseTransform;
}
