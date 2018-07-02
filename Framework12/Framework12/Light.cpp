#include "Light.h"


Light::Light(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, float diffusePower, DirectX::XMFLOAT4 specularColor, float specularHardness, float specularPower, DirectX::XMFLOAT3 dir)
{
	_ambientColor = ambientColor;
	_diffuseColor = diffuseColor;
	_diffusePower = diffusePower;
	_specularColor = specularColor;
	_specularPower = specularPower;
	_specularHardness = specularHardness;
	_dir = dir;
}


Light::~Light()
{
}
