#pragma once
#include <vector>
#include <string>
#include "ThirdParty/assimp.h"
#include "ThirdParty/glm.h"
#include "ThirdParty/noncopyable.hpp"
#include "StaticSubMesh.h"
#include "LightingSystem/LightingSystem.h"

class FStaticMesh: public boost::noncopyable
{
public:
	~FStaticMesh();

	static FStaticMesh* New(std::string ModelFilePath);

protected:
	FStaticMesh();
	std::string Directory;
	std::string ModelFilePath;
	std::vector<FStaticSubMesh*> SubMeshs;
	const aiScene* Scene = nullptr;
	Assimp::Importer* Importer = nullptr;
	glm::mat4 GlobalInverseTransform;

	void LoadModel();
	FBaseVertex CreateVertex(aiMesh * Mesh, int Index);
	void ProcessNode(aiNode * Node);
	FStaticSubMesh* CreateMesh(aiMesh * Mesh);
	std::vector<FTextureDescription*> CreateTexture(aiMaterial * Material, aiTextureType Type);

public:
	std::string Name = "";
	glm::vec3 Position = glm::vec3(0.0);
	glm::vec3 Rotation = glm::vec3(0.0);
	glm::vec3 Scale = glm::vec3(1.0);

	glm::mat4 View;
	glm::mat4 Projection;

	FLightingSystem* LightingSystem;

	bool bIsUnlit = false;
	bool bIsVisible = true;

	glm::mat4 GetGlobalInverseTransform();
	glm::mat4 GetModelMatrix() const;
	std::vector<const FStaticSubMesh*> GetSubMeshs() const;
};