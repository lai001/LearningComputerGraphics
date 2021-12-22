#pragma once
#include "ThirdParty/opengl.h"
#include "StaticMesh.h"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLStaticSubMeshDrawable.h"

class FGLStaticMeshDrawable
{
private:
	FStaticMesh * Mesh;
	FGLShader* Shader;
	FGLShader* UnlitShader;

	std::vector<FGLStaticSubMeshDrawable*> SubMeshDrawables;

public:
	FGLStaticMeshDrawable( FStaticMesh* Mesh);
	~FGLStaticMeshDrawable();

	void Draw();

	void DrawWithDepthMapShader(FGLShader& Shader);

	const FStaticMesh* GetMesh() const;
	const FGLShader* GetShader() const;
};
