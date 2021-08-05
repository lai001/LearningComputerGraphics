#pragma once
#include "ThirdParty/opengl.h"
#include "SkeletonMesh.h"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLSkeletonSubMeshDrawable.h"

class FGLSkeletonMeshDrawable
{
private:
	FSkeletonMesh * Mesh;
	FGLShader* Shader;

	std::vector<FGLSkeletonSubMeshDrawable*> SubMeshDrawables;

public:
	FGLSkeletonMeshDrawable(FSkeletonMesh* Mesh);
	~FGLSkeletonMeshDrawable();

	void Draw();
};