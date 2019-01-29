// convert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>

unsigned FileSize(FILE *f)
{
	fseek(f,0L,SEEK_END);
	unsigned  size = ftell(f);
	fseek(f,0L,SEEK_SET);
	return size;
}


int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fD = fopen("ac test3.vlt", "rb");
	FILE *fA = fopen("dc test9.vlt", "rb");
	FILE *fR = fopen("test.dat", "wb+");

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

		//while(!feof(fA))
		//{
		//	long long x;
		//	if(0 == fread(&x, sizeof(x), 1, fA))break;
		////	res = 10.0 * data[1];
		////	fwrite(&res, sizeof(res), 1, fR);
		//	++counterA;
		//}
		//
		//while(!feof(fD))
		//{
		//	long long x;
		//	if(0 == fread(&x, sizeof(x), 1, fD))break;
		//	//res = 10.0 * data[0];
		//	//fwrite(&res, sizeof(res), 1, fR);
		//	++counterB;
		//}

		fseek(fR,sizeof(int),SEEK_SET);
		fwrite(&counterA, sizeof(counterA), 1, fR);
		fwrite(&counterB, sizeof(counterB), 1, fR);

		fclose(fA);
		fclose(fD);
		fclose(fR);

		printf("%d %d\n", counterA, counterB);
	}
	getchar();
	return 0;
}

