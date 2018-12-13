#pragma once
#include "L502\Device502.h"
#include "DataItem\DataItem.h"
#include "App\App.h"

template<class T>struct l502Run;
template<class T, template<class>class W>struct l502Run<W<T>>
{
	enum
	{
		ok
		, thread_error
		, buffer_overlapped
		, l502_read_error
	}status;
	int &currentOffset;
	bool terminate;
	double (&outputData)[DataItem::output_buffer_size];
	l502Run(T &t)
		: currentOffset(t.currentOffset)
		, terminate(false)
		, outputData(t.outputData)
		, status(ok)
	{
		currentOffset = 0;
	}
	~l502Run()
	{
		terminate = true;
	}

	static DWORD WINAPI Proc(PVOID p)
	{
		((l502Run *)p)->Do();
		return 0;
	}
	
	unsigned operator()()
	{
		L502ParametersTable::TItems &param = Singleton<L502ParametersTable>::Instance().items;
		bool b = l502.SetupParams(
			&param.get<W<ChannelL502>>().value
			, &param.get<W<RangeL502>>().value
			, param.get<W<ChannelSamplingRate>>().value
			, 1
			); 
		if(b)
		{
			CloseHandle(CreateThread(NULL, 0, Proc, this, 0, NULL));
		}
		else
		{
			return thread_error;
		}
		return ok;
	};

	void Do()
	{
		l502.Start();
		while(!terminate)
		{
			unsigned startChannel;
			unsigned count = 0;
			if(0 < l502.Read(startChannel, &outputData[currentOffset], count))
			{
				currentOffset += count;
				if(currentOffset > dimention_of(outputData))
				{
					status = buffer_overlapped;
					terminate = true;
				}
			}
			else
			{
				status = l502_read_error;
				terminate = true;
			}
		}
		l502.Stop();
	}
};