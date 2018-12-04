#include "stdafx.h"
#include "SaveScreenToJpeg.h"

using namespace Gdiplus;
void SaveBitmap(wchar_t *name, Bitmap *bmp)
{
	UINT num = 0, size = 0;
	ImageCodecInfo info[20];
	GetImageEncodersSize(&num, &size);
	GetImageEncoders(num, size, info);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(info[j].MimeType, L"image/jpeg") == 0 )
		{
			bmp->Save(name, &info[j].Clsid, NULL);
			break;
		}    
	}
}
