#pragma once

namespace StatusMessages
{
	bool Message(int id, wchar_t *mess, unsigned &color);
}

int ResultMessageId(unsigned *);

struct StatusText
{
	void FromSensors(unsigned *sens, unsigned &color, wchar_t *buf);
	void operator()(int id, unsigned &color, wchar_t *);
};

struct StatusColor
{
	unsigned operator()(unsigned id) ;
};

bool IsDefect(unsigned);

int IsKlass(unsigned id);

unsigned char ResAsu(unsigned char id);

unsigned StatBits(unsigned);
