#include "GLShader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "ThirdParty/spdlog.h"
#include "glad/glad.h"

#include "GLPreProcessor.h"
#include "FileManager/FileManager.h"

std::string FGLShader::ShadersFolder = "";
std::unordered_map<std::string, FGLShader*> FGLShader::Shaders;
unsigned int FGLShader::InvalidID = -1;

FGLShader::FGLShader()
{
}

FGLShader::~FGLShader()
{
	if (RendererID != FGLShader::InvalidID)
	{
		glDeleteProgram(RendererID);
	}
}

std::string FGLShader::GetShadersFolder()
{
	return ShadersFolder;
}

void FGLShader::SetShadersFolder(const std::string & Filepath)
{
	ShadersFolder = Filepath;
}

void FGLShader::Bind() const
{
	assert(RendererID != FGLShader::InvalidID);
	glUseProgram(RendererID);
}

void FGLShader::Unbind()
{
	glUseProgram(0);
}

int FGLShader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	unsigned int ProgramID = glCreateProgram();
	try
	{
		unsigned int Vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
		unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);
		glAttachShader(ProgramID, Vs);
		glAttachShader(ProgramID, Fs);
		glLinkProgram(ProgramID);
		glValidateProgram(ProgramID);
		glDeleteShader(Vs);
		glDeleteShader(Fs);
		return ProgramID;
	}
	catch (const std::exception E)
	{
		glDeleteProgram(ProgramID);
		throw E;
	}
}

unsigned int FGLShader::CompileShader(const unsigned int ShaderType, const std::string & ShaderSource)
{
	unsigned int Shader = glCreateShader(ShaderType);
	const char* Src = ShaderSource.c_str();
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
		//spdlog::error("Failed to compile{0} shader!", (ShaderType == GL_VERTEX_SHADER ? " vertex" : " fragment"));
		//spdlog::error(DebugFilepath);
		//spdlog::error(Message);
		glDeleteShader(Shader);
		throw std::runtime_error(std::string(ShaderType == GL_VERTEX_SHADER ? " vertex" : " fragment") + std::string(" shader compile fail.") + std::string(Message));
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
		spdlog::error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ IN FILE: {}", Filepath);
		throw E;
	}

	return GLPreProcessor::Process(SourceShader);
}

void FGLShader::SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform4f(UniformLocationCache[Name], V0, V1, V2, V3);
}

void FGLShader::SetUniform4fv(const std::string & Name, const glm::vec4 & Value)
{
	if (UniformLocationCache.find(Name) == UniformLocationCache.end())
	{
		UniformLocationCache[Name] = glGetUniformLocation(RendererID, Name.c_str());
	}
	glUniform4fv(UniformLocationCache[Name], 1, &Value[0]);
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

void FGLShader::SetLight(FDirectionalLight DirLight, FPointLight PointLight, FSpotLight SpotLight)
{
	SetUniform3fv("directionalLight.direction", DirLight.Direction);
	SetUniform3fv("directionalLight.ambient", DirLight.Ambient);
	SetUniform3fv("directionalLight.diffuse", DirLight.Diffuse);
	SetUniform3fv("directionalLight.specular", DirLight.Specular);
	SetUniform1i("directionalLight.isEnable", int(DirLight.bIsDirectionalLightEnable));

	SetUniform3fv("pointLight.position", PointLight.Position);
	SetUniform3fv("pointLight.ambient", PointLight.Ambient);
	SetUniform3fv("pointLight.diffuse", PointLight.Diffuse);
	SetUniform3fv("pointLight.specular", PointLight.Specular);
	SetUniform1f("pointLight.constant", PointLight.Constant);
	SetUniform1f("pointLight.linear", PointLight.Linear);
	SetUniform1f("pointLight.quadratic", PointLight.Quadratic);
	SetUniform1i("pointLight.isEnable", int(PointLight.bIsPointLightEnable));

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
	SetUniform1i("spotLight.isEnable", int(SpotLight.bIsSpotLightEnable));
}

void FGLShader::SetMatrix(glm::mat4 Model, glm::mat4 View, glm::mat4 Projection)
{
	SetUniformMat4("model", Model);
	SetUniformMat4("view", View);
	SetUniformMat4("projection", Projection);
}

void FGLShader::SetIsUnlit(bool bIsUnlit)
{
	SetUniform1i("isUnlit", int(bIsUnlit));
}

//void FGLShader::SetSpotLightEnable(bool bEnable)
//{
//	SetUniform1i("spotLightEnable", int(bEnable));
//}

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
		//char Name[128];
		//memset(Name, 0, sizeof(Name));
		//_snprintf_s(Name, sizeof(Name), "bones[%d]", i);
		std::string Name = fmt::format("bones[{}]", i);
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
		assert(false);
		break;
	}
}

