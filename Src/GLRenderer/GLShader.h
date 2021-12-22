#pragma once
#include <string>
#include <unordered_map>
#include "ThirdParty/assimp.h"
#include "ThirdParty/glm.h"
#include "ThirdParty/noncopyable.hpp"
#include "LightStruct.h"

class FGLShader : public boost::noncopyable
{
private:
	std::string VertexFilepath;
	std::string FragmentFilepath;
	std::unordered_map<std::string, int> UniformLocationCache;
	static std::string ShadersFolder;

	static std::unordered_map<std::string, FGLShader*> Shaders;

protected:
	FGLShader();

public:
	~FGLShader();

	static unsigned int InvalidID;
	unsigned int RendererID = FGLShader::InvalidID;

	static std::string GetShadersFolder();
	static void SetShadersFolder(const std::string& Filepath);

	void Bind() const;
	void Unbind();

	static int CreateShader(const std::string& VertexShaderSource, const std::string& FragmentShaderSource);
	static unsigned int CompileShader(const unsigned int ShaderType, const std::string & ShaderSource);
	static std::string ParseShader(const std::string & ShaderFilepath);

	void SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3);
	void SetUniform4fv(const std::string& Name, const glm::vec4 &Value);
	void SetUniform3f(const std::string& Name, float V0, float V1, float V2);
	void SetUniform3fv(const std::string& Name, const glm::vec3 &Value);
	void SetUniform1i(const std::string & Name, int V0);
	void SetUniform1f(const std::string & Name, float V0);
	void SetUniformMat4(const std::string & Name, const glm::mat4& Matrix);

	void SetLight(FDirectionalLight DirLight, FPointLight PointLight, FSpotLight SpotLight);
	void SetMatrix(glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);
	void SetIsUnlit(bool bIsUnlit);
	void SetMaterial(int DiffuseTextureBindSlot, int SpecularTextureBindSlot, float Shininess);
	void SetShininess(float Shininess);
	void SetBoneTransform(std::vector<glm::mat4> Transforms);
	void SetTexture(aiTextureType Type, int Slot);
	void SetViewPosition(const glm::vec3& ViewPosition);
	void SetModelMatrix(const glm::mat4& Model);
	void SetLightSpaceMatrix(const glm::mat4& Matrix);

	static FGLShader* New(const std::string& VertexFilepath, const std::string& FragFilepath);
	static FGLShader* Cache(const std::string& Name);
	static FGLShader* NewOrCache(const std::string& VertexFilepath, const std::string& FragFilepath, const std::string& Name);
	static bool LoadShader(const std::string& VertexFilepath, const std::string& FragFilepath, const std::string& Name);
	static void LoadShaderSync();
};