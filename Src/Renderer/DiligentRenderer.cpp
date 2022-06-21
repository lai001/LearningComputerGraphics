#include "DiligentRenderer.hpp"
#include <assert.h>
#include <Foundation/Foundation.hpp>

FDiligentRenderer::FDiligentRenderer(HWND hWnd)
	:hWnd(hWnd)
{
	bool status = InitializeDiligentEngine(hWnd);
	assert(status);
	assert(Device);
	assert(ImmediateContext);
	assert(SwapChain);
	const ks::PixelBuffer PixelBuffer = ks::PixelBuffer(512, 512, ks::PixelBuffer::FormatType::rgba8);
	DefaultTexture2D = ToTexture2D("DefaultTexture2D", &PixelBuffer);
}

FDiligentRenderer::~FDiligentRenderer()
{
}

bool FDiligentRenderer::InitializeDiligentEngine(HWND hWnd)
{
	using namespace Diligent;
	SwapChainDesc SCDesc;
	switch (DeviceType)
	{
#if D3D11_SUPPORTED
	case RENDER_DEVICE_TYPE_D3D11:
	{
		EngineD3D11CreateInfo EngineCI;
#    if ENGINE_DLL
		auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#    endif
		auto* pFactoryD3D11 = GetEngineFactoryD3D11();
		pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &Device, &ImmediateContext);
		Win32NativeWindow Window{ hWnd };
		pFactoryD3D11->CreateSwapChainD3D11(Device, ImmediateContext, SCDesc, FullScreenModeDesc{}, Window, &SwapChain);
	}
	break;
#endif


#if D3D12_SUPPORTED
	case RENDER_DEVICE_TYPE_D3D12:
	{
#    if ENGINE_DLL
		auto GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#    endif
		EngineD3D12CreateInfo EngineCI;

		auto* pFactoryD3D12 = GetEngineFactoryD3D12();
		pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &Device, &ImmediateContext);
		Win32NativeWindow Window{ hWnd };
		pFactoryD3D12->CreateSwapChainD3D12(Device, ImmediateContext, SCDesc, FullScreenModeDesc{}, Window, &SwapChain);
	}
	break;
#endif


#if GL_SUPPORTED
	case RENDER_DEVICE_TYPE_GL:
	{
#    if EXPLICITLY_LOAD_ENGINE_GL_DLL
		auto GetEngineFactoryOpenGL = LoadGraphicsEngineOpenGL();
#    endif
		auto* pFactoryOpenGL = GetEngineFactoryOpenGL();

		EngineGLCreateInfo EngineCI;
		EngineCI.Window.hWnd = hWnd;

		pFactoryOpenGL->CreateDeviceAndSwapChainGL(EngineCI, &Device, &ImmediateContext, SCDesc, &SwapChain);
	}
	break;
#endif


#if VULKAN_SUPPORTED
	case RENDER_DEVICE_TYPE_VULKAN:
	{
#    if EXPLICITLY_LOAD_ENGINE_VK_DLL
		auto GetEngineFactoryVk = LoadGraphicsEngineVk();
#    endif
		EngineVkCreateInfo EngineCI;

		auto* pFactoryVk = GetEngineFactoryVk();
		pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &Device, &ImmediateContext);

		if (!SwapChain && hWnd != nullptr)
		{
			Win32NativeWindow Window{ hWnd };
			pFactoryVk->CreateSwapChainVk(Device, ImmediateContext, SCDesc, Window, &SwapChain);
		}
	}
	break;
#endif


	default:
		return false;
		break;
	}

	return true;
}

Diligent::RefCntAutoPtr<Diligent::IShader> FDiligentRenderer::VertexShader(const std::string& Name)
{
	const std::string VSSource = ks::File::read(ks::Application::getResourcePath("Shader/" + Name), nullptr);
	Diligent::ShaderCreateInfo ShaderCI;
	ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
	ShaderCI.UseCombinedTextureSamplers = true;
	ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
	ShaderCI.EntryPoint = "main";
	ShaderCI.Desc.Name = Name.c_str();
	ShaderCI.Source = VSSource.c_str();
	Diligent::RefCntAutoPtr<Diligent::IShader> Shader;
	Device->CreateShader(ShaderCI, &Shader);
	assert(Shader);
	return Shader;
}

