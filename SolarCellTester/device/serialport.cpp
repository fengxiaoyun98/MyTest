/*
Module : SERIALPORT.CPP
Purpose: Implementation for an MFC wrapper class for serial ports
Created: PJN / 31-05-1999
History: PJN / 03-06-1999 1. Fixed problem with code using CancelIo which does not exist on 95.
                          2. Fixed leaks which can occur in sample app when an exception is thrown
         PJN / 16-06-1999 1. Fixed a bug whereby CString::ReleaseBuffer was not being called in 
                             CSerialException::GetErrorMessage
         PJN / 29-09-1999 1. Fixed a simple copy and paste bug in CSerialPort::SetDTR
         PJN / 08-05-2000 1. Fixed an unreferrenced variable in CSerialPort::GetOverlappedResult in VC 6
         PJN / 10-12-2000 1. Made class destructor virtual
         PJN / 15-01-2001 1. Attach method now also allows you to specify whether the serial port
                          is being attached to in overlapped mode
                          2. Removed some ASSERTs which were unnecessary in some of the functions
                          3. Updated the Read method which uses OVERLAPPED IO to also return the bytes
                          read. This allows calls to WriteFile with a zeroed overlapped structure (This
                          is required when dealing with TAPI and serial communications)
                          4. Now includes copyright message in the source code and documentation.
         PJN / 24-03-2001 1. Added a BytesWaiting method
         PJN / 04-04-2001 1. Provided an overriden version of BytesWaiting which specifies a timeout
         PJN / 23-04-2001 1. Fixed a memory leak in DataWaiting method
         PJN / 01-05-2002 1. Fixed a problem in Open method which was failing to initialize the DCB 
                          structure incorrectly, when calling GetState. Thanks to Ben Newson for this fix.
         PJN / 29-05-2002 1. Fixed an problem where the GetProcAddress for CancelIO was using the
                          wrong calling convention
         PJN / 07-08-2002 1. Changed the declaration of CSerialPort::WaitEvent to be consistent with the
                          rest of the methods in CSerialPort which can operate in "OVERLAPPED" mode. A note
                          about the usage of this: If the method succeeds then the overlapped operation
                          has completed synchronously and there is no need to do a WaitForSingle/MultipleObjects.
                          If any other unexpected error occurs then a CSerialException will be thrown. See
                          the implementation of the CSerialPort::DataWaiting which has been rewritten to use
                          this new design pattern. Thanks to Serhiy Pavlov for spotting this inconsistency.
         PJN / 20-09-2002 1. Addition of an additional ASSERT in the internal _OnCompletion function.
                          2. Addition of an optional out parameter to the Write method which operates in 
                          overlapped mode. Thanks to Kevin Pinkerton for this addition.
         PJN / 10-04-2006 1. Updated copyright details.
                          2. Addition of a CSERIALPORT_EXT_CLASS and CSERIALPORT_EXT_API macros which makes 
                          the class easier to use in an extension dll.
                          3. Removed derivation of CSerialPort from CObject as it was not really needed.
                          4. Fixed a number of level 4 warnings in the sample app.
                          5. Reworked the overlapped IO methods to expose the LPOVERLAPPED structure to client 
                          code.
                          6. Updated the documentation to use the same style as the web site.
                          7. Did a spell check of the HTML documentation.
                          8. Updated the documentation on possible blocking in Read/Ex function. Thanks to 
                          D Kerrison for reporting this issue.
                          9. Fixed a minor issue in the sample app when the code is compiled using /Wp64
         PJN / 02-06-2006 1. Removed the bOverlapped as a member variable from the class. There was no 
                          real need for this setting, since the SDK functions will perform their own 
                          checking of how overlapped operations should
                          2. Fixed a bug in GetOverlappedResult where the code incorrectly checking against
                          the error ERROR_IO_PENDING instead of ERROR_IO_INCOMPLETE. Thanks to Sasho Darmonski
                          for reporting this bug.
                          3. Reviewed all TRACE statements for correctness.
         PJN / 05-06-2006 1. Fixed an issue with the creation of the internal event object. It was incorrectly
                          being created as an auto-reset event object instead of a manual reset event object.
                          Thanks to Sasho Darmonski for reporting this issue.
         PJN / 24-06-2006 1. Fixed some typos in the history list. Thanks to Simon Wong for reporting this.
                          2. Made the class which handles the construction of function pointers at runtime a
                          member variable of CSerialPort
                          3. Made AfxThrowSerialPortException part of the CSerialPort class. Thanks to Simon Wong
                          for reporting this.
                          4. Removed the unnecessary CSerialException destructor. Thanks to Simon Wong for 
                          reporting this.
                          5. Fixed a minor error in the TRACE text in CSerialPort::SetDefaultConfig. Again thanks
                          to Simon Wong for reporting this. 
                          6. Code now uses new C++ style casts rather than old style C casts where necessary. 
                          Again thanks to Simon Wong for reporting this.
                          7. CSerialException::GetErrorMessage now uses the strsafe functions. This does mean 
                          that the code now requires the Platform SDK if compiled using VC 6.
         PJN / 25-06-2006 1. Combined the functionality of the CSerialPortData class into the main CSerialPort class.
                          2. Renamed AfxThrowSerialPortException to ThrowSerialPortException and made the method
                          public.
         PJN / 05-11-2006 1. Minor update to stdafx.h of sample app to avoid compiler warnings in VC 2005.
                          2. Reverted the use of the strsafe.h header file. Instead now the code uses the VC 2005
                          Safe CRT and if this is not available, then we fail back to the standard CRT.
         PJN / 25-01-2007 1. Minor update to remove strsafe.h from stdafx.h of the sample app.
                          2. Updated copyright details.

Copyright (c) 1996 - 2007 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"
#ifndef _WINERROR_
#pragma message("To avoid this message, please put WinError.h in your PCH (normally stdafx.h)")
#include <WinError.h>
#endif


///////////////////////////////// Defines /////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////// Implementation ///////////////////////////////

BOOL CSerialException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
  //Validate our parameters
	ASSERT(lpszError != NULL && AfxIsValidString(lpszError, nMaxError));
	if (lpszError == NULL || nMaxError == 0) 
		return FALSE;
	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	LPTSTR lpBuffer;
	BOOL bRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			                      NULL,  m_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			                      reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

	if (bRet == FALSE)
		*lpszError = '\0';
	else
	{
	#if (_MSC_VER >= 1400)
	  _tcsncpy_s(lpszError, nMaxError, lpBuffer, _TRUNCATE);
  #else
	  lstrcpyn(lpszError, lpBuffer, nMaxError-1);
	  lpszError[nMaxError-1] = '\0';
  #endif
		bRet = TRUE;

		LocalFree(lpBuffer);
	}

	return bRet;
}

CString CSerialException::GetErrorMessage()
{
  CString rVal;
  LPTSTR pstrError = rVal.GetBuffer(4096);
  GetErrorMessage(pstrError, 4096, NULL);
  rVal.ReleaseBuffer();
  return rVal;
}

CSerialException::CSerialException(DWORD dwError)
{
	m_dwError = dwError;
}

IMPLEMENT_DYNAMIC(CSerialException, CException)

#ifdef _DEBUG
void CSerialException::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "m_dwError = " << m_dwError;
}
#endif


CSerialPort::CSerialPort() : m_hComm(INVALID_HANDLE_VALUE)
                             ,m_hEvent(NULL)
							 ,m_hLoopEvent(NULL)
							 ,m_WriteEnt(NULL)
							 ,m_ReadEnt(NULL)
{
  m_hKernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
  VERIFY(m_hKernel32 != NULL);
  m_lpfnCancelIo = (LPCANCELIO) GetProcAddress(m_hKernel32, "CancelIo");
  memset(&m_WriteOlp,0,sizeof(OVERLAPPED));
  memset(&m_ReadOlp,0,sizeof(OVERLAPPED));
  memset(&m_osReader,0,sizeof(OVERLAPPED));
  memset( sendbuf,0,sizeof(char)*DATA_LEN_MAX);
  memset( recvbuf,0,sizeof(char)*DATA_LEN_MAX);
  m_GetDataStr = _T("");
}

CSerialPort::~CSerialPort()
{
  Close();
}

void CSerialPort::ThrowSerialException(DWORD dwError /*= 0*/)
{
	if (dwError == 0)
		dwError = ::GetLastError();

	CSerialException* pException = new CSerialException (dwError);

	TRACE(_T("Warning: throwing CSerialException for error %d\n"), dwError);
	THROW(pException);
}

