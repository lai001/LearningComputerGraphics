#pragma once
#include <vector>
#include <string>
#include "ThirdParty/assimp.h"
#include "ThirdParty/glm.h"
#include "StaticSubMesh.h"
#include "LightingSystem/LightingSystem.h"

class FStaticMesh
{
public:
	FStaticMesh();
	FStaticMesh(std::string ModelFilePath);
	~FStaticMesh();

protected:
	std::string Directory;
	const aiScene* Scene;
	Assimp::Importer* Importer = nullptr;
	glm::mat4 GlobalInverseTransform;

	void LoadModel();
	FBaseVertex CreateVertex(aiMesh * Mesh, int Index);
	void ProcessNode(aiNode * Node);
	FStaticSubMesh* CreateMesh(aiMesh * Mesh);
	std::vector<FTextureDescription*> CreateTexture(aiMaterial * Material, aiTextureType Type);

public:
	std::vector<FStaticSubMesh*> SubMeshs;

	std::string	VertexShaderFilePath;
	std::string FragmentShaderFilePath;
	std::string ModelFilePath;

	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;

	FLightingSystem* LightingSystem;

	glm::mat4 GetGlobalInverseTransform();
};

