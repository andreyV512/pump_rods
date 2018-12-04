#pragma once
#include <windows.h>
#include <comutil.h>
#include "templates/typelist.hpp"
//////////////////////////////////////////////////////////////////////
 
///////////////////////////////////////////////////////////////////////
template<int>class ClassAssept;
template<>class ClassAssept<TRUE>{};

#define _ASSERT_(expression, message){ClassAssept<expression> ERROR_##message;(void)ERROR_##message;}

//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4996)
template<int COUNT = 128>struct Holder
{
	typedef wchar_t TBufer[COUNT];
	static const int count = COUNT;
	TBufer buffer;
	Holder(){}
	Holder(wchar_t *b)
	{
		wcsncpy(buffer, b, count - 1);
		buffer[count - 1] = '\0';
	}
	Holder &operator=(wchar_t *b)
	{
		wcsncpy(buffer, b, count - 1);
		buffer[count - 1] = '\0';
		return *this;
	}
	template<int C>Holder &operator=(Holder<C> &b)
	{
		wcsncpy(buffer, b.buffer, count - 1);
		buffer[count - 1] = '\0';
		return *this;
	}
	operator wchar_t *(){return buffer;}
};
//////////////////////////////////////////////////////////////////////////////////
template<size_t N = 128>class StrFromResource
{
  public:
   StrFromResource(){buf[0] = '\0';}
   wchar_t *operator[](size_t i)
   {
      LoadString((HINSTANCE)::GetModuleHandle(NULL), i, buf, N);
      return buf;
   }
   wchar_t *value(){return buf;}
  private:
   wchar_t buf[N];
};
//---------------------------------------------------------------------------
template<typename T>struct Test
{
	typedef typename T::__template_must_be_overridded__ noused; 
};

template<>struct Test<int>
{
	bool operator()(wchar_t *value)
	{
		wchar_t v = *value;
		if(v == '+' || v == '-' || (v >= '0' && v<= '9'))
		{
			++value;
			for(; *value != 0; ++value) 
			{
				v = *value;
				if(v < '0') return false;
				if(v > '9') return false;
			}
			return true;
		}
		return false;
	}
};

template<>struct Test<double>
{
	bool operator()(wchar_t *value)
	{
		wchar_t v = *value;
		if(v == '+' || v == '-' || (v >= '0' && v<= '9'))
		{
			++value;
			for(;; ++value) 
			{
				v = *value;
				if(v == '\0') return true;
				if(v == '.') break;
				if(v < '0') return false;
				if(v > '9') return false;	
			}
			++value;
            for(;; ++value) 
			{
				v = *value;
				if(v == '\0') return true;
				if(v == 'e') break;
				if(v == 'E') break;
				if(v < '0') return false;
				if(v > '9') return false;
			}
			++value;
			++value;
			v = *value;
			if(v == '-' || v == '+') 
			{
				++value;
				if(*value == '\0') return false;
			}
            for(;; ++value) 
			{
				v = *value;
				if(v == '\0') return true;
				if(v < '0') return false;
				if(v > '9') return false;
			}
			return true;
		}
		return false;
	}
};

template<>struct Test<bool>
{
	bool operator()(wchar_t *value)
	{
		return (0 == wcscmp(value, L"true")) || (0 == wcscmp(value, L"false"));
	}
};
//---------------------------------------------------------------------------------
template<typename T> struct Wchar_to;

template<> struct Wchar_to<int>
{
	int operator()(wchar_t *data)
	{
		return _wtoi(data);
	}
};
template<> struct Wchar_to<unsigned>
{
	int operator()(wchar_t *data)
	{
		return _wtoi(data);
	}
};

template<> struct Wchar_to<double>
{
	double operator()(wchar_t *data)
	{
		return _wtof(data);
	}
};
template<> struct Wchar_to<float>
{
	float operator()(wchar_t *data)
	{
		return (float)_wtof(data);
	}
};

template<> struct Wchar_to<bool>
{
	bool operator()(wchar_t *data)
	{
		return 0 == wcscmp(data, L"true");
	}
};

