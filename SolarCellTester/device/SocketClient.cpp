#include "StdAfx.h"
#include "..\StdAfx.h"
#include "SocketClient.h"

SOCKET g_sHostScoket; //�������׽���
CString g_SerialTcpip=_T("");
UINT ThreadGetSerial(LPVOID lparam);

SocketClient::SocketClient(void)
{
	m_IsConnect = false;
	pThreadGetSerial =NULL;
}

void SocketClient::InitConnect()
{
   const int BUF_SIZE = 64;

    WSADATA wsd; //WSADATA����
    SOCKADDR_IN servAddr; //��������ַ
    char buf[BUF_SIZE]; //�������ݻ�����
    char bufRecv[BUF_SIZE];
    int retVal; //����ֵ
                //��ʼ���׽��ֶ�̬��
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup failed!");
    }
    //�����׽���
    g_sHostScoket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == g_sHostScoket)
    {
        printf("socket failed!");
        WSACleanup();//�ͷ��׽�����Դ
    }

	//����SocketΪ������ģʽ
	//int iMode = 1;
	//retVal = ioctlsocket(g_sHostScoket, FIONBIO, (u_long FAR*)&iMode);
	//if (retVal == SOCKET_ERROR)
	//{
	//	printf("ioctlsocket failed!\n");
	//	WSACleanup();
	//}

    //���÷�������ַ
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("192.168.2.1");
    servAddr.sin_port = htons((short)502);
    int nServAddlen = sizeof(servAddr);

    //���ӷ�����
    retVal = connect(g_sHostScoket, (LPSOCKADDR)&servAddr, sizeof(servAddr));
    if(SOCKET_ERROR == retVal)
    {
		m_IsConnect = false;
        printf("connect PLC Modbus TCP socketserver failed!\n");
        closesocket(g_sHostScoket); //�ر��׽���
        WSACleanup(); //�ͷ��׽�����Դ
    }
	else
	{
		m_IsConnect = true;
		printf("connect PLC Modbus TCP socketserver sucess!\n");

	}
}

SocketClient::~SocketClient(void)
{
	if(m_IsConnect)
	{
		//�˳�
		closesocket(g_sHostScoket); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ
	}

	if(pThreadGetSerial!=NULL)
	{
		m_ThreadRunFlag = false;
		Sleep(100);
		pThreadGetSerial->SuspendThread();
		pThreadGetSerial->Delete();
		pThreadGetSerial = NULL;

	}
}

