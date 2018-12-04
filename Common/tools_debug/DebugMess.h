#pragma once
#define XDEBUG
#ifdef XDEBUG
#include <windows.h>
#include <typeinfo> 
struct TMapData
{
	LONG head;
	LONG tail;
	char data[256][512];
};
class DebugMess
{	
	HANDLE hMapFile;
    TMapData *map;
	DebugMess(void);
	DebugMess &operator=(const DebugMess &);
public:	
	class Initialization;
	void Destroy(void);
	void print(char *, ...);
};

class ViewerDebugMess
{
	HANDLE hMapFile;
    TMapData *map;
public:
	ViewerDebugMess(void);
public:
	~ViewerDebugMess(void);
	char *get();
};

extern DebugMess &debug;
#define dprint debug.print
#define zprint(mess, ...)debug.print("%s "##mess, __FUNCTION__, ##__VA_ARGS__)
#define xprint(mess, ...)debug.print("%s "##mess, __FUNCTION__, ##__VA_ARGS__)
#else
#define dprint
#define zprint
#define xprint
#endif
