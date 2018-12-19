#include "stdafx.h"
#pragma warning(disable :4995)
#pragma warning(disable :4996)
#include "Dialogs.h"
#include "Windows\MainWindow.h"
#include <Shlwapi.h>
#include <locale>
#include "Log\LogBuffer.h"

void SaveDateFile::Do(HWND h)
{
	KillTimer(h, MainWindow::IDT_TIMER1);
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	int len = wcslen(path);
	for(wchar_t *i = &path[len]; i != path; --i)
	{
		if('\\' == *i)
		{
			*i = 0;
			break;
		}
	}
	wcscat(path, L"\\ArchiveEvent\\");
	CreateDirectory(path, NULL);
	int length = wcslen(path);
	time_t t = time(NULL);
	tm *timeinfo = localtime (&t);
	wsprintf(&path[length], L"%.2d%.2d%.2d%.2d%.2d%.2d.txt"
		, timeinfo->tm_year - 100, 1 + timeinfo->tm_mon, timeinfo->tm_mday
		, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
		);	
	HANDLE hFile = CreateFile(path,                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_NEW,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if(hFile != INVALID_HANDLE_VALUE)
	{
		Log::TData *d = NULL;
		Log::TData *d0 = NULL;
		char c[512];
		wchar_t w[512];
		for(int i = 0; i < 1024; ++i)
		{
			if(Log::IsRow(i, d))
			{
				int tme = Log::IsRow(i + 1, d0) ? d->time - d0->time: 0;
				sprintf(c, "%7d   ", tme);
				int len = strlen(c);
				LogMess::FactoryMessages::Instance().Text(d->id, &c[len], d->value);	
				strcat(c, "\n");
				len = strlen(c);
				MultiByteToWideChar( CP_ACP, 0, c,  -1, w, 512);
				DWORD dwBytesWritten = 0;
				BOOL bErrorFlag = WriteFile( 
					hFile,           // open file handle
					w,      // start of data to write
					len * sizeof(wchar_t),  // number of bytes to write
					&dwBytesWritten, // number of bytes that were written
					NULL);            // no overlapped structure

				if (FALSE == bErrorFlag)
				{
					break;
				}
			}
		}
		CloseHandle(hFile);
	}
	wchar_t buf[1024];
	wsprintf(buf, L"Данные сохранены в файле:\n%s", path);
	MessageBox(h, buf, L"Сообщение", MB_ICONINFORMATION);
	SetTimer(h,             // хэндл главного окна
		MainWindow::IDT_TIMER1,            // идентификатор таймера
		1000,                 // интервал - 1 секунд
		(TIMERPROC) NULL);     // процедуры таймера нет
}
