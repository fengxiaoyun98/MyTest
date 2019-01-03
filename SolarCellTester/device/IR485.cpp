#include "stdafx.h"
#include "IR485.h"

IR485::IR485(void)
{
	hCom = INVALID_HANDLE_VALUE;
	BYTE orderconst[5]={'V','=','P','\r',0};
	memcpy(order,orderconst,sizeof(order));
	m_temp=ZERO;
}

IR485::~IR485(void)
{
	close();
}

// //连接IRTP300L，从串口中查寻是否存在，如果存在则打开
void IR485::ConIR(void)
{
	if(exist==0)return;

	int i=10;
	for(;i>0;i--)
		if(open(i))
			if(IsIrTemp())
				break;
	if(i==0)
	{
		close();
		//AfxMessageBox("未发现红外温度传感器！");
	}
}
//验证温度传器是否再此串口
BOOL IR485::IsIrTemp(void)
{
	int i=0,ref;
	for(;i<3;i++)
	{
		ref=WriteSync(order,5);
		ref=WriteSync(order,5);
		ref=WriteSync(order,5);//set Poll mode
		ref=ReadSync(buf,20);
		char rt[]="?DS\r";
		ref=WriteSync(rt,strlen(rt));
		Sleep(2);
		memset(buf,0,sizeof(buf));
		if(ref=ReadSync(buf,20)){
			BYTE ds[]={'!','D','S','R','A','Y'};
			ref=memcmp(ds,buf,sizeof(ds));
			if(0==ref)
				break;
		}
	}
	return (i<3);
}
BOOL IR485::Encode(void)
{
	char *p;
	p=(char*)&buf[0];
	if(*p++=='!')
		if(*p++=='T')
		{
			m_temp=atof(p);
			return true;
		}
	return FALSE;
}

//从已打开的串口中读取温度值
BOOL IR485::ReqTemp()
{
	if(exist==0)
		return FALSE;

    m_temp = ZERO;
	
	if(!IsOpen())
	{
		ConIR();
		if(!IsOpen())
			return FALSE;
	}else
	{
		if(!IsIrTemp())
		{
			close();
			return FALSE;
		}
	}

	int loop=0;
	double m_ever_temp[5];
	double m_sum_temp = 0,m_aver_temp = 0;
	for(int i = 0;i < 5;i++)
		m_ever_temp[i] = 0;
	while(loop<5)
	{
		Sleep(20);
		char rt[]="?T\r";
		ReadSync(buf,20);
		if(0==WriteSync(rt,strlen(rt)))
		{
		}else
		{
			memset(buf,0,sizeof(buf));
			Sleep(20);
			if((0==ReadSync(buf,20))||!Encode())
			{
			}else
			{
				if (IsRight(m_temp))
				{
					m_ever_temp[loop] = m_temp;
					m_sum_temp += m_ever_temp[loop];
					//break;
				}
			}
		}
		loop++;
	}
	int n=0;
	for(int m=0;m<5;m++)
		if (m_ever_temp[m] != 0)
			n++;
	if (n != 0)
	{
		m_aver_temp = m_sum_temp/n;

		double m_sub_temp[5];
		for(int x=0;x<5;x++)
			m_sub_temp[x] = fabs(m_ever_temp[x]-m_aver_temp);

		int k=0;
		for(int y = 0;y < 5; y++)
			if (m_sub_temp[y] < m_sub_temp[k])
				k=y;
		m_temp = m_ever_temp[k];
	}

	if ((loop == 5) && (!IsRight(m_temp)))
	{
		close();
		return FALSE;
	}

// 	CStdioFile file;
// 	CString fn;
// 	fn=_T("SurfTemp.daq");
// 	if(!PathFileExists(fn))
// 	{
// 		if(!file.Open(fn,CFile::modeCreate|CFile::modeWrite))
// 			return FALSE;
// 	}else if(!file.Open(fn,CFile::modeWrite))
// 		return FALSE;
// 	file.Seek(0,CFile::end);
// 
// 	CString str;
// 	for(int i = 0;i<loop;i++)
// 	{		
// 		str.Format(_T("%f"),m_ever_temp[i]);
// 		file.WriteString(str+_T("\n"));
// 	}
// 	file.Close();

	return TRUE;
}

// //返回一个可通信的串口
BOOL IR485::open(int no)
{
	if((no<1)&&(no>256))
		return FALSE;

	close();
	
	CString sPort;
	sPort.Format(_T("\\\\.\\COM%d"), no);
	hCom=CreateFile(sPort,//COM口
		GENERIC_READ|GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if( !SetPara()){
		close();
		return FALSE;
	}else 
		return TRUE;
}
//
BOOL IR485::SetPara(void)
{
	if(!IsOpen())return FALSE;
	
	DCB dcb;
	dcb.DCBlength = sizeof( DCB );

	if(!GetCommState(hCom,&dcb))
		return FALSE;
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.StopBits =ONESTOPBIT;
	dcb.Parity = NOPARITY;

	dcb.fBinary = TRUE; 
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	if(!SetCommState(hCom, &dcb))
		return FALSE;

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
	CommTimeOuts.ReadTotalTimeoutConstant = 10;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 1;
	CommTimeOuts.WriteTotalTimeoutConstant = 10;
	if(!SetCommTimeouts( hCom, &CommTimeOuts ))
		return FALSE;

	if(!SetupComm( hCom, 512, 512 ))
		return FALSE;

	return TRUE;
}
//读串口 同步应用
DWORD IR485::ReadSync(LPVOID Buffer, size_t dwBufferLength)
{
	if(!IsOpen())
		return 0;

	DWORD dwError;
	if(ClearCommError(hCom, &dwError, NULL) && dwError > 0)
	{
		PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR);
		return 0;
	}
	DWORD uReadLength = 0;
	if(!ReadFile(hCom, Buffer, dwBufferLength, &uReadLength, NULL))
		if(GetLastError() != ERROR_IO_PENDING)
			uReadLength = 0;
	return uReadLength;
}


//写串口 同步应用
DWORD IR485::WriteSync(LPVOID Buffer, size_t dwBufferLength)
{
	if(!IsOpen())
		return 0;
	DWORD dwError;
	if(ClearCommError(hCom, &dwError, NULL) && dwError > 0) 
		PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR); 
	unsigned long uWriteLength = 0;
	if(!WriteFile(hCom, Buffer, dwBufferLength, &uWriteLength, NULL))
		if(GetLastError() != ERROR_IO_PENDING)
			uWriteLength = 0;
	return uWriteLength;
}
// 关闭串口
void IR485::close(void)
{
	if(IsOpen())
	{
		PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);
		CloseHandle(hCom);
		hCom = INVALID_HANDLE_VALUE;
	}
}

CString IR485::GetTempStr(void)
{
	CString str=_T("");
	if(exist!=0)
		if(IsOpen())
			if(IsRight(m_temp))
				str.Format(_T("SurfTemp:%.1fDegree"),m_temp);
	return str;
}
double IR485::GetTemp(void)
{
	return m_temp;
};
void IR485::SetTemp(double val)
{
	if(IsRight(val))
		m_temp=val;
	else 
		m_temp=ZERO;
};
