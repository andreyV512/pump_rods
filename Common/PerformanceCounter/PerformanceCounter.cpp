#include "PerformanceCounter.h"
#include <WTypes.h>
namespace Performance
{
	LARGE_INTEGER frequency;
	void Init()
	{
		QueryPerformanceFrequency(&frequency);
	}
	unsigned Counter()
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return unsigned(1000.0 * t.QuadPart/frequency.QuadPart);
	}
}