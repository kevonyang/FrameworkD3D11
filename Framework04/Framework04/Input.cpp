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

bool Input::Initialize()
{
	for (int i = 0; i < 256; ++i)
	{
		_keys[i] = 0;
	}
	return true;
}

void Input::KeyDown(unsigned int key)
{
	_keys[key] = 1;
}

void Input::KeyUp(unsigned int key)
{
	_keys[key] = 0;
}

bool Input::IsKeyDown(unsigned int key)
{
	return _keys[key] == 1;
}
