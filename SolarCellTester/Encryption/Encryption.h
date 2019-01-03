/*/////////////////////////使用说明//////////////////////////////////
1.工程中必须包含cserilaport类和cseril类，m_com作为一个全局变量在stdafx中声明。
2.必须包含singsleep函数和getpath函数，这两个函数作为全局函数
void SingleSleep(DWORD nMilliseconds)
{
HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
LARGE_INTEGER liDuring;
liDuring.QuadPart = - (__int64)nMilliseconds * 10000; //单位是100纳秒

if (hTimer == NULL)
RaiseException(EXCEPTION_INVALID_HANDLE, 0, 0, 0);

SetWaitableTimer(hTimer, &liDuring, 0, 0, 0, 0);

for(;;) {
DWORD dwRet = MsgWaitForMultipleObjects(1, &hTimer, FALSE, INFINITE, QS_ALLEVENTS);
if (dwRet == WAIT_OBJECT_0)
break;
else if (dwRet == WAIT_OBJECT_0 + 1) {
MSG msg;
while(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) != 0) {
if (AfxGetThread()->PumpMessage() == 0) {
AfxPostQuitMessage(0);
break;
}
}
}
}

CloseHandle(hTimer);

}
void Getpath(CString &szPath)
{
TCHAR buf[256] = {0};  
GetModuleFileName(NULL, buf, sizeof(buf)/sizeof(TCHAR));  
szPath = buf;         
szPath = szPath.Left(szPath.ReverseFind('\\') + 1); 
}
3.必须包含sqlite类CppSQLite3U以及压缩文件类zip_encrypt。
4.将以下部分放到测试部分中，保证每次测试都进行检测是否需要注册。
public:
CEncryption m_encr;
CEncrEdit   m_encr_edit;
int m_ifneedactive;


m_ifneedactive = m_encr.NeedActive();
if (m_ifneedactive == 0)
{
return FALSE;
}
if (m_ifneedactive == 1)
{
m_encr_edit.SetCpuId(m_encr.GetCPUID());
if (m_encr_edit.DoModal() == IDOK)
{
if (m_encr_edit.m_authcode.IsEmpty())
{
AfxMessageBox(_T("Authorization code can not be empty!"));
return FALSE;
}
if (!m_encr.WriteRegitInfToBoard(m_encr_edit.m_authcode))
return FALSE;
}else
return FALSE;
}
if (m_ifneedactive == 2)
{
m_set->m_aperson = m_encr.GetAssigner();
m_set->m_atype = _T("Temporary");
m_set->m_atime = m_encr.GetSurplustime()+_T("days");
}
if (m_ifneedactive == 3)
{
m_set->m_aperson = m_encr.GetAssigner();
m_set->m_atype = _T("Permanent");
m_set->m_atime = _T("Permanent");
}
5.剪贴板代码
UpdateData(FALSE);
CString str(m_cpuid);
if (OpenClipboard())   
{
EmptyClipboard();  

HGLOBAL hClip;
hClip = GlobalAlloc(GMEM_MOVEABLE, (str.GetLength() * 2) + 2); 
TCHAR *pBuf;
pBuf = (TCHAR *)GlobalLock(hClip);
lstrcpy(pBuf, str);
GlobalUnlock(hClip);
SetClipboardData(CF_UNICODETEXT, hClip);
CloseClipboard();
} 
5.具体协议请参考相关协议文件。
/////////////////////////////////////////////////////////////////////////*/
#pragma  once
#include "../TimerUS.h"
class  CEncryption
{
public:
	CEncryption(void);
	~CEncryption(void);
public:
	CString GetCPUID();                                      //获得CPUID函数
	CString GetSurplustime(){return m_Surplustime;}          //获得剩余时间

	int NeedActive();                                        //判断是否需要授权返回0为错误，1为需要，2为临时注册的不需要，3为永久的不需要
	BOOL WriteRegitInfToBoard(CString RegitNumber);          //授权函数

	BOOL GetAuthInf();
	int	 ReSet();
	BOOL SetAuthInf();
//private:
	BOOL GetDataBaseDict(CString filezip,CString pwd);       //从压缩加密文件中获得字典数据库
	BOOL ZipDict(CString dictforzip,CString pwd);            //压缩并加密使用过的字典
	int CheckNumberFromDataBase(CString number);             //检测授权码是否存在于数据库中
	int Random();                                            //获得随机数函数

	int ReadSign();                                          //读取下位机注册标志位0为未授权，1为月临时，2为年临时，3为永久
	void Read2time(UINT& lasttime,UINT& regtime);              //读取下位机注册时间和最后一次的测试时间
	USHORT ReadFC();                                         //读功能码
	int ReadRegCnt();                                        //读注册次数
	int ReadRegitNumberCRC(std::vector<USHORT> &regCRC);            //读已注册的授权码的CRC
	
	BOOL WriteNowTime(UINT nowtime);                          //将当前时间写入下位机
	
	BOOL ReadEEPROM(UCHAR head_address,UCHAR length,std::vector<UCHAR> &read_data);     //读函数
	BOOL WriteEEPROM(UCHAR head_address,UCHAR *data,UCHAR length);                      //写函数

	void Destroy(CDialog* dlg);

private:
	CString m_assigner;
	CString m_Surplustime;

	CStringArray m_assigner_temp;
	CStringArray m_ranumber_temp;

	int m_Auth_flag_temp;
	UINT m_Reg_time_temp;
	UINT m_LastTest_time_temp;

	int m_authcnt_temp;

	TimerUS m_TimeOut;     //读超时
	UCHAR m_head_address;
	UCHAR m_length;
	std::vector<UCHAR> m_ReadData;
};