#pragma once
#include "Common.h"
class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();
	bool Initialize(ID3D11Device *device, int width, int height);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext *deviceContext, ID3D11DepthStencilView *depthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext *deviceContext, ID3D11DepthStencilView *depthStencilView, float r, float g, float b, float a);

	ID3D11ShaderResourceView *GetRenderTargetTextureView(){ return _renderTargetTextureView; }
private:
	ID3D11Texture2D *_renderTargetTexture;
	ID3D11RenderTargetView *_renderTargetView;
	ID3D11ShaderResourceView *_renderTargetTextureView;
};

