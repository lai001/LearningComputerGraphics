#include "IBLBake.hpp"
#include <functional>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include "Util/Math.hpp"
#include "Util/Util.hpp"

FIBLBake::FIBLBake(const FIBLBake::BakeInfo& Info,
	const ks::PixelBuffer& EquirectangularHDRPixelBuffer,
	FDiligentRenderer& Renderer)
	:Info(Info), EquirectangularHDRPixelBuffer(&EquirectangularHDRPixelBuffer), Renderer(&Renderer)
{
	assert(Info.BRDFLUTMapLength > 0);
	assert(Info.EnvironmentCubeMapLength > 0);
	assert(Info.IrradianceCubeMapLength > 0);
	assert(Info.PreFilterCubeMapLength > 4);
	assert(Info.PreFilterCubeMapMaxMipmapLevel > 1);
	assert(EquirectangularHDRPixelBuffer.getType() == ks::PixelBuffer::FormatType::rgba8);

	spdlog::info("CreateEnvironmentCubeMapPixelBuffers");
	EnvironmentCubeMapPixelBuffers = CreateEnvironmentCubeMapPixelBuffers();
	spdlog::info("CreateEnvironmentCubeMapTexture");
	EnvironmentCubeMapTexture = CreateEnvironmentCubeMapTexture();
	spdlog::info("CreateIrradianceCubeMapPixelBuffers");
	IrradianceCubeMapPixelBuffers = CreateIrradianceCubeMapPixelBuffers();
	spdlog::info("CreateIrradianceCubeMapTexture");
	IrradianceCubeMapTexture = CreateIrradianceCubeMapTexture();
	spdlog::info("CreatePreFilterCubeMapTexture");
	PreFilterCubeMapTexture = CreatePreFilterCubeMapTexture();
	spdlog::info("CreatePreFilterCubeMapPixelBuffers");
	PreFilterCubeMapPixelBuffers = CreatePreFilterCubeMapPixelBuffers();
	spdlog::info("CreateBRDFLUTMapPixelBuffer");
	BRDFLUTMapPixelBuffer = CreateBRDFLUTMapPixelBuffer();
	spdlog::info("CreateBRDFLUTMapTexture");
	BRDFLUTMapTexture = CreateBRDFLUTMapTexture();
	spdlog::info("Done");

	assert(EnvironmentCubeMapTexture);
	assert(IrradianceCubeMapTexture);
	assert(PreFilterCubeMapTexture);
	assert(BRDFLUTMapTexture);
}

