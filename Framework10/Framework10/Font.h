#pragma once
#include "Common.h"
#include "Texture.h"
class Texture;
struct FontVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Texcoord;
};

struct FontDefine
{
	float left;
	float right;
	int width;
};

class Font
{
public:
	Font();
	Font(Font &);
	~Font();

	bool Initialize(ID3D11Device *device, char *fontFileName, char *textureFileName);
	void Shutdown();
	bool LoadFontData(char *fileName);
	void BuildVertexArray(void *vertices, char *sentence, float posX, float posY);
	ID3D11ShaderResourceView *GetTextureView(){ return _texture->GetTextureView(); }

private:
	Texture *_texture;
	FontDefine *_define;
	int _defineCount;

};

