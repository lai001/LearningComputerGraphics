#pragma once
class FGLIndexBuffer
{
private:
	unsigned int RendererID;
	unsigned int Count;

public:
	FGLIndexBuffer(const unsigned int* Data, unsigned int Count);
	~FGLIndexBuffer();

	void Bind();
	void Unbind();

	inline unsigned int GetCount() const
	{
		return Count;
	}
};

