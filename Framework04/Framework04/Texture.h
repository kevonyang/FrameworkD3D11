#pragma once
#include "Common.h"

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();
	bool Initialize(ID3D11Device *device);
	void Shutdown();
	ID3D11ShaderResourceView *GetTextureView(){ return _textureView; }
private:
	ID3D11ShaderResourceView *_textureView;
};

