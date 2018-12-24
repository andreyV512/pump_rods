#include "Device502.h"
#include "App/AppBase.h"
#include "App/Config.h"
#include "tools_debug/DebugMess.h"
#ifndef DEBUG_ITEMS
#include "L502/l502api.h"
#pragma comment(lib, "../Common/L502/l502api.lib")
#include <stdio.h>
#include "App/AppBase.h"

//Device502::Device502()
//	//: ADC_FREQ(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
//	//, referenceV(Singleton<SolenoidParametersTable>::Instance().items.get<ReferenceRangeSignal>().value)
//	//, dataV(Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value)
//	//, READ_TIMEOUT(100)
//{}

bool Device502::Init()
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

void Device502::Destroy()
{
	 /* закрываем связь с модулем */
        L502_Close((t_l502_hnd)hnd);
        /* освобождаем описатель */
        L502_Free((t_l502_hnd)hnd);
}

bool Device502::SetupParams(int *f_channels, int *f_ch_ranges, int ADC_FREQ, int countChannels)
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
		dprint("L502_SetAdcFreq frequency adc = %0.0f frequency chenell = %0.0f\n", f_adc, f_frame);
    }

    /* записываем настройки в модуль */
    if (!err)
	{		
        err = L502_Configure((t_l502_hnd)hnd, 0);
		dprint("L502_Configure((t_l502_hnd)hnd, 0); %d\n", err);
	}

    /* разрешаем синхронные потоки */
    if (!err)
    {
        err = L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC);
		dprint("L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC) %d\n", err);
    }

    return 0 == err;
}
int Device502::Start()
{
	int err = L502_StreamsStart((t_l502_hnd)hnd);
	if (err)
		dprint("error collections date: %s!", L502_GetErrorString(err));
	return err;
}

int Device502::Stop()
{
	/* останавливаем поток сбора данных (независимо от того, была ли ошибка) */
	int err = L502_StreamsStop((t_l502_hnd)hnd);
	if (err)
	{
		dprint("error colecton date: %s", L502_GetErrorString(err));
	}
	return err;
}

int Device502::Read(unsigned &startChennel, double *data, unsigned &count)
{
	unsigned rcv_buf[buffer_length];
	int cnt = L502_Recv((t_l502_hnd)hnd, rcv_buf, buffer_length, 100);//READ_TIMEOUT);
	if(cnt > 0)
	{
		L502_GetNextExpectedLchNum((t_l502_hnd)hnd, &startChennel);
		count = buffer_length;
		int err = L502_ProcessData((t_l502_hnd)hnd, rcv_buf, cnt, L502_PROC_FLAGS_VOLT,data, &count, NULL, NULL);
		if (err < 0)
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