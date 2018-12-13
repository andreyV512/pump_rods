#include "Device502.h"
#include "App/AppBase.h"
#include "App/Config.h"
#include "tools_debug/DebugMess.h"
#ifndef DEBUG_ITEMS
#include "L502/l502api.h"
#pragma comment(lib, "../Common/L502/l502api.lib")
#include <stdio.h>
#include "App/AppBase.h"

L502SolidGroup::L502SolidGroup()
	: ADC_FREQ(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
	, referenceV(Singleton<SolenoidParametersTable>::Instance().items.get<ReferenceRangeSignal>().value)
	, dataV(Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value)
	, READ_TIMEOUT(100)
{}

bool L502SolidGroup::Init()
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

void L502SolidGroup::Destroy()
{
	 /* закрываем связь с модулем */
        L502_Close((t_l502_hnd)hnd);
        /* освобождаем описатель */
        L502_Free((t_l502_hnd)hnd);
}

bool L502SolidGroup::SetupParams(int *f_channels, int *f_ch_ranges, int ADC_FREQ, int countChannels)
{
//	int f_channels[] = {
//		adcParam.get<InputSignal>().value
//		, adcParam.get<ReferenceSignal>().value
//	};
//	int f_ch_ranges[] = {referenceV, dataV};

	//static const int count_inputs = dimention_of(f_channels);
	int err = L502_SetLChannelCount((t_l502_hnd)hnd, countChannels);
	for (int i=0; (i < countChannels) && !err; i++)
        err = L502_SetLChannel((t_l502_hnd)hnd, i, f_channels[i], L502_LCH_MODE_COMM, f_ch_ranges[i], 0);
	/* устанавливаем частоты ввода для АЦП и цифровых входов */
	double f_adc = ADC_FREQ;
	double f_frame = (double)ADC_FREQ/countChannels;
    if (!err)
    {
        err = L502_SetAdcFreq((t_l502_hnd)hnd, &f_adc, &f_frame);
    }

    /* записываем настройки в модуль */
    if (!err)
	{
		dprint("frequency adc = %0.0f frequency chenell = %0.0f", f_adc, f_frame);
        err = L502_Configure((t_l502_hnd)hnd, 0);
	}

    /* разрешаем синхронные потоки */
    if (!err)
    {
        err = L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC);
    }

    return 0 == err;
}
int L502SolidGroup::Start()
{
	int err = L502_StreamsStart((t_l502_hnd)hnd);
	if (err)
		dprint("error collections date: %s!", L502_GetErrorString(err));
	return err;
}

int L502SolidGroup::Stop()
{
	/* останавливаем поток сбора данных (независимо от того, была ли ошибка) */
	int err = L502_StreamsStop((t_l502_hnd)hnd);
	if (err)
	{
		dprint("error colecton date: %s", L502_GetErrorString(err));
	}
	return err;
}

int L502SolidGroup::Read(unsigned &startChennel, double *data, unsigned &count)
{
	unsigned rcv_buf[buffer_length];
	int cnt = L502_Recv((t_l502_hnd)hnd, rcv_buf, buffer_length, READ_TIMEOUT);
	if(cnt > 0)
	{
		L502_GetNextExpectedLchNum((t_l502_hnd)hnd, &startChennel);
		int err = L502_ProcessData((t_l502_hnd)hnd, rcv_buf, cnt, L502_PROC_FLAGS_VOLT,
			data, &count, NULL, NULL);
		if (err)
		{
			dprint("error computing date: %s", L502_GetErrorString(err));
		}
		return err;
	}
	return cnt;
}
#else
bool  Device502::Init(){return true;}
void  Device502::Destroy(){}
bool   Device502::SetupParams(int *, int *, int, int){return true;}
int   Device502::Start(){return 0;}
int   Device502::Stop(){return 0;}
int Device502::Read(unsigned &startChennel, double *data, unsigned &count){return 0;}
#endif