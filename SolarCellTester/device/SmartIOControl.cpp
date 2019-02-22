#include "stdafx.h"
#include "SmartIOControl.h"

CSmartIOControl::CSmartIOControl(void)
{
	//默认的IO端口映射
	  m_o_map_Grade1=0x0001;
	  m_o_map_Grade2=0x0002;
	  m_o_map_Grade4=0x0004;
	  m_o_map_Grade8=0x0008;
	  m_o_map_TestOK=0x0010;
	  m_o_map_TestNG=0x0020;
	  m_o_map_ScanF=0x0020;
	  m_o_map_Red=0x0040;
	  m_o_map_Green=0x0080;
	  m_o_map_Yellow=0x0100;
	  m_o_map_Beep=0x0200;
	  m_i_map_IsTest=0x0001;
	  m_i_map_IsOut=0x0002;
	  m_i_map_IsScan=0x0004;
	  m_i_map_IsReset=0x0008;

	  m_error_grade = 0;//坏档
	  m_alarm_time = 3;//报警时长
}
CSmartIOControl::~CSmartIOControl(void)
{

}

bool CSmartIOControl::GetGlassIO(int &port)
{
	USES_CONVERSION;
	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	CString m_str=_T("");
	::GetModuleFileName(NULL,chBuf,nBufSize);
	TCHAR* lpStrPath = chBuf;
	PathRemoveFileSpec(lpStrPath);
	CString pathname = lpStrPath;
	pathname+=_T("\\io.ini");
	DWORD dwAttrib = GetFileAttributesW(pathname);
	if(dwAttrib == -1)
		return false;
	GetPrivateProfileStringW(L"IO",L"COM",L"2",m_str.GetBuffer(2),2,pathname);
	port =  atoi(T2A(m_str));
	
	GetPrivateProfileStringW(L"IO",L"m_o_map_Grade1",L"0x0001",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);    
    sscanf(T2A(m_str),"%x",&m_o_map_Grade1); 
	m_LogTrace.WriteLine(_T("m_o_map_Grade1=%x"),m_o_map_Grade1);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Grade2",L"0x0002",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Grade2); 
	m_LogTrace.WriteLine(_T("m_o_map_Grade2=%x"),m_o_map_Grade2);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Grade4",L"0x0004",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Grade4); 
	m_LogTrace.WriteLine(_T("m_o_map_Grade4=%x"),m_o_map_Grade4);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Grade8",L"0x0008",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Grade8); 
	m_LogTrace.WriteLine(_T("m_o_map_Grade8=%x"),m_o_map_Grade8);

	GetPrivateProfileStringW(L"IO",L"m_o_map_TestOK",L"0x0010",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_TestOK); 
	m_LogTrace.WriteLine(_T("m_o_map_TestOK=%x"),m_o_map_TestOK);

	GetPrivateProfileStringW(L"IO",L"m_o_map_TestNG",L"0x0020",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_TestNG); 
	m_LogTrace.WriteLine(_T("m_o_map_TestNG=%x"),m_o_map_TestNG);

	GetPrivateProfileStringW(L"IO",L"m_o_map_ScanF",L"0x0020",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_ScanF); 
	m_LogTrace.WriteLine(_T("m_o_map_ScanF=%x"),m_o_map_ScanF);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Red",L"0x0040",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Red); 
	m_LogTrace.WriteLine(_T("m_o_map_Red=%x"),m_o_map_Red);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Green",L"0x0080",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Green); 
	m_LogTrace.WriteLine(_T("m_o_map_Green=%x"),m_o_map_Green);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Yellow",L"0x0100",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Yellow); 
	m_LogTrace.WriteLine(_T("m_o_map_Yellow=%x"),m_o_map_Yellow);

	GetPrivateProfileStringW(L"IO",L"m_o_map_Beep",L"0x0200",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_o_map_Beep); 
	m_LogTrace.WriteLine(_T("m_o_map_Beep=%x"),m_o_map_Beep);

	GetPrivateProfileStringW(L"IO",L"m_i_map_IsTest",L"0x0001",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_i_map_IsTest); 
	m_LogTrace.WriteLine(_T("m_i_map_IsTest=%x"),m_i_map_IsTest);

	GetPrivateProfileStringW(L"IO",L"m_i_map_IsOut",L"0x0002",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_i_map_IsOut); 
	m_LogTrace.WriteLine(_T("m_i_map_IsOut=%x"),m_i_map_IsOut);

	GetPrivateProfileStringW(L"IO",L"m_i_map_IsScan",L"0x0004",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_i_map_IsScan); 
	m_LogTrace.WriteLine(_T("m_i_map_IsScan=%x"),m_i_map_IsScan);

	GetPrivateProfileStringW(L"IO",L"m_i_map_IsReset",L"0x0008",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%x",&m_i_map_IsReset); 
	m_LogTrace.WriteLine(_T("m_i_map_IsReset=%x"),m_i_map_IsReset);

	GetPrivateProfileStringW(L"IO",L"m_error_grade",L"0",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%d",&m_error_grade); 
	m_LogTrace.WriteLine(_T("m_error_grade=%d"),m_error_grade);

	GetPrivateProfileStringW(L"IO",L"m_alarm_time",L"3",m_str.GetBuffer(MAX_PATH),MAX_PATH,pathname);
    sscanf(T2A(m_str),"%d",&m_alarm_time); 
	m_LogTrace.WriteLine(_T("m_alarm_time=%d"),m_alarm_time);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////输出
//PutOutBuf[7]
//Y1  分档1    0x0001
//Y2  分档2    0x0002
//Y3  分档4    0x0004
//Y4  分档8    0x0008
//Y5  测试完成 0x0010
//Y6  扫描完成 0x0020    Y6 测试NG 0x0020
//Y7  红灯     0x0040
//Y8  绿       0x0080
//PutOutBuf[8]
//Y9  黄       0x0100
//Y10 蜂鸣器   0x0200
/////////////////////////////////////////////////////////////////////////////////
void CSmartIOControl::SetIOPin(unsigned char mgrade,bool mIsTestOK,bool mIsTestNG,bool mIsScanFinish,IO_LED mLedColor,bool mIsBeep)
{
	int mPINOUT1_10=0;

	if((mgrade&0xF)&0x01)//识别分档1
	{
		mPINOUT1_10|=m_o_map_Grade1;
	}

	if((mgrade&0xF)&0x02)//识别分档2
	{
		mPINOUT1_10|=m_o_map_Grade2;
	}

	if((mgrade&0xF)&0x04)//识别分档4
	{
		mPINOUT1_10|=m_o_map_Grade4;
	}

	if((mgrade&0xF)&0x08)//识别分档8
	{
		mPINOUT1_10|=m_o_map_Grade8;
	}

	if(mIsTestOK)//识别测试完成0x10
	{
		mPINOUT1_10|=m_o_map_TestOK;
	}

	if(mIsTestNG)
	{
		mPINOUT1_10|=m_o_map_TestNG;
	}

	if(mIsScanFinish)//识别扫描完成0x20
	{
		mPINOUT1_10|=m_o_map_ScanF;
	}

	switch(mLedColor)//识别报警灯颜色0x40
	{
	case IO_RED_LED:
					mPINOUT1_10|=m_o_map_Red;
					break;
	case IO_GREEN_LED:
					mPINOUT1_10|=m_o_map_Green;
					break;
	case IO_YELLOW_LED:
					mPINOUT1_10|=m_o_map_Yellow;
					break;
	default:break;
	}

	if(mIsBeep)//识别蜂鸣器0x50
	{
		mPINOUT1_10|=m_o_map_Beep;
	}

	unsigned char PutOutBuf[11]={0};
	m_comm.ClearALL();
	m_comm.ClearWriteBuffer();
	Sleep(50);
	PutOutBuf[0] = 0X01;
	PutOutBuf[1] = 0X0F;
	PutOutBuf[2] = 0X00;
	PutOutBuf[3] = 0X00;
	PutOutBuf[4] = 0X00;
	PutOutBuf[5] = 0X10;
	PutOutBuf[6] = 0X02;
	PutOutBuf[7] = mPINOUT1_10&0xFF;
	PutOutBuf[8] = (mPINOUT1_10>>8)&0xFF;
	PutOutBuf[9] = LOBYTE (m_comm.CRC16(PutOutBuf,9));
	PutOutBuf[10] = HIBYTE (m_comm.CRC16(PutOutBuf,9));
	m_comm.Write( PutOutBuf,11);
	Sleep(50);

	for(int i=0;i<11;i++)
	{
		printf("%x ",PutOutBuf[i]);
	}
	printf(" SetIOPin\n");
}

////////////////////////输入
//X1  申请测试 0x01
//X2  出料允许 0x02
//X3  扫描申请 0x04
//X4  复位按钮 0x08
void CSmartIOControl::GetIOPutIn(unsigned char &x1,unsigned char &x2,unsigned char &x3,unsigned char &x4)
{
	//01 02 00 00 00 10 79 C6 
	x1=0xFF;
	x2=0xFF;
	x3=0xFF;
	x4=0xFF;
	unsigned char PutIn[8] = {0x01,0x02,0x00,0x00,0x00,0x10,0x79,0xC6};
	m_comm.ClearALL();
	m_comm.ClearWriteBuffer();
	Sleep(50);
 	m_comm.Write( PutIn,8);
	Sleep(50);
 	unsigned char GetOut[20]={8};
 	memset(GetOut,0xFF,8*sizeof(char));

	for(int i=0;i<20;i++)
	{
		DWORD dwBytesRead = m_comm.BytesWaiting();
		if(dwBytesRead>=7)
		{
			m_comm.Read(GetOut,dwBytesRead);	
			for(int i=0;i<dwBytesRead;i++)
			{
				printf("%x ",GetOut[i]);
			}

			printf("  GetIOPutIn\n");

			if(0x01==GetOut[0] && 0x02==GetOut[1])
			{
				x1 = GetOut[3] & m_i_map_IsTest;
				x2 = GetOut[3] & m_i_map_IsOut;
				x3 = GetOut[3] & m_i_map_IsScan;
				x4 = GetOut[3] & m_i_map_IsReset;
			}

			break;
		}
		Sleep(10);
	}
}

void CSmartIOControl::Reset(void)
{
	unsigned char PutOutBuf[11]={0};
	//01 0F 00 00 00 10 02 30 00 F6 20 同时打开
	m_comm.ClearALL();
	m_comm.ClearWriteBuffer();
	//01 0F 00 00 00 10 02 00 00 E2 20 同时关闭
	//unsigned char PutOutBuf[11] = {0X01,0X0F,0X00,0X00,0X00,0X10,0X02,0X00,0X00,0XE2,0X20};
	PutOutBuf[0] = 0X01;
	PutOutBuf[1] = 0X0F;
	PutOutBuf[2] = 0X00;
	PutOutBuf[3] = 0X00;
	PutOutBuf[4] = 0X00;
	PutOutBuf[5] = 0X10;
	PutOutBuf[6] = 0X02;
	PutOutBuf[7] = 0X00;
	PutOutBuf[8] = 0X00;
	PutOutBuf[9] = 0XE2;
	PutOutBuf[10] = 0X20;
	m_comm.Write( PutOutBuf,11);
	Sleep(50);

	for(int i=0;i<11;i++)
	{
		printf("%x ",PutOutBuf[i]);
	}
	printf(" Reset\n");
}

