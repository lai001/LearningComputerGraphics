#pragma once
#include <string>
#include <unordered_map>
#include "ThirdParty/assimp.h"
#include "ThirdParty/glm.h"
#include "LightStruct.h"

class FGLShader
{
private:
	std::string VertexFilepath;
	std::string FragmentFilepath;
	std::unordered_map<std::string, int> UniformLocationCache;
	static std::string ShadersFolder;

public:
	FGLShader(const std::string & Vertexfilepath, const std::string & Fragmentfilepath);
	~FGLShader();

	unsigned int RendererID;

	static std::string GetShadersFolder();
	static void SetShadersFolder(const std::string& Filepath);

	void Bind() const;
	void Unbind();

	int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	unsigned int CompileShader(unsigned int Type, const std::string & Source, const std::string DebugFilepath);
	std::string ParseShader(const std::string & Filepath);
	void SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3);
	void SetUniform4fv(const std::string& Name, const glm::vec4 &value);
	void SetUniform3f(const std::string& Name, float V0, float V1, float V2);
	void SetUniform3fv(const std::string& Name, const glm::vec3 &value);
	void SetUniform1i(const std::string & Name, int V0);
	void SetUniform1f(const std::string & Name, float V0);
	void SetUniformMat4(const std::string & Name, const glm::mat4& Matrix);

	void SetLight(FDirLight DirLight, FPointLight PointLight, FSpotLight SpotLight);
	void SetMatrix(glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);
	void SetSpotLightEnable(bool bEnable);
	void SetMaterial(int DiffuseTextureBindSlot, int SpecularTextureBindSlot, float Shininess);
	void SetShininess(float Shininess);
	void SetBoneTransform(std::vector<glm::mat4> Transforms);
	void SetTexture(aiTextureType Type, int Slot);
	void SetViewPosition(const glm::vec3& ViewPosition);
};