template<> struct Wchar_to<wchar_t *>
{
	wchar_t *operator()(wchar_t *data)
	{
		return data;
	}
};

template<int N> struct Wchar_to<Holder<N>>
{
	wchar_t *operator()(wchar_t *data)
	{
		return data;
	}
};

template<>struct Test<wchar_t *>
{
	bool operator()(wchar_t *value)
	{
		return NULL != value;
	}
};
//-----------------------------------------------------------------
template<typename, int N = 5>struct Wchar_from;

template<>struct Wchar_from<int>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(int data)
	{
       _itow_s(data, buf, 10);
	}
	wchar_t *operator()(){return buf;}
};
template<>struct Wchar_from<bool>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(bool data)
	{
       _itow_s(data, buf, 10);
	}
	wchar_t *operator()(){return buf;}
};
template<int N>struct Wchar_from<Holder<N>>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(Holder<N> &data)
	{
		memmove(buf, data.buffer, sizeof(buf));
	}
	wchar_t *operator()(){return buf;}
};
template<>struct Wchar_from<unsigned>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(unsigned data)
	{
       _itow_s(data, buf, 10);
	}
	wchar_t *operator()(){return buf;}
};

template<int NUM>struct Wchar_from<double, NUM>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(double data)
	{
		char c[_CVTBUFSIZE];
		_gcvt_s(c, 64, data, NUM);
		int len = (int)strlen(c) - 1;
		if('.' == c[len]) c[len] = 0;
		size_t ConvertedChars;
		mbstowcs_s(&ConvertedChars, buf, c, 60);
	}
	wchar_t *operator()(){return buf;}
	wchar_t *operator()(double data)
	{
		char c[_CVTBUFSIZE];
		_gcvt_s(c, 64, data, NUM);
		size_t ConvertedChars;
		mbstowcs_s(&ConvertedChars, buf, c, 60);
		return buf;
	}
};
#pragma warning(disable : 4995 4996)
template<>struct Wchar_from<double, 1>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(double data)
	{
		(*this)(data);
	}
	wchar_t *operator()(){return buf;}
	wchar_t *operator()(double data)
	{
		char c[_CVTBUFSIZE];
		sprintf(c, "%.1f", data);
		wsprintf(buf, L"%S", c);
		return buf;
	}
};

template<int NUM>struct Wchar_from<float, NUM>
{
	wchar_t buf[_CVTBUFSIZE];
	Wchar_from(float data)
	{
		char c[_CVTBUFSIZE];
        _gcvt_s(c, 64, data, NUM);
        size_t ConvertedChars;
		mbstowcs_s(&ConvertedChars, buf, c, 60);
	}
	wchar_t *operator()(){return buf;}
};

template<>struct Wchar_from<wchar_t *>
{
	wchar_t *buf;
	Wchar_from(wchar_t *data)
	   : buf(data){}
	wchar_t *operator()(){return buf;}
};

//----------------------------------------------------------------------------
template<typename T>struct from_variant;
template<>struct from_variant<int>
{
	int operator()(_variant_t &v)
	{
		return v.intVal;
	}
};
template<>struct from_variant<double>
{
	double operator()(_variant_t &v)
	{
		return v.dblVal;
	}
};
template<>struct from_variant<float>
{
	float operator()(_variant_t &v)
	{
		return v.fltVal;
	}
};
template<>struct from_variant<wchar_t *>
{
	wchar_t *operator()(_variant_t &v)
	{
		return v.bstrVal;
	}
};
template<>struct from_variant<bool>
{
	bool operator()(_variant_t &v)
	{
		return v.boolVal != (VARIANT_BOOL)false;
	}
};
//------------------------------------------------------------------------------------
template<typename T, int IND>struct Ind : T
{
	static const int value = IND;
};
template<typename U>struct Upper
{
	template<typename T>struct Wapper
	{
		static char Tst(...);
		template<int N>static double Tst(Ind<U, N>);
		typedef typename TL::_if<
			sizeof(Tst(T())) == sizeof(double)
			, T
			, NullType
		>::Result Result;
	};
};

