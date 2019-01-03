#include "stdafx.h"
#include "Registry.h"
#include   <shlwapi.h>     //   使用   PathRemoveFileSpec（）函数  
#pragma   comment(lib,"shlwapi")  
/////////////////////////////////////////////////////////////////////////////
// CRegistry

CRegistry::CRegistry(HKEY hKey)
{
	m_hKey=hKey;
	m_compname=TEXT("GSOLAR");
	m_path=TEXT("SOFTWARE\\");
	if(!m_compname.IsEmpty())
		m_path=m_path+m_compname+TEXT("\\");
    TCHAR lpFileName[MAX_PATH];
    ::GetModuleFileName(NULL, lpFileName, MAX_PATH);
	PathRemoveExtension(lpFileName);
	m_path=m_path+PathFindFileName(lpFileName)+TEXT("\\");
}

CRegistry::~CRegistry()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegistry Functions
 
BOOL CRegistry::CreateKey(CString lpSubKey)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);

	if(Open(lpSubKey))
		return TRUE;
	HKEY hKey;
	DWORD dw;
	//REG_OPTION_NON_VOLATILE重启动后有效//REG_OPTION_VOLATILE重启动后无效
	long lReturn=RegCreateKeyEx(m_hKey,m_path+lpSubKey+TEXT("\0\0"),0L,REG_NONE,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);
    
	if(lReturn==ERROR_SUCCESS)
	{
		m_hKey=hKey;
		return TRUE;
	}
	
	return FALSE;
	
}

BOOL CRegistry::Open(CString lpSubKey)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);
    
	HKEY hKey;
	long lReturn=RegOpenKeyEx(m_hKey,m_path+lpSubKey+TEXT("\0\0"),0L,KEY_ALL_ACCESS,&hKey);
    
	if(lReturn==ERROR_SUCCESS)
	{
        m_hKey=hKey;
		return TRUE;
	}
	return FALSE;
	
}

void CRegistry::Close()
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey=NULL;
	}
	
}

BOOL CRegistry::DeleteValue(LPCTSTR lpValueName)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	
	long lReturn=RegDeleteValue(m_hKey,lpValueName);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
	ASSERT(hKey);
	ASSERT(lpSubKey);
	
	long lReturn=RegDeleteValue(hKey,lpSubKey);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, int nVal)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);
	
	DWORD dwValue;
	dwValue=(DWORD)nVal;
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}
BOOL CRegistry::Write(LPCTSTR lpSubKey, UINT dwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(UINT));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}
BOOL CRegistry::Write(LPCTSTR lpSubKey, double dwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpSubKey);
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(double));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::Write(LPCTSTR lpValueName, CString lpValue)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(lpValue);

	long lReturn=RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ,(const BYTE *)lpValue.GetBuffer(),lpValue.GetLength()*2);
	lpValue.ReleaseBuffer();

   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}
BOOL CRegistry::Write(LPCTSTR lpValueName, const BYTE * lpValue,DWORD size)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(lpValue);	

	long lReturn=RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ, lpValue,size);
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}


BOOL CRegistry::Read(LPCTSTR lpValueName, int* pnVal)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(pnVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pnVal=(int)dwDest;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistry::Read(LPCTSTR lpValueName, UINT* pdwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(pdwVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pdwVal=(UINT)dwDest;
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpValueName, DWORD* pdwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(pdwVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pdwVal=dwDest;
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpValueName, double* pdwVal)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(pdwVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(double);
	double dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pdwVal=(double)dwDest;
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistry::RestoreKey(LPCTSTR lpFileName)
{
	ASSERT(m_hKey);
	ASSERT(lpFileName);
	
	long lReturn=RegRestoreKey(m_hKey,lpFileName,REG_WHOLE_HIVE_VOLATILE);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::SaveKey(LPCTSTR lpFileName)
{
	ASSERT(m_hKey);
	ASSERT(lpFileName);
	
	long lReturn=RegSaveKey(m_hKey,lpFileName,NULL);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}


BOOL CRegistry::Read(LPCTSTR lpValueName, CString &lpVal)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(lpVal);

	TCHAR buffer[255];
	DWORD size=sizeof(buffer);
	DWORD dwType;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)buffer,&size);

	CString str(buffer);

	lpVal=str;
	
	if(lReturn==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpValueName, byte* buffer,DWORD dwSize)
{
	ASSERT(m_hKey);
	ASSERT(lpValueName);
	ASSERT(buffer);

	DWORD dwType;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,buffer,(LPDWORD)&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
	
}