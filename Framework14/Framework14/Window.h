#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Common.h"
class Window
{
public:
	Window();
	~Window();

	bool InitializeWindow(char *appName, int width, int height, bool fullScreen);
	void ShutdownWindow();
	int GetWidth(){ return _width; }
	int GetHeight(){ return _height; }
	HWND GetHWND(){ return _hWnd; }
	bool ReceiveMessage();

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
private:
	HINSTANCE _hInstance;
	HWND _hWnd;
	LPCSTR _appName;
	int _width;
	int _height;
	bool _fullScreen;
};

