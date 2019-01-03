#pragma once  
#include <Dbghelp.h>    
  
#pragma comment(lib, "Dbghelp.lib")    
  
class CDumpFile  
{  
public:  
    CDumpFile();  
    ~CDumpFile();  
  
private:  
    static LONG WINAPI UnhandledExceptionFilterEx(EXCEPTION_POINTERS* pException);  
    static void CreateDumpFile(LPCTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException);  
public:  
    void Initialize(CString strFileDir = L""); // strFileDir为崩溃文件保存目录，默认为模块所在目录  
};  