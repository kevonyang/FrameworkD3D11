#pragma once

#include "Common.h"

class GraphicsDriverD3D11
{
public:
	GraphicsDriverD3D11();
	~GraphicsDriverD3D11();

	bool Initialize(HWND hWnd, int width, int height, bool fullScreen, bool vSync);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	DirectX::XMMATRIX GetWorldMatrix(){ return _worldMatrix; }
	DirectX::XMMATRIX GetProjectionMatrix() { return _projectionMatrix; }
	DirectX::XMMATRIX GetOrthoMatrix() { return _orthoMatrix; }
	ID3D11Device *GetD3D11Device() { return _device; }
	ID3D11DeviceContext *GetD3D11DeviceContext(){ return _deviceContext; }

	void SwitchZBuffer(bool on);
	void SwitchBlend(bool on);
private:
	ID3D11Device *_device;
	ID3D11DeviceContext *_deviceContext;
	IDXGISwapChain *_swapChain;
	D3D_FEATURE_LEVEL _featureLevel;
	ID3D11Texture2D *_depthStencilTex;
	ID3D11DepthStencilState *_depthStencilState;
	ID3D11DepthStencilState *_depthDisableStencilState;
	ID3D11DepthStencilView *_depthStencilView;
	ID3D11RenderTargetView *_renderTargetView;
	ID3D11RasterizerState *_rasterState;
	ID3D11BlendState *_blendEnableState, *_blendDisableState;
	int _videoMemory;
	char _videoDescription[128];
	bool _vSync;

	DirectX::XMMATRIX _worldMatrix;
	DirectX::XMMATRIX _viewMatrix;
	DirectX::XMMATRIX _projectionMatrix;
	DirectX::XMMATRIX _orthoMatrix;
};

