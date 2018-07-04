#pragma once

#include "Common.h"

class ColorShader
{
	struct MatrixBuffer
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialize(ID3D11Device *device, HWND hWnd);
	void Shutdown();
	void Render(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);
private:
	bool InitializeShader(ID3D11Device *device , HWND hWnd, char *vsFileName, char *psFileName);
	void ShowErrorMessage(HWND hWnd, ID3DBlob *errBlob, char *fileName);
	bool SetShaderParameters(ID3D11DeviceContext *deviceContext, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);
	void RenderShader(ID3D11DeviceContext *deviceContext, int indexCount);
private:
	ID3D11VertexShader *_vertexShader;
	ID3D11PixelShader *_pixelShader;
	ID3D11InputLayout *_inputLayout;
	ID3D11Buffer *_matrixBuffer;
};

