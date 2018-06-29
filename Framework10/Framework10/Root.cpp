#include "Root.h"


#include "Window.h"
#include "Input.h"
#include "Graphics.h"
#include "Fps.h"
Root *Root::_instance = NULL;
Root::Root() :_appName(NULL),
			  _appQuit(false),
			  _input(NULL),
			  _graphics(NULL),
			  _fps(NULL)
{
	
}

Root::Root(const Root&)
{
}

Root::~Root()
{
}

bool Root::Initialize()
{
	_appName = "FrameworkD3D11";

	_fps = new Fps();
	_fps->Initialize();

	_graphics = new Graphics();
	if (!_graphics->Initialize(_appName, 1280, 800, false, false))
		return false;

	_input = new Input();
	Window *window = _graphics->GetWindow();
	if (!_input->Initialize(window->GetHInstance(), window->GetHWND(), window->GetWidth(), window->GetHeight()))
		return false;
	
	return true;
}

void Root::Shutdown()
{
	_appQuit = true;
	if (_input)
	{
		_input->Shutdown();
		delete _input;
		_input = NULL;
	}

	if (_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = NULL;
	}

	if (_fps)
	{
		delete _fps;
		_fps = NULL;
	}
}

void Root::Run()
{
	while (!_appQuit)
	{
		if (!RunOneFrame())
		{
			_appQuit = true;
			return;
		}
	}
}

bool Root::RunOneFrame()
{
	if (!_graphics->RenderOneFrame())
		return false;
	if (!_input->Frame())
		return false;
	if (_input->IsEscapePressed())
		return false;

	_fps->Frame();

	return true;
}

