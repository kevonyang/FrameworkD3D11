#include "TextureArray.h"


TextureArray::TextureArray()
{
	_textures[0] = NULL;
	_textures[1] = NULL;
}


TextureArray::~TextureArray()
{
}

bool TextureArray::Initialize(ID3D11Device *device, char *filename1, char *filename2)
{
	HRESULT res = D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &_textures[0], NULL);
	if (FAILED(res))
		return false;

	D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &_textures[1], NULL);
	if (FAILED(res))
		return false;

	return true;
}

void TextureArray::Shutdown()
{
	if (_textures[0])
	{
		_textures[0]->Release();
		_textures[0] = NULL;
	}

	if (_textures[1])
	{
		_textures[1]->Release();
		_textures[1] = NULL;
	}
}