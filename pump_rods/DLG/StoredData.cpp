#include "Dialogs.h"
#include <stdio.h>
#include "DataItem\DataItem.h"
#include "window_tool\DlgFiles.h"
#include "Compute/Compute.h"

namespace Store
{
	static const int magic_number = 0xCAFE0000;
	void StoreData_0001(FILE *f, DefectSig<DataItem::Buffer> &def, StructSig<DataItem::Buffer> &str)
	{
		int magic = magic_number + 1;
		fwrite(&magic  , sizeof(magic), 1, f);

		fwrite(&def.currentOffset  , sizeof(def.currentOffset), 1, f);
		fwrite(&def.deathZoneFirst , sizeof(def.deathZoneFirst), 1, f);
		fwrite(&def.deathZoneSecond, sizeof(def.deathZoneSecond), 1, f);
		fwrite(&def.threshSortDown , sizeof(def.threshSortDown), 1, f);
		fwrite(&def.threshDefect   , sizeof(def.threshDefect), 1, f);

		fwrite(&str.currentOffset  , sizeof(str.currentOffset), 1, f);
		fwrite(&str.deathZoneFirst , sizeof(str.deathZoneFirst), 1, f);
		fwrite(&str.deathZoneSecond, sizeof(str.deathZoneSecond), 1, f);
		fwrite(&str.threshSortDown , sizeof(str.threshSortDown), 1, f);
		fwrite(&str.threshDefect   , sizeof(str.threshDefect), 1, f);

		fwrite(&def.inputData   , sizeof(double), def.currentOffset, f);
		fwrite(&str.inputData   , sizeof(double), str.currentOffset, f);
	}

	void LoadData_0001(FILE *f, DefectSig<DataItem::Buffer> &def, StructSig<DataItem::Buffer> &str)
	{
		fread(&def.currentOffset  , sizeof(def.currentOffset), 1, f);
		fread(&def.deathZoneFirst , sizeof(def.deathZoneFirst), 1, f);
		fread(&def.deathZoneSecond, sizeof(def.deathZoneSecond), 1, f);
		fread(&def.threshSortDown , sizeof(def.threshSortDown), 1, f);
		fread(&def.threshDefect   , sizeof(def.threshDefect), 1, f);

		fread(&str.currentOffset  , sizeof(str.currentOffset), 1, f);
		fread(&str.deathZoneFirst , sizeof(str.deathZoneFirst), 1, f);
		fread(&str.deathZoneSecond, sizeof(str.deathZoneSecond), 1, f);
		fread(&str.threshSortDown , sizeof(str.threshSortDown), 1, f);
		fread(&str.threshDefect   , sizeof(str.threshDefect), 1, f);

		fread(&def.inputData   , sizeof(double), def.currentOffset, f);
		fread(&str.inputData   , sizeof(double), str.currentOffset, f);
	}
}
#pragma warning(disable: 4996)
void StoreDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить");
	o.ofn.lpstrFilter = L"*.dat\0*.dat\0*.*\0*.*\0";
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"wb");
		if(f)
		{
			Store::StoreData_0001(
				f
				, Singleton<DefectSig<DataItem::Buffer>>::Instance()
				, Singleton<StructSig<DataItem::Buffer>>::Instance()
				);
			fclose(f);
			MessageBox(h, L"Файл сохранён", L"Сообщение!!!", MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(h, L"Файл не сохранён", L"Ошибка!!!", MB_ICONEXCLAMATION);
		}
	}
}

void LoadDlg::Do(HWND h)
{
	OpenData o(h, L"Загрузить");
	o.ofn.lpstrFilter = L"*.dat\0*.dat\0*.*\0*.*\0";
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"rb");
		bool b = false;
		if(f)
		{
			b = true;
			int magic;
			fread(&magic, sizeof(magic), 1, f);
			if(Store::magic_number == (magic & 0xffff0000))
			{
				DefectSig<DataItem::Buffer> &def = Singleton<DefectSig<DataItem::Buffer>>::Instance();
				StructSig<DataItem::Buffer> &str = Singleton<StructSig<DataItem::Buffer>>::Instance();

				switch(magic & 0xffff)
				{
				case 1: Store::LoadData_0001(f, def, str);break;
				default:
					b = false;
				}
				if(b)
				{
					Compute::Recalculation();
					wchar_t buf[2048];
					wsprintf(buf, L"<ff>Загружен файл <a142f4>\"%s\"", o.sFile);
					App::TopLabel(buf);
				}
			}
			else
			{
				b = false;
			}

			fclose(f);
		}
		if(!b)
		{
			MessageBox(h, L"Файл не загружен", L"Ошибка!!!", MB_ICONEXCLAMATION);
		}
	}
}