#include "Input.h"

Input::Input()
{
}

Input::Input(const Input&)
{
}

Input::~Input()
{
}

bool Input::Initialize(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	_width = width;
	_height = height;

	HRESULT res = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&_directInput, NULL);
	if (FAILED(res))
		return false;

	res = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(res))
		return false;

	res = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(res))
		return false;

	res = _keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(res))
		return false;

	res = _keyboard->Acquire(); 
	if (FAILED(res))
		return false;

	res = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(res))
		return false;

	res = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(res))
		return false;

	res = _mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(res))
		return false;

	res = _mouse->Acquire();
	if (FAILED(res))
		return false;

	for (int i = 0; i < 256; ++i)
	{
		_keys[i] = 0;
	}
	return true;
}

void Input::Shutdown()
{
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = NULL;
	}
	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = NULL;
	}
	if (_directInput)
	{
		_directInput->Release();
		_directInput = NULL;
	}
}

bool Input::IsKeyDown(unsigned int key)
{
	return (_keys[key] & 0x80) != 0;
}

bool Input::IsEscapePressed()
{
	if (_keys[DIK_ESCAPE] & 0x80)
		return true;
	return false;
}

bool Input::Frame()
{
	HRESULT res = _keyboard->GetDeviceState(_ARRAYSIZE(_keys), (LPVOID)&_keys);
	if (FAILED(res))
	{
		if (res == DIERR_INPUTLOST || res == DIERR_NOTACQUIRED)
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	res = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(res))
	{
		if (res == DIERR_INPUTLOST || res == DIERR_NOTACQUIRED)
		{
			_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;

	if (_mouseX < 0) _mouseX = 0;
	if (_mouseY < 0) _mouseY = 0;
	if (_mouseX > _width) _mouseX = _width;
	if (_mouseY > _height) _mouseY = _height;

	return true;
}