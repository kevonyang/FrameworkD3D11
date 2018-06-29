#pragma once
#include "Common.h"
#include "Texture.h"

struct BitmapVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Texcoord;
};

class Bitmap
{
public:
	Bitmap();
	Bitmap(Bitmap &);
	~Bitmap();
	bool Initialize(ID3D11Device *device, char *textureFileName, int width, int height);
	void Shutdown();
	bool Render(ID3D11DeviceContext *deviceContext);
	int GetIndexCount(){ return _indexCount; }
	ID3D11ShaderResourceView *GetTextureView(){ return _texture->GetTextureView(); }
	void SetPos(int posX, int posY);
private:
	bool InitializeBuffers(ID3D11Device *device);
	bool UpdateBuffers(ID3D11DeviceContext *deviceContext);
	void RenderBuffers(ID3D11DeviceContext *deviceContext);

	int _width;
	int _height;
	int _posX;
	int _posY;
	bool _updateBuffer;
	Texture *_texture;
	int _vertexCount;
	int _indexCount;
	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
};

