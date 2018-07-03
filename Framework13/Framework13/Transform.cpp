#include "Transform.h"


Transform::Transform() : 
	_position(DirectX::XMVectorSet(0, 0, 0, 0)),
	_rotation(DirectX::XMQuaternionIdentity())
{
}


Transform::~Transform()
{
}
