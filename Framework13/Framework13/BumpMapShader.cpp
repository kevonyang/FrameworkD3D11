#include "BumpMapShader.h"
#include "Light.h"

BumpMapShader::BumpMapShader()
{
}


BumpMapShader::~BumpMapShader()
{
}

bool BumpMapShader::Initialize(ID3D11Device *device, HWND hWnd)
{
	if (!InitializeShader(device, hWnd, "BumpMapShader.vs", "BumpMapShader.ps"))
		return false;
	return true;
}

bool BumpMapShader::InitializeShader(ID3D11Device *device, HWND hWnd, char *vsFileName, char *psFileName)
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	res = device->CreateSamplerState(&sd, &_samplerState);
	if (FAILED(res))
		return false;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(MatrixBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	res = device->CreateBuffer(&bd, NULL, &_matrixBuffer);
	if (FAILED(res))
		return false;

	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(LightBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	res = device->CreateBuffer(&bd, NULL, &_lightBuffer);
	if (FAILED(res))
		return false;

	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(CameraBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	res = device->CreateBuffer(&bd, NULL, &_cameraBuffer);
	if (FAILED(res))
		return false;

	return true;
}

void BumpMapShader::ShowErrorMessage(HWND hWnd, ID3DBlob *errBlob, char *fileName)
{
	char *errBuffer = (char*)errBlob->GetBufferPointer();
	UInt32 errSize = errBlob->GetBufferSize();
	char buffer[256];
	strcpy_s(buffer, sizeof(buffer), errBuffer);

	MessageBox(hWnd, buffer, fileName, MB_OK);
}

void BumpMapShader::Shutdown()
{
	if (_lightBuffer)
	{
		_lightBuffer->Release();
		_lightBuffer = NULL;
	}
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = NULL;
	}
	if (_samplerState)
	{
		_samplerState->Release();
		_samplerState = NULL;
	}
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
}
void BumpMapShader::Render(ID3D11DeviceContext *deviceContext, UInt32 indexCount,
	DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix,
	ID3D11ShaderResourceView **textureArray,
	Light *light, DirectX::XMVECTOR cameraPos)
{
	SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, light, cameraPos);
	RenderShader(deviceContext, indexCount);
}

bool BumpMapShader::SetShaderParameters(ID3D11DeviceContext *deviceContext,
	DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix,
	ID3D11ShaderResourceView **textureArray,
	Light *light, DirectX::XMVECTOR cameraPos)
{
	D3D11_MAPPED_SUBRESOURCE matrixSubResource;
	HRESULT res = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &matrixSubResource);
	if (FAILED(res))
		return false;
	MatrixBuffer *ptr = (MatrixBuffer*)matrixSubResource.pData;
	ptr->World = DirectX::XMMatrixTranspose(worldMatrix);
	ptr->View = DirectX::XMMatrixTranspose(viewMatrix);
	ptr->Projection = DirectX::XMMatrixTranspose(projectionMatrix);
	deviceContext->Unmap(_matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	deviceContext->PSSetShaderResources(0, 2, textureArray);

	D3D11_MAPPED_SUBRESOURCE lightSubResource;
	res = deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSubResource);
	if (FAILED(res))
		return false;
	LightBuffer *ptr1 = (LightBuffer*)lightSubResource.pData;
	ptr1->AmbientColor = light->GetAmbientColor();
	ptr1->DiffuseColor = light->GetDiffuseColor();
	ptr1->SpecularColor = light->GetSpecularColor();
	ptr1->Dir = light->GetDir();
	ptr1->DiffusePower = light->GetDiffusePower();
	ptr1->SpecularPower = light->GetSpecularPower();
	ptr1->SpecularHardness = light->GetSpecularHardness();
	deviceContext->Unmap(_lightBuffer, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &_lightBuffer);

	D3D11_MAPPED_SUBRESOURCE cameraSubResource;
	res = deviceContext->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cameraSubResource);
	if (FAILED(res))
		return false;
	CameraBuffer *ptr2 = (CameraBuffer*)cameraSubResource.pData;
	ptr2->CameraPos = cameraPos;
	deviceContext->Unmap(_cameraBuffer, 0);

	deviceContext->VSSetConstantBuffers(1, 1, &_cameraBuffer);

	return true;
}

void BumpMapShader::RenderShader(ID3D11DeviceContext *deviceContext, UInt32 indexCount)
{
	deviceContext->IASetInputLayout(_inputLayout);
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &_samplerState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}