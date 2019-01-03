#include "StdAfx.h"
#include "Sqlsave.h"

Sqlsave::Sqlsave(void)
{
	
}

Sqlsave::~Sqlsave(void)
{

}

void Sqlsave::ConnectDatabase(CString ip,CString databasename,CString username,CString password)                      //连接SQL server2000数据库
{
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{
		CString  strConnect = _T("Provider=SQLOLEDB;Server="+ip+";Database="+databasename+";uid="+username+";pwd="+password+";");
		m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //连接数据库
	}
	catch(_com_error &e)
	{

		::MessageBox(NULL,e.Description(),TEXT("警告"),MB_OK | MB_ICONWARNING);
	}
}


void Sqlsave::ConnectDatabase(CString datasource,CString username,CString password)                                  //连接oracle数据库
{
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{
		CString  strConnect = _T("Provider=OraOLEDB.Oracle.1;Data Source="+datasource+";User ID="+username+";Password="+password+";");
		m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //连接数据库
	}
	catch(_com_error &e)
	{

		::MessageBox(NULL,e.Description(),TEXT("警告"),MB_OK | MB_ICONWARNING);
	}
}

void Sqlsave::ConnectDatabase(CString serverip,CString database,CString username,CString password,CString port)               //连接Mysql数据库
{
	CoInitialize(NULL);   
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{  

			CString  strConnect = _T("Driver=MySQL ODBC 5.1 Driver;server="+serverip+";database="+database+";uid="+username+";pwd="+password+";port="+port+";");
			m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //连接数据库
		}
	catch(_com_error &e) 
	{
		::MessageBox(NULL,e.Description(),TEXT("警告"),MB_OK | MB_ICONWARNING);
	}
}
//执行有返回值的SQL语句
_RecordsetPtr& Sqlsave::GetRecordSet(CString bstrSQL)
{
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(bstrSQL),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		::MessageBox(NULL,e.Description(),TEXT("警告"),MB_OK | MB_ICONWARNING);       //改动

	}
	return m_pRecordset;
}
//执行无返回值的SQL语句
BOOL Sqlsave::ExecuteSQL( CString bstrSQL )  
{  
	try 
	{  
		m_pConnection->Execute(_bstr_t(bstrSQL),NULL,adCmdText);  
		return true;  
	}  
	catch(_com_error e)  
	{  
		::MessageBox(NULL,e.Description(),TEXT("警告"),MB_OK | MB_ICONWARNING);       //改动
		return false;  
	}  
} 
CString Sqlsave::GetFields(CString bstrSQL)
{
	CString   sFields;
	CString   strColName;
	BSTR      bstrColName;
	long      ColCount,i; 	
	HRESULT   hr;
	Fields *  fields = NULL;

	GetRecordSet(bstrSQL);
	//得到记录集的字段集和	
	hr = m_pRecordset->get_Fields (&fields);	

	//得到记录集的字段集合中的字段的总个数	
	if(SUCCEEDED(hr)) 
		fields->get_Count(&ColCount);	

	// 得到字段序列
	for(i=0;i<ColCount;i++)			
	{			
		fields->Item[i]->get_Name(&bstrColName);	// 得到记录集中的字段名
		strColName=bstrColName;
		sFields += L"[";
		sFields += strColName;
		if(i == ColCount-1)
			sFields += + L"]";
		else
			sFields += + L"],";
	}		

	if(SUCCEEDED(hr))
		fields->Release();//释放指针

	return sFields;
}

void Sqlsave::Close()
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}
