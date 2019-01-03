#pragma once
#include <icrsint.h>
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") rename("BOF","adoBOF")



class Sqlsave
{
public:
	Sqlsave(void);
	virtual ~Sqlsave(void);

public:
	void ConnectDatabase(CString ip,CString databasename,CString username,CString password);   //����SQL server2000���ݿ�
	void ConnectDatabase(CString datasource,CString username,CString password);                        //����oracle���ݿ�
	void ConnectDatabase(CString serverip,CString database,CString username,CString password,CString port);   //����Mysql���ݿ�
	BOOL ExecuteSQL(CString bstrSQL);
	_RecordsetPtr& GetRecordSet(CString bstrSQL);
	CString GetFields(CString bstrSQL);
	 void Close();

public:	
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;
	
};
