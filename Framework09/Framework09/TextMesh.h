#pragma once
#include "Common.h"
class Font;
class FontShader;
class TextMesh
{
public:
	TextMesh();
	TextMesh(TextMesh&);
	~TextMesh();
	bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, HWND hWnd);
	void Shutdown();

	bool InitializeSentence(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *sentence, int maxLength, int posX, int posY);
	bool UpdateSentence(ID3D11DeviceContext *deviceContext, char *sentence, int posX, int posY);

	void Render(ID3D11DeviceContext *deviceContext);

	int GetIndexCount(){ return _indexCount; }
	Font *GetFont(){ return _font; }
private:
	Font *_font;
	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
	int _vertexCount;
	int _indexCount;
	int _maxLength;
};