#ifdef _DEBUG
void CSerialPort::Dump(CDumpContext& dc) const
{
	dc << _T("m_hComm = ") << m_hComm << _T("\n");
}
#endif

void CSerialPort::Open(int nPort, DWORD dwBaud, Parity parity, BYTE DataBits, StopBits stopbits, FlowControl fc, BOOL bOverlapped)
{
  //Validate our parameters
  ASSERT(nPort>0 && nPort<=255);

  Close(); //In case we are already open

  //Call CreateFile to open the comms port
  CString sPort;
  sPort.Format(_T("\\\\.\\COM%d"), nPort);
  m_hComm = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, bOverlapped ?  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED : 0, NULL);
  if (m_hComm == INVALID_HANDLE_VALUE)
  {
    TRACE(_T("CSerialPort::Open, Failed to open the comms port\n"));
	return;
    ThrowSerialException();
  }

  //Create the event we need for later synchronisation use
  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_hEvent == NULL)
  {
    Close();
    TRACE(_T("CSerialPort::Open, Failed in call to CreateEvent in Open\n"));
    ThrowSerialException();
  }

  if(bOverlapped){
  //OVERLAPPED查询事件对象
  m_hLoopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_hLoopEvent == NULL)
  {
    Close();
    TRACE(_T("CSerialPort::Open, Failed in call to CreateEvent in Open\n"));
    ThrowSerialException();
  }
  m_osReader.hEvent=m_hLoopEvent;
  m_WriteEnt = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_WriteEnt == NULL){
	  Close();
	  TRACE(_T("CSerialPort::Open, Failed in call to CreateEvent in Open\n"));
	  ThrowSerialException();
  }
  m_WriteOlp.hEvent=m_WriteEnt;
  m_ReadEnt = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_ReadEnt == NULL){
	  Close();
	  TRACE(_T("CSerialPort::Open, Failed in call to CreateEvent in Open\n"));
	  ThrowSerialException();
  }
  m_ReadOlp.hEvent=m_ReadEnt;
  }


  //Get the current state prior to changing it
  DCB dcb;
  dcb.DCBlength = sizeof(DCB);
  GetState(dcb);

  //Setup the baud rate
  dcb.BaudRate = dwBaud; 

  //Setup the Parity
  switch (parity)
  {
    case EvenParity:  dcb.Parity = EVENPARITY;  break;
    case MarkParity:  dcb.Parity = MARKPARITY;  break;
    case NoParity:    dcb.Parity = NOPARITY;    break;
    case OddParity:   dcb.Parity = ODDPARITY;   break;
    case SpaceParity: dcb.Parity = SPACEPARITY; break;
    default:          ASSERT(FALSE);            break;
  }

  //Setup the data bits
  dcb.ByteSize = DataBits;

  //Setup the stop bits
  switch (stopbits)
  {
    case OneStopBit:           dcb.StopBits = ONESTOPBIT;   break;
    case OnePointFiveStopBits: dcb.StopBits = ONE5STOPBITS; break;
    case TwoStopBits:          dcb.StopBits = TWOSTOPBITS;  break;
    default:                   ASSERT(FALSE);               break;
  }

  //Setup the flow control 
  dcb.fDsrSensitivity = FALSE;
  switch (fc)
  {
    case NoFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsRtsFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsDtrFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrRtsFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrDtrFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case XonXoffFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = TRUE;
      dcb.fInX = TRUE;
      dcb.XonChar = 0x11;
      dcb.XoffChar = 0x13;
      dcb.XoffLim = 100;
      dcb.XonLim = 100;
      break;
    }
    default:
    {
      ASSERT(FALSE);
      break;
    }
  }
  
  //Now that we have all the settings in place, make the changes
  SetState(dcb);
}

