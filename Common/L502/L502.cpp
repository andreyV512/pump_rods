#include "stdafx.h"
#include "L502.h"
#include "App/AppBase.h"
#include "App/Config.h"
#include "tools_debug\DebugMess.h"
#ifndef DEBUG_ITEMS
#include "L502/l502api.h"
#pragma comment(lib, "../../Common/L502/l502api.lib")
#include <stdio.h>
#include "App/AppBase.h"
#include "App/SyncChannel.hpp"

L502::L502()
	: READ_TIMEOUT(250)
{}

bool L502::Init()
{
	hnd = L502_Create();
	if (hnd==NULL)
	{
		dprint("error module!");
	}
	else
	{
		int err = L502_Open((t_l502_hnd)hnd, NULL);
		if (err)
		{
			dprint("error connection with module: %s!", L502_GetErrorString(err));
			L502_Free((t_l502_hnd)hnd);
			hnd = NULL;
		}
	}
	return NULL != hnd;
}

void L502::Destroy()
{
	 /* закрываем связь с модулем */
        L502_Close((t_l502_hnd)hnd);
        /* освобождаем описатель */
        L502_Free((t_l502_hnd)hnd);
}

namespace
{
	template<class List>struct __set_array__
	{
		template<class O, class P>struct loc
		{
			void operator()(O *o, P *p)
			{
				p[TL::IndexOf<List, O>::value] = o->value;
			}
		};
	};
};

#define SETUP(table, arr)TL::foreach<SYNC(table::items_list), __set_array__<SYNC(table::items_list)>::loc>()(&Singleton<table>::Instance().items, arr);

bool L502::SetupParams()
{
	static const int buf_size = App::count_analog_sensors;//TL::Length<SYNC(L502RangeTable::items_list)>::value;
	
	int f_ch_ranges[buf_size];
	int f_ch_modes[buf_size];
	int f_channels[buf_size];
	//TL::foreach<SYNC(L502RangeTable::items_list)  , __set_array__<L502RangeTable::items_list>::loc>()(&Singleton<L502RangeTable>::Instance().items, f_ch_ranges);
	//TL::foreach<SYNC(L502ModeTable::items_list)   , __set_array__<L502ModeTable::items_list>::loc>()(&Singleton<L502ModeTable>::Instance().items, f_ch_modes);
	//TL::foreach<SYNC(L502OffsetsTable::items_list), __set_array__<L502OffsetsTable::items_list>::loc>()(&Singleton<L502OffsetsTable>::Instance().items, f_channels);
	SETUP(L502RangeTable, f_ch_ranges)
	SETUP(L502ModeTable, f_ch_modes)
	SETUP(L502OffsetsTable, f_channels)
	int err = L502_SetLChannelCount((t_l502_hnd)hnd, buf_size);
	for (int i=0; (i < buf_size) && !err; i++)
        err = L502_SetLChannel((t_l502_hnd)hnd, i, f_channels[i], f_ch_modes[i], f_ch_ranges[i], 0);
	/* устанавливаем частоты ввода для АЦП и цифровых входов */
	int ADC_FREQ = Singleton<L502ParametersTable>::Instance().items.get<ChannelSamplingRate>().value;
	double f_adc = ADC_FREQ * buf_size;
	double f_frame = (double)ADC_FREQ;
    if (!err)
    {
        err = L502_SetAdcFreq((t_l502_hnd)hnd, &f_adc, &f_frame);
    }

    /* записываем настройки в модуль */
    if (!err)
	{
		dprint("frequency adc = %0.0f frequency chenell = %0.0f\n", f_adc, f_frame);
        err = L502_Configure((t_l502_hnd)hnd, 0);
	}

    /* разрешаем синхронные потоки */
    if (!err)
    {
        err = L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC);
    }
    if (err)
	{
		zprint("error L502_StreamsEnable date: %d!\n", err);//L502_GetErrorString(err));
	}
    return 0 == err;
}
#undef SETUP

int L502::Start()
{
	int err = L502_StreamsStart((t_l502_hnd)hnd);
	if (err)
	{
		zprint("error L502_StreamsStart date: %d!\n", err);//L502_GetErrorString(err));
	}
	return err;
}

int L502::Stop()
{
	/* останавливаем поток сбора данных (независимо от того, была ли ошибка) */
	int err = L502_StreamsStop((t_l502_hnd)hnd);
	if (err)
	{
		zprint("error L502_StreamsStop date: %d\n", err);//L502_GetErrorString(err));
	}
	return err;
}

