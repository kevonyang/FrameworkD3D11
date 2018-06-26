#pragma once
#include "Common.h"
class Light;
class LightShader
{
	struct  MatrixBuffer
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT4 AmbientColor;
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT4 SpecularColor;
		DirectX::XMFLOAT3 Dir;
		float DiffusePower;
		float SpecularPower;
		float SpecularHardness;
		DirectX::XMFLOAT2 Padding;
	};

	struct CameraBuffer
	{
		DirectX::XMVECTOR CameraPos;
	};

public:
	LightShader();
	~LightShader();
	bool Initialize(ID3D11Device *device, HWND hWnd);
	void Shutdown();
	void Render(ID3D11DeviceContext *deviceContext, UInt32 indexCount,
		DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix,
		ID3D11ShaderResourceView *textureView,
		Light *light, DirectX::XMVECTOR cameraPos);

private:
	bool InitializeShader(ID3D11Device *device, HWND hWnd, char *vsFileName, char *psFileName);
	void ShowErrorMessage(HWND hWnd, ID3DBlob *errBlob, char *fileName);
	bool SetShaderParameters(ID3D11DeviceContext *deviceContext,
		DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix,
		ID3D11ShaderResourceView *textureView,
		Light *light, DirectX::XMVECTOR cameraPos);
	void RenderShader(ID3D11DeviceContext *deviceContext, UInt32 indexCount);

	ID3D11VertexShader *_vertexShader;
	ID3D11PixelShader *_pixelShader;
	ID3D11InputLayout *_inputLayout;
	ID3D11Buffer *_matrixBuffer;
	ID3D11Buffer *_lightBuffer;
	ID3D11Buffer *_cameraBuffer;
	ID3D11SamplerState *_samplerState;
};

