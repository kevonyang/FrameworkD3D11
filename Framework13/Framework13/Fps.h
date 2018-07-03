#pragma once

#include "Common.h"

#include <Pdh.h>
class Fps
{
public:
	Fps();
	Fps(Fps &);
	~Fps();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetFps(){ return _fps; }
	int GetCpuUsage(){ return _cpuUsage; }
private:
	int _fps;
	int _count;
	Int64 _lastTimeMS;

	float _frequency;
	Int64 _startCount;

	PDH_HQUERY _queryHandle;
	HCOUNTER _counterHandle;
	bool _canReadCpu;
	LONG _cpuUsage;
};

