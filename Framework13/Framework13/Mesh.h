#pragma once

#include "Common.h"
#include "Texture.h"
#include "TextureArray.h"
class Mesh
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Texcoord;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT3 Binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};
public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();
	bool LoadModel(char *fileName);
	void ReleaseModel();
	bool Initialize(ID3D11Device *device, char *modelFileName, char *textureFileName1, char *textureFileName2);
	void Shutdown();
	void Render(ID3D11DeviceContext *deviceContext);
	UInt32 GetVertexCount() { return _vertexCount; }
	UInt32 GetIndexCount() { return _indexCount; }

	ID3D11ShaderResourceView **GetTextureArray(){ return _textureArray->GetTextureArray(); }

	void CalculateTangentBinormal(ModelType v1, ModelType v2, ModelType v3, DirectX::XMFLOAT3 &tangent, DirectX::XMFLOAT3 &binormal);
	void CalculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3& normal);
	void CalculateModelVectors();
private:
	bool InitializeBuffers(ID3D11Device *device);
	void RenderBuffers(ID3D11DeviceContext *deviceContext);

	Vertex *_vertices;
	UInt32 *_indices;
	UInt32 _vertexCount;
	UInt32 _indexCount;

	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
	TextureArray *_textureArray;

	ModelType *_model;
};

