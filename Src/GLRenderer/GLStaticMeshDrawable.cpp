#include "GLStaticMeshDrawable.h"

FGLStaticMeshDrawable::FGLStaticMeshDrawable(FStaticMesh * Mesh)
	:Mesh(Mesh)
{
	assert(Mesh);
	std::vector<const FStaticSubMesh*> SubMeshs = Mesh->GetSubMeshs();
	for (int i = 0; i < SubMeshs.size(); i++)
	{
		const FStaticSubMesh* SubMesh = SubMeshs[i];
		FGLStaticSubMeshDrawable* SubMeshDrawable = new FGLStaticSubMeshDrawable(SubMesh);
		SubMeshDrawables.push_back(SubMeshDrawable);
	}
	const std::string VertexShaderPath = FGLShader::GetShadersFolder().append("/GLStaticMesh.vert");
	const std::string FragmentShaderPath = FGLShader::GetShadersFolder().append("/GLStaticMesh.frag");
	Shader = FGLShader::New(VertexShaderPath, FragmentShaderPath);
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
	if (Mesh->bIsVisible == false)
	{
		return;
	}
	FLightingSystem* LightingSystem = Mesh->LightingSystem;
	glm::mat4 Model = Mesh->GetModelMatrix() * Mesh->GetGlobalInverseTransform();
	glm::mat4 View = Mesh->View;
	glm::mat4 Projection = Mesh->Projection;

	Shader->Bind();
	Shader->SetMatrix(Model, View, Projection);
	Shader->SetIsUnlit(Mesh->bIsUnlit);
	Shader->SetLight(LightingSystem->DirLight, LightingSystem->PointLight, LightingSystem->SpotLight);
	Shader->SetShininess(32.0f);
	Shader->SetViewPosition(LightingSystem->ViewPosition);

	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		FGLStaticSubMeshDrawable* SubMeshDrawable = SubMeshDrawables[i];
		SubMeshDrawable->Draw(Shader);
	}
}

void FGLStaticMeshDrawable::DrawWithDepthMapShader(FGLShader & Shader)
{
	if (Mesh->bIsUnlit || (Mesh->bIsVisible == false))
	{
		return;
	}
	glm::mat4 Model = Mesh->GetModelMatrix() * Mesh->GetGlobalInverseTransform();
	Shader.Bind();
	Shader.SetModelMatrix(Model);
	Shader.SetLightSpaceMatrix(Mesh->LightingSystem->Shadow.GetLightSpaceMat());
	for (int i = 0; i < SubMeshDrawables.size(); i++)
	{
		FGLStaticSubMeshDrawable* SubMeshDrawable = SubMeshDrawables[i];
		SubMeshDrawable->Draw(&Shader);
	}
}

const FStaticMesh * FGLStaticMeshDrawable::GetMesh() const
{
	return Mesh;
}

const FGLShader * FGLStaticMeshDrawable::GetShader() const
{
	return Shader;
}