bool L502::Read(unsigned &startChannel, double *data, unsigned &count)
{
	unsigned rcv_buf[buffer_length] = {};
	int err = 0;
	int cnt = L502_Recv((t_l502_hnd)hnd, rcv_buf, buffer_length, READ_TIMEOUT);
	if(cnt > 0)
	{
		err = L502_GetNextExpectedLchNum((t_l502_hnd)hnd, &startChannel);
		if (err < 0)
		{
			zprint("error L502_GetNextExpectedLchNum date: %d\n", err);//L502_GetErrorString(err));
			return false;
		}	
		err = L502_ProcessData((t_l502_hnd)hnd, rcv_buf, cnt, L502_PROC_FLAGS_VOLT, data, &count, NULL, NULL);
		if (err < 0)
		{
			zprint("error Read date: %d\n", err);//L502_GetErrorString(err));
			return false;
		}		
	}
	else if(cnt < 0)
	{
		zprint("error L502_Recv date: %d\n", cnt);
		return false;
	}
	return true;
}
bool L502::ReadAsync(unsigned ch, int mode, int range, double &value)
{
	/* устанавливаем 1 логический канал в управляющей таблице */
	int32_t err = L502_SetLChannelCount((t_l502_hnd)hnd, 1);
	if (err == L502_ERR_OK) {
		/* логический канал соответствует измерению 1 канала
		в диф. режиме */
		err = L502_SetLChannel((t_l502_hnd)hnd,0 ,ch,mode,range,0);
	}
	if (err == L502_ERR_OK) {
		/* передаем настройки в модуль */
		err = L502_Configure((t_l502_hnd)hnd,0);
	}
	if (err == L502_ERR_OK) {
		/* Считываем кадр данных АЦП из одного отсчета */
		return L502_ERR_OK == L502_AsyncGetAdcFrame((t_l502_hnd)hnd, L502_PROC_FLAGS_VOLT, 1000, &value);
	}
	else if(err < 0)
	{
		zprint("error ReadAsync date: %d\n", err);
		return false;
	}
	return false;
}

bool L502::ReadAsyncChannels(int count, unsigned *ch, int *mode, int *range, double *value)
{
	/*
	/* устанавливаем 3 логических канала
int32_t err = X502_SetLChannelCount(hnd, 3);
if (err == X502_ERR_OK) {
/* первый логический канал соответствует измерению 1 канала
относительно общей земли 
err = X502_SetLChannel(hnd,0,0,X502_LCH_MODE_COMM,
X502_ADC_RANGE_10,0);
}
if (err == X502_ERR_OK) {
/* второй логический канал соответствует измерению 16 канала
в диф. режиме 
err = X502_SetLChannel(hnd,1,15,X502_LCH_MODE_DIFF,
X502_ADC_RANGE_1, 0);
}
if (err == X502_ERR_OK) {
/* третий логический канал - измерение 17-го канала
относительно общей земли 
err = X502_SetLChannel(hnd,2,16,X502_LCH_MODE_COMM,
X502_ADC_RANGE_02, 0);
}
if (err == X502_ERR_OK) {
/* установка других настроек 
}
if (err == X502_ERR_OK) {
/* передаем настройки в модуль 
err = X502_Configure(hnd,0);
}
if (err != X502_ERR_OK) {
/* произошла ошибка при настройке параметров... 
}
	*/
	int err = L502_SetLChannelCount((t_l502_hnd)hnd, count);
	if(err == L502_ERR_OK) 
	{
		for(int i = 0; i < count; ++i)
		{
			err = L502_SetLChannel((t_l502_hnd)hnd,i, ch[i],mode[i],range[i],0);
			if(err != L502_ERR_OK) return false;
		}
		err = L502_Configure((t_l502_hnd)hnd,0);
		if (err == L502_ERR_OK) {
			/* Считываем кадр данных АЦП из одного отсчета */
			return L502_ERR_OK == L502_AsyncGetAdcFrame((t_l502_hnd)hnd, L502_PROC_FLAGS_VOLT, 1000, value);
		}
	}
	return L502_ERR_OK == err;
}

bool L502::BitOut(unsigned ch, bool value)
{
	int t = ch - 1;
	if(t < 0)
	{
		zprint("error channel date: %d\n", t);
	}
	unsigned bits = 1 << (t);
	return L502_ERR_OK == L502_AsyncOutDig((t_l502_hnd)hnd, value ? bits: 0, ~bits);
}
bool L502::BitIn(unsigned &value)
{
	return L502_ERR_OK == L502_AsyncInDig((t_l502_hnd)hnd, &value);
}
#else
#include "Emulator\Emulator.h"
L502::L502(){}
bool  L502::Init(){return true;}
void  L502::Destroy(){}
bool   L502::SetupParams(){return true;}
int   L502::Start(){return 0;}
int   L502::Stop(){return 0;}
bool L502::Read(unsigned &startChennel, double *data, unsigned &count)
{
	Singleton<Emulator>::Instance().Read(startChennel, data, count, 50);
	return true;
}
bool L502::ReadAsync(unsigned ch, int mode, int range, double &value)
{
	value = ch;
	return true;
}

bool L502::ReadAsyncChannels(int count, unsigned *ch, int *mode, int *range, double *value)
{
	return true;
}

bool L502::BitOut(unsigned ch, bool value)
{
	return true;
}

bool L502::BitIn(unsigned &value)
{
	return true;
}
#endif