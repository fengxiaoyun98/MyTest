/*

*/

#pragma once

#include "SerialPort.h"

class CSerial : public CSerialPort
{
public:
	CSerial(void);
	~CSerial(void);
public:
	enum COMCMD{
		IDLE=0,			LIGHTSTRONG=0x11,	TESTSTART=0x22 ,
		READXRAM=0x33,	GETPMAX=0x44,		READCODE=0x77,
		READPARA=0x88,	ONLINE=0x99,		VERSION=0xBB,
		GETNXCUR=0xCC,	TASKERROR=0xEE,		SETREF25=0xDD,
		READIRAM=0x55,	READTEMP=0x66,		SWOPERATE=0xAA,
		PUSHKEY=0x01,	SERIALNO=0xA2,
	};
	//MCU�����¼�����
	enum ENVENT{
		PUSHSTARTKEY=1
	};
	//��ʼ�ͽ�����
	enum FRMCTRL{
		HEADFRM=58,
		ENDFRM=13,
		DATALEN=16,
		FRMLEN=21
	};

	/* �ɹ��򷵻��յ��ֽ��������ɹ�����0*/ 	
	unsigned int RecvFrm(void);
	int SendFrm(unsigned char cmd, unsigned char* data, unsigned char len);
	void SetCmd(byte cmd){m_cmd=cmd;};
	int AutoOpenPort(CString &strvalue);
	int mcucomreset(void);
	void ConfigCom_S(void);
//protected:
	byte m_cmd;
	unsigned int DataCRC( unsigned char *puchMsg, unsigned int usDataLen );
	unsigned char sendbuf[FRMLEN];
	unsigned char recvbuf[FRMLEN];
	/* CRC ��λ�ֽ�ֵ�� */ 
	static unsigned char auchCRCHi[256]; 
	/* CRC��λ�ֽ�ֵ��*/ 
	static unsigned char auchCRCLo[256];
	///////////////////////���У��///////////////////////////////
	UINT16 crc16(UINT16 crc, const UINT8 *buffer, size_t len);
	UINT16 crc16vector(UINT16 crc, std::vector<unsigned char> buffer, size_t len);

	UINT16 crc16_byte(UINT16 crc, const UINT8 data);

};
