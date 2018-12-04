#include "ColorLabel.h"
#include "templates/typelist.hpp"
using namespace Gdiplus;
//---------------------------------------------------------------------------
wchar_t *ColorLabel::operator=(wchar_t *b)
{
	wcsncpy_s((wchar_t *)buffer, dimention_of(buffer), b, dimention_of(buffer));
    return buffer;
}
//---------------------------------------------------------------------------
#pragma warning(disable : 4996)
wchar_t *ColorLabel::operator=(char *b)
{
	MultiByteToWideChar(CP_ACP, 0, b, -1, buffer, dimention_of(buffer));
    return buffer;
}
//----------------------------------------------------------------------------
struct __ColorLabel__
{
	struct TData
	{
		__int64 x, y;
		int color;
	};
	Graphics &g;
	wchar_t *buf;
    wchar_t *const startChar;
	int &left;
	int &top;
	int &fontHeight;
	unsigned &backColor;
	int offs;
	RectF &labelRect;
	TData point[32];
	__ColorLabel__(Graphics &g, ColorLabel &l)
		: g(g)
		, buf(l.buffer)
		, startChar(buf)
		, left(l.left)
		, top(l.top)
	    , fontHeight(l.fontHeight)	
		, backColor(l.backColor)
		, offs(0)
		, labelRect(l.rect)
	{
		memset(point, 0, sizeof(point));
	}	
	void ColorStr(int &d)
	{
		d = 0;
		while(wchar_t c = *buf++)
		{			
			if(c >= '0' && c <= '9')
			{
				d <<= 4;
				d += c - '0';
			}
			else if(c >= 'a' && c <= 'f')
			{
				d <<= 4;
				d += c - 'a' + 10;
			}
			else switch(c)
			{
	           case 'l': Sys(left);return;
	           case 't': Sys(top);return;
	           case 'n': Sys(fontHeight);return;
			   case '>': Txt(); return;
			}
		}	
	}
	void Sys(int &d)
	{
		d = 0;
		while(wchar_t c = *buf++)
		{			
			if(c >= '0' && c <= '9')
			{
				d *= 10;
				d += c - '0';
			}			
			else switch(c)
			{
	           case 'l': Sys(left);return;
	           case 't': Sys(top);return;
	           case 'n': Sys(fontHeight);return;
			   case '>': Txt(); return;
			}
		}	
	}
	void Txt()
	{
		point[offs].x = buf - startChar;		
		while(wchar_t c = *buf++)
		{
			if('<' == c)
			{
				point[offs].y = buf - startChar - 1;
				if(point[offs].y - point[offs].x > 0) ++offs;
				ColorStr(point[offs].color);
				return;
			}
		}
	}
	inline int Change(int &d)
	{	
		return((d & 0xff000000) == 0) ? d |= 0xff000000 : d;
	} 
	void SubStr()
	{
		point[offs].y = buf - startChar - 1;
		Font font(L"Arial", (REAL)fontHeight, FontStyleBoldItalic);

		StringFormat format;
		PointF origin((REAL)left, (REAL)top);
		RectF rect;
		labelRect.X = (REAL)left;
		labelRect.Y = (REAL)top;
		for(int i = 0; i <= offs; ++i)
		{
			int len = int(point[i].y - point[i].x);
			if(len > 0)
			{
				Color fc(Change(point[i].color));
				SolidBrush brushFontColor(fc);
				wchar_t *s = &startChar[point[i].x];
				g.MeasureString(s, len, &font, origin, &format, &rect);    
				g.FillRectangle(&SolidBrush(Color(backColor)), rect);
				g.DrawString(s, len, &font, origin, &brushFontColor);
				origin.X += rect.Width;
				labelRect.Height = rect.Height;
				labelRect.Width = origin.X;
			}
		}
	}
};
void ColorLabel::Draw(Graphics &g)
{
	__ColorLabel__ x(g, *this);
	x.Txt();
	x.SubStr();
}
