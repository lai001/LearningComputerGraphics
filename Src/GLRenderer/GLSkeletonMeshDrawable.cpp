#include "GLSkeletonMeshDrawable.h"

FGLSkeletonMeshDrawable::FGLSkeletonMeshDrawable(FSkeletonMesh * Mesh)
	:Mesh(Mesh)
{
	for (int i = 0; i < Mesh->SubMeshs.size(); i++)
	{
		FSkeletionSubMesh* SubMesh = Mesh->SubMeshs[i];
		FGLSkeletonSubMeshDrawable* SubMeshDrawable = new FGLSkeletonSubMeshDrawable(SubMesh);
		SubMeshDrawables.push_back(SubMeshDrawable);
	}
	
	const std::string VertexShaderFilePath = FGLShader::GetShadersFolder().append("/GLSkeletonMesh.vert");
	const std::string FragmentShaderFilePath = FGLShader::GetShadersFolder().append("/GLSkeletonMesh.frag");
	Shader = new FGLShader(VertexShaderFilePath, FragmentShaderFilePath);
}

FGLSkeletonMeshDrawable::~FGLSkeletonMeshDrawable()
{
}

void FGLSkeletonMeshDrawable::Draw()
{
	Shader->Bind();
	Shader->SetBoneTransform(Mesh->GetTransforms());
	glm::mat4 Model = Mesh->Model;
	glm::mat4 View = Mesh->View;
	glm::mat4 Projection = Mesh->Projection;
	Shader->SetMatrix(Model, View, Projection);

	FLightingSystem* LightingSystem = Mesh->LightingSystem;
	Shader->SetLight(LightingSystem->DirLight, LightingSystem->PointLight, LightingSystem->SpotLight);
	Shader->SetSpotLightEnable(LightingSystem->bIsSpotLightEnable);
	Shader->SetShininess(32.0f);
	Shader->SetViewPosition(LightingSystem->ViewPosition);

	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		FGLSkeletonSubMeshDrawable* SubMeshDrawable = SubMeshDrawables[i];
		SubMeshDrawable->Draw(Shader);
	}
}
