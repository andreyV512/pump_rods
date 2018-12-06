#include "DebugMess.h"
#ifdef XDEBUG
#include <stdio.h>
#define d_mess debug.print
namespace {
	wchar_t name[] = L"Parallel hatch in space";
	wchar_t eventName[] = L"debug event 2015 12 16 08 29";
	HANDLE h;
}
DebugMess::DebugMess()
{
	h = CreateEvent(NULL, TRUE, FALSE, eventName);
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TMapData),                // buffer size  
		name);                 // name of mapping object

	int res = GetLastError();
	map = (TMapData *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		sizeof(TMapData));           

	if (map == NULL) 
	{ 
		d_mess("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
	if(0 == res)
	{
		map->head = 0;
		map->tail = 0;
	}
}

void DebugMess::Destroy(void)
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
#pragma warning(disable : 4996)
void DebugMess::print(const char *format, ...)
{
	if(NULL != map)
	{
		LONG i = InterlockedIncrement(&map->head);
		--i;
		i &= 0xff;
		char *b = map->data[i];
		va_list args;
		va_start (args, format);
		vsprintf(b, format, args);
		va_end (args);
		b[strlen(b)] = '\0';
		SetEvent(h);
	}
}
//---------------------------------------------------------------------------------------
ViewerDebugMess::ViewerDebugMess() : map(NULL)
{	
	h = CreateEvent(NULL, TRUE, FALSE, eventName);
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TMapData),                // buffer size  
		name);                 // name of mapping object
	if (hMapFile == NULL) 
	{ 
		d_mess("Could not open file mapping object (%d).\n", 
			GetLastError());
		return;
	} 

	map = (TMapData *)MapViewOfFile(hMapFile,    // handle to mapping object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,                    
		0,                    
		sizeof(TMapData));                   

	if (map == NULL) 
	{ 
		d_mess("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
}
//----------------------------------------------------------------------------
ViewerDebugMess::~ViewerDebugMess()
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
//----------------------------------------------------------------------------
char *ViewerDebugMess::get()
{
	static char b[512];		
	WaitForSingleObject(h, INFINITE);
	while(map && map->tail < map->head)
	{
		CharToOemA(map->data[map->tail & 0xff], b);
		++map->tail;
		return b;
	}
	ResetEvent(h);
	return NULL;
}
//-----------------------------------------------------------------------------
class  DebugMess::Initialization
{
public:
	DebugMess &operator()()
	{
		static DebugMess x;
		return x;
	}
};
DebugMess &debug = DebugMess::Initialization()();
#endif
