#pragma once

#include <Winsock2.h>
#pragma comment (lib, "ws2_32.lib");



class SocketClient
{
public:
	SocketClient(void);

	bool m_IsConnect;
	int SendTestResultData();
	CWinThread * pThreadGetSerial;
	bool m_ThreadRunFlag;
	int SendGetSeialCmd(unsigned int addr,unsigned int len);
	int RecvData(char *recvbuf,int len,int revlen);
	WORD CRC16 (const BYTE *nData, WORD wLength);

	virtual int WritePLC(unsigned int addr, unsigned int val);//∏∏¿‡
	virtual unsigned int ReadPLC(unsigned int addr);

	void InitConnect();
public:
	~SocketClient(void);
};
