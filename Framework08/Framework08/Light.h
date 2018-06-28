#pragma once
#include "Common.h"
class Light
{
public:
	Light(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, float diffusePower, DirectX::XMFLOAT4 specularColor, float specularHardness, float specularPower, DirectX::XMFLOAT3 dir);
	~Light();
	DirectX::XMFLOAT4 GetDiffuseColor(){ return _diffuseColor; }
	DirectX::XMFLOAT3 GetDir(){ return _dir; }
	DirectX::XMFLOAT4 GetAmbientColor() { return _ambientColor; }
	DirectX::XMFLOAT4 GetSpecularColor() { return _specularColor; }
	float GetDiffusePower(){ return _diffusePower; }
	float GetSpecularHardness(){ return _specularHardness; }
	float GetSpecularPower() { return _specularPower; }
private:
	DirectX::XMFLOAT4 _ambientColor;
	DirectX::XMFLOAT4 _diffuseColor;
	DirectX::XMFLOAT4 _specularColor;
	float _diffusePower;
	float _specularHardness;
	float _specularPower;
	DirectX::XMFLOAT3 _dir;
};

