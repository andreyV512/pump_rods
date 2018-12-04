#pragma once 

struct MedianFiltre
{
	int index;
	int width;
	int medianIndex;
	double buf[16];
	double bit2[16];
	int status[16];
	void *data[16];
	int ind[16];
	MedianFiltre();
	void Init(int , double *);
	void AbsInit(int width_, double *d);
	void InitWidth(int);
	void InitData(double *);
	int Add(double);
	double operator()(double);
	int Add(double, int);
	int Add(double, double, int);
	int Add(double d, int st, void *p);
	int Add(double d, double b, int st, void *p);
	int Add(double d, void *p);
	int Add(double d, double b);
	void Clear(int);
	void SetVal(int, double );
public:
	int indexX;
	double bufX[16];
	int indX[16];
	int AddX(double);
};