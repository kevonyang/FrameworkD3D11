#pragma once
class Window;
class GraphicsDriverD3D11;

class Graphics
{
public:
	friend class Root;
public:
	bool Initialize(char *appName, int width, int height, bool fullScreen, bool vSync);
	void Shutdown();
	Window *GetWindow() { return _window; }

	bool RenderOneFrame();
	bool Render();
private:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	Window *_window;
	GraphicsDriverD3D11 *_driver;
};

