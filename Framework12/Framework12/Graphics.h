#pragma once
class Window;
class GraphicsDriverD3D11;
class Camera;
class Mesh;
class LightShader;
class TextureShader;
class Light;
class Bitmap;
class FontShader;
class TextMesh;
class MeshList;
class MultiTextureShader;

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

	Window *_window;
	GraphicsDriverD3D11 *_driver;
	Camera *_camera;
	Mesh *_mesh;
	MeshList *_meshList;
	LightShader *_lightShader;
	Bitmap *_bitmap;
	TextureShader *_textureShader;
	MultiTextureShader *_multiTextureShader;
	Light *_dirLight;
	TextMesh *_text;
	FontShader *_fontShader;
};

