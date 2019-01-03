#pragma once

class CDirPath  
{
	BOOL TranslateFile(CString fname=_T("SunAds.mdb"));
	void SetCopyFile(CString list,int kind =1);
	CString m_FileList;
	CString m_SelPath;
	CString m_CurPath;
	CString m_ExePath;
public:
	CString GetExePath(){return m_ExePath;}
	CString GetSelectPath(){return m_SelPath;}
	BOOL CreateWorkDir(CString works=_T("Works"));
	BOOL SelectFolder(CString sTitle=_T("Please Select"));
	CDirPath();
	virtual ~CDirPath();

};
