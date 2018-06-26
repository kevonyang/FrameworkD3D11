#include "Window.h"
#include "Root.h"
#include "Input.h"
#include "Graphics.h"

Window::Window() :_hInstance(NULL), _hWnd(NULL), _appName(""), _width(800), _height(600), _fullScreen(false)
{
}


Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		{
			Graphics *grahics = Root::GetInstance()->GetGraphics();
			if (!grahics) return 0;
			Window *window = grahics->GetWindow();
			if (!window) return 0;
			return window->WindowProc(hWnd, message, wparam, lparam);
		}
	}
	return 0;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		Root::GetInstance()->GetInput()->KeyDown(wparam);
		break;
	case WM_KEYUP:
		Root::GetInstance()->GetInput()->KeyUp(wparam);
		break;
	default:
		return DefWindowProc(hWnd, message, wparam, lparam);
		break;
	}
	return 0;
}

bool Window::InitializeWindow(char *appName, int width, int height, bool fullScreen)
{
	_appName = appName;
	_width = width;
	_height = height;
	_fullScreen = fullScreen;
	int offsetX = 0;
	int offsetY = 0;

	_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _appName;
	wcex.hIconSm = wcex.hIcon;

	RegisterClassEx(&wcex);

	if (fullScreen)
	{
		_width = GetSystemMetrics(SM_CXSCREEN);
		_height = GetSystemMetrics(SM_CYSCREEN);
		
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)_width;
		dmScreenSettings.dmPelsHeight = (unsigned long)_height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		offsetX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
		offsetY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;
	}

	UINT32 dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	if (!fullScreen)
		dwStyle |= WS_OVERLAPPEDWINDOW;
	
	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, _appName, _appName, dwStyle,
		offsetX, offsetY, _width, _height, NULL, NULL, _hInstance, NULL);

	if (!_hWnd)
		return false;

	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	//ShowCursor(false);

	return true;
}

void Window::ShutdownWindow()
{
	if (_fullScreen)
		ChangeDisplaySettings(NULL, 0);
	
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	UnregisterClass(_appName, _hInstance);
	_hInstance = NULL;
}

bool Window::ReceiveMessage()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
		return false;

	return true;
}
