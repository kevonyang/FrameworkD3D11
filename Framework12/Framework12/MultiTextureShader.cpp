#include "MultiTextureShader.h"

MultiTextureShader::MultiTextureShader()
{
}


MultiTextureShader::~MultiTextureShader()
{
}

bool MultiTextureShader::Initialize(ID3D11Device *device, HWND hWnd)
{
	if (!InitializeShader(device, hWnd, "MultiTextureShader.vs", "MultiTextureShader.ps"))
		return false;
	return true;
}

bool MultiTextureShader::InitializeShader(ID3D11Device *device, HWND hWnd, char *vsFileName, char *psFileName)
{
	ID3DBlob *vsBlob, *psBlob, *errBlob;
	HRESULT res = D3DX11CompileFromFile(vsFileName, NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &vsBlob, &errBlob, NULL);
	if (FAILED(res))
	{
		if (errBlob)
		{
			ShowErrorMessage(hWnd, errBlob, vsFileName);
			errBlob->Release();
			errBlob = NULL;
		}
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	res = device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
	if (FAILED(res))
	{
		vsBlob->Release();
		vsBlob = NULL;
		return false;
	}

	res = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &_vertexShader);
	vsBlob->Release();
	vsBlob = NULL;
	if (FAILED(res))
		return false;

	res = D3DX11CompileFromFile(psFileName, NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &psBlob, &errBlob, NULL);
	if (FAILED(res))
	{
		if (errBlob)
		{
			ShowErrorMessage(hWnd, errBlob, psFileName);
			errBlob->Release();
			errBlob = NULL;
		}
		return false;
	}

	res = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &_pixelShader);
	psBlob->Release();
	psBlob = NULL;
	if (FAILED(res))
		return false;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(MatrixBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE_DYNAMIC;

	res = device->CreateBuffer(&bd, NULL, &_matrixBuffer);
	if (FAILED(res))
		return false;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MaxAnisotropy = 1;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	res = device->CreateSamplerState(&sd, &_samplerState);
	if (FAILED(res))
		return false;

	return true;
}

void MultiTextureShader::ShowErrorMessage(HWND hWnd, ID3DBlob *errBlob, char *fileName)
{
	char *errBuffer = (char*)errBlob->GetBufferPointer();
	UInt32 errSize = errBlob->GetBufferSize();
	char buffer[256];
	strcpy_s(buffer, sizeof(buffer), errBuffer);

	MessageBox(hWnd, buffer, fileName, MB_OK);
}

void MultiTextureShader::Shutdown()
{
	if (_inputLayout)
	{
		_inputLayout->Release();
		_inputLayout = NULL;
	}
	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = NULL;
	}
	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = NULL;
	}
	if (_samplerState)
	{
		_samplerState->Release();
		_samplerState = NULL;
	}
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = NULL;
	}
}

void MultiTextureShader::Render(ID3D11DeviceContext *deviceContext, UInt32 indexCount, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView **textureArray)
{
	SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray);
	RenderShader(deviceContext, indexCount);
}

bool MultiTextureShader::SetShaderParameters(ID3D11DeviceContext *deviceContext, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView **textureArray)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT res = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	if (FAILED(res))
		return false;
	MatrixBuffer *ptr = (MatrixBuffer*)subResource.pData;

	ptr->World = DirectX::XMMatrixTranspose(worldMatrix);
	ptr->View = DirectX::XMMatrixTranspose(viewMatrix);
	ptr->Projection = DirectX::XMMatrixTranspose(projectionMatrix);

	deviceContext->Unmap(_matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	deviceContext->PSSetShaderResources(0, 2, textureArray);

	return true;
}

void MultiTextureShader::RenderShader(ID3D11DeviceContext *deviceContext, UInt32 indexCount)
{
	deviceContext->IASetInputLayout(_inputLayout);
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &_samplerState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}