int SocketClient::SendTestResultData()
{
		//char SendBuffer[10240] = { 0};
		//int len  = (unsigned short)TestItemObjectstr.length();
		//memcpy(&SendBuffer, TestItemObjectstr.c_str(), len);
		//SendBuffer[len - 1] = 0x0D0A;
		//int ret = send(sHost, SendBuffer, strlen(SendBuffer), 0);

		return 1;
}
WORD SocketClient::CRC16 (const BYTE *nData, WORD wLength)
{
	static const WORD wCRCTable[] = {
		0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
		0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
		0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
		0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
		0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
		0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
		0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
		0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
		0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
		0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
		0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
		0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
		0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
		0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
		0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
		0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
		0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
		0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
		0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
		0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
		0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
		0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
		0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
		0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
		0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
		0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
		0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
		0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
		0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
		0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
		0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
		0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

		BYTE nTemp;
		WORD wCRCWord = 0xFFFF;        //0xFFFFΪmoubusУ��   0x0000Ϊ16λУ��

		while (wLength--)
		{
			nTemp = *nData++ ^ wCRCWord;
			wCRCWord >>= 8;
			wCRCWord ^= wCRCTable[nTemp];
		}
		return wCRCWord;

}
int SocketClient::SendGetSeialCmd(unsigned int addr,unsigned int len)
{
	//char cmd[8];
	//char sendbuf[20];
	//DWORD rtncrc;
	//cmd[0] = 3;
	//cmd[1] = HIBYTE(addr);
	//cmd[2] = LOBYTE(addr);
	//cmd[3] = HIBYTE(len);
	//cmd[4] = LOBYTE(len);
	//memcpy( sendbuf,cmd,sizeof(char)*6);
	//rtncrc =  CRC16(  (BYTE *)sendbuf,sizeof(char)*(CMD_DATALEN-2) );
	//sendbuf[W_CRC_L] = LOBYTE(rtncrc);
	//sendbuf[W_CRC_H] = HIBYTE(rtncrc);	
	//int ret = send(g_sHostScoket, cmd, 5, 0);

    char SendBuf[12] = {0};
	SendBuf[0] = 0;//����Ԫ��ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[1] = 0;
	SendBuf[2] = 0;//Э���ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[3] = 0;
	SendBuf[4] = 0x00;//�����ֽڳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[5] = 0x06;
	SendBuf[6] = 01;//��Ԫ��ʶ��
	SendBuf[7] = 0x03;//m_cmdword;//������
	SendBuf[8] = HIBYTE(addr);//������ʼ��ַ,���ֽ���ǰ,���ֽ��ں�
	SendBuf[9] = LOBYTE(addr);
	SendBuf[10] = HIBYTE(len);//���ݳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[11] = LOBYTE(len);
	int ret = send(g_sHostScoket, SendBuf, 12, 0);

	return ret;
}

int SocketClient::RecvData(char *recvbuf,int len,int revlen)
{

	if(revlen<9+len*2)return -1;
	if(recvbuf[6]==0x01 && recvbuf[7]==0x03)
	{
		char *buf=new char[len+1];
		for(int i=0;i<len;i++)
		{
			buf[i]=recvbuf[9+i*2+1];
		}
		 buf[len]='\0';
		 CString tempstr(buf);
		 g_SerialTcpip = tempstr;
		 printf("Reved Serial:%s \r\n",buf);

		 delete[] buf;

	}
	return 1;
}

int SocketClient::ReadSignelData(char *recvbuf,int revlen)
{
	WORD mtempdata=0;
	if(revlen<11)return -1;
	if(recvbuf[6]==0x01 && recvbuf[7]==0x03)
	{
		 mtempdata = MAKEWORD(recvbuf[10],recvbuf[9]);

		 printf("ReadSignelData:%d \r\n",mtempdata);

	}
	return mtempdata;
}

UINT ThreadGetSerial(LPVOID lparam)
{
	SocketClient *pDlg=(SocketClient*)lparam;

	pDlg->m_ThreadRunFlag = true;
	while(true == pDlg->m_ThreadRunFlag)
	{
		pDlg->SendGetSeialCmd(0,15);
		Sleep(100);
		DWORD dwBytesRead = 0;
		ioctlsocket(g_sHostScoket, FIONREAD, &dwBytesRead);
		if(dwBytesRead>0)
		{
			char *recvbuf=new char[dwBytesRead];
			 //����ս��ջ�����
			 recv(g_sHostScoket, recvbuf, dwBytesRead, 0);//
#ifdef _DEBUG
			 printf("dwBytesRead:%d \r\n",dwBytesRead);
			 for(int x=0;x<dwBytesRead;x++)
			 {
				printf("%02X ",recvbuf[x]);
			 }
			 printf("\r\n");
#endif
			 pDlg->RecvData(recvbuf,15,dwBytesRead);
		}

		Sleep(100);
	}

	return 1;
}

int SocketClient::WritePLC(unsigned int addr, unsigned int val)
{

    char SendBuf[12] = {0};
	SendBuf[0] = 0;//����Ԫ��ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[1] = 0;
	SendBuf[2] = 0;//Э���ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[3] = 0;
	SendBuf[4] = 0x00;//�����ֽڳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[5] = 0x06;
	SendBuf[6] = 01;//��Ԫ��ʶ��
	SendBuf[7] = 0x06;//m_cmdword;//������
	SendBuf[8] = HIBYTE(addr);//������ʼ��ַ,���ֽ���ǰ,���ֽ��ں�
	SendBuf[9] = LOBYTE(addr);
	SendBuf[10] = HIBYTE(0x01);//���ݳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[11] = LOBYTE(0x01);
	int ret = send(g_sHostScoket, SendBuf, 12, 0);
	Sleep(100);
	DWORD dwBytesRead = 0;
	ioctlsocket(g_sHostScoket, FIONREAD, &dwBytesRead);
	if(dwBytesRead>0)
	{
		char *recvbuf=new char[dwBytesRead];
			 //����ս��ջ�����
			 recv(g_sHostScoket, recvbuf, dwBytesRead, 0);//
#ifdef _DEBUG
			 printf("dwBytesRead:%d \r\n",dwBytesRead);
			 for(int x=0;x<dwBytesRead;x++)
			 {
				printf("%02X ",recvbuf[x]);
			 }
			 printf("\r\n");
#endif
			 
	}
	return 1;
}

unsigned int SocketClient::ReadPLC(unsigned int addr)
{

   char SendBuf[12] = {0};
	SendBuf[0] = 0;//����Ԫ��ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[1] = 0;
	SendBuf[2] = 0;//Э���ʶ��,���ֽ���ǰ,���ֽ��ں�
	SendBuf[3] = 0;
	SendBuf[4] = 0x00;//�����ֽڳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[5] = 0x06;
	SendBuf[6] = 01;//��Ԫ��ʶ��
	SendBuf[7] = 0x03;//m_cmdword;//������
	SendBuf[8] = HIBYTE(addr);//������ʼ��ַ,���ֽ���ǰ,���ֽ��ں�
	SendBuf[9] = LOBYTE(addr);
	SendBuf[10] = HIBYTE(0x01);//���ݳ���,���ֽ���ǰ,���ֽ��ں�
	SendBuf[11] = LOBYTE(0x01);
	int ret = send(g_sHostScoket, SendBuf, 12, 0);
	Sleep(100);
	DWORD dwBytesRead = 0;
	ioctlsocket(g_sHostScoket, FIONREAD, &dwBytesRead);
	if(dwBytesRead>0)
	{
		char *recvbuf=new char[dwBytesRead];
			 //����ս��ջ�����
			 recv(g_sHostScoket, recvbuf, dwBytesRead, 0);//
#ifdef _DEBUG
			 printf("dwBytesRead:%d \r\n",dwBytesRead);
			 for(int x=0;x<dwBytesRead;x++)
			 {
				printf("%02X ",recvbuf[x]);
			 }
			 printf("\r\n");
#endif
			return ReadSignelData(recvbuf,dwBytesRead);
			 
	}

	return 0;
}