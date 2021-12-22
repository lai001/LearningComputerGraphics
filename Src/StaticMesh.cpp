#include "StaticMesh.h"

#include "ThirdParty/spdlog.h"
#include "FileManager/FileManager.h"
#include "StructTransform.h"
#include "BaseVertex.h"

FStaticMesh::FStaticMesh()
{
}

FStaticMesh::~FStaticMesh()
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

FStaticMesh * FStaticMesh::New(std::string ModelFilePath)
{
	int ASSIMP_LOAD_FLAGS = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	Assimp::Importer* Importer = new Assimp::Importer();
	const aiScene* Scene = Importer->ReadFile(ModelFilePath, ASSIMP_LOAD_FLAGS);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		spdlog::error("Import model failed: {}", Importer->GetErrorString());
		delete Importer;
		return nullptr;
	}
	FStaticMesh* Mesh = new FStaticMesh();
	Mesh->Directory = FFileManager::GetDir(ModelFilePath);
	Mesh->Importer = Importer;
	Mesh->Scene = Scene;
	Mesh->LoadModel();
	return Mesh;
}

void FStaticMesh::LoadModel()
{
	aiMatrix4x4 MTransformation = Scene->mRootNode->mTransformation;
	GlobalInverseTransform = ConvertMat4(MTransformation);
	GlobalInverseTransform = glm::inverse(GlobalInverseTransform);
	ProcessNode(Scene->mRootNode);
}

void FStaticMesh::ProcessNode(aiNode* Node)
{
	for (int i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* Aimesh = Scene->mMeshes[Node->mMeshes[i]];
		FStaticSubMesh* Mesh = CreateMesh(Aimesh);
		SubMeshs.push_back(Mesh);
	}

	for (int i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i]);
	}
}

FStaticSubMesh* FStaticMesh::CreateMesh(aiMesh* Mesh)
{
	std::vector<FBaseVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;

	for (int i = 0; i < Mesh->mNumVertices; i++)
	{
		FBaseVertex Vertex = CreateVertex(Mesh, i);
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

	return new FStaticSubMesh(Vertices, Indices, Textures);
}

std::vector<FTextureDescription*> FStaticMesh::CreateTexture(aiMaterial * Material, aiTextureType Type)
{
	std::vector<FTextureDescription*> Textures;
	for (int i = 0; i < Material->GetTextureCount(Type); i++)
	{
		aiString TextureFilepath;
		Material->GetTexture(Type, i, &TextureFilepath);
		FTextureDescription* Texture = FTextureDescription::New(FFileManager::Join({ Directory , TextureFilepath.C_Str() }), Type);
		Textures.push_back(Texture);
	}
	return Textures;
}

glm::mat4 FStaticMesh::GetGlobalInverseTransform()
{
	return GlobalInverseTransform;
}

glm::mat4 FStaticMesh::GetModelMatrix() const
{
	glm::mat4 ModelMatrix(1.0);
	ModelMatrix = glm::translate(ModelMatrix, Position);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1.0, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0.0, 1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0.0, 0.0f, 1.0f));
	return ModelMatrix;
}

std::vector<const FStaticSubMesh*> FStaticMesh::GetSubMeshs() const
{
	std::vector<const FStaticSubMesh*> Meshs;
	for (FStaticSubMesh* Item : SubMeshs)
	{
		Meshs.push_back(Item);
	}
	return Meshs;
}

FBaseVertex FStaticMesh::CreateVertex(aiMesh* Mesh, int Index)
{
	FBaseVertex Vertex;
	Vertex.Position = ConvertVec3(&Mesh->mVertices[Index]);
	Vertex.Normal = ConvertVec3(&Mesh->mNormals[Index]);
	glm::vec3 TextureCoords = ConvertVec3(&Mesh->mTextureCoords[0][Index]);
	Vertex.TexCoords = glm::vec2(TextureCoords.x, TextureCoords.y);
	return Vertex;
}
