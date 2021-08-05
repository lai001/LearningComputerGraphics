#include "GLStaticMeshDrawable.h"

FGLStaticMeshDrawable::FGLStaticMeshDrawable( FStaticMesh * Mesh)
	:Mesh(Mesh)
{
	for (int i = 0; i < Mesh->SubMeshs.size(); i++)
	{
		FStaticSubMesh* SubMesh = Mesh->SubMeshs[i];
		FGLStaticSubMeshDrawable* SubMeshDrawable = new FGLStaticSubMeshDrawable(SubMesh);
		SubMeshDrawables.push_back(SubMeshDrawable);
	}
	Shader = new FGLShader(Mesh->VertexShaderFilePath, Mesh->FragmentShaderFilePath);
}

FGLStaticMeshDrawable::~FGLStaticMeshDrawable()
{
	delete Shader;
	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		delete SubMeshDrawables[i];
	}
}

void FGLStaticMeshDrawable::Draw()
{
	Shader->Bind();
	glm::mat4 Model = Mesh->Model * Mesh->GetGlobalInverseTransform();
	glm::mat4 View = Mesh->View;
	glm::mat4 Projection = Mesh->Projection;
	Shader->SetMatrix(Model, View, Projection);

	FLightingSystem* LightingSystem = Mesh->LightingSystem;
	Shader->SetLight(LightingSystem->DirLight, LightingSystem->PointLight, LightingSystem->SpotLight);
	Shader->SetSpotLightEnable(LightingSystem->bIsSpotLightEnable);
	Shader->SetShininess(32.0f);

	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		FGLStaticSubMeshDrawable* SubMeshDrawable = SubMeshDrawables[i];
		SubMeshDrawable->Draw(Shader);
	}
}