void CSerialPort::Close()
{
  if (IsOpen())
  {
	ClearALL();
	::CancelIo(m_hComm);

    BOOL bSuccess = CloseHandle(m_hComm);
    m_hComm = INVALID_HANDLE_VALUE;
    if (!bSuccess)
      TRACE(_T("CSerialPort::Close, Failed to close up the comms port, GetLastError:%d\n"), GetLastError());

    //Free the event object we are using
    if (m_hEvent)
    {
      CloseHandle(m_hEvent);
      m_hEvent = NULL;
    }
    if (m_hLoopEvent)
    {
      CloseHandle(m_hLoopEvent);
      m_hLoopEvent = NULL;
    }
	if(m_WriteEnt){
		::CloseHandle(m_WriteEnt);
		m_WriteEnt=NULL;
	}
	if(m_ReadEnt){
		::CloseHandle(m_ReadEnt);
		m_ReadEnt=NULL;
	}

  }
}

void CSerialPort::Attach(HANDLE hComm)
{
  Close();

  //Validate our parameters, now that the port has been closed
  ASSERT(m_hComm == INVALID_HANDLE_VALUE);
  ASSERT(m_hEvent == NULL);

  m_hComm = hComm;  

  //Create the event we need for later synchronisation use
  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_hEvent == NULL)
  {
    DWORD dwError = GetLastError();
    Close();
    TRACE(_T("CSerialPort::Attach, Failed in call to CreateEvent in Attach\n"));
    ThrowSerialException(dwError);
  }
}

HANDLE CSerialPort::Detach()
{
  //What will be the return value from this function
  HANDLE hrVal = m_hComm;

  m_hComm = INVALID_HANDLE_VALUE;

  if (m_hEvent)
  {
    CloseHandle(m_hEvent);
    m_hEvent = NULL;
  }

  return hrVal;
}