FIBLBake::~FIBLBake()
{
	for (ks::PixelBuffer* Buffer : EnvironmentCubeMapPixelBuffers)
	{
		assert(Buffer);
		delete Buffer;
	}
	for (ks::PixelBuffer* Buffer : IrradianceCubeMapPixelBuffers)
	{
		assert(Buffer);
		delete Buffer;
	}
	assert(BRDFLUTMapPixelBuffer);
	delete BRDFLUTMapPixelBuffer;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::GetEnvironmentCubeMapTexture() const
{
	return EnvironmentCubeMapTexture;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::GetIrradianceCubeMapTexture() const
{
	return IrradianceCubeMapTexture;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::GetPreFilterCubeMapTexture() const
{
	return PreFilterCubeMapTexture;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::GetBRDFLUTMapTexture() const
{
	return BRDFLUTMapTexture;
}

std::array<ks::PixelBuffer*, 6> FIBLBake::GetEnvironmentCubeMapPixelBuffers() const
{
	return EnvironmentCubeMapPixelBuffers;
}

std::array<ks::PixelBuffer*, 6> FIBLBake::GetIrradianceCubeMapPixelBuffers() const
{
	return IrradianceCubeMapPixelBuffers;
}

std::vector<std::array<ks::PixelBuffer*, 6>> FIBLBake::GetPreFilterCubeMapPixelBuffers() const
{
	return PreFilterCubeMapPixelBuffers;
}

ks::PixelBuffer* FIBLBake::GetBRDFLUTMapPixelBuffer() const
{
	return BRDFLUTMapPixelBuffer;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::CreateEnvironmentCubeMapTexture()
{
	return Renderer->CreateTextureCube("EnvironmentCubeMapTexture", EnvironmentCubeMapPixelBuffers);
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::CreateIrradianceCubeMapTexture()
{
	return Renderer->CreateTextureCube("IrradianceCubeMapTexture", IrradianceCubeMapPixelBuffers);
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::CreateBRDFLUTMapTexture()
{
	return Renderer->ToTexture2D("BRDFLUTMapTexture", BRDFLUTMapPixelBuffer);
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FIBLBake::CreatePreFilterCubeMapTexture()
{
	std::function<unsigned int(unsigned int)> CalculateMipmapLevel = [](unsigned int Length)
	{
		unsigned int MipmapLevel = 1;
		while (Length > 4)
		{
			Length /= 2;
			MipmapLevel += 1;
		}
		return MipmapLevel;
	};

	struct PreFilterEnvironmentCubeMapConstants
	{
		float Roughness = 0.0f;
	};
	
	PreFilterEnvironmentCubeMapConstants PreFilterEnvironmentCubeMapConstants;
	const unsigned int ActualMaxMipmapLevel = glm::min(CalculateMipmapLevel(Info.PreFilterCubeMapLength), Info.PreFilterCubeMapMaxMipmapLevel);
	assert(ActualMaxMipmapLevel > 1);
	const float RoughnessDelta = 1.0f / ((float)ActualMaxMipmapLevel - 1.0f);

	Diligent::PipelineResourceLayoutDesc PipelineResourceLayoutDesc;
	PipelineResourceLayoutDesc.DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
	const std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs = {
		{ Diligent::SHADER_TYPE_COMPUTE, "SpecularMapFilterSettings", Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE },
		{ Diligent::SHADER_TYPE_COMPUTE, "environmentCubeMap", Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE },
	};
	const Diligent::SamplerDesc SamLinearClampDesc
	{
		Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR,
		Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP
	};
	const std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers = {
		{ Diligent::SHADER_TYPE_COMPUTE, "environmentCubeMap", SamLinearClampDesc }
	};
	PipelineResourceLayoutDesc.Variables = ShaderResourceVariableDescs.data();
	PipelineResourceLayoutDesc.NumVariables = ShaderResourceVariableDescs.size();
	PipelineResourceLayoutDesc.ImmutableSamplers = ImtblSamplers.data();
	PipelineResourceLayoutDesc.NumImmutableSamplers = ImtblSamplers.size();
	
	Diligent::RefCntAutoPtr<Diligent::IShader> CSShader = Renderer->ComputeShader("PreFilterEnvironmentCubeMap.hlsl");
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> ComputePipelineState;
	Diligent::ComputePipelineStateCreateInfo PSOCreateInfo;
	Diligent::PipelineStateDesc& PSODesc = PSOCreateInfo.PSODesc;
	PSODesc.Name = CSShader->GetDesc().Name;
	PSODesc.PipelineType = Diligent::PIPELINE_TYPE_COMPUTE;
	PSODesc.ResourceLayout = PipelineResourceLayoutDesc;
	PSOCreateInfo.pCS = CSShader;
	Renderer->GetDevice()->CreateComputePipelineState(PSOCreateInfo, &ComputePipelineState);
	assert(ComputePipelineState);
	Diligent::RefCntAutoPtr<Diligent::IBuffer> CSConstants = Renderer->CreateConstantsBuffer("PreFilterEnvironmentCubeMapConstants",
		nullptr,
		sizeof(PreFilterEnvironmentCubeMapConstants));
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> SRB;
	ComputePipelineState->CreateShaderResourceBinding(&SRB);

	Diligent::TextureDesc PreFilterTextureDesc;
	PreFilterTextureDesc.Type = Diligent::RESOURCE_DIMENSION::RESOURCE_DIM_TEX_2D_ARRAY;
	PreFilterTextureDesc.ArraySize = 6;
	PreFilterTextureDesc.MipLevels = ActualMaxMipmapLevel;
	PreFilterTextureDesc.CPUAccessFlags = Diligent::CPU_ACCESS_FLAGS::CPU_ACCESS_READ;
	PreFilterTextureDesc.Format = Diligent::TEXTURE_FORMAT::TEX_FORMAT_RGBA8_UNORM;
	PreFilterTextureDesc.Width = Info.PreFilterCubeMapLength;
	PreFilterTextureDesc.Height = Info.PreFilterCubeMapLength;
	PreFilterTextureDesc.BindFlags = Diligent::BIND_FLAGS::BIND_UNORDERED_ACCESS;
	PreFilterTextureDesc.Usage = Diligent::USAGE::USAGE_DEFAULT;
	PreFilterTextureDesc.Name = "PreFilterTexture2DArray";
	Diligent::RefCntAutoPtr<Diligent::ITexture> PreFilterTexture2DArray;
	Renderer->GetDevice()->CreateTexture(PreFilterTextureDesc, nullptr, &PreFilterTexture2DArray);
	assert(PreFilterTexture2DArray);

	{
		const unsigned int ComputeHeight = Info.PreFilterCubeMapLength / (1 << (ActualMaxMipmapLevel - 1));
		const unsigned int ComputeWidth = ComputeHeight;
		const Diligent::DispatchComputeAttribs DCA(ComputeWidth / 32, ComputeHeight / 32, 6 / 1);
		assert(DCA.ThreadGroupCountX > 0);
		assert(DCA.ThreadGroupCountY > 0);
		assert(DCA.ThreadGroupCountZ > 0);
	}

	for (size_t CurrentMipmapLevel = 0; CurrentMipmapLevel < ActualMaxMipmapLevel; CurrentMipmapLevel++)
	{
		PreFilterEnvironmentCubeMapConstants.Roughness = RoughnessDelta * (float)CurrentMipmapLevel;
		FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
			&PreFilterEnvironmentCubeMapConstants,
			CSConstants);
		if (Diligent::IShaderResourceVariable* ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_COMPUTE, "prefilterMap"))
		{
			Diligent::TextureViewDesc TextureViewDesc;
			TextureViewDesc.Name = "prefilterMap";
			TextureViewDesc.ViewType = Diligent::TEXTURE_VIEW_UNORDERED_ACCESS;
			TextureViewDesc.TextureDim = Diligent::RESOURCE_DIM_TEX_2D_ARRAY;
			TextureViewDesc.Format = PreFilterTextureDesc.Format;
			TextureViewDesc.MostDetailedMip = CurrentMipmapLevel;
			TextureViewDesc.NumArraySlices = 6;
			TextureViewDesc.FirstArraySlice = 0;
			TextureViewDesc.AccessFlags = Diligent::UAV_ACCESS_FLAG::UAV_ACCESS_FLAG_READ_WRITE;
			TextureViewDesc.Flags = Diligent::TEXTURE_VIEW_FLAGS::TEXTURE_VIEW_FLAG_NONE;
			Diligent::ITextureView* ITextureView = nullptr;
			PreFilterTexture2DArray->CreateView(TextureViewDesc, &ITextureView);
			assert(ITextureView);
			ResourceVariable->Set(ITextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
		}
		if (Diligent::IShaderResourceVariable* ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_COMPUTE, "environmentCubeMap"))
		{
			ResourceVariable->Set(EnvironmentCubeMapTexture->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE),
				Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
		}
		if (Diligent::IShaderResourceVariable* ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_COMPUTE, "PreFilterEnvironmentCubeMapConstants"))
		{
			ResourceVariable->Set(CSConstants);
		}
		Renderer->GetImmediateContext()->SetPipelineState(ComputePipelineState);
		Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		const unsigned int ComputeHeight = Info.PreFilterCubeMapLength / (1 << CurrentMipmapLevel);
		const unsigned int ComputeWidth = ComputeHeight;
		const Diligent::DispatchComputeAttribs DCA(ComputeWidth / 32, ComputeHeight / 32, 6 / 1);
		Renderer->GetImmediateContext()->DispatchCompute(DCA);
	}

	PreFilterTextureDesc.Type = Diligent::RESOURCE_DIMENSION::RESOURCE_DIM_TEX_CUBE;
	PreFilterTextureDesc.BindFlags = Diligent::BIND_FLAGS::BIND_SHADER_RESOURCE;
	PreFilterTextureDesc.Usage = Diligent::USAGE::USAGE_DEFAULT;
	PreFilterTextureDesc.MiscFlags = Diligent::MISC_TEXTURE_FLAGS::MISC_TEXTURE_FLAG_NONE;
	PreFilterTextureDesc.Name = "PreFilterCubeTexture";

	Diligent::RefCntAutoPtr<Diligent::ITexture> PreFilterCubeTexture;
	Renderer->GetDevice()->CreateTexture(PreFilterTextureDesc, nullptr, &PreFilterCubeTexture);
	assert(PreFilterCubeTexture);
	Diligent::CopyTextureAttribs CopyAttribs(PreFilterTexture2DArray,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
		PreFilterCubeTexture,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

	for (unsigned int CurrentMipmapLevel = 0; CurrentMipmapLevel < ActualMaxMipmapLevel; CurrentMipmapLevel++)
	{
		for (unsigned int ArraySlice = 0; ArraySlice < 6; ArraySlice++)
		{
			CopyAttribs.SrcMipLevel = CurrentMipmapLevel;
			CopyAttribs.DstMipLevel = CurrentMipmapLevel;
			CopyAttribs.SrcSlice = ArraySlice;
			CopyAttribs.DstSlice = ArraySlice;
			Renderer->GetImmediateContext()->CopyTexture(CopyAttribs);
		}
	}
	return PreFilterCubeTexture;
}

std::array<ks::PixelBuffer*, 6> FIBLBake::CreateEnvironmentCubeMapPixelBuffers()
{
	std::array<ks::PixelBuffer*, 6> PixelBuffers;
	
	const unsigned int TargetWidth = Info.EnvironmentCubeMapLength;
	const unsigned int TargetHeight = Info.EnvironmentCubeMapLength;

	for (size_t i = 0; i < PixelBuffers.size(); i++)
	{
		ks::PixelBuffer* PixelBuffer = new ks::PixelBuffer(TargetWidth, TargetHeight, ks::PixelBuffer::FormatType::rgba8);
		PixelBuffers[i] = PixelBuffer;
		for (size_t HeightIdx = 0; HeightIdx < TargetHeight; HeightIdx++)
		{
			for (size_t WidthIdx = 0; WidthIdx < TargetWidth; WidthIdx++)
			{
				const glm::vec2 UV = glm::vec2((float)WidthIdx / (float)TargetWidth, 1.0 - (float)HeightIdx / (float)TargetHeight) * 2.0f - 1.0f;
				glm::vec3 SamplePicker;
				if (i == 0) { SamplePicker = glm::vec3(1.0f, UV.y, -UV.x); }
				if (i == 1) { SamplePicker = glm::vec3(-1.0, UV.y, UV.x); }
				if (i == 2) { SamplePicker = glm::vec3(UV.x, 1.0, -UV.y); }
				if (i == 3) { SamplePicker = glm::vec3(UV.x, -1.0, UV.y); }
				if (i == 4) { SamplePicker = glm::vec3(UV.x, UV.y, 1.0); }
				if (i == 5) { SamplePicker = glm::vec3(-UV.x, UV.y, -1.0); }
				SamplePicker = glm::normalize(SamplePicker);
				const unsigned char* ImmutableData = EquirectangularHDRPixelBuffer->getImmutableData()[0];
				unsigned int SourcePixelIndexX = ((atan2(SamplePicker.z, SamplePicker.x) + M_PI) / (M_PI * 2.0f)) * EquirectangularHDRPixelBuffer->getWidth();
				unsigned int SourcePixelIndexY = (acos(SamplePicker.y) / M_PI) * EquirectangularHDRPixelBuffer->getHeight();
				SourcePixelIndexX = glm::clamp<unsigned int>(SourcePixelIndexX, 0, EquirectangularHDRPixelBuffer->getWidth() - 1);
				SourcePixelIndexY = glm::clamp<unsigned int>(SourcePixelIndexY, 0, EquirectangularHDRPixelBuffer->getHeight() - 1);
				const unsigned int SourcePixelIndex = SourcePixelIndexY * EquirectangularHDRPixelBuffer->getWidth() + SourcePixelIndexX;
				const unsigned int TargetPixelIndex = HeightIdx * TargetWidth + WidthIdx;
				memcpy(PixelBuffer->getMutableData()[0] + (TargetPixelIndex * 4),
					ImmutableData + (SourcePixelIndex * 4),
					4);
			}
		}
	}

	return PixelBuffers;
}

std::vector<std::array<ks::PixelBuffer*, 6>> FIBLBake::CreatePreFilterCubeMapPixelBuffers()
{
	Diligent::TextureDesc PreFilterTextureDesc;
	PreFilterTextureDesc.ArraySize = 6;
	PreFilterTextureDesc.MipLevels = PreFilterCubeMapTexture->GetDesc().MipLevels;
	PreFilterTextureDesc.CPUAccessFlags = Diligent::CPU_ACCESS_FLAGS::CPU_ACCESS_READ;
	PreFilterTextureDesc.Format = Diligent::TEXTURE_FORMAT::TEX_FORMAT_RGBA8_UNORM;
	PreFilterTextureDesc.Width = Info.PreFilterCubeMapLength;
	PreFilterTextureDesc.Height = Info.PreFilterCubeMapLength;
	PreFilterTextureDesc.Type = Diligent::RESOURCE_DIMENSION::RESOURCE_DIM_TEX_CUBE;
	PreFilterTextureDesc.BindFlags = Diligent::BIND_FLAGS::BIND_NONE;
	PreFilterTextureDesc.Usage = Diligent::USAGE::USAGE_STAGING;
	PreFilterTextureDesc.MiscFlags = Diligent::MISC_TEXTURE_FLAGS::MISC_TEXTURE_FLAG_NONE;
	PreFilterTextureDesc.Name = "PreFilterCubeStagingTexture";

	Diligent::RefCntAutoPtr<Diligent::ITexture> PreFilterCubeStagingTexture;
	Renderer->GetDevice()->CreateTexture(PreFilterTextureDesc, nullptr, &PreFilterCubeStagingTexture);
	assert(PreFilterCubeStagingTexture);
	Diligent::CopyTextureAttribs CopyAttribs(PreFilterCubeMapTexture,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
		PreFilterCubeStagingTexture,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

	for (unsigned int CurrentMipmapLevel = 0; CurrentMipmapLevel < PreFilterTextureDesc.MipLevels; CurrentMipmapLevel++)
	{
		for (unsigned int ArraySlice = 0; ArraySlice < 6; ArraySlice++)
		{
			CopyAttribs.SrcMipLevel = CurrentMipmapLevel;
			CopyAttribs.DstMipLevel = CurrentMipmapLevel;
			CopyAttribs.SrcSlice = ArraySlice;
			CopyAttribs.DstSlice = ArraySlice;
			Renderer->GetImmediateContext()->CopyTexture(CopyAttribs);
		}
	}

	std::vector<std::array<ks::PixelBuffer*, 6>> LocalPreFilterCubeMapPixelBuffers;
	for (unsigned int CurrentMipmapLevel = 0; CurrentMipmapLevel < PreFilterTextureDesc.MipLevels; CurrentMipmapLevel++)
	{
		std::array<ks::PixelBuffer*, 6> PixelBuffers = Renderer->CreatePixelBufferFrom2DArrayTexture(*PreFilterCubeStagingTexture, CurrentMipmapLevel);
		LocalPreFilterCubeMapPixelBuffers.push_back(PixelBuffers);
	}
	return LocalPreFilterCubeMapPixelBuffers;
}

std::array<ks::PixelBuffer*, 6> FIBLBake::CreateIrradianceCubeMapPixelBuffers()
{
	std::array<ks::PixelBuffer*, 6> LocalIrradianceCubeMapPixelBuffers = std::array<ks::PixelBuffer*, 6>();
	const unsigned int TargetWidth = Info.IrradianceCubeMapLength;
	const unsigned int TargetHeight = Info.IrradianceCubeMapLength;

	std::vector<std::thread> Threads;

	for (size_t i = 0; i < LocalIrradianceCubeMapPixelBuffers.size(); i++)
	{
		std::thread Thread([=, &LocalIrradianceCubeMapPixelBuffers]()
		{
			LocalIrradianceCubeMapPixelBuffers[i] = new ks::PixelBuffer(TargetWidth, TargetHeight, ks::PixelBuffer::FormatType::rgba8);
			ks::PixelBuffer* PixelBuffer = LocalIrradianceCubeMapPixelBuffers[i];
			for (size_t HeightIdx = 0; HeightIdx < TargetHeight; HeightIdx++)
			{
				for (size_t WidthIdx = 0; WidthIdx < TargetWidth; WidthIdx++)
				{
					const glm::vec2 UV = glm::vec2((float)WidthIdx / (float)TargetWidth, 1.0 - (float)HeightIdx / (float)TargetHeight) * 2.0f - 1.0f;
					glm::vec3 SamplePicker;
					if (i == 0) { SamplePicker = glm::vec3(1.0f, UV.y, -UV.x); }
					if (i == 1) { SamplePicker = glm::vec3(-1.0, UV.y, UV.x); }
					if (i == 2) { SamplePicker = glm::vec3(UV.x, 1.0, -UV.y); }
					if (i == 3) { SamplePicker = glm::vec3(UV.x, -1.0, UV.y); }
					if (i == 4) { SamplePicker = glm::vec3(UV.x, UV.y, 1.0); }
					if (i == 5) { SamplePicker = glm::vec3(-UV.x, UV.y, -1.0); }
					SamplePicker = glm::normalize(SamplePicker);
					const unsigned char* ImmutableData = EquirectangularHDRPixelBuffer->getImmutableData()[0];
					constexpr unsigned int NumSamples = 1 << 12;
					constexpr glm::vec3 UpVector = glm::vec3(0.0, 1.0, 0.0);
					const glm::vec3 TangentVector = glm::normalize(glm::cross(SamplePicker, UpVector));
					const glm::vec3 BitangentVector = glm::normalize(glm::cross(SamplePicker, TangentVector));
					glm::vec3 Irradiance = glm::vec3(0.0f);
					for (size_t SampleIndex = 0; SampleIndex < NumSamples; SampleIndex++)
					{
						const glm::vec2 H = FMath::Hammersley2D(SampleIndex, NumSamples);
						const glm::vec3 L = FMath::ConvertCoordinateSystem(FMath::HemisphereSampleUniform(H.x, H.y), BitangentVector, TangentVector, SamplePicker);
						unsigned int SourcePixelIndexX = ((atan2(L.z, L.x) + M_PI) / (M_PI * 2.0f)) * EquirectangularHDRPixelBuffer->getWidth();
						unsigned int SourcePixelIndexY = (acos(L.y) / M_PI) * EquirectangularHDRPixelBuffer->getHeight();
						SourcePixelIndexX = glm::clamp<unsigned int>(SourcePixelIndexX, 0, EquirectangularHDRPixelBuffer->getWidth() - 1);
						SourcePixelIndexY = glm::clamp<unsigned int>(SourcePixelIndexY, 0, EquirectangularHDRPixelBuffer->getHeight() - 1);
						const unsigned int SourcePixelIndex = SourcePixelIndexY * EquirectangularHDRPixelBuffer->getWidth() + SourcePixelIndexX;
						const unsigned char* SourcePixel = ImmutableData + (SourcePixelIndex * 4);
						const glm::vec3 SourcePixelVec3 = glm::vec3((float)SourcePixel[0], (float)SourcePixel[1], (float)SourcePixel[2]) / 255.0f;
						Irradiance = Irradiance + (2.0f * SourcePixelVec3 * std::max(0.0f, glm::dot(L, SamplePicker)));
					}
					Irradiance = glm::clamp(Irradiance / (float)NumSamples, glm::vec3(0.0f), glm::vec3(1.0f));
					const glm::u8vec4 IrradianceU8Vec4 = glm::vec4(Irradiance, 1.0f) * 255.0f;
					const unsigned int TargetPixelIndex = HeightIdx * TargetWidth + WidthIdx;
					memcpy(PixelBuffer->getMutableData()[0] + (TargetPixelIndex * 4),
						glm::value_ptr(IrradianceU8Vec4),
						4);
				}
			}
		});
		Threads.push_back(std::forward<std::thread>(Thread));
	}
	for (std::thread& Thread : Threads)
	{
		Thread.join();
	}

	return LocalIrradianceCubeMapPixelBuffers;
}

ks::PixelBuffer* FIBLBake::CreateBRDFLUTMapPixelBuffer()
{
	std::function<glm::vec2(float, float)> IntegrateBRDF = [](float NdotV, float Roughness)
	{
		const glm::vec3 V(sqrt(1.0 - NdotV * NdotV), 0.0f, NdotV);

		float A = 0.0;
		float B = 0.0;

		const glm::vec3 N = glm::vec3(0.0, 0.0, 1.0);
		const glm::vec3 TangentVector = glm::normalize(glm::cross(glm::vec3(1.0, 0.0, 0.0), N));
		const glm::vec3 BitangentVector = glm::normalize(glm::cross(N, TangentVector));

		const unsigned int sampleCount = 1024u;
		for (unsigned int i = 0u; i < sampleCount; ++i)
		{
			glm::vec2 Xi = FMath::Hammersley2D(i, sampleCount);
			glm::vec3 H = FMath::ImportanceSampleGGX(Xi, Roughness);
			H = FMath::ConvertCoordinateSystem(H, TangentVector, BitangentVector, N);

			glm::vec3 L = glm::reflect(-V, H);

			float NdotL = glm::max(L.z, 0.0f);
			float NdotH = glm::max(H.z, 0.0f);
			float VdotH = glm::max(dot(V, H), 0.0f);

			if (NdotL > 0.0)
			{
				float G = FMath::GeometrySmith(N, V, L, Roughness);
				float G_Vis = (G * VdotH) / (NdotH * NdotV);
				float Fc = pow(1.0 - VdotH, 5.0);
				A += (1.0 - Fc) * G_Vis;
				B += Fc * G_Vis;
			}
		}
		A /= float(sampleCount);
		B /= float(sampleCount);
		return glm::vec2(A, B);
	};

	ks::PixelBuffer* LocalBRDFLUTMapPixelBuffer = new ks::PixelBuffer(Info.BRDFLUTMapLength, Info.BRDFLUTMapLength, ks::PixelBuffer::FormatType::rgba8);
	for (unsigned int HeightIdx = 0; HeightIdx < LocalBRDFLUTMapPixelBuffer->getHeight(); HeightIdx++)
	{
		for (unsigned int WidthIdx = 0; WidthIdx < LocalBRDFLUTMapPixelBuffer->getWidth(); WidthIdx++)
		{
			const glm::vec2 Texcoord = glm::vec2((float)WidthIdx / (float)LocalBRDFLUTMapPixelBuffer->getWidth(), 1.0f - (float)HeightIdx / (float)LocalBRDFLUTMapPixelBuffer->getHeight());
			glm::vec4 PixelColor = glm::vec4(IntegrateBRDF(Texcoord.x, Texcoord.y), 0.0f, 1.0f);
			PixelColor = glm::clamp(PixelColor, glm::vec4(0.0f), glm::vec4(1.0f));
			const glm::u8vec4 Color = glm::u8vec4(PixelColor * 255.0f);
			unsigned char* MutableData = LocalBRDFLUTMapPixelBuffer->getMutableData()[0];
			const unsigned int PixelIndex = HeightIdx * LocalBRDFLUTMapPixelBuffer->getWidth() + WidthIdx;
			reinterpret_cast<glm::u8vec4*>(MutableData)[PixelIndex] = Color;
		}
	}
	return LocalBRDFLUTMapPixelBuffer;
}
