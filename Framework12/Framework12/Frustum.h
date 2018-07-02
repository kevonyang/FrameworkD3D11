#pragma once
#include "Common.h"

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void ConstructFrustum(int zFar, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

	bool CheckPoint(float x, float y, float z);
	bool CheckSphere(float x, float y, float z, float radius);
	bool CheckCuboid(float x, float y, float z, float xSize, float ySize, float zSize);
protected:
	DirectX::XMVECTOR _planes[6];
};

