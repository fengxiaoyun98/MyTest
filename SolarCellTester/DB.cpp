#include "StdAfx.h"
#include "DB.h"

#import "c:\program files\common files\system\ado\msado15.dll" 	

DB::DB(void)
{
}

DB::~DB(void)
{
}
BOOL DB::CreateDatabase(CString dbPath,CString pass)
{
	try{//�½����ݿ�
		ADOX::_CatalogPtr m_pCatalog;
		m_pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
		CString strConnection = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source="); 
		strConnection += dbPath; //myDataBasePath
		strConnection += _T(";Jet OLEDB:Database Password="); 
		strConnection += pass+_T(";"); 
		if(!PathFileExists(dbPath)) 
			m_pCatalog-> Create(_bstr_t(strConnection ));
	}catch (_com_error & e){
		CString strComError;
		strComError.Format(_T("error no: %08lx\n info: %s\nsource: %s\ndesc: %s"),
			e.Error(),                  // ������
			e.ErrorMessage(),           // ������Ϣ
			(LPCTSTR)e.Source(),        // ����Դ
			(LPCTSTR)e.Description());  // ��������      
		::MessageBox(NULL,strComError,_T("����"),MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
} 

void DB::ExeSql(CString dbPath,CString sqlcmd,CString pass)
{
/*	CoInitialize(NULL);        //��ʼ��
	_ConnectionPtr m_pCnt;
	m_pCnt.CreateInstance(__uuidof(Connection));
	_RecordsetPtr m_pRst;
	m_pRst.CreateInstance(__uuidof(Recordset));

	CString str=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	str+=dbPath;
	str += _T(";Jet OLEDB:Database Password="); 
	str += pass+_T(";"); 
    
	try                 
	{	
		m_pCnt->Open(_bstr_t(str),_T(""),_T(""),adModeUnknown);
		m_pRst->Open((_variant_t)("SELECT * FROM SunData"),                // ��ѯDemoTable���������ֶ�
			         (IDispatch*)m_pCnt,   // ��ȡ��ӿ��IDispatchָ��
			         adOpenDynamic,
			         adLockOptimistic,
			         adCmdText);
//		int x = m_pRst->GetFields()->Count;    // ���еļ�¼��Ŀ
//		if(!m_pRst->BOF)
			m_pRst->MoveFirst();
		m_pRst->AddNew();
		m_pRst->PutCollect(_variant_t("Serial"),(_variant_t)("FIRST"));
		m_pRst->Update();

//		if (!m_pRst->adoEOF)
//		{
			m_pRst->MoveLast();
//		}
		m_pRst->AddNew();
		m_pRst->PutCollect(_variant_t("Serial"),(_variant_t)("LAST"));
		m_pRst->Update();
	}
	catch(_com_error &e)
	{
		CString strComError;
		strComError.Format(_T("������: %08lx\n������Ϣ: %s\n����Դ: %s\n��������: %s"),
			e.Error(),                  // ������
			e.ErrorMessage(),           // ������Ϣ
			(LPCTSTR)e.Source(),        // ����Դ
			(LPCTSTR)e.Description());  // ��������
		::MessageBox(NULL,strComError,_T("����"),MB_ICONEXCLAMATION);
	}
//	m_pRst->State;
//	m_pRst->Close(); 
//	m_pRst.Release();
//	if(m_pCnt->State)
//		m_pCnt->Close();
//	m_pCnt= NULL; 
*/
	_ConnectionPtr	m_pCnt;
	m_pCnt.CreateInstance(__uuidof(Connection));
	CString str=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	str+=dbPath;
	str += _T(";Jet OLEDB:Database Password="); 
	str += pass+_T(";");
	try                 
	{			
		m_pCnt->Open(_bstr_t(str),_T(""),_T(""),adModeUnknown);
		m_pCnt->put_ConnectionTimeout(long(5));//�������ӳ�ʱ
		m_pCnt->Execute( _bstr_t(sqlcmd),NULL,adCmdText );
	}
	catch(_com_error &e)
	{
		CString strComError;
		strComError.Format(_T("������: %08lx\n������Ϣ: %s\n����Դ: %s\n��������: %s"),
			e.Error(),                  // ������
			e.ErrorMessage(),           // ������Ϣ
			(LPCTSTR)e.Source(),        // ����Դ
			(LPCTSTR)e.Description());  // ��������
		::MessageBox(NULL,strComError,_T("����"),MB_ICONEXCLAMATION);
	}
	if(m_pCnt->State)
		m_pCnt->Close();
	m_pCnt= NULL;

}

CString DB::GetFields(CString dbPath,CString sqlcmd,CString pass, CString *spField)
{
	CString sFields;
	sFields = L"";
    // �����ݿ⽨������
    _ConnectionPtr	m_pCnt;
	m_pCnt.CreateInstance(__uuidof(Connection));
	CString str=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	str+=dbPath;
	str += _T(";Jet OLEDB:Database Password="); 
	str += pass+_T(";");
	try                 
	{			
		m_pCnt->Open(_bstr_t(str),_T(""),_T(""),adModeUnknown);
		m_pCnt->put_ConnectionTimeout(long(5));//�������ӳ�ʱ
	}
	catch(_com_error &e)
	{
		CString strComError;
		strComError.Format(_T("������: %08lx\n������Ϣ: %s\n����Դ: %s\n��������: %s"),
			e.Error(),                  // ������
			e.ErrorMessage(),           // ������Ϣ
			(LPCTSTR)e.Source(),        // ����Դ
			(LPCTSTR)e.Description());  // ��������
		::MessageBox(NULL,strComError,_T("����"),MB_ICONEXCLAMATION);
	}

    // ���岢��ʼ������
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	_bstr_t   mStrSQL;
	CString   strColName;
	BSTR      bstrColName;
	long      ColCount,i; 
	Field *   field = NULL;		
	HRESULT   hr;
	Fields *  fields = NULL;

	
	//�򿪼�¼�����õ��ֶ����������ֶ������лش�
	m_pRecordset->Open((_variant_t)sqlcmd, m_pCnt.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	
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
		spField[i] = strColName;
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
void DB::CreateTable(CString dbPath,CString sqlcmd)
{
/*
_variant_t RecordsAffected;
///ִ��SQL���CREATE TABLE�������users,users�����ĸ��ֶ�:����ID,�ַ���username,����old,������birthday
m_pConnection->Execute("CREATE TABLE users(ID INTEGER,username 
					   TEXT,old INTEGER,birthday DATETIME)",&RecordsAffected,adCmdText);
*/
}
void DB::AppendItem(CString dbPath,CString sqlcmd)
{
/*
///�����������Ӽ�¼
m_pConnection->Execute("INSERT INTO users(ID,username,old,birthday)
VALUES (1, 'Washington',25,'1970/1/1')",&RecordsAffected,adCmdText);
*/
}
