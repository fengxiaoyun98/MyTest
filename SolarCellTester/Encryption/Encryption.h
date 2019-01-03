/*/////////////////////////ʹ��˵��//////////////////////////////////
1.�����б������cserilaport���cseril�࣬m_com��Ϊһ��ȫ�ֱ�����stdafx��������
2.�������singsleep������getpath������������������Ϊȫ�ֺ���
void SingleSleep(DWORD nMilliseconds)
{
HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
LARGE_INTEGER liDuring;
liDuring.QuadPart = - (__int64)nMilliseconds * 10000; //��λ��100����

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
3.�������sqlite��CppSQLite3U�Լ�ѹ���ļ���zip_encrypt��
4.�����²��ַŵ����Բ����У���֤ÿ�β��Զ����м���Ƿ���Ҫע�ᡣ
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
5.���������
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
5.����Э����ο����Э���ļ���
/////////////////////////////////////////////////////////////////////////*/
#pragma  once
#include "../TimerUS.h"
class  CEncryption
{
public:
	CEncryption(void);
	~CEncryption(void);
public:
	CString GetCPUID();                                      //���CPUID����
	CString GetSurplustime(){return m_Surplustime;}          //���ʣ��ʱ��

	int NeedActive();                                        //�ж��Ƿ���Ҫ��Ȩ����0Ϊ����1Ϊ��Ҫ��2Ϊ��ʱע��Ĳ���Ҫ��3Ϊ���õĲ���Ҫ
	BOOL WriteRegitInfToBoard(CString RegitNumber);          //��Ȩ����

	BOOL GetAuthInf();
	int	 ReSet();
	BOOL SetAuthInf();
//private:
	BOOL GetDataBaseDict(CString filezip,CString pwd);       //��ѹ�������ļ��л���ֵ����ݿ�
	BOOL ZipDict(CString dictforzip,CString pwd);            //ѹ��������ʹ�ù����ֵ�
	int CheckNumberFromDataBase(CString number);             //�����Ȩ���Ƿ���������ݿ���
	int Random();                                            //������������

	int ReadSign();                                          //��ȡ��λ��ע���־λ0Ϊδ��Ȩ��1Ϊ����ʱ��2Ϊ����ʱ��3Ϊ����
	void Read2time(UINT& lasttime,UINT& regtime);              //��ȡ��λ��ע��ʱ������һ�εĲ���ʱ��
	USHORT ReadFC();                                         //��������
	int ReadRegCnt();                                        //��ע�����
	int ReadRegitNumberCRC(std::vector<USHORT> &regCRC);            //����ע�����Ȩ���CRC
	
	BOOL WriteNowTime(UINT nowtime);                          //����ǰʱ��д����λ��
	
	BOOL ReadEEPROM(UCHAR head_address,UCHAR length,std::vector<UCHAR> &read_data);     //������
	BOOL WriteEEPROM(UCHAR head_address,UCHAR *data,UCHAR length);                      //д����

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

	TimerUS m_TimeOut;     //����ʱ
	UCHAR m_head_address;
	UCHAR m_length;
	std::vector<UCHAR> m_ReadData;
};