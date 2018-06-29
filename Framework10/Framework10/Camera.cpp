#include "Camera.h"

Camera::Camera():
	_initEye(DirectX::XMVectorSet(0, 0, 0, 1)),
	_initFocus(DirectX::XMVectorSet(0, 0, 1, 1)),
	_initUp(DirectX::XMVectorSet(0, 1, 0, 1)),
	_position(DirectX::XMVectorSet(0, 0, -5, 0)),
	_rotation(DirectX::XMQuaternionIdentity())
{
}

Camera::Camera(const Camera&)
{}

Camera::~Camera()
{
}

void Camera::Render()
{
	DirectX::XMMATRIX rotateMatrix = DirectX::XMMatrixRotationQuaternion(_rotation);
	DirectX::XMMATRIX translateMatrix = DirectX::XMMatrixTranslationFromVector(_position);

	DirectX::XMMATRIX matrix = DirectX::XMMatrixMultiply(translateMatrix, rotateMatrix);

	DirectX::XMVECTOR eye = DirectX::XMVector4Transform(_initEye, matrix);
	DirectX::XMVECTOR focus = DirectX::XMVector4Transform(_initFocus, matrix);
	DirectX::XMVECTOR up = DirectX::XMVector4Transform(_initUp, matrix);

	_viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);
}