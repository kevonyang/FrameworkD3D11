#include "Font.h"
#include <fstream>

Font::Font()
{
}

Font::Font(Font &)
{
}

Font::~Font()
{
}

bool Font::Initialize(ID3D11Device *device, char *fontFileName, char *textureFileName)
{
	if (!LoadFontData(fontFileName))
		return false;

	_texture = new Texture();
	if (!_texture->Initialize(device, textureFileName))
		return false;

	return true;
}

void Font::Shutdown()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}

	if (_define)
	{
		delete[]_define;
		_define = NULL;
	}
}

bool Font::LoadFontData(char *fileName)
{
	std::ifstream fin;
	fin.open(fileName);
	if (fin.fail())
		return false;

	_defineCount = 95;
	_define = new FontDefine[_defineCount];
	for (int i = 0; i < _defineCount; ++i)
	{
		int num;
		fin >> num;
		char tmp;
		fin.get(tmp);
		fin.get(tmp);
		fin.get(tmp);

		fin >> _define[i].left;
		fin >> _define[i].right;
		fin >> _define[i].width;
	}
	fin.close();

	return true;
}

void Font::BuildVertexArray(void *vertices, char *sentence, float posX, float posY)
{
	FontVertex *ptr = (FontVertex*)vertices;
	int num = strlen(sentence);

	int index = 0;
	for (int i = 0; i < num; ++i)
	{
		int ch = (int)sentence[i] - 32;
		if (ch == 0)
		{
			posX = posX + 5.0f;
		}
		else
		{
			ptr[index].Pos = DirectX::XMFLOAT3(posX, posY, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].left, 1.0f);
			++index;

			ptr[index].Pos = DirectX::XMFLOAT3(posX, posY + 16, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].left, 0.0f);
			++index;

			ptr[index].Pos = DirectX::XMFLOAT3(posX + _define[ch].width, posY, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].right, 1.0f);
			++index;

			ptr[index].Pos = DirectX::XMFLOAT3(posX, posY + 16, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].left, 0.0f);
			++index;

			ptr[index].Pos = DirectX::XMFLOAT3(posX + _define[ch].width, posY + 16, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].right, 0.0f);
			++index;

			ptr[index].Pos = DirectX::XMFLOAT3(posX + _define[ch].width, posY, 0.0f);
			ptr[index].Texcoord = DirectX::XMFLOAT2(_define[ch].right, 1.0f);
			++index;

			posX = posX + _define[ch].width + 3.0f;
		}
	}
}

