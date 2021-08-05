#include "GLShader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "ThirdParty/spdlog.h"
#include "glad/glad.h"

FGLShader::FGLShader(const std::string & VertexFilepath, const std::string & FragmentFilepath)
	: VertexFilepath(VertexFilepath), FragmentFilepath(FragmentFilepath), RendererID(0)
{
	std::string VertexShader = ParseShader(VertexFilepath);
	std::string FragmentShader = ParseShader(FragmentFilepath);

	RendererID = CreateShader(VertexShader, FragmentShader);
}

FGLShader::~FGLShader()
{
	glDeleteProgram(RendererID);
}

void FGLShader::Bind() const
{
	glUseProgram(RendererID);
}

void FGLShader::Unbind()
{
	glUseProgram(0);
}

int FGLShader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	unsigned int Program = glCreateProgram();

	unsigned int Vs = CompileShader(GL_VERTEX_SHADER, VertexShader, VertexFilepath);
	unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader, FragmentFilepath);

	glAttachShader(Program, Vs);
	glAttachShader(Program, Fs);
	glLinkProgram(Program);
	glValidateProgram(Program);

	glDeleteShader(Vs);
	glDeleteShader(Fs);

	return Program;
}

unsigned int FGLShader::CompileShader(unsigned int Type, const std::string & Source, const std::string DebugFilepath)
{
	unsigned int Shader = glCreateShader(Type);
	const char* Src = Source.c_str();
	glShaderSource(Shader, 1, &Src, nullptr);
	glCompileShader(Shader);

	int Result;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Result);

	if (Result == false)
	{
		int Length;
		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &Length);
		char* Message = (char*)alloca(Length * sizeof(char));

		glGetShaderInfoLog(Shader, Length, &Length, Message);
		spdlog::error("Failed to compile{0} shader!", (Type == GL_VERTEX_SHADER ? " vertex" : " fragment"));
		spdlog::error(DebugFilepath);
		spdlog::error(Message);
		glDeleteShader(Shader);

		return 0;
	}

	return Shader;
}

std::string FGLShader::ParseShader(const std::string & Filepath)
{
	std::string SourceShader;
	std::ifstream InputFileStream;
	std::stringstream ShaderStream;

	InputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		InputFileStream.open(Filepath);
		ShaderStream << InputFileStream.rdbuf();
		SourceShader = ShaderStream.str();
		InputFileStream.close();
	}
	catch (std::ifstream::failure E)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ IN FILE: " << Filepath << std::endl;
	}
	return SourceShader;
}

void FGLShader::SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform4f(UniformLocationCache[Name], V0, V1, V2, V3);
}

void FGLShader::SetUniform3f(const std::string & Name, float V0, float V1, float V2)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform3f(UniformLocationCache[Name], V0, V1, V2);
}

void FGLShader::SetUniform3fv(const std::string & Name, const glm::vec3 & Value)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform3fv(UniformLocationCache[Name], 1, &Value[0]);
}

void FGLShader::SetUniform1i(const std::string & Name, int V0)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform1i(UniformLocationCache[Name], V0);
}

void FGLShader::SetUniform1f(const std::string & Name, float V0)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform1f(UniformLocationCache[Name], V0);
}

void FGLShader::SetUniformMat4(const std::string & Name, const glm::mat4& Matrix)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniformMatrix4fv(UniformLocationCache[Name], 1, GL_FALSE, &Matrix[0][0]);
}

void FGLShader::SetLight(FDirLight DirLight, FPointLight PointLight, FSpotLight SpotLight)
{
	SetUniform3fv("directionalLight.direction", DirLight.Direction);
	SetUniform3fv("directionalLight.ambient", DirLight.Ambient);
	SetUniform3fv("directionalLight.diffuse", DirLight.Diffuse);
	SetUniform3fv("directionalLight.specular", DirLight.Specular);

	SetUniform3fv("pointLight.position", PointLight.Position);
	SetUniform3fv("pointLight.ambient", PointLight.Ambient);
	SetUniform3fv("pointLight.diffuse", PointLight.Diffuse);
	SetUniform3fv("pointLight.specular", PointLight.Specular);
	SetUniform1f("pointLight.constant", PointLight.Constant);
	SetUniform1f("pointLight.linear", PointLight.Linear);
	SetUniform1f("pointLight.quadratic", PointLight.Quadratic);

	SetUniform3fv("spotLight.position", SpotLight.Position);
	SetUniform3fv("spotLight.direction", SpotLight.Direction);
	SetUniform3fv("spotLight.ambient", SpotLight.Ambient);
	SetUniform3fv("spotLight.diffuse", SpotLight.Diffuse);
	SetUniform3fv("spotLight.specular", SpotLight.Specular);
	SetUniform1f("spotLight.constant", SpotLight.Constant);
	SetUniform1f("spotLight.linear", SpotLight.Linear);
	SetUniform1f("spotLight.quadratic", SpotLight.Quadratic);
	SetUniform1f("spotLight.cutOff", SpotLight.CutOff);
	SetUniform1f("spotLight.outerCutOff", SpotLight.OuterCutOff);
}

void FGLShader::SetMatrix(glm::mat4 Model, glm::mat4 View, glm::mat4 Projection)
{
	SetUniformMat4("model", Model);
	SetUniformMat4("view", View);
	SetUniformMat4("projection", Projection);
}

void FGLShader::SetSpotLightEnable(bool bEnable)
{
	SetUniform1i("spotLightEnable", int(bEnable));
}

void FGLShader::SetMaterial(int DiffuseTextureBindSlot, int SpecularTextureBindSlot, float Shininess)
{
	SetUniform1i("material.diffuseTexture", DiffuseTextureBindSlot);
	SetUniform1i("material.specularTexture", SpecularTextureBindSlot);
	SetUniform1f("material.shininess", Shininess);
}

void FGLShader::SetShininess(float Shininess)
{
	SetUniform1f("material.shininess", Shininess);
}

void FGLShader::SetBoneTransform(std::vector<glm::mat4> Transforms)
{
	for (int i = 0; i < Transforms.size(); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "bones[%d]", i);
		SetUniformMat4(Name, Transforms[i]);
	}
}

void FGLShader::SetTexture(aiTextureType Type, int Slot)
{
	switch (Type)
	{
	case aiTextureType_NONE:
		break;
	case aiTextureType_DIFFUSE:
		SetUniform1i("material.diffuseTexture", Slot);
		break;
	case aiTextureType_SPECULAR:
		SetUniform1i("material.specularTexture", Slot);
		break;
		//case aiTextureType_AMBIENT:
		//	break;
		//case aiTextureType_EMISSIVE:
		//	break;
		//case aiTextureType_HEIGHT:
		//	break;
		//case aiTextureType_NORMALS:
		//	break;
		//case aiTextureType_SHININESS:
		//	break;
		//case aiTextureType_OPACITY:
		//	break;
		//case aiTextureType_DISPLACEMENT:
		//	break;
		//case aiTextureType_LIGHTMAP:
		//	break;
		//case aiTextureType_REFLECTION:
		//	break;
		//case aiTextureType_BASE_COLOR:
		//	break;
		//case aiTextureType_NORMAL_CAMERA:
		//	break;
		//case aiTextureType_EMISSION_COLOR:
		//	break;
		//case aiTextureType_METALNESS:
		//	break;
		//case aiTextureType_DIFFUSE_ROUGHNESS:
		//	break;
		//case aiTextureType_AMBIENT_OCCLUSION:
		//	break;
		//case aiTextureType_UNKNOWN:
		//	break;
		//case _aiTextureType_Force32Bit:
		//	break;
	default:
		break;
	}
}
