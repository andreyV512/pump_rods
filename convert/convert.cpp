// convert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>

#pragma warning(disable : 4996)

unsigned FileSize(FILE *f)
{
	fseek(f,0L,SEEK_END);
	unsigned  size = ftell(f);
	fseek(f,0L,SEEK_SET);
	return size;
}

void Conv()
{
	FILE *fD = fopen("ac test3.vlt", "rb");
	FILE *fA = fopen("dc test.vlt", "rb");
	FILE *fR = fopen("test3.dat", "wb+");

	if(NULL != fA && NULL != fD && NULL != fR)
	{
		int counterA = 0;
		int counterB = 0;
		double data[2];

		double res;

		int magic_number = 0xCAFE0002;
		fwrite(&magic_number, sizeof(magic_number), 1, fR);

		fwrite(&counterA, 2 * sizeof(counterA), 1, fR);

		while(!feof(fA))
		{
			if(0 == fread(data, sizeof(data), 1, fA))break;
			res = 1.0 * data[1];
			fwrite(&res, sizeof(res), 1, fR);
			++counterA;
		}

		while(!feof(fD))
		{
			if(0 == fread(data, sizeof(data), 1, fD))break;
			res = 1.0 * data[0];
			fwrite(&res, sizeof(res), 1, fR);
			++counterB;
		}

		fseek(fR,sizeof(int),SEEK_SET);
		fwrite(&counterA, sizeof(counterA), 1, fR);
		fwrite(&counterB, sizeof(counterB), 1, fR);

		fclose(fA);
		fclose(fD);
		fclose(fR);

		printf("%d %d\n", counterA, counterB);
	}
}

double def[6000000];
double str[6000000];
double res[6000000];

void Ch()
{
	FILE *f= fopen("D:\\Projects\\Work\\pump_rods\\convert\\test3.dat", "rb");
	if(NULL == f)
	{
		printf("err open file");
		//	getchar();
		return;
	}
	int magicNumber;
	fread(&magicNumber, 1, sizeof(int), f);

	int defCount;
	fread(&defCount, sizeof(int), 1, f);

	int strCount;
	fread(&strCount, sizeof(int), 1, f);

	fread(def, defCount, sizeof(double), f);
	fread(str, strCount, sizeof(double), f);

	fclose(f);


	int count = 243450 - 123950;
	double delta = 441500.0 / count;

	int pred = 0;

	for(int i = 123950; i < 123950 + count; ++i)
	{
		int last = int(delta * (1 + i - 123950));
		for(int j = pred; j < last; ++j)
		{
			res[j] = str[i];
		}
		pred = last;
	}

	f= fopen("D:\\Projects\\Work\\pump_rods\\convert\\test4.dat", "wb+");
	fwrite(&magicNumber  , sizeof(magicNumber), 1, f);

	fwrite(&defCount  , sizeof(defCount), 1, f);
	fwrite(&strCount  , sizeof(strCount), 1, f);

	fwrite(def   , sizeof(double), defCount, f);
	fwrite(res   , sizeof(double), strCount, f);
	fclose(f);
	printf("ok\n");
}


int _tmain(int argc, _TCHAR* argv[])
{
	//Conv();
	//Ch();
	//getchar();
	//char x[] = {(char)0xc9, 0x01, 0xaf, 0x3f};
	char x[] = {(char)0x9a, 0xd3, 0xae, 0x3f, 0};

	//int  z = *(int *)x;

	printf("%s\n", x);

	getchar();
	return 0;
}

