#pragma once
#include "Common.h"

class MeshList
{
	struct MeshInfo
	{
		DirectX::XMFLOAT3 Pos;
	};
public:
	MeshList();
	MeshList(const MeshList&);
	~MeshList();

	bool Initialize(int num);
	void Shutdown();

	int GetMeshNum(){ return _meshNum; }
	void GetMeshPos(int i, float &x, float &y, float &z)
	{
		if (i >= _meshNum) return;
		x = _meshInfos[i].Pos.x;
		y = _meshInfos[i].Pos.y;
		z = _meshInfos[i].Pos.z;
	}
private:
	int _meshNum;
	MeshInfo *_meshInfos;
};

