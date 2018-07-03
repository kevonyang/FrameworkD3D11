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
_textureArray(NULL)
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

bool Mesh::Initialize(ID3D11Device *device, char *modelFileName, char *textureFileName1, char *textureFileName2)
{
	if (!LoadModel(modelFileName))
		return false;

	CalculateModelVectors();

	if (!InitializeBuffers(device))
		return false;

	_textureArray = new TextureArray();
	if (!_textureArray->Initialize(device, textureFileName1, textureFileName2))
		return false;
	return true;
}

void Mesh::Shutdown()
{
	if (_textureArray)
	{
		_textureArray->Shutdown();
		delete _textureArray;
		_textureArray = NULL;
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
		_vertices[i].Tangent = DirectX::XMFLOAT3(_model[i].tx, _model[i].ty, _model[i].tz);
		_vertices[i].Binormal = DirectX::XMFLOAT3(_model[i].bx, _model[i].by, _model[i].bz);
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

void Mesh::CalculateTangentBinormal(ModelType v1, ModelType v2, ModelType v3, DirectX::XMFLOAT3 &tangent, DirectX::XMFLOAT3 &binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	
	vector1[0] = v2.x - v1.x;
	vector1[1] = v2.y - v1.y;
	vector1[2] = v2.z - v1.z;

	vector2[0] = v3.x - v1.x;
	vector2[1] = v3.y - v1.y;
	vector2[2] = v3.z - v1.z;

	tuVector[0] = v2.tu - v1.tu;
	tvVector[0] = v2.tv - v1.tv;

	tvVector[0] = v3.tu - v1.tu;
	tvVector[1] = v3.tv - v1.tv;

	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Mesh::CalculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3& normal)
{
	float length;

	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}


void Mesh::CalculateModelVectors()
{
	int faceCount = _vertexCount / 3;

	int index = 0;
	for (int i = 0; i < faceCount; ++i)
	{
		DirectX::XMFLOAT3 tangent, binormal, normal;
		CalculateTangentBinormal(_model[index], _model[index + 1], _model[index + 2], tangent, binormal);
		CalculateNormal(tangent, binormal, normal);

		for (int j = 0; j < 3; ++j)
		{
			_model[index].nx = normal.x;
			_model[index].ny = normal.y;
			_model[index].nz = normal.z;
			_model[index].tx = tangent.x;
			_model[index].ty = tangent.y;
			_model[index].tz = tangent.z;
			_model[index].bx = binormal.x;
			_model[index].by = binormal.y;
			_model[index].bz = binormal.z;
			++index;
		}
	}
}