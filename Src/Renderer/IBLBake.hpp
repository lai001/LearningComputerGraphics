#pragma once

#include <Foundation/Foundation.hpp>
#include "DiligentRenderer.hpp"

class FIBLBake: public ks::noncopyable
{
public:
	struct BakeInfo
	{
		unsigned int EnvironmentCubeMapLength;
		unsigned int IrradianceCubeMapLength;
		unsigned int PreFilterCubeMapLength;
		unsigned int PreFilterCubeMapMaxMipmapLevel;
		unsigned int BRDFLUTMapLength;
	};

public:
	FIBLBake(const FIBLBake::BakeInfo& Info,
		const ks::PixelBuffer& EquirectangularHDRPixelBuffer, 
		FDiligentRenderer& Renderer);
	~FIBLBake();

	Diligent::RefCntAutoPtr<Diligent::ITexture> GetEnvironmentCubeMapTexture() const;
	Diligent::RefCntAutoPtr<Diligent::ITexture>  GetIrradianceCubeMapTexture() const;
	Diligent::RefCntAutoPtr<Diligent::ITexture> GetPreFilterCubeMapTexture() const;
	Diligent::RefCntAutoPtr<Diligent::ITexture> GetBRDFLUTMapTexture() const;

	std::array<ks::PixelBuffer*, 6> GetEnvironmentCubeMapPixelBuffers() const;
	std::array<ks::PixelBuffer*, 6>  GetIrradianceCubeMapPixelBuffers() const;
	std::vector<std::array<ks::PixelBuffer*, 6>> GetPreFilterCubeMapPixelBuffers() const;
	ks::PixelBuffer* GetBRDFLUTMapPixelBuffer() const;

private:
	FIBLBake::BakeInfo Info;
	FDiligentRenderer* Renderer = nullptr;
	const ks::PixelBuffer* EquirectangularHDRPixelBuffer = nullptr;

	Diligent::RefCntAutoPtr<Diligent::ITexture> EnvironmentCubeMapTexture;
	Diligent::RefCntAutoPtr<Diligent::ITexture> IrradianceCubeMapTexture;
	Diligent::RefCntAutoPtr<Diligent::ITexture> PreFilterCubeMapTexture;
	Diligent::RefCntAutoPtr<Diligent::ITexture> BRDFLUTMapTexture;

	std::array<ks::PixelBuffer*, 6> EnvironmentCubeMapPixelBuffers;
	std::array<ks::PixelBuffer*, 6> IrradianceCubeMapPixelBuffers;
	std::vector<std::array<ks::PixelBuffer*, 6>> PreFilterCubeMapPixelBuffers;
	ks::PixelBuffer* BRDFLUTMapPixelBuffer = nullptr;

	Diligent::RefCntAutoPtr<Diligent::ITexture> CreateEnvironmentCubeMapTexture();
	Diligent::RefCntAutoPtr<Diligent::ITexture>  CreateIrradianceCubeMapTexture();
	Diligent::RefCntAutoPtr<Diligent::ITexture> CreatePreFilterCubeMapTexture();
	Diligent::RefCntAutoPtr<Diligent::ITexture> CreateBRDFLUTMapTexture();

private:
	std::array<ks::PixelBuffer*, 6> CreateEnvironmentCubeMapPixelBuffers();
	std::vector<std::array<ks::PixelBuffer*, 6>> CreatePreFilterCubeMapPixelBuffers();
	std::array<ks::PixelBuffer*, 6> CreateIrradianceCubeMapPixelBuffers();
	ks::PixelBuffer* CreateBRDFLUTMapPixelBuffer();
};