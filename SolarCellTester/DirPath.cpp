// DirPath.cpp: implementation of the CDirPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirPath.h"
#include <Shlwapi.h> 
#include <windows.h>
//#include <imagehlp.h> 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirPath::CDirPath()
{
	
	//Use GetModuleFileName 获得应用程序的路径，然后去掉可执行文件名。Example:
	//TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在API中定义了吧，好象是128
	//GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	//确定待导出文件的存放路径
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	m_ExePath = CString(exeFullPath);
	m_ExePath = m_ExePath.Left(m_ExePath.ReverseFind(_T('\\')));
	if(m_ExePath.GetAt(m_ExePath.GetLength()-1)!=_T('\\'))m_ExePath+=_T("\\");
	SetCurrentDirectory(m_ExePath);
	m_SelPath = _T("");

	TCHAR szHome[MAX_PATH];
	DWORD RightWrong=GetCurrentDirectory(MAX_PATH,szHome);
	m_CurPath=szHome;
	if(m_CurPath.GetAt(m_CurPath.GetLength()-1)!=_T('\\'))m_CurPath+=_T("\\");
	m_FileList=_T("");
}

CDirPath::~CDirPath()
{

}

BOOL CDirPath::SelectFolder(CString sTitle)
{
	BROWSEINFO bi;
	TCHAR Buffer[_MAX_PATH];

	do{
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = sTitle;
	bi.ulFlags = 0x0040;
	bi.lpfn = NULL;
	LPITEMIDLIST pIDList = SHBrowseForFolder(& bi);
	if (!pIDList)
		return FALSE;
	SHGetPathFromIDList(pIDList, Buffer);
	LPMALLOC lpMalloc;
	if (FAILED(SHGetMalloc(& lpMalloc)))
		return FALSE;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_CurPath=Buffer;
	if(MessageBox(NULL,m_CurPath,_T("Is this dir?"),MB_YESNO)==6)
		break;
	}while(TRUE);

	if(m_CurPath.GetAt(m_CurPath.GetLength()-1)!=_T('\\'))m_CurPath+=_T("\\");//给末尾加上或者去掉"\"
	m_SelPath=m_CurPath;

	return TRUE;
}


BOOL CDirPath::CreateWorkDir(CString works)
{
	BOOL lt;
	if(works.ReverseFind(_T(':'))!=-1&&works.ReverseFind(_T('\\'))!=-1)
		lt=CreateDirectory(works,NULL);
	else lt=CreateDirectory(m_CurPath=m_CurPath+works,NULL);
	if(m_CurPath.GetAt(m_CurPath.GetLength()-1)!=_T('\\'))m_CurPath+=_T("\\");//给末尾加上或者去掉"\"
	return lt&&SetCurrentDirectory(m_CurPath);
}

void CDirPath::SetCopyFile(CString list, int kind)
{
	if(kind)m_FileList=list;
}

BOOL CDirPath::TranslateFile(CString fname)
{
	return CopyFile(m_ExePath+fname,m_CurPath+fname,1);
}
