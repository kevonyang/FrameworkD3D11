#pragma once
class Input;
class Graphics;

class Root
{
public:

	static Root *GetInstance()
	{
		if (!_instance)
		{
			_instance = new Root();
		}
		return _instance;
	}
	bool Initialize();
	void Shutdown();
	void Run();
	void Quit(){ _appQuit = true; }
	
	Input *GetInput() { return _input; }
	Graphics *GetGraphics() { return _graphics; }
private:
	Root();
	Root(const Root&);
	~Root();
	bool RunOneFrame();

private:
	static Root *_instance;

	char *_appName;
	bool _appQuit;

	Input *_input;
	Graphics *_graphics;
};

