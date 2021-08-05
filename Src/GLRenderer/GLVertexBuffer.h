#pragma once

class FGLVertexBuffer
{

private:
	unsigned int RendererID;
public:
	FGLVertexBuffer(const void* Data, unsigned int Size);
	~FGLVertexBuffer();

	void Bind() const;
	void Unbind();
};

