/*
Module : SerialPort.H
Purpose: Interface for an MFC wrapper class for serial ports

Copyright (c) 1999 - 2007 by PJ Naughter.  

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////// Macros / Structs etc //////////////////////////

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#ifndef CSERIALPORT_EXT_CLASS
#define CSERIALPORT_EXT_CLASS
#endif

#ifndef CSERIALPORT_EXT_API
#define CSERIALPORT_EXT_API
#endif

#ifndef		DATA_LEN_MAX
#define		DATA_LEN_MAX	200
#endif

#define		DA				0	//设备地址
#define		FC				1	//功能码
#define		W_ADD_H			2
#define		W_ADD_L			3
#define		W_DATA_H		4
#define		W_DATA_L		5
#define		DL_H			2   //数据量高
#define		DL_L			3	//数据量低
#define		W_CRC_H			7
#define		W_CRC_L			6

#define		FC_WRITE		6
#define		FC_READ			3

#define		DATA_LEN		1
#define		R_LEN			2
#define		R_DATA_H		3
#define		R_DATA_L		4
#define		R_CRC_L			5
#define		R_CRC_H			6


#define		CMD_DATALEN		8

#define		ERR_CRC			10001
#define		ERR_REC_LEN		10002

/////////////////////////// Classes ///////////////////////////////////////////

///////////////// Serial port exception class /////////////////////////////////

class CSERIALPORT_EXT_CLASS CSerialException : public CException
{
public:
//Constructors / Destructors
	CSerialException(DWORD dwError);

//Methods
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);
	CString GetErrorMessage();

//Data members
	DWORD m_dwError;

protected:
	DECLARE_DYNAMIC(CSerialException)
};

/////////////// The main serial port class ////////////////////////////////////

class CSERIALPORT_EXT_CLASS CSerialPort
{
public:
//Enums
  enum FlowControl
  {
    NoFlowControl,
    CtsRtsFlowControl,
    CtsDtrFlowControl,
    DsrRtsFlowControl,
    DsrDtrFlowControl,
    XonXoffFlowControl
  };

  enum Parity
  {    
    EvenParity,
    MarkParity,
    NoParity,
    OddParity,
    SpaceParity
  };

  enum StopBits
  {
    OneStopBit,
    OnePointFiveStopBits,
    TwoStopBits
  };

//Constructors / Destructors
  CSerialPort();
  virtual ~CSerialPort();

//General Methods
  void Open(int nPort, DWORD dwBaud = 115200, Parity parity = NoParity, BYTE DataBits = 8, 
            StopBits stopbits = OneStopBit, FlowControl fc = NoFlowControl, BOOL bOverlapped = TRUE);
  void Close();
  void Attach(HANDLE hComm);
  HANDLE Detach();
  operator HANDLE() const { return m_hComm; };
  BOOL IsOpen() const { return m_hComm != INVALID_HANDLE_VALUE; };
#ifdef _DEBUG
  void Dump(CDumpContext& dc) const;
#endif

//Reading / Writing Methods
  void ClearALL(void);
  DWORD ReadWithOlp (LPVOID lpBuf, DWORD dwToRead);
  DWORD SendWithOlp(LPCVOID lpBuf, DWORD dwToWrite);
  DWORD Read(void* lpBuf, DWORD dwCount);
  BOOL  Read(void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped, DWORD* pBytesRead=NULL);
  void  ReadEx(void* lpBuf, DWORD dwCount);
  DWORD Write(const void* lpBuf, DWORD dwCount);
  BOOL  Write(const void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped, DWORD* pBytesWritten=NULL);
  void  WriteEx(const void* lpBuf, DWORD dwCount);
  void  TransmitChar(char cChar);
  void  GetOverlappedResult(OVERLAPPED& overlapped, DWORD& dwBytesTransferred, BOOL bWait);
  void  CancelIo();
  DWORD BytesWaiting();
  BOOL  DataWaiting(DWORD dwTimeout);

//Configuration Methods
  void GetConfig(COMMCONFIG& config);
  static void GetDefaultConfig(int nPort, COMMCONFIG& config);
  void SetConfig(COMMCONFIG& Config);
  static void SetDefaultConfig(int nPort, COMMCONFIG& config);

//Misc RS232 Methods
  void ClearBreak();
  void SetBreak();
  void ClearError(DWORD& dwErrors);
  void GetStatus(COMSTAT& stat);
  void GetState(DCB& dcb);
  void SetState(DCB& dcb);
  void Escape(DWORD dwFunc);
  void ClearDTR();
  void ClearRTS();
  void SetDTR();
  void SetRTS();
  void SetXOFF();
  void SetXON();
  void GetProperties(COMMPROP& properties);
  void GetModemStatus(DWORD& dwModemStatus); 

//Timeouts
  void SetTimeouts(COMMTIMEOUTS& timeouts);
  void GetTimeouts(COMMTIMEOUTS& timeouts);
  void Set0Timeout();
  void Set0WriteTimeout();
  void Set0ReadTimeout();

//Event Methods
  void SetMask(DWORD dwMask);
  void GetMask(DWORD& dwMask);
  void WaitEvent(DWORD& dwMask);
  BOOL WaitEvent(DWORD& dwMask, OVERLAPPED& overlapped);
  
//Queue Methods
  void Flush();
  void Purge(DWORD dwFlags);
  void TerminateOutstandingWrites();
  void TerminateOutstandingReads();
  void ClearWriteBuffer();
  void ClearReadBuffer();
  void Setup(DWORD dwInQueue, DWORD dwOutQueue);

//Overridables
  virtual void OnCompletion(DWORD dwErrorCode, DWORD dwCount, LPOVERLAPPED lpOverlapped);

//Static methods
  static void ThrowSerialException(DWORD dwError = 0);

protected:
//Typedefs
  typedef BOOL (WINAPI CANCELIO)(HANDLE);
  typedef CANCELIO* LPCANCELIO;

//Static methods
  static void WINAPI _OnCompletion(DWORD dwErrorCode, DWORD dwCount, LPOVERLAPPED lpOverlapped); 

//Member variables
  HANDLE     m_hComm;        //Handle to the comms port
  HANDLE     m_hEvent;       //A event handle we need for internal synchronisation
  HINSTANCE  m_hKernel32;    //Kernel32 handle
  LPCANCELIO m_lpfnCancelIo; //CancelIO function pointer

  HANDLE     m_hLoopEvent;   //重叠IO异步查询方式读取的句柄
  OVERLAPPED m_osReader;
  HANDLE m_ReadEnt;
  OVERLAPPED m_ReadOlp;
  HANDLE m_WriteEnt;
  OVERLAPPED m_WriteOlp;
public:
	WORD CRC16 (const BYTE *nData, WORD wLength);
	unsigned char sendbuf[DATA_LEN_MAX];
	unsigned char recvbuf[DATA_LEN_MAX];
	int WritePLC(unsigned int addr, unsigned int val);
	int WritePLC(unsigned int addr, unsigned int val,unsigned int val2);
	unsigned int ReadPLC(unsigned int addr);
	CString ReadPLC(unsigned int addr,unsigned int len ,int & rtnlen );
	int m_getdata;
	CString m_GetDataStr;
	void ConfigCom(void);
public:
	int Write_IO_Out(unsigned int addr,unsigned int open_or_close);
	unsigned int Read_IO_In();

};

#endif //__SERIALPORT_H__
