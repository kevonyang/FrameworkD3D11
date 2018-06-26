#pragma once
#include "Common.h"
class Light
{
public:
	Light(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 dir);
	~Light();
	DirectX::XMFLOAT4 GetColor(){ return _Color; }
	DirectX::XMFLOAT3 GetDir(){ return _Dir; }
private:
	DirectX::XMFLOAT4 _Color;
	DirectX::XMFLOAT3 _Dir;
};

