#include "TextMesh.h"
#include "Font.h"
#include "FontShader.h"

TextMesh::TextMesh()
{
}

TextMesh::TextMesh(TextMesh&)
{
}

TextMesh::~TextMesh()
{
}

bool TextMesh::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, HWND hWnd)
{
	_font = new Font();
	if (!_font->Initialize(device, "font.txt", "font.dds"))
		return false;
	
	if (!InitializeSentence(device, deviceContext, "aaa bbb ccc", 100, 0, 0))
		return false;

	return true;
}

void TextMesh::Shutdown()
{
	if (_font)
	{
		_font->Shutdown();
		delete _font;
		_font = NULL;
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

bool TextMesh::InitializeSentence(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *sentence, int maxLength, int posX, int posY)
{
	_vertexCount = 6 * maxLength;
	_indexCount = _vertexCount;
	_maxLength = maxLength;

	FontVertex *vertices = new FontVertex[_vertexCount];
	UInt32 *indices = new UInt32[_indexCount];
	memset(vertices, 0, sizeof(FontVertex) * _vertexCount);
	for (int i = 0; i < _indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(FontVertex) * _vertexCount;
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
	if (!UpdateSentence(deviceContext, sentence, posX, posY))
		return false;
	return true;
}

bool TextMesh::UpdateSentence(ID3D11DeviceContext *deviceContext, char *sentence, int posX, int posY)
{
	int len = strlen(sentence);
	if (len > _maxLength)
		return false;

	FontVertex *vertices = new FontVertex[6*len];
	memset(vertices, 0, sizeof(FontVertex)*6*len);
	_font->BuildVertexArray(vertices, sentence, posX, posY);

	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT res = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	if (FAILED(res))
		return false;
	FontVertex *ptr = (FontVertex*)subResource.pData;
	memcpy(ptr, vertices, sizeof(FontVertex)*6*len);
	deviceContext->Unmap(_vertexBuffer, 0);

	delete[]vertices;
	return true;
}

void TextMesh::Render(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(FontVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}