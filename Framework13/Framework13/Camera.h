#pragma once
#include "Common.h"
#include "Frustum.h"
#include "Transform.h"

class Camera : public Frustum, public Transform
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

	DirectX::XMMATRIX _viewMatrix;
};

