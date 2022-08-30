#pragma once

#include <memory>
#include <iomanip>
#include <iostream>
#include <vector>
#include <array>
#include <Windows.h>
#include <crtdbg.h>
#include <glm/glm.hpp>
#include <Foundation/Foundation.hpp>

#include <DiligentCore/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <DiligentCore/Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <DiligentCore/Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <DiligentCore/Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

#include <DiligentCore/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/SwapChain.h>

#include <DiligentCore/Common/interface/RefCntAutoPtr.hpp>

class FDiligentRenderer
{
private:
	Diligent::RENDER_DEVICE_TYPE						DeviceType = Diligent::RENDER_DEVICE_TYPE_D3D11;
	Diligent::RefCntAutoPtr<Diligent::IEngineFactory>   EngineFactory;
	Diligent::RefCntAutoPtr<Diligent::IRenderDevice>	Device;
	Diligent::RefCntAutoPtr<Diligent::IDeviceContext>	ImmediateContext;
	Diligent::RefCntAutoPtr<Diligent::ISwapChain>		SwapChain;
	Diligent::GraphicsAdapterInfo						AdapterAttribs;
	std::vector<Diligent::DisplayModeAttribs>			DisplayModes;
	Diligent::RefCntAutoPtr<Diligent::ITexture>			DefaultTexture2D;
	Diligent::RefCntAutoPtr<Diligent::ITexture>			DefaultNormalTexture2D;
	HWND hWnd;

public:
	FDiligentRenderer(HWND hWnd);
	~FDiligentRenderer();

	bool InitializeDiligentEngine(HWND hWnd);

	Diligent::RefCntAutoPtr<Diligent::IShader> VertexShader(const std::string& Name);
	Diligent::RefCntAutoPtr<Diligent::IShader> FragmentShader(const std::string& Name);
	Diligent::RefCntAutoPtr<Diligent::IShader> ComputeShader(const std::string& Name);

	void ClearDepth();
	void ClearColor();
	void ClearStencil();

	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer(
		const std::string& Name, 
		const void* BufferData,
		unsigned int Count,
		unsigned int Stride);

	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer(
		const std::string& Name,
		const void* BufferData,
		unsigned int Count);

	Diligent::GraphicsPipelineDesc DefaultGraphicsPipelineDesc();

	Diligent::RefCntAutoPtr<Diligent::IPipelineState> DefaultPipelineState(
		const std::string& Name, 
		const std::string& VSName,
		const std::string& PSName,
		Diligent::InputLayoutDesc& InputLayoutDesc,
		Diligent::PipelineResourceLayoutDesc& PipelineResourceLayoutDesc);

	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PipelineState(
		const std::string& Name,
		const std::string& VSName,
		const std::string& PSName,
		Diligent::PipelineResourceLayoutDesc& PipelineResourceLayoutDesc,
		Diligent::GraphicsPipelineDesc GraphicsPipeline);

	void SetPipelineState(Diligent::IPipelineState* PipelineState);

	void Draw(Diligent::DrawAttribs DrawAttribs);

	void DrawIndexed(Diligent::DrawIndexedAttribs DrawIndexedAttribs);

	void Present(unsigned int SyncInterval);

	void SetRenderTarget();

	Diligent::RefCntAutoPtr<Diligent::IRenderDevice>	GetDevice() const;
	Diligent::RefCntAutoPtr<Diligent::IDeviceContext>	GetImmediateContext() const;
	Diligent::RefCntAutoPtr<Diligent::ISwapChain>		GetSwapChain() const;
	HWND GetHWND() const;

	Diligent::RefCntAutoPtr<Diligent::IBuffer> CreateConstantsBuffer(
		const std::string& Name,
		void* InitialData,
		unsigned long Size,
		Diligent::USAGE Usage = Diligent::USAGE_DYNAMIC,
		Diligent::BIND_FLAGS BindFlags  = Diligent::BIND_UNIFORM_BUFFER,
		Diligent::CPU_ACCESS_FLAGS CPUAccessFlags = Diligent::CPU_ACCESS_WRITE);

	glm::ivec2 CurrentBackBufferSize() noexcept;

	Diligent::RefCntAutoPtr<Diligent::ITexture> CreateTextureCube(
		const std::string& Name,
		const std::array<ks::PixelBuffer*, 6>& PixelBuffers);

	void PipelineResourceLayoutDesc(
		const Diligent::SHADER_TYPE ShaderType,
		const std::vector<std::string>* TextureNames,
		Diligent::PipelineResourceLayoutDesc* OutPipelineResourceLayoutDesc,
		std::vector<Diligent::ShaderResourceVariableDesc>* OutShaderResourceVariableDescs,
		std::vector<Diligent::ImmutableSamplerDesc>* OutImtblSamplers);

	Diligent::RefCntAutoPtr<Diligent::ITexture> ToTexture2D(
		const std::string& Name,
		const ks::PixelBuffer* PixelBuffer);

	Diligent::RefCntAutoPtr<Diligent::ITexture> GetDefaultTexture2D() const noexcept;

	Diligent::RefCntAutoPtr<Diligent::ITexture>	GetDefaultNormalTexture2D() const noexcept;

	ks::PixelBuffer* CreatePixelBuffer(Diligent::ITexture* Texture);

	std::array<ks::PixelBuffer*, 6> CreatePixelBufferFrom2DArrayTexture(Diligent::ITexture& CubeTexture, const unsigned int MipLevel = 0);

};
