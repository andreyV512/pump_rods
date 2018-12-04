#pragma once
#include "tools_debug/DebugMess.h"
#pragma warning( disable : 4146 )
#import "c:\program files\common files\system\ado\msado15.dll" rename("EOF", "EndOfFile")
#import "c:\program files\common files\system\ado\msadox.dll" 
namespace
{
static wchar_t *const udl = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
//static wchar_t *const udl = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";
static wchar_t *const mdb = L".mdb;Mode=ReadWrite;Persist Security Info=False";
static wchar_t *const mdbRead = L".mdb;Mode=Read;Persist Security Info=False";
}
//------------------------------------------------------------------------------------------
class Initialize
{
public:
	Initialize(){CoInitializeEx(NULL,COINIT_MULTITHREADED);}
	~Initialize(){CoUninitialize();}
};
//------------------------------------------------------------------------------------------
class CBase
{	
public:
	ADODB::_ConnectionPtr  conn;
	CBase(){}
	template<typename ConstructorTables, typename Tables>CBase(const wchar_t *path, ConstructorTables &o, Tables &tables)
	{
		_bstr_t strcnn(udl);
		strcnn += path;
		strcnn += mdb;
		try
		{
			try
			{
				conn.CreateInstance(__uuidof(ADODB::Connection)); 
				conn->Open( strcnn, L"", L"", NULL);
			}
			catch(...)
			{
				_bstr_t cn(udl);
				cn += path;
				cn += L".mdb";
				ADOX::_CatalogPtr catalog;
				catalog.CreateInstance(__uuidof ( ADOX::Catalog));
				catalog->Create(cn);
				conn->Open( strcnn, L"", L"", NULL);	
				o(&tables, this);
				dprint("tables create OK\n");
			}
		}
		catch(...){}
	}

	template<typename Tables>CBase(const wchar_t *path, Tables &tables)
	{
		try
		{
			_bstr_t strcnn(udl);
			strcnn += path;
			strcnn += mdb;
			conn.CreateInstance(__uuidof(ADODB::Connection)); 
			conn->Open( strcnn, L"", L"", NULL);
		}
		catch(...){}
	}

	CBase(const wchar_t *path, bool ReadOnly = false)
	{
		_bstr_t strcnn(udl);
		strcnn += path;
		if(ReadOnly) strcnn += mdbRead; else strcnn += mdb;
	
		try
		{
		  conn.CreateInstance(__uuidof(ADODB::Connection)); 
		  conn->Open( strcnn, L"", L"", NULL);
		}
		catch(...){}
	}

	~CBase()
	{
		if(IsOpen())  conn->Close();
	}

	void Close()
	{
        if(IsOpen())  conn->Close();
	}

	bool IsOpen()
	{
		try
		{
			return conn->State == ADODB::adStateOpen;
		}
		catch(...)
		{
			return false;
		}
	}

	bool RecordSetOpen(wchar_t *strsql, ADODB::_RecordsetPtr &rst)
	{
		try
		{
			rst.CreateInstance(__uuidof(ADODB::Recordset));
			rst->Open(_bstr_t(strsql), conn.GetInterfacePtr(),
				ADODB::adOpenStatic, ADODB::adLockOptimistic, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	ADODB::_RecordsetPtr ConnectionSQL(wchar_t *strsql)
	{
		 return conn->Execute(strsql, NULL, ADODB::adExecuteNoRecords);
	}
	bool OpenTable(const wchar_t *table, ADODB::_RecordsetPtr &rec)
	{
		try
		{
			rec.CreateInstance(__uuidof(ADODB::Recordset));
			rec->Open(_variant_t(table), conn.GetInterfacePtr()
				, ADODB::adOpenStatic
				, ADODB::adLockOptimistic,ADODB::adCmdTable);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
};