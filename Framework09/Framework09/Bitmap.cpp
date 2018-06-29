#include "Bitmap.h"
#include "Texture.h"

Bitmap::Bitmap() :_posX(0), _posY(0), _updateBuffer(true)
{
}

Bitmap::Bitmap(Bitmap &)
{
}

Bitmap::~Bitmap()
{
}

bool Bitmap::Initialize(ID3D11Device *device, char *textureFileName, int width, int height)
{
	_width = width;
	_height = height;

	if (!InitializeBuffers(device))
		return false;

	_texture = new Texture();
	if (!_texture->Initialize(device, textureFileName))
		return false;

	return true;
}

bool Bitmap::InitializeBuffers(ID3D11Device *device)
{
	_vertexCount = 6;
	_indexCount = _vertexCount;
	BitmapVertex *vertices = new BitmapVertex[_vertexCount];
	UInt32 *indices = new UInt32[_indexCount];

	memset(vertices, 0, sizeof(BitmapVertex) * _vertexCount);
	for (int i = 0; i < _indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(BitmapVertex) * _vertexCount;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = vertices;
	HRESULT res = device->CreateBuffer(&bd, &srd, &_vertexBuffer);
	if (FAILED(res))
		return false;

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = sizeof(UInt32) * _indexCount;
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;

	srd.pSysMem = indices;
	res = device->CreateBuffer(&bd, &srd, &_indexBuffer);
	if (FAILED(res))
		return false;

	delete[]vertices;
	delete[]indices;

	return true;
}

void Bitmap::Shutdown()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}

bool Bitmap::Render(ID3D11DeviceContext *deviceContext)
{
	if (_updateBuffer)
		if (!UpdateBuffers(deviceContext))
			return false;

	RenderBuffers(deviceContext);

	return true;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext *deviceContext)
{
	BitmapVertex *vertices = new BitmapVertex[_vertexCount];

	int left = _posX - _width / 2;
	int right = left + _width;
	int bottom = _posY - _height / 2;
	int top = bottom + _height;

	vertices[0].Pos = DirectX::XMFLOAT3(left, bottom, 0.0f);
	vertices[0].Texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].Pos = DirectX::XMFLOAT3(left, top, 0.0f);
	vertices[1].Texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[2].Pos = DirectX::XMFLOAT3(right, bottom, 0.0f);
	vertices[2].Texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[3].Pos = DirectX::XMFLOAT3(left, top, 0.0f);
	vertices[3].Texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[4].Pos = DirectX::XMFLOAT3(right, top, 0.0f);
	vertices[4].Texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[5].Pos = DirectX::XMFLOAT3(right, bottom, 0.0f);
	vertices[5].Texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT res = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	if (FAILED(res))
	{
		delete[]vertices;
		return false;
	}
	BitmapVertex *ptr = (BitmapVertex*)subResource.pData;
	memcpy(ptr, vertices, sizeof(BitmapVertex) * _vertexCount);
	deviceContext->Unmap(_vertexBuffer, 0);

	delete[]vertices;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(BitmapVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Bitmap::SetPos(int posX, int posY)
{
	if (posX == _posX && posY == _posY)
		return;
	_posX = posX;
	_posY = posY;
	_updateBuffer = true;
}