#pragma once
#include <icrsint.h>
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") rename("BOF","adoBOF")



class Sqlsave
{
public:
	Sqlsave(void);
	virtual ~Sqlsave(void);

public:
	void ConnectDatabase(CString ip,CString databasename,CString username,CString password);   //连接SQL server2000数据库
	void ConnectDatabase(CString datasource,CString username,CString password);                        //连接oracle数据库
	void ConnectDatabase(CString serverip,CString database,CString username,CString password,CString port);   //连接Mysql数据库
	BOOL ExecuteSQL(CString bstrSQL);
	_RecordsetPtr& GetRecordSet(CString bstrSQL);
	CString GetFields(CString bstrSQL);
	 void Close();

public:	
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;
	
};
