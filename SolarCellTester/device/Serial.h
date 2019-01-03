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
	//MCU发生事件定义
	enum ENVENT{
		PUSHSTARTKEY=1
	};
	//起始和结束码
	enum FRMCTRL{
		HEADFRM=58,
		ENDFRM=13,
		DATALEN=16,
		FRMLEN=21
	};

	/* 成功则返回收到字节数；不成功返回0*/ 	
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
	/* CRC 高位字节值表 */ 
	static unsigned char auchCRCHi[256]; 
	/* CRC低位字节值表*/ 
	static unsigned char auchCRCLo[256];
	///////////////////////混合校验///////////////////////////////
	UINT16 crc16(UINT16 crc, const UINT8 *buffer, size_t len);
	UINT16 crc16vector(UINT16 crc, std::vector<unsigned char> buffer, size_t len);

	UINT16 crc16_byte(UINT16 crc, const UINT8 data);

};
