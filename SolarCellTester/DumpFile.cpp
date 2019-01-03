#include "stdafx.h"  
#include "DumpFile.h"  
  
CDumpFile::CDumpFile()  
{  
}  
  
CDumpFile::~CDumpFile()  
{     
}  
  
static WCHAR g_szDumpFileDir[MAX_PATH] = { 0 };  
  
CString GetFileVersion(const CString& strFilePath)  
{  
    CString strAppVersion;  
    DWORD u32RessourceVersionInfoSize;  
    DWORD u32JustAJunkVariabel;  
    char* ps8VersionInfoPtr;  
    struct LANGANDCODEPAGE {  
        WORD wLanguage;  
        WORD wCodePage;  
    } *pstTranslationPtr(NULL);  
    wchar_t* ps16InformationPtr;  
    UINT  u32VersionInfoSize;  
    wchar_t  as16VersionValue[255];  
  
    u32RessourceVersionInfoSize = GetFileVersionInfoSize(strFilePath, &u32JustAJunkVariabel);  
    if (0 != u32RessourceVersionInfoSize)  
    {  
        ps8VersionInfoPtr = new char[u32RessourceVersionInfoSize];  
        if (GetFileVersionInfo(strFilePath, 0, u32RessourceVersionInfoSize, ps8VersionInfoPtr))  
        {  
            if (!VerQueryValue(  
                ps8VersionInfoPtr,  
                TEXT("VarFileInfo\\Translation"),  
                (LPVOID*)&pstTranslationPtr,  
                &u32VersionInfoSize))  
            {  
                delete[] ps8VersionInfoPtr;  
                return strAppVersion;  
            }  
        }  
  
        wsprintf(as16VersionValue,  
            L"\\StringFileInfo\\%04x%04x\\FileVersion",  
            pstTranslationPtr[0].wLanguage,  
            pstTranslationPtr[0].wCodePage);  
  
        if (!VerQueryValue(  
            ps8VersionInfoPtr,  
            as16VersionValue,  
            (LPVOID*)&ps16InformationPtr,  
            &u32VersionInfoSize))  
        {  
            delete[] ps8VersionInfoPtr;  
            return strAppVersion;  
        }  
  
        if (wcslen(ps16InformationPtr) > 0)  
        {  
            strAppVersion = CString(ps16InformationPtr);  
        }  
        delete[] ps8VersionInfoPtr;  
    }  
    return strAppVersion;  
}  
  
void CDumpFile::Initialize(CString strFileDir)  
{  
    wcsncpy_s(g_szDumpFileDir, strFileDir, strFileDir.GetLength());  
  
    SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);  
}  
  
LONG WINAPI CDumpFile::UnhandledExceptionFilterEx(EXCEPTION_POINTERS* pException)  
{  
    WCHAR szModuleFilePath[MAX_PATH] = { 0 };  
    GetModuleFileName(NULL, szModuleFilePath, MAX_PATH);  
  
    CString strFileVersion = GetFileVersion(szModuleFilePath);  
  
    WCHAR* pszModualFilename = PathFindFileName(szModuleFilePath);  
    CString strFileName = CString(pszModualFilename) + L"_" + strFileVersion + CTime::GetCurrentTime().Format(" %Y-%m-%d %H_%M_%S") + L".dmp";  
  
    if (wcslen(g_szDumpFileDir) == 0)  
    {  
        /* 保存在模块所在目录 */  
        GetModuleFileName(NULL, g_szDumpFileDir, MAX_PATH);  
        PathRemoveFileSpec(g_szDumpFileDir);  
    }  
  
    WCHAR szDumpFilePath[MAX_PATH] = { 0 };  
    PathAppend(szDumpFilePath, g_szDumpFileDir);  
    if (!PathIsDirectory(szDumpFilePath))  
    {  
        SHCreateDirectoryEx(NULL, szDumpFilePath, NULL);  
    }  
  
    PathAppend(szDumpFilePath, strFileName);  
    CreateDumpFile(szDumpFilePath, pException);  
  
    // 崩溃时弹框    
    FatalAppExit(-1, _T("提示信息：软件崩溃，请重启。"));  
    return EXCEPTION_CONTINUE_SEARCH;  
}  
  
void CDumpFile::CreateDumpFile(LPCTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)  
{  
    // 创建Dump文件    
    HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
  
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
    dumpInfo.ExceptionPointers = pException;  
    dumpInfo.ThreadId = GetCurrentThreadId();  
    dumpInfo.ClientPointers = TRUE;  
  
    // 写入Dump文件内容    
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  
  
    CloseHandle(hDumpFile);  
}  