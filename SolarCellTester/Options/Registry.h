#pragma once

#include <winReg.h>
/////////////////////////////////////////////////////////////////////////////
// CRegistry window

class CRegistry : public CObject
{
// Construction
public:
	CRegistry(HKEY hKey=HKEY_LOCAL_MACHINE);

public:
	BOOL SaveKey(LPCTSTR lpFileName);
	BOOL RestoreKey(LPCTSTR lpFileName);
	BOOL Write(LPCTSTR lpValueName, const BYTE * lpValue,DWORD size);
	BOOL Read(LPCTSTR lpValueName, byte* buffer,DWORD dwSize);
	BOOL Read(LPCTSTR lpValueName, CString& lpVal);
	BOOL Read(LPCTSTR lpValueName, DWORD* pdwVal);
	BOOL Read(LPCTSTR lpValueName, UINT* pdwVal);
	BOOL Read(LPCTSTR lpValueName, double* pdwVal);
	BOOL Read(LPCTSTR lpValueName, int* pnVal);
	BOOL Write(LPCTSTR lpSubKey,CString lpValue);
	BOOL Write(LPCTSTR lpSubKey, DWORD dwVal);
	BOOL Write(LPCTSTR lpSubKey, UINT dwVal);
	BOOL Write(LPCTSTR lpSubKey, double dwVal);
	BOOL Write(LPCTSTR lpSubKey, int nVal);
	BOOL DeleteKey(HKEY hKey, LPCTSTR lpSubKey);
	BOOL DeleteValue(LPCTSTR lpValueName);
	void Close();
	BOOL Open(CString lpSubKey);
	BOOL CreateKey(CString lpSubKey);
	virtual ~CRegistry();
	void SetCompName(CString name){m_compname=name;}

protected:
	HKEY m_hKey;
	CString m_path;
	CString m_compname;
	
};

/////////////////////////////////////////////////////////////////////////////