DWORD CSerialPort::Read(void* lpBuf, DWORD dwCount)
{
	return ReadWithOlp(lpBuf,dwCount);
}
void CSerialPort::ClearALL(void)
{
	::PurgeComm(m_hComm,0xF);
}
//OVERLAPPED方式写入
DWORD CSerialPort::SendWithOlp(LPCVOID lpBuf, DWORD dwToWrite)
{
	if(!IsOpen())
		return FALSE;
	DWORD dwWritten;
	if (!WriteFile(m_hComm, lpBuf, dwToWrite, &dwWritten, &m_WriteOlp))
		if (GetLastError()== ERROR_IO_PENDING)
			if(::GetOverlappedResult(m_hComm, &m_WriteOlp, &dwWritten, TRUE))
				return dwWritten;
	return FALSE;
}
//OVERLAPPED方式读取
DWORD CSerialPort::ReadWithOlp (LPVOID lpBuf, DWORD dwToRead)
{ 
	if(!IsOpen())
		return FALSE;
	DWORD dwRead;
	if (!ReadFile(m_hComm, lpBuf, dwToRead, &dwRead, &m_ReadOlp) )
		if (GetLastError()== ERROR_IO_PENDING)
			if(::GetOverlappedResult(m_hComm, &m_ReadOlp, &dwRead, TRUE))
				return dwRead;
	return FALSE;
}

BOOL CSerialPort::Read(void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped, DWORD* pBytesRead)
{
  ASSERT(IsOpen());

  DWORD dwBytesRead = 0;
  BOOL bSuccess = ReadFile(m_hComm, lpBuf, dwCount, &dwBytesRead, &overlapped);
  if (!bSuccess)
  {
    if (GetLastError() != ERROR_IO_PENDING)
    {
      TRACE(_T("CSerialPort::Read, Failed in call to ReadFile\n"));
      ThrowSerialException();
    }
  }
  else
  {
    if (pBytesRead)
      *pBytesRead = dwBytesRead;
  }
  return bSuccess;
}

DWORD CSerialPort::Write(const void* lpBuf, DWORD dwCount)
{
	return SendWithOlp(lpBuf,dwCount);
}

BOOL CSerialPort::Write(const void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped, DWORD* pBytesWritten)
{
  ASSERT(IsOpen());

  DWORD dwBytesWritten = 0;
  BOOL bSuccess = WriteFile(m_hComm, lpBuf, dwCount, &dwBytesWritten, &overlapped);
  if (!bSuccess)
  {
    if (GetLastError() != ERROR_IO_PENDING)
    {
      TRACE(_T("CSerialPort::Write, Failed in call to WriteFile\n"));
      ThrowSerialException();
    }
  }
  else
  {
    if (pBytesWritten)
      *pBytesWritten = dwBytesWritten;
  }

  return bSuccess;
}

void CSerialPort::GetOverlappedResult(OVERLAPPED& overlapped, DWORD& dwBytesTransferred, BOOL bWait)
{
  ASSERT(IsOpen());

  if (!::GetOverlappedResult(m_hComm, &overlapped, &dwBytesTransferred, bWait))
  {
    if (GetLastError() != ERROR_IO_INCOMPLETE)
    {
      TRACE(_T("CSerialPort::GetOverlappedResult, Failed in call to GetOverlappedResult\n"));
      ThrowSerialException();
    }
  }
}

void CSerialPort::_OnCompletion(DWORD dwErrorCode, DWORD dwCount, LPOVERLAPPED lpOverlapped)
{
  //Validate our parameters
  ASSERT(lpOverlapped);

  //Convert back to the C++ world
  CSerialPort* pSerialPort = static_cast<CSerialPort*>(lpOverlapped->hEvent);
  ASSERT(pSerialPort);

  //Call the C++ function
  pSerialPort->OnCompletion(dwErrorCode, dwCount, lpOverlapped);
}

void CSerialPort::OnCompletion(DWORD /*dwErrorCode*/, DWORD /*dwCount*/, LPOVERLAPPED lpOverlapped)
{
  //Just free the memory which was previously allocated for the OVERLAPPED structure
  delete lpOverlapped;

  //Your derived classes can do something useful in OnCompletion, but don't forget to
  //call CSerialPort::OnCompletion to ensure the memory is freed
}

void CSerialPort::CancelIo()
{
  ASSERT(IsOpen());

  if (m_lpfnCancelIo == NULL)
  {
    TRACE(_T("CSerialPort::CancelIo, CancelIo function is not supported on this OS. You need to be running at least NT 4 or Win 98 to use this function\n"));
    ThrowSerialException(ERROR_CALL_NOT_IMPLEMENTED);  
  }

  if (!m_lpfnCancelIo(m_hComm))
  {
    TRACE(_T("Failed in call to CancelIO\n"));
    ThrowSerialException();
  }
}

DWORD CSerialPort::BytesWaiting()
{
  ASSERT(IsOpen());

  //Check to see how many characters are unread
  COMSTAT stat;
  GetStatus(stat);
  return stat.cbInQue;
}

