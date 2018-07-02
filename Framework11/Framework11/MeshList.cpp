#include "MeshList.h"


MeshList::MeshList()
{
}

MeshList::MeshList(const MeshList&)
{}

MeshList::~MeshList()
{
}

bool MeshList::Initialize(int num)
{
	_meshNum = num;
	_meshInfos = new MeshInfo[num];
	for (int i = 0; i < num; ++i)
	{
		_meshInfos[i].Pos.x = (float)(rand() - rand()) / RAND_MAX * 10;
		_meshInfos[i].Pos.y = (float)(rand() - rand()) / RAND_MAX * 10;
		_meshInfos[i].Pos.z = (float)(rand() - rand()) / RAND_MAX * 10;
	}

	return true;
}

void MeshList::Shutdown()
{
	if (_meshInfos)
	{
		delete _meshInfos;
		_meshInfos = NULL;
	}
}
