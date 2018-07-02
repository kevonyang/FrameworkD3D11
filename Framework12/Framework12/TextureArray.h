#pragma once
#include "Common.h"
class TextureArray
{
public:
	TextureArray();
	~TextureArray();
	bool Initialize(ID3D11Device *device, char *filename1, char *filename2);
	void Shutdown();
	ID3D11ShaderResourceView **GetTextureArray(){ return _textures; }
private:
	ID3D11ShaderResourceView *_textures[2];
};

