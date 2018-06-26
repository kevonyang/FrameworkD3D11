#include "Mesh.h"
#include "Texture.h"
#include <fstream>

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

bool Mesh::LoadModel(char *fileName)
{
	std::ifstream fin;

	fin.open(fileName);
	if (fin.fail())
		return false;

	char input;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	
	fin >> _vertexCount;
	_indexCount = _vertexCount;
	_model = new ModelType[_vertexCount];
	
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);

	for (int i = 0; i < _vertexCount; ++i)
	{
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	fin.close();

	return true;
}

void Mesh::ReleaseModel()
{
	if (_model)
	{
		delete[]_model;
		_model = NULL;
	}
}

bool Mesh::Initialize(ID3D11Device *device, char *modelFileName, char *textureFileName)
{
	if (!LoadModel(modelFileName))
		return false;

	if (!InitializeBuffers(device))
		return false;

	_texture = new Texture();
	if (!_texture->Initialize(device, textureFileName))
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
	_vertices = new Vertex[_vertexCount];
	_indices = new UInt32[_indexCount];
	for (int i = 0; i < _vertexCount; ++i)
	{
		_vertices[i].Pos = DirectX::XMFLOAT3(_model[i].x, _model[i].y, _model[i].z);
		_vertices[i].Texcoord = DirectX::XMFLOAT2(_model[i].tu, _model[i].tv);
		_vertices[i].Normal = DirectX::XMFLOAT3(_model[i].nx, _model[i].ny, _model[i].nz);
		_indices[i] = i;
	}

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