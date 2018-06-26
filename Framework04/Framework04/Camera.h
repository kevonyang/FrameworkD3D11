#pragma once
#include "Common.h"

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	void Render();
	DirectX::XMMATRIX GetViewMatrix() { return _viewMatrix; }
private:
	DirectX::XMVECTOR _initEye;
	DirectX::XMVECTOR _initFocus;
	DirectX::XMVECTOR _initUp;

	DirectX::XMVECTOR _position;
	DirectX::XMVECTOR _rotation;
	DirectX::XMMATRIX _viewMatrix;
};

