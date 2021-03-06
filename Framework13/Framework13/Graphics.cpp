#include "Graphics.h"

#include "Window.h"
#include "GraphicsDriverD3D11.h"
#include "Camera.h"
#include "Mesh.h"
#include "ColorShader.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "MultiTextureShader.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshList.h"
#include "Bitmap.h"
#include "TextMesh.h"
#include "FontShader.h"
#include "Font.h"
#include "Root.h"
#include "Input.h"
#include "Fps.h"
#include "BumpMapShader.h"

Graphics::Graphics() : _window(NULL), _driver(NULL), _camera(NULL), _mesh(NULL), _meshList(NULL), _lightShader(NULL), _bitmap(NULL), _textureShader(NULL), _dirLight(NULL), _fontShader(NULL), _text(NULL), _bumpMapShader(NULL)
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
	_camera->SetPosition(0, 0, -10);
	_camera->SetRotation(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), 45));

	_mesh = new Mesh();
	if (!_mesh->Initialize(_driver->GetD3D11Device(), "cube.txt", "stone01.dds", "bump01.dds"))
		return false;

	_meshList = new MeshList();
	if (!_meshList->Initialize(10))
		return false;

	_lightShader = new LightShader();
	if (!_lightShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	_bitmap = new Bitmap();
	if (!_bitmap->Initialize(_driver->GetD3D11Device(), "dirt01.dds", "stone01.dds", 100, 100))
		return false;
	_bitmap->SetPos(200, 100);

	_textureShader = new TextureShader();
	if (!_textureShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	_multiTextureShader = new MultiTextureShader();
	if (!_multiTextureShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	_dirLight = new Light(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), 0.5,
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 32, 0.5,
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));

	_fontShader = new FontShader();
	if (!_fontShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	_text = new TextMesh();
	if (!_text->Initialize(_driver->GetD3D11Device(), _driver->GetD3D11DeviceContext(), _window->GetHWND()))
		return false;
	_bumpMapShader = new BumpMapShader();
	if (!_bumpMapShader->Initialize(_driver->GetD3D11Device(), _window->GetHWND()))
		return false;

	return true;
}

void Graphics::Shutdown()
{
	if (_dirLight)
	{
		delete _dirLight;
		_dirLight = NULL;
	}
	if (_textureShader)
	{
		_textureShader->Shutdown();
		delete _textureShader;
		_textureShader = NULL;
	}
	if (_lightShader)
	{
		_lightShader->Shutdown();
		delete _lightShader;
		_lightShader = NULL;
	}
	if (_bumpMapShader)
	{
		_bumpMapShader->Shutdown();
		delete _bumpMapShader;
		_bumpMapShader = NULL;
	}
	if (_mesh)
	{
		_mesh->Shutdown();
		delete _mesh;
		_mesh = NULL;
	}
	if (_meshList)
	{
		_meshList->Shutdown();
		delete _meshList;
		_meshList = NULL;
	}
	if (_bitmap)
	{
		_bitmap->Shutdown();
		delete _bitmap;
		_bitmap = NULL;
	}
	if (_fontShader)
	{
		_fontShader->Shutdown();
		delete _fontShader;
		_fontShader = NULL;
	}
	if (_text)
	{
		_text->Shutdown();
		delete _text;
		_text = NULL;
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

	Input *input = Root::GetInstance()->GetInput();

	//if (input->IsKeyDown(DIK_A))
	//{
	//	DirectX::XMVECTOR rotate = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), 0.001f);
	//	_camera->SetRotation(DirectX::XMQuaternionMultiply(_camera->GetRotation(), rotate));
	//}
	//else if(input->IsKeyDown(DIK_D))
	//{
	//	DirectX::XMVECTOR rotate = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), -0.001f);
	//	_camera->SetRotation(DirectX::XMQuaternionMultiply(_camera->GetRotation(), rotate));
	//}

	_camera->Render();
	DirectX::XMMATRIX viewMatrix = _camera->GetViewMatrix();

	DirectX::XMMATRIX worldMatrix = _driver->GetWorldMatrix();
	DirectX::XMMATRIX projectionMatrix = _driver->GetProjectionMatrix();
	DirectX::XMMATRIX orthoMatrix = _driver->GetOrthoMatrix();

	_camera->ConstructFrustum(100.0f, viewMatrix, projectionMatrix);
	
	_mesh->Render(_driver->GetD3D11DeviceContext());
	_bumpMapShader->Render(_driver->GetD3D11DeviceContext(), _mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, _mesh->GetTextureArray(), _dirLight, _camera->GetPosition());


	_driver->SwitchZBuffer(false);
	_driver->SwitchBlend(true);
	_bitmap->Render(_driver->GetD3D11DeviceContext());
	_multiTextureShader->Render(_driver->GetD3D11DeviceContext(), _bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, _bitmap->GetTextureArray());
	
	Fps *fps = Root::GetInstance()->GetFps();
	std::ostringstream oss;
	oss << input->GetMouseX() << "  " << input->GetMouseY() << "  " << fps->GetFps() << "  " << fps->GetCpuUsage();
	std::string str = oss.str();
	int x = input->GetMouseX() - _window->GetWidth() / 2;
	int y = _window->GetHeight() / 2 - input->GetMouseY();
	_text->UpdateSentence(_driver->GetD3D11DeviceContext(), (char*)str.c_str(), x, y);

	_text->Render(_driver->GetD3D11DeviceContext());
	_fontShader->Render(_driver->GetD3D11DeviceContext(), _text->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, _text->GetFont()->GetTextureView());
	_driver->SwitchBlend(false);
	_driver->SwitchZBuffer(true);

	_driver->EndScene();

	return true;
}