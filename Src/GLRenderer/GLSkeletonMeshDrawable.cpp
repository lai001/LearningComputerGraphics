#include "GLSkeletonMeshDrawable.h"

FGLSkeletonMeshDrawable::FGLSkeletonMeshDrawable(FSkeletonMesh * Mesh)
	:Mesh(Mesh)
{
	auto SubMeshs = Mesh->GetSubMeshs();
	for (int i = 0; i < SubMeshs.size(); i++)
	{
		const FSkeletionSubMesh* SubMesh = SubMeshs[i];
		FGLSkeletonSubMeshDrawable* SubMeshDrawable = new FGLSkeletonSubMeshDrawable(SubMesh);
		SubMeshDrawables.push_back(SubMeshDrawable);
	}

	const std::string VertexShaderFilePath = FGLShader::GetShadersFolder().append("/GLSkeletonMesh.vert");
	const std::string FragmentShaderFilePath = FGLShader::GetShadersFolder().append("/GLSkeletonMesh.frag");
	Shader = FGLShader::New(VertexShaderFilePath, FragmentShaderFilePath);
}

FGLSkeletonMeshDrawable::~FGLSkeletonMeshDrawable()
{
}

void FGLSkeletonMeshDrawable::Draw()
{
	if (Mesh->bIsVisible == false)
	{
		return;
	}
	FLightingSystem* LightingSystem = Mesh->LightingSystem;
	glm::mat4 Model = Mesh->GetModelMatrix();
	glm::mat4 View = Mesh->View;
	glm::mat4 Projection = Mesh->Projection;

	Shader->Bind();
	Shader->SetBoneTransform(Mesh->GetTransforms());
	Shader->SetMatrix(Model, View, Projection);
	Shader->SetIsUnlit(Mesh->bIsUnlit);
	Shader->SetLight(LightingSystem->DirLight, LightingSystem->PointLight, LightingSystem->SpotLight);
	Shader->SetShininess(32.0f);
	Shader->SetViewPosition(LightingSystem->ViewPosition);

	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		FGLSkeletonSubMeshDrawable* SubMeshDrawable = SubMeshDrawables[i];
		SubMeshDrawable->Draw(Shader);
	}
}

const FSkeletonMesh * FGLSkeletonMeshDrawable::GetMesh() const
{
	return Mesh;
}

const FGLShader * FGLSkeletonMeshDrawable::GetShader() const
{
	return Shader;
}