BOOL CSerialPort::DataWaiting(DWORD dwTimeout)
{
  ASSERT(IsOpen());

  //Setup to wait for incoming data
  DWORD dwOldMask;
  GetMask(dwOldMask);
  SetMask(EV_RXCHAR);

  //Setup the overlapped structure
  OVERLAPPED o;
  o.hEvent = m_hEvent;

  //Assume the worst;
  BOOL bSuccess = FALSE;

  DWORD dwEvent;
  bSuccess = WaitEvent(dwEvent, o);
  if (!bSuccess)
  {
    if (WaitForSingleObject(o.hEvent, dwTimeout) == WAIT_OBJECT_0)
    {
      DWORD dwBytesTransferred;
      GetOverlappedResult(o, dwBytesTransferred, FALSE);
      bSuccess = TRUE;
    }
  }

  //Reset the event mask
  SetMask(dwOldMask);

  return bSuccess;
}

void CSerialPort::WriteEx(const void* lpBuf, DWORD dwCount)
{
  ASSERT(IsOpen());

  OVERLAPPED* pOverlapped = new OVERLAPPED;
  ZeroMemory(pOverlapped, sizeof(OVERLAPPED));
  pOverlapped->hEvent = static_cast<HANDLE>(this);
  if (!WriteFileEx(m_hComm, lpBuf, dwCount, pOverlapped, _OnCompletion))
  {
    delete pOverlapped;
    TRACE(_T("CSerialPort::WriteEx, Failed in call to WriteFileEx\n"));
    ThrowSerialException();
  }
}

void CSerialPort::ReadEx(void* lpBuf, DWORD dwCount)
{
  ASSERT(IsOpen());

  OVERLAPPED* pOverlapped = new OVERLAPPED;
  ZeroMemory(pOverlapped, sizeof(OVERLAPPED));
  pOverlapped->hEvent = static_cast<HANDLE>(this);
  if (!ReadFileEx(m_hComm, lpBuf, dwCount, pOverlapped, _OnCompletion))
  {
    delete pOverlapped;
    TRACE(_T("CSerialPort::ReadEx, Failed in call to ReadFileEx\n"));
    ThrowSerialException();
  }
}