void FGLShader::SetViewPosition(const glm::vec3 & ViewPosition)
{
	SetUniform3fv("viewPos", ViewPosition);
}

void FGLShader::SetModelMatrix(const glm::mat4& Model)
{
	SetUniformMat4("model", Model);
}

void FGLShader::SetLightSpaceMatrix(const glm::mat4 & Matrix)
{
	SetUniformMat4("lightSpaceMat", Matrix);
}

FGLShader * FGLShader::New(const std::string & VertexFilepath, const std::string & FragFilepath)
{
	FGLShader * Shader = nullptr;
	try
	{
		std::string VertexShaderSource = FGLShader::ParseShader(VertexFilepath);
		std::string FragmentShaderSource = FGLShader::ParseShader(FragFilepath);
		Shader = new FGLShader();
		Shader->VertexFilepath = VertexFilepath;
		Shader->FragmentFilepath = FragFilepath;
		Shader->RendererID = FGLShader::CreateShader(VertexShaderSource, FragmentShaderSource);
		return Shader;
	}
	catch (const std::exception E)
	{
		spdlog::error("{}, {}, {}", VertexFilepath, FragFilepath, E.what());
		if (Shader)
		{
			delete Shader;
		}
		return nullptr;
	}
}

FGLShader * FGLShader::Cache(const std::string & Name)
{
	FGLShader* Shader = FGLShader::Shaders[Name];
	return Shader;
}

FGLShader * FGLShader::NewOrCache(const std::string & VertexFilepath, const std::string & FragFilepath, const std::string & Name)
{
	FGLShader* Shader = Cache(Name);
	if (Shader == nullptr)
	{
		bool Result = LoadShader(VertexFilepath, FragFilepath, Name);
		if (Result)
		{
			Shader = Cache(Name);
		}
		else
		{
			return nullptr;
		}
	}
	return Shader;
}

bool FGLShader::LoadShader(const std::string & VertexFilepath, const std::string & FragFilepath, const std::string& Name)
{
	FGLShader* Shader = FGLShader::Shaders[Name];
	if (Shader == nullptr)
	{
		Shader = FGLShader::New(VertexFilepath, FragFilepath);
		if (Shader)
		{
			FGLShader::Shaders[Name] = Shader;
		}
	}

	if (Shader)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void FGLShader::LoadShaderSync()
{
	std::function<void(std::string)> Load = [](const std::string& Name)
	{
		LoadShader(FFileManager::Join({ ShadersFolder , Name + ".vert" }),
			FFileManager::Join({ ShadersFolder , Name + ".frag" }),
			Name);
	};
	std::vector<std::string> Filenames;
	Filenames.push_back("GLDepthMap");
	Filenames.push_back("GLDepthMapDebug");
	Filenames.push_back("GLFramebuffers");
	Filenames.push_back("GLSkeletonMesh");
	Filenames.push_back("GLSkybox");
	Filenames.push_back("GLStaticMesh");
	Filenames.push_back("GLSwirlFragmentShader");
	Filenames.push_back("GLUnlitStaticMesh");
	for (const std::string& Filename : Filenames)
	{
		Load(Filename);
	}
}
