#pragma once

namespace StatusMessages
{
	bool Message(int id, wchar_t *mess, unsigned &color);
}

int ResultMessageId(unsigned *);

struct ColorBar
{
	void ColorBar::operator()(double &data, unsigned &color, int id, double defData);
};

struct StatusText
{
	void FromSensors(unsigned *sens, unsigned &color, bool &visible, wchar_t *buf);
	void operator()(int id, unsigned &color, bool &visible, wchar_t *);
};

struct StatusColor
{
	unsigned operator()(unsigned id) ;
};

bool IsDefect(unsigned);

int IsKlass(unsigned id);

unsigned char ResAsu(unsigned char id);

unsigned StatBits(unsigned);

//template<class T>struct AsuResBits
//{
//	unsigned operator()(unsigned char id)
//	{
//		unsigned bits = StatBits(id);
//		bool t2 = 0 != (bits & (1 << TL::IndexOf<status_list, BorderKlass2<T> >::value));
//		bool tb = 0 != (bits & (1 << TL::IndexOf<status_list, BorderDefect<T> >::value));
//		
//		static const unsigned char ok = 1;
//		static const unsigned char k2 = 2;
//		static const unsigned char br = 3;
//
//		unsigned t = 0;
//
//		if(t2) t |= k2;
//		if(tb) t |= br;
//		if(!t2 && !tb) t |= ok;
//
//		return t;
//	}
//};

wchar_t *StatusTextZone(int, unsigned &);