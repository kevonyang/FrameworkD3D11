#pragma once
#include "Common.h"
#include <dinput.h>
class Input
{
public:
	friend class Root;
public:
	bool Initialize(HINSTANCE hInstance, HWND hWnd, int width, int height);
	void Shutdown();

	bool IsKeyDown(unsigned int key);
	bool IsEscapePressed();
	bool Frame();
	int GetMouseX(){ return _mouseX; }
	int GetMouseY(){ return _mouseY; }
private:
	Input();
	Input(const Input&);
	~Input();
	unsigned char _keys[256];
	DIMOUSESTATE _mouseState;
	IDirectInput8 *_directInput;
	IDirectInputDevice8 *_keyboard;
	IDirectInputDevice8 *_mouse;
	int _width;
	int _height;
	int _mouseX;
	int _mouseY;
};

