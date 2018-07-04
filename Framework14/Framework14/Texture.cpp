#include "Texture.h"


Texture::Texture()
{
}

Texture::Texture(const Texture&)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device *device, char *textureFileName)
{
	HRESULT res = D3DX11CreateShaderResourceViewFromFile(device, textureFileName, NULL, NULL, &_textureView, NULL);
	if (FAILED(res))
		return false;

	return true;
}

void Texture::Shutdown()
{
	if (_textureView)
	{
		_textureView->Release();
		_textureView = NULL;
	}
}