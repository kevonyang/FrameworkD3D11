#pragma once

#include "Common.h"
#include "Texture.h"

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Texcoord;
	DirectX::XMFLOAT3 Normal;
};

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();
	bool Initialize(ID3D11Device *device);
	void Shutdown();
	void Render(ID3D11DeviceContext *deviceContext);
	UInt32 GetVertexCount() { return _vertexCount; }
	UInt32 GetIndexCount() { return _indexCount; }

	ID3D11ShaderResourceView *GetTextureView() { return _texture->GetTextureView(); }

private:
	bool InitializeBuffers(ID3D11Device *device);
	void RenderBuffers(ID3D11DeviceContext *deviceContext);

	Vertex *_vertices;
	UInt32 *_indices;
	UInt32 _vertexCount;
	UInt32 _indexCount;

	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
	Texture *_texture;
};

