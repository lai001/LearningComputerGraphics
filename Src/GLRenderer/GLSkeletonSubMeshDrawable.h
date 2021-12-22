#pragma once
#include "ThirdParty/opengl.h"
#include "SkeletionSubMesh.h"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"

class FGLSkeletonSubMeshDrawable
{
private:
	const FSkeletionSubMesh * SubMesh;
	FGLVertexArray* Va;
	FGLVertexBuffer* Vb;
	FGLIndexBuffer* Ib;
	std::vector<FGLTexture*> Textures;

public:
	FGLSkeletonSubMeshDrawable(const FSkeletionSubMesh* SubMesh);
	~FGLSkeletonSubMeshDrawable();

	void Draw(FGLShader* Shader);
};

