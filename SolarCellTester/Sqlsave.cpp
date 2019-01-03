#include "StdAfx.h"
#include "Sqlsave.h"

Sqlsave::Sqlsave(void)
{
	
}

Sqlsave::~Sqlsave(void)
{

}

void Sqlsave::ConnectDatabase(CString ip,CString databasename,CString username,CString password)                      //����SQL server2000���ݿ�
{
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{
		CString  strConnect = _T("Provider=SQLOLEDB;Server="+ip+";Database="+databasename+";uid="+username+";pwd="+password+";");
		m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //�������ݿ�
	}
	catch(_com_error &e)
	{

		::MessageBox(NULL,e.Description(),TEXT("����"),MB_OK | MB_ICONWARNING);
	}
}


void Sqlsave::ConnectDatabase(CString datasource,CString username,CString password)                                  //����oracle���ݿ�
{
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{
		CString  strConnect = _T("Provider=OraOLEDB.Oracle.1;Data Source="+datasource+";User ID="+username+";Password="+password+";");
		m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //�������ݿ�
	}
	catch(_com_error &e)
	{

		::MessageBox(NULL,e.Description(),TEXT("����"),MB_OK | MB_ICONWARNING);
	}
}

void Sqlsave::ConnectDatabase(CString serverip,CString database,CString username,CString password,CString port)               //����Mysql���ݿ�
{
	CoInitialize(NULL);   
	m_pConnection.CreateInstance(__uuidof(Connection));
	try{  

			CString  strConnect = _T("Driver=MySQL ODBC 5.1 Driver;server="+serverip+";database="+database+";uid="+username+";pwd="+password+";port="+port+";");
			m_pConnection ->Open(_bstr_t(strConnect),"","",adModeUnknown);                  //�������ݿ�
		}
	catch(_com_error &e) 
	{
		::MessageBox(NULL,e.Description(),TEXT("����"),MB_OK | MB_ICONWARNING);
	}
}
//ִ���з���ֵ��SQL���
_RecordsetPtr& Sqlsave::GetRecordSet(CString bstrSQL)
{
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(bstrSQL),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		::MessageBox(NULL,e.Description(),TEXT("����"),MB_OK | MB_ICONWARNING);       //�Ķ�

	}
	return m_pRecordset;
}
//ִ���޷���ֵ��SQL���
BOOL Sqlsave::ExecuteSQL( CString bstrSQL )  
{  
	try 
	{  
		m_pConnection->Execute(_bstr_t(bstrSQL),NULL,adCmdText);  
		return true;  
	}  
	catch(_com_error e)  
	{  
		::MessageBox(NULL,e.Description(),TEXT("����"),MB_OK | MB_ICONWARNING);       //�Ķ�
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
	//�õ���¼�����ֶμ���	
	hr = m_pRecordset->get_Fields (&fields);	

	//�õ���¼�����ֶμ����е��ֶε��ܸ���	
	if(SUCCEEDED(hr)) 
		fields->get_Count(&ColCount);	

	// �õ��ֶ�����
	for(i=0;i<ColCount;i++)			
	{			
		fields->Item[i]->get_Name(&bstrColName);	// �õ���¼���е��ֶ���
		strColName=bstrColName;
		sFields += L"[";
		sFields += strColName;
		if(i == ColCount-1)
			sFields += + L"]";
		else
			sFields += + L"],";
	}		

	if(SUCCEEDED(hr))
		fields->Release();//�ͷ�ָ��

	return sFields;
}

void Sqlsave::Close()
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}
