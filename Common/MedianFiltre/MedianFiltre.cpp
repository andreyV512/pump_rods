#include "MedianFiltre\MedianFiltre.h"
#include <windows.h>

MedianFiltre::MedianFiltre()
{
	width = 0;
	index = 0;
	medianIndex = 0;
	memset(buf, 0, sizeof(buf));
}

void MedianFiltre::Init(int width_, double *d)
{
	width_ |= 1;
	width_ &= 0xf;
	index = width_;
	medianIndex = width_ / 2;
	width = width_;
	memcpy(buf, d, sizeof(double) * width_);
}

void MedianFiltre::AbsInit(int width_, double *d)
{
	width_ |= 1;
	width_ &= 0xf;
	index = width_;
	medianIndex = width_ / 2;
	width = width_;
	for(int i = 0; i < width_; ++i)
	{
		buf[i] = d[i] > 0 ? d[i] : -d[i];
	}
}

void MedianFiltre::InitWidth(int width_)
{
	width_ |= 1;
	width_ &= 0xf;
	width = width_;
	index = width_;
	medianIndex = width_ / 2;
}

void MedianFiltre::InitData(double *d)
{
	index = width;

	medianIndex = width / 2;
	ZeroMemory(ind, sizeof(ind));
	memcpy(buf, d, sizeof(double) * width);
}

double MedianFiltre::operator()(double d)
{
	int index_ = index % width;
	++index;
	
	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;

	for(int i = 0; i < width - 1; ++i)
	{
		for(int j = i + 1; j < width; ++j)
		{
		   if(buf[i] >= buf[j]) ++ind[i];
		   else if(buf[i] < buf[j]) ++ind[j];
		}
	}

	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return buf[i];
	}
	return buf[0];
}
//----------------------------------------------------------------------
int MedianFiltre::Add(double d, int st)
{
	int index_ = index % width;
	++index;
	
	buf[index_] = d;
	status[index_] = st;

	memset(ind, 0, width * sizeof(int));
	for(int i = 0; i < width - 1; ++i)
	{
		for(int j = i + 1; j < width; ++j)
		{
		   if(buf[i] >= buf[j]) ++ind[i];
		   else if(buf[i] < buf[j]) ++ind[j];
		}
	}

	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}

int MedianFiltre::Add(double d, double bit)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;
	bit2[index_] = bit;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}

int MedianFiltre::Add(double d, int st, void *p)
{
	int index_ = index % width;
	++index;
	
	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;
	status[index_] = st;
	data[index_] = p;

	for(int i = 0; i < width - 1; ++i)
	{
		for(int j = i + 1; j < width; ++j)
		{
		   if(buf[i] >= buf[j]) ++ind[i];
		   else if(buf[i] < buf[j]) ++ind[j];
		}
	}

	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
int MedianFiltre::Add(double d)
{
	int index_ = index % width;
	++index;
	
	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;

	for(int i = 0; i < width - 1; ++i)
	{
		for(int j = i + 1; j < width; ++j)
		{
		   if(buf[i] >= buf[j]) ++ind[i];
		   else if(buf[i] < buf[j]) ++ind[j];
		}
	}

	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i])
		{
			if(index_ >= i) return i - index_;
			return i - index_ - width;
		}
	}
	return 0;
}
//--------------------------------------------------------------------
void MedianFiltre::Clear(int width_)
{
	medianIndex = width_ / 2;
	index = 0;
	indexX = 0;
	width = width_;
	ZeroMemory(buf, sizeof(buf));
	ZeroMemory(bufX, sizeof(bufX));
	ZeroMemory(status, sizeof(status));
	for(int i = 0; i < width; ++i)
	{
		ind[i] = i;
		indX[i] = i;
	}
}
//------------------------------------------------------------------
void MedianFiltre::SetVal(int width_, double val)
{
	medianIndex = width_ / 2;
	index = 0;
	width = width_;
	ZeroMemory(buf, sizeof(buf));
	for(int i = 0; i < width; ++i)
	{
		ind[i] = i;
		buf[i] = val;
	}
}
//------------------------------------------------------------------------------
