#pragma once

class Input
{
public:
	friend class Root;
public:
	bool Initialize();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);
private:
	Input();
	Input(const Input&);
	~Input();
	unsigned int _keys[256];
};

