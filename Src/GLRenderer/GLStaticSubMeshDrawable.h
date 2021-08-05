#pragma once
#include "ThirdParty/opengl.h"
#include "StaticSubMesh.h"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"

class FGLStaticSubMeshDrawable
{
private:
	FStaticSubMesh * SubMesh;
	FGLVertexArray* Va;
	FGLVertexBuffer* Vb;
	FGLIndexBuffer* Ib;
	FGLVertexBufferLayout* Layout;
	std::vector<FGLTexture*> Textures;

public:
	FGLStaticSubMeshDrawable(FStaticSubMesh* SubMesh);
	~FGLStaticSubMeshDrawable();

	void Draw(FGLShader* Shader);
};

