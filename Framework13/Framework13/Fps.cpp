#include "Fps.h"


Fps::Fps()
{
}

Fps::Fps(Fps&)
{
}

Fps::~Fps()
{
}

void Fps::Initialize()
{
	_fps = 0;
	_lastTimeMS = 0;

	LARGE_INTEGER frequency, performanceCount;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&performanceCount);

	_frequency = (float)frequency.QuadPart / 1000.0f;
	_startCount = performanceCount.QuadPart;
	
	PDH_STATUS status;
	status = PdhOpenQuery(NULL, NULL, &_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}

	status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}
}

void Fps::Shutdown()
{
	if (_canReadCpu)
	{
		PdhCloseQuery(_queryHandle);
	}
}

void Fps::Frame()
{
	++_count;
	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter(&performanceCount);
	Int64 nowTimeMS = (performanceCount.QuadPart - _startCount) / _frequency;
	if (nowTimeMS >= _lastTimeMS + 1000)
	{
		_fps = _count;
		_count = 0;
		_lastTimeMS = nowTimeMS;
		if (_canReadCpu)
		{
			PdhCollectQueryData(_queryHandle);
			PDH_FMT_COUNTERVALUE value;
			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);
			_cpuUsage = value.longValue;
		}
	}
}
