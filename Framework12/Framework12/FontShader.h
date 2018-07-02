#pragma once
#include "Common.h"
class FontShader
{
	struct MatrixBuffer
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
public:
	FontShader();
	FontShader(FontShader&);
	~FontShader();

	bool Initialize(ID3D11Device *device, HWND hWnd);
	void Shutdown();
	void Render(ID3D11DeviceContext *deviceContext, UInt32 indexCount, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView *textureView);
private:
	bool InitializeShader(ID3D11Device  *device, HWND hWnd, char *vsFileName, char *psFileName);
	void ShowErrorMessage(HWND hWnd, ID3DBlob *errBlob, char *fileName);
	bool SetShaderParameters(ID3D11DeviceContext *deviceContext, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView *textureView);
	void RenderShader(ID3D11DeviceContext *deviceContext, UInt32 indexCount);

	ID3D11InputLayout *_inputLayout;
	ID3D11VertexShader *_vertexShader;
	ID3D11PixelShader *_pixelShader;
	ID3D11Buffer *_matrixBuffer;
	ID3D11SamplerState *_samplerState;
};

