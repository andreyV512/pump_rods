#pragma once 
#include "Base.hpp"

namespace{
static const wchar_t *connStr = L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Data Source=(local);Initial Catalog=%s;";
}

class CExpressBase: public CBase
{
public:
	template<typename ConstructorTables, typename Tables>CExpressBase(const wchar_t *name, ConstructorTables &o, Tables &tables)
	{
		wchar_t buf[1024];
		wsprintf(buf, connStr, name);
		try
		{
			conn.CreateInstance(__uuidof(ADODB::Connection)); 
			conn->Open(L"File Name=base.udl", L"", L"",NULL);
		}
		catch(...)
		{   
			//conn->Open(L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Data Source=(local)", L"", L"",NULL);
			conn->Open(L"File Name=base.udl", L"", L"",NULL);
			wsprintf(buf, L"CREATE DATABASE %s", name);
			conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 
			wsprintf(buf, L"ALTER DATABASE %s MODIFY FILE ( NAME = '%s_log', MAXSIZE = 5GB , FILEGROWTH = 10%)", name, name);			
			conn->Execute(buf,  NULL, ADODB::adExecuteNoRecords); 			
			conn->Close();

			wsprintf(buf, connStr, name);
			conn->Open(buf, L"", L"",NULL);
			o(&tables, this);

			wsprintf(buf
				, L"IF NOT EXISTS (SELECT * FROM sys.server_principals WHERE name = N'user')"\
				     L"CREATE LOGIN [user] WITH PASSWORD=N'password'"\
				        L", DEFAULT_DATABASE=[%s], DEFAULT_LANGUAGE=[us_english], CHECK_EXPIRATION=OFF, CHECK_POLICY=OFF"
				, name
				);
			 conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 

			wsprintf(buf, L"CREATE USER [user] FOR LOGIN [user] WITH DEFAULT_SCHEMA=[dbo]");
		    conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 

			wsprintf(buf, L"EXEC sp_addrolemember 'db_datareader','user'");
		    conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 

			wsprintf(buf, L"EXEC sp_addrolemember 'db_datawriter','user'");
		    conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 
			
			wsprintf(buf, L"EXEC sp_addrolemember 'db_accessadmin','user'");
		    conn->Execute(buf, NULL, ADODB::adExecuteNoRecords); 
		}		
	}
};
