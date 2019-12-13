#pragma once

namespace Automat
{
	struct Status
	{
		enum e{
		    contine
			, exit_loop
			, time_out
			, start
			, stop
			, contine_btn
			, alarm_bits
			, alarm_l502
			, undefined
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
	template<>struct Key<Status::contine_btn>
	{
		static const int value = Status::contine_btn;
		static HANDLE hEvent;
	};
	struct Result
	{
		unsigned currentTime;
		unsigned error;
		unsigned bits;
		unsigned ret;
		InputBitTable::TItems &inputs_bits;
		//unsigned last_input_bits;
		//unsigned last_output_bits;
		Result();		
	};
	extern HANDLE hEvent;
	extern Result result;
	extern int sortResult;
	void Init();
	void Destroy();
	void Run();
	void Stop();
	void Contine();

	void UpSort();
}