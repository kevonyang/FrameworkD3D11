#include "Light.h"


Light::Light(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 dir)
{
	_Color = color;
	_Dir = dir;
}


Light::~Light()
{
}
