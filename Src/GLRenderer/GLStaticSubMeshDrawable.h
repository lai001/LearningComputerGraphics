#pragma once
#include "ThirdParty/opengl.h"
#include "StaticSubMesh.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLVertexObject.h"

class FGLStaticSubMeshDrawable
{
private:
	const FStaticSubMesh* SubMesh;
	FGLVertexObject* VertexObject;
	std::vector<FGLTexture*> Textures;

public:
	FGLStaticSubMeshDrawable(const FStaticSubMesh* SubMesh);
	~FGLStaticSubMeshDrawable();

	void Draw(FGLShader* Shader);
};

