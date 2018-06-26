#include "Mesh.h"
#include "Texture.h"

Mesh::Mesh():
_vertices(NULL),
_indices(NULL),
_vertexCount(0),
_indexCount(0),
_vertexBuffer(NULL),
_indexBuffer(NULL),
_texture(NULL)
{
}

Mesh::Mesh(const Mesh&)
{}

Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D11Device *device)
{
	if (!InitializeBuffers(device))
		return false;

	_texture = new Texture();
	if (!_texture->Initialize(device))
		return false;
	return true;
}

void Mesh::Shutdown()
{
	if (_texture)
	{
		_texture->Shutdown();
		_texture = NULL;
	}
	if (_vertices)
	{
		delete[] _vertices;
		_vertices = NULL;
	}
	if (_indices)
	{
		delete[] _indices;
		_indices = NULL;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
	}
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = NULL;
	}
}

bool Mesh::InitializeBuffers(ID3D11Device *device)
{
	_vertices = new Vertex[3];
	_vertexCount = 3;
	_vertices[0].Pos = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
	_vertices[1].Pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	_vertices[2].Pos = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	_vertices[0].Texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	_vertices[1].Texcoord = DirectX::XMFLOAT2(0.5f, 1.0f);
	_vertices[2].Texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);
	_vertices[0].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	_vertices[1].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	_vertices[2].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	_indices = new UInt32[3];
	_indexCount = 3;
	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = _vertexCount * sizeof(Vertex);
	bd.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = _vertices;
	HRESULT res = device->CreateBuffer(&bd, &srd, &_vertexBuffer);
	if (FAILED(res))
		return false;
	
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = _indexCount * sizeof(UInt32);
	bd.Usage = D3D11_USAGE_DEFAULT;
	
	srd.pSysMem = _indices;
	
	res = device->CreateBuffer(&bd, &srd, &_indexBuffer);
	if (FAILED(res))
		return false;

	return true;
}

void Mesh::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);
}

void Mesh::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}