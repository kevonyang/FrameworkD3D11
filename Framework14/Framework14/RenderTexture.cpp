#include "RenderTexture.h"


RenderTexture::RenderTexture()
{
}


RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(ID3D11Device *device, int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT res = device->CreateTexture2D(&texDesc, NULL, &_renderTargetTexture);
	if (FAILED(res))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	res = device->CreateRenderTargetView(_renderTargetTexture, &viewDesc, &_renderTargetView);
	if (FAILED(res))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
	ZeroMemory(&resViewDesc, sizeof(resViewDesc));
	resViewDesc.Format = texDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = 1;
	resViewDesc.Texture2D.MostDetailedMip = 0;
	res = device->CreateShaderResourceView(_renderTargetTexture, &resViewDesc, &_renderTargetTextureView);
	if (FAILED(res))
		return false;

	return true;
}

void RenderTexture::Shutdown()
{
	if (_renderTargetTextureView)
	{
		_renderTargetTextureView->Release();
		_renderTargetTextureView = 0;
	}
	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}
	if (_renderTargetTexture)
	{
		_renderTargetTexture->Release();
		_renderTargetTexture = 0;
	}
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext *deviceContext, ID3D11DepthStencilView *depthStencilView)
{
	deviceContext->OMSetRenderTargets(1, &_renderTargetView, depthStencilView);
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext *deviceContext, ID3D11DepthStencilView *depthStencilView, float r, float g, float b, float a)
{
	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	deviceContext->ClearRenderTargetView(_renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
