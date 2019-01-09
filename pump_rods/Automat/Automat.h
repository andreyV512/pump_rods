#pragma once

namespace Automat
{
	struct Status
	{
		enum e{
			undefined
			, exit_loop
			, time_out
			, start
			, stop
			, alarm_bits
			, alarm_l502
			, contine
		};
	};
	template<int>struct Key;
	template<>struct Key<Status::start>
	{
		static const int value = Status::start;
		static HANDLE hEvent;
	};
	template<>struct Key<Status::stop>
	{
		static const int value = Status::stop;
		static HANDLE hEvent;
	};
	struct Result
	{
		unsigned error;
		unsigned bits;
		unsigned ret;
		InputBitTable::TItems &inputs_bits;
		Result();		
	};
	extern HANDLE hEvent;
	extern Result result;
	void Init();
	void Destroy();
	void Run();
	void Stop();
}