#pragma once
#include <vector>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <glm/glm.hpp>
#include <Foundation/Foundation.hpp>
#include "BridgingModels/BaseVertex.hpp"
#include "TextureDescription.hpp"

struct FMeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangents;
	glm::vec3 Bitangents;
	glm::vec2 TexCoords;
};

struct FStaticSubMesh
{
	std::vector<FMeshVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;

	explicit FStaticSubMesh::FStaticSubMesh(std::vector<FMeshVertex> Vertices,
		std::vector<unsigned int> Indices, 
		std::vector<FTextureDescription*> Textures)
		:Vertices(Vertices), Indices(Indices), Textures(Textures)
	{
	}
};

class FStaticMesh: public ks::noncopyable
{
public:
	~FStaticMesh();

	static FStaticMesh* New(std::string ModelFilePath);
	static FStaticMesh* New(std::vector<FStaticSubMesh*> SubMeshs);

protected:
	std::string Directory;
	std::string ModelFilePath;
	std::vector<FStaticSubMesh*> SubMeshs;
	const aiScene* Scene = nullptr;
	std::unique_ptr<Assimp::Importer> Importer = nullptr;
	glm::mat4 GlobalInverseTransform;
	bool IsFlipVertically = true;

	void LoadModel();
	FMeshVertex CreateVertex(aiMesh * Mesh, int Index);
	void ProcessNode(aiNode * Node);
	FStaticSubMesh* CreateMesh(aiMesh * Mesh);
	std::vector<FTextureDescription*> CreateTexture(aiMaterial * Material, aiTextureType Type);

public:
	std::string Name = "";
	glm::vec3 Position = glm::vec3(0.0);
	glm::vec3 Rotation = glm::vec3(0.0);
	glm::vec3 Scale = glm::vec3(1.0);

	//bool bIsUnlit = false;
	bool bIsVisible = true;

	glm::mat4 GetGlobalInverseTransform();
	glm::mat4 GetModelMatrix() const;
	std::vector<const FStaticSubMesh*> GetSubMeshs() const;
};