void CSerialPort::TransmitChar(char cChar)
{
  ASSERT(IsOpen());

  if (!TransmitCommChar(m_hComm, cChar))
  {
    TRACE(_T("CSerialPort::TransmitChar, Failed in call to TransmitCommChar\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetConfig(COMMCONFIG& config)
{
  ASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetCommConfig(m_hComm, &config, &dwSize))
  {
    TRACE(_T("CSerialPort::GetConfig, Failed in call to GetCommConfig\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetConfig(COMMCONFIG& config)
{
  ASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetCommConfig(m_hComm, &config, dwSize))
  {
    TRACE(_T("CSerialPort::SetConfig, Failed in call to SetCommConfig\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetBreak()
{
  ASSERT(IsOpen());

  if (!SetCommBreak(m_hComm))
  {
    TRACE(_T("CSerialPort::SetBreak, Failed in call to SetCommBreak\n"));
    ThrowSerialException();
  }
}

void CSerialPort::ClearBreak()
{
  ASSERT(IsOpen());

  if (!ClearCommBreak(m_hComm))
  {
    TRACE(_T("CSerialPort::ClearBreak, Failed in call to SetCommBreak\n"));
    ThrowSerialException();
  }
}

void CSerialPort::ClearError(DWORD& dwErrors)
{
  ASSERT(IsOpen());

  if (!ClearCommError(m_hComm, &dwErrors, NULL))
  {
    TRACE(_T("CSerialPort::ClearError, Failed in call to ClearCommError\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetDefaultConfig(int nPort, COMMCONFIG& config)
{
  //Validate our parameters
  ASSERT(nPort>0 && nPort<=255);

  //Create the device name as a string
  CString sPort;
  sPort.Format(_T("COM%d"), nPort);

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetDefaultCommConfig(sPort, &config, &dwSize))
  {
    TRACE(_T("CSerialPort::GetDefaultConfig, Failed in call to GetDefaultCommConfig\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetDefaultConfig(int nPort, COMMCONFIG& config)
{
  //Validate our parameters
  ASSERT(nPort>0 && nPort<=255);

  //Create the device name as a string
  CString sPort;
  sPort.Format(_T("COM%d"), nPort);

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetDefaultCommConfig(sPort, &config, dwSize))
  {
    TRACE(_T("CSerialPort::SetDefaultConfig, Failed in call to SetDefaultCommConfig\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetStatus(COMSTAT& stat)
{
  ASSERT(IsOpen());

  DWORD dwErrors;
  if (!ClearCommError(m_hComm, &dwErrors, &stat))
  {
    TRACE(_T("CSerialPort::GetStatus, Failed in call to ClearCommError\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetState(DCB& dcb)
{
  ASSERT(IsOpen());

  if (!GetCommState(m_hComm, &dcb))
  {
    TRACE(_T("CSerialPort::GetState, Failed in call to GetCommState\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetState(DCB& dcb)
{
  ASSERT(IsOpen());

  if (!SetCommState(m_hComm, &dcb))
  {
    TRACE(_T("CSerialPort::SetState, Failed in call to SetCommState\n"));
    ThrowSerialException();
  }
}

void CSerialPort::Escape(DWORD dwFunc)
{
  ASSERT(IsOpen());

  if (!EscapeCommFunction(m_hComm, dwFunc))
  {
    TRACE(_T("CSerialPort::Escape, Failed in call to EscapeCommFunction\n"));
    ThrowSerialException();
  }
}

void CSerialPort::ClearDTR()
{
  Escape(CLRDTR);
}

void CSerialPort::ClearRTS()
{
  Escape(CLRRTS);
}

void CSerialPort::SetDTR()
{
  Escape(SETDTR);
}

void CSerialPort::SetRTS()
{
  Escape(SETRTS);
}

void CSerialPort::SetXOFF()
{
  Escape(SETXOFF);
}

void CSerialPort::SetXON()
{
  Escape(SETXON);
}

void CSerialPort::GetProperties(COMMPROP& properties)
{
  ASSERT(IsOpen());

  if (!GetCommProperties(m_hComm, &properties))
  {
    TRACE(_T("CSerialPort::GetProperties, Failed in call to GetCommProperties\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetModemStatus(DWORD& dwModemStatus)
{
  ASSERT(IsOpen());

  if (!GetCommModemStatus(m_hComm, &dwModemStatus))
  {
    TRACE(_T("CSerialPort::GetModemStatus, Failed in call to GetCommModemStatus\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetMask(DWORD dwMask)
{
  ASSERT(IsOpen());

  if (!SetCommMask(m_hComm, dwMask))
  {
    TRACE(_T("CSerialPort::SetMask, Failed in call to SetCommMask\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetMask(DWORD& dwMask)
{
  ASSERT(IsOpen());

  if (!GetCommMask(m_hComm, &dwMask))
  {
    TRACE(_T("CSerialPort::GetMask, Failed in call to GetCommMask\n"));
    ThrowSerialException();
  }
}

void CSerialPort::Flush()
{
  ASSERT(IsOpen());

  if (!FlushFileBuffers(m_hComm))
  {
    TRACE(_T("CSerialPort::Flush, Failed in call to FlushFileBuffers\n"));
    ThrowSerialException();
  }
}

void CSerialPort::Purge(DWORD dwFlags)
{
  ASSERT(IsOpen());

  if (!PurgeComm(m_hComm, dwFlags))
  {
    TRACE(_T("CSerialPort::Purge, Failed in call to PurgeComm\n"));
    ThrowSerialException();
  }
}

void CSerialPort::TerminateOutstandingWrites()
{
  Purge(PURGE_TXABORT);
}

void CSerialPort::TerminateOutstandingReads()
{
  Purge(PURGE_RXABORT);
}

void CSerialPort::ClearWriteBuffer()
{
  Purge(PURGE_TXCLEAR);
}

void CSerialPort::ClearReadBuffer()
{
  Purge(PURGE_RXCLEAR);
}

void CSerialPort::Setup(DWORD dwInQueue, DWORD dwOutQueue)
{
  ASSERT(IsOpen());

  if (!SetupComm(m_hComm, dwInQueue, dwOutQueue))
  {
    TRACE(_T("CSerialPort::Setup, Failed in call to SetupComm\n"));
    ThrowSerialException();
  }
}

void CSerialPort::SetTimeouts(COMMTIMEOUTS& timeouts)
{
  ASSERT(IsOpen());

  if (!SetCommTimeouts(m_hComm, &timeouts))
  {
    TRACE(_T("CSerialPort::SetTimeouts, Failed in call to SetCommTimeouts\n"));
    ThrowSerialException();
  }
}

void CSerialPort::GetTimeouts(COMMTIMEOUTS& timeouts)
{
  ASSERT(IsOpen());

  if (!GetCommTimeouts(m_hComm, &timeouts))
  {
    TRACE(_T("CSerialPort::GetTimeouts, Failed in call to GetCommTimeouts\n"));
    ThrowSerialException();
  }
}

void CSerialPort::Set0Timeout()
{
  COMMTIMEOUTS Timeouts;
  ZeroMemory(&Timeouts, sizeof(COMMTIMEOUTS));
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0WriteTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.WriteTotalTimeoutMultiplier = 0;
  Timeouts.WriteTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0ReadTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  Timeouts.ReadTotalTimeoutMultiplier = 0;
  Timeouts.ReadTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::WaitEvent(DWORD& dwMask)
{
  ASSERT(IsOpen());

  if (!WaitCommEvent(m_hComm, &dwMask, NULL))
  {
    TRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent\n"));
    ThrowSerialException();
  }
}

BOOL CSerialPort::WaitEvent(DWORD& dwMask, OVERLAPPED& overlapped)
{
  ASSERT(IsOpen());
  ASSERT(overlapped.hEvent);

  BOOL bSuccess = WaitCommEvent(m_hComm, &dwMask, &overlapped);
  if (!bSuccess)
  {
    if (GetLastError() != ERROR_IO_PENDING)
    {
      TRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent\n"));
      ThrowSerialException();
    }
  }

  return bSuccess;
}
void CSerialPort::ConfigCom(void)
{
	ASSERT(IsOpen());
	COMMTIMEOUTS tm;
	GetTimeouts(tm);
	tm.ReadIntervalTimeout =20;									//间隔超时 MAXDWORD;
	tm.ReadTotalTimeoutConstant =200;							//附加超时
	tm.ReadTotalTimeoutMultiplier =20;							//单个字符最大超时
	tm.WriteTotalTimeoutConstant = 100;
	tm.WriteTotalTimeoutMultiplier = 10;
	SetTimeouts (tm);   
	Setup( 2000,2000 );	

	DWORD dw=0xF;
	Purge(dw);	//ClearReadBuffer ()+ClearWriteBuffer();
}
WORD CSerialPort::CRC16 (const BYTE *nData, WORD wLength)
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
		WORD wCRCWord = 0xFFFF;        //0xFFFF为moubus校验   0x0000为16位校验

		while (wLength--)
		{
			nTemp = *nData++ ^ wCRCWord;
			wCRCWord >>= 8;
			wCRCWord ^= wCRCTable[nTemp];
		}
		return wCRCWord;

}
int CSerialPort::WritePLC(unsigned int addr, unsigned int val)
{
	byte cmd[8];
	DWORD rtncrc,rtn;
	memset( cmd,0,sizeof(char)*8);
	cmd[DA] = 1;
	cmd[FC] = 6;
	cmd[W_ADD_H] = HIBYTE(addr);
	cmd[W_ADD_L] = LOBYTE(addr);
	cmd[W_DATA_H] = HIBYTE(val);
	cmd[W_DATA_L] = LOBYTE(val);
	memcpy( sendbuf,cmd,sizeof(char)*6);
	rtncrc =  CRC16(  sendbuf,sizeof(char)*(CMD_DATALEN-2) );
	sendbuf[W_CRC_L] = LOBYTE(rtncrc);
	sendbuf[W_CRC_H] = HIBYTE(rtncrc);	
	rtn = Write( sendbuf,sizeof(char)*CMD_DATALEN);
	Sleep(50);
	ClearWriteBuffer();
	ClearReadBuffer();
	memset( sendbuf, 0,sizeof(char)*100);
	return rtn;
}

int CSerialPort::WritePLC(unsigned int addr, unsigned int val,unsigned int val2)
{
	byte cmd[10];
	DWORD rtncrc,rtn;
	memset( cmd,0,sizeof(char)*10);
	cmd[DA] = 1;
	cmd[FC] = 6;
	cmd[W_ADD_H] = HIBYTE(addr);
	cmd[W_ADD_L] = LOBYTE(addr);
	cmd[W_DATA_H] = HIBYTE(val);
	cmd[W_DATA_L] = LOBYTE(val);
	cmd[6] = HIBYTE(val2);
	cmd[7] = LOBYTE(val2);
	memcpy( sendbuf,cmd,sizeof(char)*8);
	rtncrc =  CRC16(  sendbuf,sizeof(char)*(CMD_DATALEN) );
	sendbuf[9] = LOBYTE(rtncrc);
	sendbuf[10] = HIBYTE(rtncrc);	
	Sleep(10);
	rtn = Write( sendbuf,sizeof(char)*10);
	Sleep(50);
	ClearWriteBuffer();
	ClearReadBuffer();
	memset( sendbuf, 0,sizeof(char)*100);
	return rtn;
}

unsigned int CSerialPort::ReadPLC(unsigned int addr)
{
	byte cmd[8];
	DWORD rtncrc;
	cmd[DA] = 1;
	cmd[FC] = 3;
	cmd[2] = HIBYTE(addr);
	cmd[3] = LOBYTE(addr);
	cmd[4] = 0;
	cmd[5] = DATA_LEN;
	memcpy( sendbuf,cmd,sizeof(char)*6);
	rtncrc =  CRC16(  sendbuf,sizeof(char)*(CMD_DATALEN-2) );
	sendbuf[W_CRC_L] = LOBYTE(rtncrc);
	sendbuf[W_CRC_H] = HIBYTE(rtncrc);	
	Sleep(10);
	Write( sendbuf,sizeof(char)*CMD_DATALEN);
	Sleep(50);
	ClearWriteBuffer();
	memset( sendbuf, 0,sizeof(char)*100);

	Sleep(50);
	Read( recvbuf,sizeof(char)*CMD_DATALEN );
	Sleep(50);
	ClearReadBuffer();	
	if(( recvbuf[2] == 2*DATA_LEN ))
	{
		rtncrc =  CRC16( recvbuf,sizeof(char)*(5) );
		if( ( recvbuf[R_CRC_L] == LOBYTE( rtncrc))&&( recvbuf[R_CRC_H] == HIBYTE(rtncrc)) )
		{
			m_getdata = MAKEWORD( recvbuf[R_DATA_L], recvbuf[R_DATA_H]);
			ClearReadBuffer();
			memset( recvbuf, 0,sizeof(char)*100);
			return m_getdata;
		}
		else
		{
			return ERR_CRC;
		}
	}
	else
		return ERR_REC_LEN;
}

CString CSerialPort::ReadPLC(unsigned int addr,unsigned int len,int & rtulen )
{
	byte cmd[8];
	DWORD rtncrc;
	cmd[DA] = 1;
	cmd[FC] = 3;
	cmd[2] = HIBYTE(addr);
	cmd[3] = LOBYTE(addr);
	cmd[4] = 0;
	cmd[5] = len;
	memcpy( sendbuf,cmd,sizeof(char)*6);
	rtncrc =  CRC16(  sendbuf,sizeof(char)*(CMD_DATALEN-2) );
	sendbuf[W_CRC_L] = LOBYTE(rtncrc);
	sendbuf[W_CRC_H] = HIBYTE(rtncrc);	
	Sleep(10);
	Write( sendbuf,sizeof(char)*CMD_DATALEN);
	Sleep(50);
	ClearWriteBuffer();
	memset( sendbuf, 0,sizeof(char)*200);

	Sleep(50);
	Read( recvbuf,sizeof(char)*(CMD_DATALEN+len*2) );
	Sleep(50);
	ClearReadBuffer();
	int m = 0;
	TCHAR ch[30];
	m_GetDataStr = _T("");
	if(( recvbuf[2] == 2*len ))
	{
		rtncrc =  CRC16( recvbuf,sizeof(char)*( 2*len + 3) );
		if( ( recvbuf[2*len + 3] == LOBYTE( rtncrc))&&( recvbuf[2*len + 4] == HIBYTE(rtncrc)) )
		{
			for( int i = 0;i< len*2; )
			{
				ch[m] = MAKEWORD( recvbuf[i+3], recvbuf[i+4]);
				if( ch[m] < 13 )
				{
					rtulen = 1000;
					i = len*2;
				}
				if( (ch[m] > 13 )
					&& (ch[m] < 45 ) )
				{
					rtulen = 1000;
					i = len*2;
				}
				if( (ch[m] > 45) && (ch[m] < 123 ))
				{
					rtulen = m;
				}
				i++;i++;
				m++;
			}
			m_GetDataStr = ch;
			m_GetDataStr = m_GetDataStr.Left(rtulen);
			ClearReadBuffer();
			memset( recvbuf, 0,sizeof(char)*100);
			return m_GetDataStr;
		}
		else
		{
			return _T("crc error");
		}
	}
	else
		return _T("len error");
}

int CSerialPort::Write_IO_Out(unsigned int addr,unsigned int open_or_close)
{
	ClearWriteBuffer();
	memset( sendbuf, 0,sizeof(char)*200);
	DWORD rtn;
	sendbuf[0] = 0xFE;
	sendbuf[1] = 0x05;
	sendbuf[2] = 0x00;
	sendbuf[3] = addr;
	sendbuf[4] = open_or_close;
	sendbuf[5] = 0x00;
	sendbuf[6] = LOBYTE(CRC16(sendbuf,6));
	sendbuf[7] = HIBYTE(CRC16(sendbuf,6));
	ClearALL();
	rtn = Write( sendbuf,sizeof(char)*8);
	Sleep(50);

	return rtn;
}
unsigned int CSerialPort::Read_IO_In()
{
	ClearALL();
	memset( sendbuf, 0,sizeof(char)*200);
	memset( recvbuf, 0,sizeof(char)*200);
	DWORD rtn;
	int m_data=0;
	sendbuf[0] = 0xFE;
	sendbuf[1] = 0x02;
	sendbuf[2] = 0x00;
	sendbuf[3] = 0x00;
	sendbuf[4] = 0x00;
	sendbuf[5] = 0x04;
	sendbuf[6] = LOBYTE(CRC16(sendbuf,6));
	sendbuf[7] = HIBYTE(CRC16(sendbuf,6));
	ClearALL();
	rtn = Write( sendbuf,sizeof(char)*8);
	Sleep(50);
	Read(recvbuf,sizeof(char)*6);
	if(recvbuf[1] == 0x02)
	{
		m_data = recvbuf[3];
	}
	return m_data;
}