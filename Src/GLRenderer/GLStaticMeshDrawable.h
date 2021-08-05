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

	std::vector<FGLStaticSubMeshDrawable*> SubMeshDrawables;

public:
	FGLStaticMeshDrawable( FStaticMesh* Mesh);
	~FGLStaticMeshDrawable();

	void Draw();
};
