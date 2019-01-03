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
	try{//新建数据库
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
			e.Error(),                  // 错误编号
			e.ErrorMessage(),           // 错误信息
			(LPCTSTR)e.Source(),        // 错误源
			(LPCTSTR)e.Description());  // 错误描述      
		::MessageBox(NULL,strComError,_T("错误"),MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
} 

void DB::ExeSql(CString dbPath,CString sqlcmd,CString pass)
{
/*	CoInitialize(NULL);        //初始化
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
		m_pRst->Open((_variant_t)("SELECT * FROM SunData"),                // 查询DemoTable表中所有字段
			         (IDispatch*)m_pCnt,   // 获取库接库的IDispatch指针
			         adOpenDynamic,
			         adLockOptimistic,
			         adCmdText);
//		int x = m_pRst->GetFields()->Count;    // 表中的记录数目
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
		strComError.Format(_T("错误编号: %08lx\n错误信息: %s\n错误源: %s\n错误描述: %s"),
			e.Error(),                  // 错误编号
			e.ErrorMessage(),           // 错误信息
			(LPCTSTR)e.Source(),        // 错误源
			(LPCTSTR)e.Description());  // 错误描述
		::MessageBox(NULL,strComError,_T("错误"),MB_ICONEXCLAMATION);
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
		m_pCnt->put_ConnectionTimeout(long(5));//设置连接超时
		m_pCnt->Execute( _bstr_t(sqlcmd),NULL,adCmdText );
	}
	catch(_com_error &e)
	{
		CString strComError;
		strComError.Format(_T("错误编号: %08lx\n错误信息: %s\n错误源: %s\n错误描述: %s"),
			e.Error(),                  // 错误编号
			e.ErrorMessage(),           // 错误信息
			(LPCTSTR)e.Source(),        // 错误源
			(LPCTSTR)e.Description());  // 错误描述
		::MessageBox(NULL,strComError,_T("错误"),MB_ICONEXCLAMATION);
	}
	if(m_pCnt->State)
		m_pCnt->Close();
	m_pCnt= NULL;

}

CString DB::GetFields(CString dbPath,CString sqlcmd,CString pass, CString *spField)
{
	CString sFields;
	sFields = L"";
    // 与数据库建立连接
    _ConnectionPtr	m_pCnt;
	m_pCnt.CreateInstance(__uuidof(Connection));
	CString str=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	str+=dbPath;
	str += _T(";Jet OLEDB:Database Password="); 
	str += pass+_T(";");
	try                 
	{			
		m_pCnt->Open(_bstr_t(str),_T(""),_T(""),adModeUnknown);
		m_pCnt->put_ConnectionTimeout(long(5));//设置连接超时
	}
	catch(_com_error &e)
	{
		CString strComError;
		strComError.Format(_T("错误编号: %08lx\n错误信息: %s\n错误源: %s\n错误描述: %s"),
			e.Error(),                  // 错误编号
			e.ErrorMessage(),           // 错误信息
			(LPCTSTR)e.Source(),        // 错误源
			(LPCTSTR)e.Description());  // 错误描述
		::MessageBox(NULL,strComError,_T("错误"),MB_ICONEXCLAMATION);
	}

    // 定义并初始化集合
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	_bstr_t   mStrSQL;
	CString   strColName;
	BSTR      bstrColName;
	long      ColCount,i; 
	Field *   field = NULL;		
	HRESULT   hr;
	Fields *  fields = NULL;

	
	//打开记录集，得到字段名，并将字段名序列回传
	m_pRecordset->Open((_variant_t)sqlcmd, m_pCnt.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	
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
		spField[i] = strColName;
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
void DB::CreateTable(CString dbPath,CString sqlcmd)
{
/*
_variant_t RecordsAffected;
///执行SQL命令：CREATE TABLE创建表格users,users包含四个字段:整形ID,字符串username,整形old,日期型birthday
m_pConnection->Execute("CREATE TABLE users(ID INTEGER,username 
					   TEXT,old INTEGER,birthday DATETIME)",&RecordsAffected,adCmdText);
*/
}
void DB::AppendItem(CString dbPath,CString sqlcmd)
{
/*
///往表格里面添加记录
m_pConnection->Execute("INSERT INTO users(ID,username,old,birthday)
VALUES (1, 'Washington',25,'1970/1/1')",&RecordsAffected,adCmdText);
*/
}
