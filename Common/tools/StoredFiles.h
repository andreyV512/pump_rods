#pragma once

namespace StoredFiles
{
	//PathExec(buf);
	wchar_t *PathExec(wchar_t (&buf)[1024]);
	//PathUp(buf, L"Stored");
	wchar_t *PathUp(wchar_t (&buf)[1024], wchar_t *dir);
	//PathFile(buf, L"Stored", "dat");
	wchar_t *PathFile(wchar_t(&buf)[1024], wchar_t *dir, wchar_t *ext);
	//DeleteExtraFiles(L"Stored", "dat", 10);
	void DeleteExtraFiles(wchar_t *dir, wchar_t *ext, int maxCount);
}