// testZip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <shlwapi.h>
#include "tools/Zip.h"
#include "tools/ZipAll.h"
#include "tools/StoredFiles.h"
#include "templates/typelist.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	wchar_t *p = PathFindExtension(path);
	wprintf(L"%s\n", p);

	//for(int i = 0; i < 100; ++i)
	//{
	//	wchar_t *p = StoredFiles::PathFile(path, L"Stored", L"dat");
	//	FILE *f = _wfopen(p, L"wb+");
	//	fwrite(p, 1, 1000, f);
	//	Sleep(1000);
	//	fclose(f);
	//	wprintf(L"%s\n", p);
	//}


//	Zip::ZipFile(L"C:\\Users\\Andrey\\Desktop\\aktau\\190619_1046.dat");
	
//	Zip::UnZipFile(L"C:\\Users\\Andrey\\Desktop\\eclipce\\readme.txt.bz2");

//	Zip::ZipAll();
	StoredFiles::DeleteExtraFiles(L"Stored", L"dat.bz2", 10);

	getchar();
	return 0;

}