Diligent::RefCntAutoPtr<Diligent::IShader> FDiligentRenderer::FragmentShader(const std::string& Name)
{
	const std::string PSSource = ks::File::read(ks::Application::getResourcePath("Shader/" + Name), nullptr);
	Diligent::ShaderCreateInfo ShaderCI;
	ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
	ShaderCI.UseCombinedTextureSamplers = true;
	ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
	ShaderCI.EntryPoint = "main";
	ShaderCI.Desc.Name = Name.c_str();
	ShaderCI.Source = PSSource.c_str();
	Diligent::RefCntAutoPtr<Diligent::IShader> Shader;
	Device->CreateShader(ShaderCI, &Shader);
	assert(Shader);
	return Shader;
}

void FDiligentRenderer::ClearDepth()
{
	auto* pDSV = SwapChain->GetDepthBufferDSV();
	ImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void FDiligentRenderer::ClearColor()
{
	float m_ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	auto pRTV = SwapChain->GetCurrentBackBufferRTV();
	ImmediateContext->ClearRenderTarget(pRTV, m_ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void FDiligentRenderer::ClearStencil()
{
	auto* pDSV = SwapChain->GetDepthBufferDSV();
	ImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_STENCIL_FLAG, 1.0f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FDiligentRenderer::VertexBuffer(
	const std::string & Name,
	const void * BufferData,
	unsigned int Count, 
	unsigned int Stride)
{
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer;
	Diligent::BufferDesc VertBuffDesc;
	VertBuffDesc.Name = Name.c_str();
	VertBuffDesc.Usage = Diligent::USAGE_IMMUTABLE;
	VertBuffDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
	VertBuffDesc.Size = Count * Stride;
	Diligent::BufferData VBData;
	VBData.pData = BufferData;
	VBData.DataSize = VertBuffDesc.Size;
	Device->CreateBuffer(VertBuffDesc, &VBData, &VertexBuffer);
	assert(VertexBuffer);
	return VertexBuffer;
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FDiligentRenderer::IndexBuffer(const std::string & Name, 
	const void * BufferData,
	unsigned int Count)
{
	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer;
	Diligent::BufferDesc IndBuffDesc;
	IndBuffDesc.Name = Name.c_str();
	IndBuffDesc.Usage = Diligent::USAGE_IMMUTABLE;
	IndBuffDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
	IndBuffDesc.Size = Count * sizeof(unsigned int);
	Diligent::BufferData IBData;
	IBData.pData = BufferData;
	IBData.DataSize = IndBuffDesc.Size;
	Device->CreateBuffer(IndBuffDesc, &IBData, &IndexBuffer);
	assert(IndexBuffer);
	return IndexBuffer;
}

Diligent::GraphicsPipelineDesc FDiligentRenderer::DefaultGraphicsPipelineDesc()
{
	Diligent::GraphicsPipelineDesc GraphicsPipeline;
	GraphicsPipeline.NumRenderTargets = 1;
	GraphicsPipeline.RTVFormats[0] = SwapChain->GetDesc().ColorBufferFormat;
	GraphicsPipeline.DSVFormat = SwapChain->GetDesc().DepthBufferFormat;
	GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
	return GraphicsPipeline;
}

Diligent::RefCntAutoPtr<Diligent::IPipelineState> FDiligentRenderer::DefaultPipelineState(
	const std::string& Name,
	const std::string& VSName,
	const std::string& PSName,
	Diligent::InputLayoutDesc& InputLayoutDesc,
	Diligent::PipelineResourceLayoutDesc& PipelineResourceLayoutDesc)
{
	Diligent::GraphicsPipelineDesc GraphicsPipeline = DefaultGraphicsPipelineDesc();
	GraphicsPipeline.InputLayout = InputLayoutDesc;
	Diligent::RefCntAutoPtr<Diligent::IShader> pVS = VertexShader(VSName);
	Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
	if (PSName.empty() == false)
	{
		pPS = FragmentShader(PSName);
	}
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PipelineState;
	Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;
	PSOCreateInfo.PSODesc.Name = Name.c_str();
	PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
	PSOCreateInfo.PSODesc.ResourceLayout = PipelineResourceLayoutDesc;
	PSOCreateInfo.GraphicsPipeline = GraphicsPipeline;
	PSOCreateInfo.pPS = pPS;
	PSOCreateInfo.pVS = pVS;
	Device->CreateGraphicsPipelineState(PSOCreateInfo, &PipelineState);
	assert(PipelineState);
	return PipelineState;
}

Diligent::RefCntAutoPtr<Diligent::IPipelineState> FDiligentRenderer::PipelineState(
	const std::string & Name, 
	const std::string& VSName,
	const std::string& PSName,
	Diligent::PipelineResourceLayoutDesc & PipelineResourceLayoutDesc,
	Diligent::GraphicsPipelineDesc GraphicsPipeline)
{
	Diligent::RefCntAutoPtr<Diligent::IShader> pVS = VertexShader(VSName);
	Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
	if (PSName.empty() == false)
	{
		pPS = FragmentShader(PSName);
	}
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PipelineState;
	Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;
	PSOCreateInfo.PSODesc.Name = Name.c_str();
	PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
	PSOCreateInfo.PSODesc.ResourceLayout = PipelineResourceLayoutDesc;
	PSOCreateInfo.GraphicsPipeline = GraphicsPipeline;
	PSOCreateInfo.pPS = pPS;
	PSOCreateInfo.pVS = pVS;
	Device->CreateGraphicsPipelineState(PSOCreateInfo, &PipelineState);
	assert(PipelineState);
	return PipelineState;
}

void FDiligentRenderer::SetPipelineState(Diligent::IPipelineState * pPipelineState)
{
	ImmediateContext->SetPipelineState(pPipelineState);
}

void FDiligentRenderer::Draw(Diligent::DrawAttribs DrawAttribs)
{
	ImmediateContext->Draw(DrawAttribs);
}

void FDiligentRenderer::DrawIndexed(Diligent::DrawIndexedAttribs DrawIndexedAttribs)
{
	ImmediateContext->DrawIndexed(DrawIndexedAttribs);
}

void FDiligentRenderer::Present(unsigned int SyncInterval)
{
	SwapChain->Present(SyncInterval);
}

void FDiligentRenderer::SetRenderTarget()
{
	Diligent::ITextureView* RTV = SwapChain->GetCurrentBackBufferRTV();
	Diligent::ITextureView* DSV = SwapChain->GetDepthBufferDSV();
	ImmediateContext->SetRenderTargets(1, &RTV, DSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

Diligent::RefCntAutoPtr<Diligent::IRenderDevice> FDiligentRenderer::GetDevice() const
{
	return Device;
}

Diligent::RefCntAutoPtr<Diligent::IDeviceContext> FDiligentRenderer::GetImmediateContext() const
{
	return ImmediateContext;
}

Diligent::RefCntAutoPtr<Diligent::ISwapChain> FDiligentRenderer::GetSwapChain() const
{
	return SwapChain;
}

HWND FDiligentRenderer::GetHWND() const
{
	return hWnd;
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FDiligentRenderer::CreateConstantsBuffer(
	const std::string & Name,
	void * InitialData, 
	unsigned long Size,
	Diligent::USAGE Usage,
	Diligent::BIND_FLAGS BindFlags, 
	Diligent::CPU_ACCESS_FLAGS CPUAccessFlags)
{
	Diligent::RefCntAutoPtr<Diligent::IBuffer> CBBuffer;

	Diligent::BufferDesc CBDesc;
	CBDesc.Name = Name.c_str();
	CBDesc.Size = Size;
	CBDesc.Usage = Usage;
	CBDesc.BindFlags = BindFlags;
	CBDesc.CPUAccessFlags = CPUAccessFlags;
	Diligent::BufferData InitialBufferData;
	if (InitialData != nullptr)
	{
		InitialBufferData.pData = InitialData;
		InitialBufferData.DataSize = Size;
	}
	Device->CreateBuffer(CBDesc, InitialData != nullptr ? &InitialBufferData : nullptr, &CBBuffer);
	assert(CBBuffer);
	return CBBuffer;
}

glm::ivec2 FDiligentRenderer::CurrentBackBufferSize() noexcept
{
	const Diligent::SwapChainDesc & SwapChainDesc = SwapChain->GetDesc();
	return glm::ivec2(SwapChainDesc.Width, SwapChainDesc.Height);
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FDiligentRenderer::CreateTextureCube(
	const std::string& Name,
	const std::array<ks::PixelBuffer*, 6>& PixelBuffers)
{
	Diligent::TextureDesc TextureDesc;
	TextureDesc.Name = Name.c_str();
	TextureDesc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
	TextureDesc.Usage = Diligent::USAGE_DEFAULT;
	TextureDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
	TextureDesc.Type = Diligent::RESOURCE_DIM_TEX_CUBE;
	TextureDesc.Width = PixelBuffers[0]->getWidth();
	TextureDesc.Height = PixelBuffers[0]->getHeight();
	TextureDesc.ArraySize = PixelBuffers.size();
	TextureDesc.MipLevels = 1;

	std::array<Diligent::TextureSubResData, 6> m_SubResources;
	for (size_t i = 0; i < m_SubResources.size(); i++)
	{
		m_SubResources[i].Stride = TextureDesc.Width * 4;
		m_SubResources[i].pData = PixelBuffers[i]->getImmutableData()[0];
	}
	Diligent::TextureData TextureData{ m_SubResources.data(), static_cast<unsigned int>(m_SubResources.size()) };
	Diligent::RefCntAutoPtr<Diligent::ITexture> TextureCube;
	Device->CreateTexture(TextureDesc, &TextureData, &TextureCube);
	assert(TextureCube);
	return TextureCube;
}

void FDiligentRenderer::PipelineResourceLayoutDesc(
	const Diligent::SHADER_TYPE ShaderType,
	const std::vector<std::string>* TextureNames,
	Diligent::PipelineResourceLayoutDesc* OutPipelineResourceLayoutDesc,
	std::vector<Diligent::ShaderResourceVariableDesc>* OutShaderResourceVariableDescs,
	std::vector<Diligent::ImmutableSamplerDesc>* OutImtblSamplers)
{
	const Diligent::SamplerDesc SamLinearClampDesc
	{
		Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR,
		Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP
	};
	for (const std::string& TextureName : *TextureNames)
	{
		OutShaderResourceVariableDescs->push_back({ ShaderType, TextureName.c_str(), Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });
		OutImtblSamplers->push_back({ ShaderType, TextureName.c_str(), SamLinearClampDesc });
	}
	OutPipelineResourceLayoutDesc->DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
	OutPipelineResourceLayoutDesc->Variables = OutShaderResourceVariableDescs->data();
	OutPipelineResourceLayoutDesc->NumVariables = OutShaderResourceVariableDescs->size();
	OutPipelineResourceLayoutDesc->ImmutableSamplers = OutImtblSamplers->data();
	OutPipelineResourceLayoutDesc->NumImmutableSamplers = OutImtblSamplers->size();
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FDiligentRenderer::ToTexture2D(
	const std::string & Name, 
	const ks::PixelBuffer * PixelBuffer)
{
	assert(PixelBuffer);
	assert(PixelBuffer->getType() == ks::PixelBuffer::FormatType::rgba8);
	std::vector<Diligent::TextureSubResData>  m_SubResources;
	m_SubResources.resize(1);
	m_SubResources[0].Stride = PixelBuffer->getWidth() * 4;
	m_SubResources[0].pData = PixelBuffer->getImmutableData()[0];
	Diligent::TextureData InitData{ m_SubResources.data(), static_cast<unsigned int>(m_SubResources.size()) };
	Diligent::RefCntAutoPtr<Diligent::ITexture> TextureResource;
	Diligent::TextureDesc TexDesc;
	TexDesc.Name = Name.c_str();
	TexDesc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
	TexDesc.Usage = Diligent::USAGE_DYNAMIC;
	TexDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
	TexDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
	TexDesc.Width = PixelBuffer->getWidth();
	TexDesc.Height = PixelBuffer->getHeight();
	GetDevice()->CreateTexture(TexDesc, &InitData, &TextureResource);
	assert(TextureResource);
	return TextureResource;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FDiligentRenderer::GetDefaultTexture2D() const noexcept
{
	return DefaultTexture2D;
}
