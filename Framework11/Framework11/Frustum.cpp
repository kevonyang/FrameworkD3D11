#include "Frustum.h"


Frustum::Frustum()
{
}

Frustum::Frustum(const Frustum&)
{
}

Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(int zFar, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	float zMinimum, r;

	DirectX::XMFLOAT4X4 projectionF;
	DirectX::XMStoreFloat4x4(&projectionF, projectionMatrix);

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionF._43 / projectionF._33;
	r = (float)zFar / ((float)zFar - zMinimum);
	projectionF._33 = r;
	projectionF._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	DirectX::XMMATRIX projextionM = DirectX::XMLoadFloat4x4(&projectionF);
	DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixMultiply(viewMatrix, projextionM);

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, tempMatrix);

	// Calculate near plane of frustum.
	DirectX::XMFLOAT4 nearPlane;
	nearPlane.x = matrix._13;
	nearPlane.y = matrix._23;
	nearPlane.z = matrix._33;
	nearPlane.w = matrix._43;
	_planes[0] = DirectX::XMLoadFloat4(&nearPlane);
	DirectX::XMPlaneNormalize(_planes[0]);

	// Calculate far plane of frustum.
	DirectX::XMFLOAT4 farPlane;
	farPlane.x = matrix._14 - matrix._13;
	farPlane.y = matrix._24 - matrix._23;
	farPlane.z = matrix._34 - matrix._33;
	farPlane.w = matrix._44 - matrix._43;
	_planes[1] = DirectX::XMLoadFloat4(&farPlane);
	DirectX::XMPlaneNormalize(_planes[1]);

	// Calculate left plane of frustum.
	DirectX::XMFLOAT4 leftPlane;
	leftPlane.x = matrix._14 + matrix._11;
	leftPlane.y = matrix._24 + matrix._21;
	leftPlane.z = matrix._34 + matrix._31;
	leftPlane.w = matrix._44 + matrix._41;
	_planes[2] = DirectX::XMLoadFloat4(&leftPlane);
	DirectX::XMPlaneNormalize(_planes[2]);

	// Calculate right plane of frustum.
	DirectX::XMFLOAT4 rightPlane;
	rightPlane.x = matrix._14 - matrix._11;
	rightPlane.y = matrix._24 - matrix._21;
	rightPlane.z = matrix._34 - matrix._31;
	rightPlane.w = matrix._44 - matrix._41;
	_planes[3] = DirectX::XMLoadFloat4(&rightPlane);
	DirectX::XMPlaneNormalize(_planes[3]);

	// Calculate top plane of frustum.
	DirectX::XMFLOAT4 topPlane;
	topPlane.x = matrix._14 - matrix._12;
	topPlane.y = matrix._24 - matrix._22;
	topPlane.z = matrix._34 - matrix._32;
	topPlane.w = matrix._44 - matrix._42;
	_planes[4] = DirectX::XMLoadFloat4(&topPlane);
	DirectX::XMPlaneNormalize(_planes[4]);

	// Calculate bottom plane of frustum.
	DirectX::XMFLOAT4 bottomPlane;
	bottomPlane.x = matrix._14 + matrix._12;
	bottomPlane.y = matrix._24 + matrix._22;
	bottomPlane.z = matrix._34 + matrix._32;
	bottomPlane.w = matrix._44 + matrix._42;
	_planes[5] = DirectX::XMLoadFloat4(&bottomPlane);
	DirectX::XMPlaneNormalize(_planes[5]);

}

bool Frustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i<6; ++i)
	{
		DirectX::XMVECTOR v = DirectX::XMPlaneDotCoord(_planes[i], DirectX::XMVectorSet(x, y, z, 1.0f));
		if (DirectX::XMVectorGetX(v) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckSphere(float x, float y, float z, float radius)
{
	for (int i = 0; i<6; ++i)
	{
		DirectX::XMVECTOR v = DirectX::XMPlaneDotCoord(_planes[i], DirectX::XMVectorSet(x, y, z, 1.0f));
		if (DirectX::XMVectorGetX(v) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckCuboid(float x, float y, float z, float xSize, float ySize, float zSize)
{
	if (!CheckPoint(x - xSize, y - ySize, z - zSize))
		return false;
	if (!CheckPoint(x + xSize, y - ySize, z - zSize))
		return false;
	if (!CheckPoint(x - xSize, y + ySize, z - zSize))
		return false;
	if (!CheckPoint(x + xSize, y + ySize, z - zSize))
		return false;
	if (!CheckPoint(x - xSize, y - ySize, z + zSize))
		return false;
	if (!CheckPoint(x + xSize, y - ySize, z + zSize))
		return false;
	if (!CheckPoint(x - xSize, y + ySize, z + zSize))
		return false;
	if (!CheckPoint(x + xSize, y + ySize, z + zSize))
		return false;

	return true;
}