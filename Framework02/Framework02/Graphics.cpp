#include "Graphics.h"

#include "Window.h"
#include "GraphicsDriverD3D11.h"
#include "Camera.h"
#include "Mesh.h"
#include "ColorShader.h"

Graphics::Graphics() : _window(NULL), _driver(NULL), _camera(NULL), _mesh(NULL), _colorShader(NULL)
{
}

Graphics::Graphics(const Graphics&)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(char *appName, int width, int height, bool fullScreen, bool vSync)
{
	_window = new Window();
	if (!_window->InitializeWindow(appName, width, height, fullScreen))
		return false;

	//init d3d11
	_driver = new GraphicsDriverD3D11();
	if (!_driver->Initialize(_window->GetHWND(), width, height, fullScreen, vSync))
		return false;

	_camera = new Camera();

	_mesh = new Mesh();
	if (!_mesh->Initialize(_driver->GetD3D11Device()))
		return false;
	
	_colorShader = new ColorShader();
	if (!_colorShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	return true;
}

void Graphics::Shutdown()
{
	if (_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = NULL;
	}
	if (_mesh)
	{
		_mesh->Shutdown();
		delete _mesh;
		_mesh = NULL;
	}
	if (_camera)
	{
		delete _camera;
		_camera = NULL;
	}

	if (_driver)
	{
		_driver->Shutdown();
		delete _driver;
		_driver = NULL;
	}

	if (_window)
	{
		_window->ShutdownWindow();
		delete _window;
		_window = NULL;
	}
}

bool Graphics::RenderOneFrame()
{
	if (!_window->ReceiveMessage())
		return false;
	
	if (!Render())
		return false;

	return true;
}

bool Graphics::Render()
{
	_driver->BeginScene(0.f, 0.f, 0.f, 1.f);

	_camera->Render();
	DirectX::XMMATRIX viewMatrix = _camera->GetViewMatrix();

	DirectX::XMMATRIX worldMatrix = _driver->GetWorldMatrix();
	DirectX::XMMATRIX projectionMatrix = _driver->GetProjectionMatrix();

	_mesh->Render(_driver->GetD3D11DeviceContext());

	_colorShader->Render(_driver->GetD3D11DeviceContext(), _mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	
	_driver->EndScene();

	return true;
}