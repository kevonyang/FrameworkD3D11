#include "GraphicsDriverD3D11.h"

GraphicsDriverD3D11::GraphicsDriverD3D11()
{
}


GraphicsDriverD3D11::~GraphicsDriverD3D11()
{
}

bool GraphicsDriverD3D11::Initialize(HWND hWnd, int width, int height, bool fullScreen, bool vSync)
{
	_vSync = vSync;

	IDXGIFactory *factory;
	IDXGIAdapter *adapter;
	IDXGIOutput *adapterOutput;
	UINT numModes = 0;
	UINT numerator = 0;
	UINT denominator = 1;
	DXGI_MODE_DESC *displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	if (FAILED(res))
		return false;

	res = factory->EnumAdapters(0, &adapter);
	if (FAILED(res))
		return false;

	res = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(res))
		return false;

	//getnum
	res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(res))
		return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(res))
		return false;

	for (int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == width && displayModeList[i].Height == height)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	//adapterdesc and memory
	res = adapter->GetDesc(&adapterDesc);
	if (FAILED(res))
		return false;

	_videoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	UINT32 stringLength;
	errno_t err = wcstombs_s(&stringLength, _videoDescription, 128, adapterDesc.Description, 128);
	if (err != 0)
	{
		return false;
	}

	//release adapter and factory
	delete[]displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = NULL;

	//swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = !fullScreen;

	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
		&swapChainDesc, &_swapChain, &_device, &_featureLevel, &_deviceContext);
	if (FAILED(res))
		return false;

	ID3D11Texture2D *backBuffer;
	res = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);
	if (FAILED(res))
		return false;

	res = _device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
	backBuffer->Release();
	backBuffer = NULL;
	if (FAILED(res))
		return false;

	D3D11_TEXTURE2D_DESC depthStencilTexDesc;
	ZeroMemory(&depthStencilTexDesc, sizeof(depthStencilTexDesc));
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTexDesc.CPUAccessFlags = 0;
	depthStencilTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTexDesc.Width = width;
	depthStencilTexDesc.Height = height;
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.MiscFlags = 0;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.SampleDesc.Quality = 0;
	depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;
	
	res = _device->CreateTexture2D(&depthStencilTexDesc, NULL, &_depthStencilTex);
	if (FAILED(res))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	res = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(res))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
	ZeroMemory(&depthDisableStencilDesc, sizeof(depthDisableStencilDesc));
	depthDisableStencilDesc.DepthEnable = false;
	depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisableStencilDesc.StencilEnable = true;
	depthDisableStencilDesc.StencilReadMask = 0xFF;
	depthDisableStencilDesc.StencilWriteMask = 0xFF;

	depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	res = _device->CreateDepthStencilState(&depthDisableStencilDesc, &_depthDisableStencilState);
	if (FAILED(res))
		return false;

	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthStencilTexDesc.Format;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	res = _device->CreateDepthStencilView(_depthStencilTex, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(res))
		return false;
	
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	res = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(res))
		return false;

	_deviceContext->RSSetState(_rasterState);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	res = _device->CreateBlendState(&blendDesc, &_blendEnableState);
	if (FAILED(res))
		return false;

	blendDesc.RenderTarget[0].BlendEnable = false;
	res = _device->CreateBlendState(&blendDesc, &_blendDisableState);
	if (FAILED(res))
		return false;

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	_deviceContext->RSSetViewports(1, &vp);

	_worldMatrix = DirectX::XMMatrixIdentity();

	_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)width / (float)height, 1.0f, 100.0f);

	_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)width, (float)height, 1.0f, 100.0f);

	return true;
}

void GraphicsDriverD3D11::Shutdown()
{
	if (_deviceContext) _deviceContext->ClearState();

	if (_swapChain) _swapChain->SetFullscreenState(false, NULL);

	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = NULL;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = NULL;
	}

	if (_depthDisableStencilState)
	{
		_depthDisableStencilState->Release();
		_depthDisableStencilState = NULL;
	}

	if (_depthStencilTex)
	{
		_depthStencilTex->Release();
		_depthStencilTex = NULL;
	}
	
	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = NULL;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = NULL;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = NULL;
	}

	if (_device)
	{
		_device->Release();
		_device = NULL;
	}

	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = NULL;
	}
}

void GraphicsDriverD3D11::BeginScene(float red, float green, float blue, float alpha)
{
	static int start = 0;
	int now = (int)GetTickCount();
	if (start == 0)
		start = now;
	int delta = now - start;
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), 0.001 * delta);
	//_worldMatrix = DirectX::XMMatrixRotationQuaternion(rotation);

	float clearColor[4] = {red, green, blue, alpha};
	_deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void GraphicsDriverD3D11::EndScene()
{
	_swapChain->Present(_vSync ? 1 : 0, 0);
}

void GraphicsDriverD3D11::SwitchZBuffer(bool on)
{
	if (on)
		_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
	else
		_deviceContext->OMSetDepthStencilState(_depthDisableStencilState, 1);
}

void GraphicsDriverD3D11::SwitchBlend(bool on)
{
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (on)
		_deviceContext->OMSetBlendState(_blendEnableState, blendFactor, 0xffffffff);
	else
		_deviceContext->OMSetBlendState(_blendDisableState, blendFactor, 0xffffffff);
}
