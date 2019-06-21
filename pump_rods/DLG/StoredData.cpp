#include "Dialogs.h"
#include <stdio.h>
#include "DataItem\DataItem.h"
#include "window_tool\DlgFiles.h"
#include "Compute/Compute.h"
#include "tools/StoredFiles.h"
#include "tools/Zip.h"
#include "tools/ZipAll.h"

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

	void StoreData_0002(FILE *f, DefectSig<DataItem::Buffer> &def, StructSig<DataItem::Buffer> &str)
	{
		int magic = magic_number + 2;
		fwrite(&magic  , sizeof(magic), 1, f);

		fwrite(&def.currentOffset  , sizeof(def.currentOffset), 1, f);
		fwrite(&str.currentOffset  , sizeof(str.currentOffset), 1, f);

		fwrite(&def.inputData   , sizeof(double), def.currentOffset, f);
		fwrite(&str.inputData   , sizeof(double), str.currentOffset, f);
	}

	void LoadData_0002(FILE *f, DefectSig<DataItem::Buffer> &def, StructSig<DataItem::Buffer> &str)
	{
		fread(&def.currentOffset  , sizeof(def.currentOffset), 1, f);
		fread(&str.currentOffset  , sizeof(str.currentOffset), 1, f);

		fread(&def.inputData   , sizeof(double), def.currentOffset, f);
		fread(&str.inputData   , sizeof(double), str.currentOffset, f);
	}
}

#define STORED_FILE Store::StoreData_0002

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
			STORED_FILE(
				f
				, Singleton<DefectSig<DataItem::Buffer>>::Instance()
				, Singleton<StructSig<DataItem::Buffer>>::Instance()
				);
			fclose(f);
			Zip::ZipAll();
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
	o.ofn.lpstrFilter = L"*.bz2\0*.bz2\0*.dat\0*.dat\0*.*\0*.*\0";
	if(o())
	{
		int len = wcslen(o.sFile) - 4;
		if(0 == wcscmp(&o.sFile[len], L".bz2"))
		{
			Zip::UnZipFile(o.sFile);
			o.sFile[len] = '\0';
		}
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
				case 2: Store::LoadData_0002(f, def, str);break;
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


void AutoStoredData()
{
	CountStoredFilesTable::TItems &items = Singleton<CountStoredFilesTable>::Instance().items;
	if(items.get<CountStoredFilesOn>().value)
	{
		wchar_t buf[1024];

		StoredFiles::PathFile(buf, L"Stored", L"dat");
		FILE *f = _wfopen(buf, L"wb");
		if(f)
		{
			STORED_FILE(
				f
				, Singleton<DefectSig<DataItem::Buffer>>::Instance()
				, Singleton<StructSig<DataItem::Buffer>>::Instance()
				);
			fclose(f);

			Zip::ZipAll();

			StoredFiles::DeleteExtraFiles(L"Stored", L"dat.bz2", items.get<CountStoredFiles>().value);
			StoredFiles::DeleteExtraFiles(L"Stored", L"dat", items.get<CountStoredFiles>().value);
		}
	}
}