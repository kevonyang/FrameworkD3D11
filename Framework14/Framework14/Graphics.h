#pragma once
class Window;
class GraphicsDriverD3D11;
class Camera;
class Mesh;
class TextureShader;
class Light;
class Bitmap;
class Mesh;
class RenderTexture;

class Graphics
{
public:
	friend class Root;
public:
	bool Initialize(char *appName, int width, int height, bool fullScreen, bool vSync);
	void Shutdown();
	Window *GetWindow() { return _window; }

	bool RenderOneFrame();
private:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Render();
	bool RenderScene();

	Window *_window;
	GraphicsDriverD3D11 *_driver;
	Camera *_camera;
	Bitmap *_bitmap;
	Mesh *_mesh;
	TextureShader *_textureShader;
	Light *_dirLight;
	RenderTexture *_renderTexture;
};

