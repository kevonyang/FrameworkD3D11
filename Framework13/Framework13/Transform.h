#pragma once
#include "Common.h"

class Transform
{
public:
	Transform();
	~Transform();
	void SetPosition(float x, float y, float z, float w = 1.0f) { _position = DirectX::XMVectorSet(x, y, z, w); }
	DirectX::XMVECTOR GetPosition() { return _position; }
	void SetRotation(float x, float y, float z, float w){ _rotation = DirectX::XMVectorSet(x, y, z, w); }
	void SetRotation(DirectX::XMVECTOR q){ _rotation = q; }
	DirectX::XMVECTOR GetRotation() { return _rotation; }
protected:
	DirectX::XMVECTOR _position;
	DirectX::XMVECTOR _rotation;
};

