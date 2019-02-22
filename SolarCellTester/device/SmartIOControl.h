#pragma once

class CSmartIOControl
{
public:
	CSmartIOControl(void);

	enum IO_LED{IO_NULL_LED,IO_RED_LED,IO_GREEN_LED,IO_YELLOW_LED};

	//IO¶Ë¿ÚÓ³Éä
	 int m_o_map_Grade1;
	 int m_o_map_Grade2;
	 int m_o_map_Grade4;
	 int m_o_map_Grade8;
	 int m_o_map_TestOK;
	 int m_o_map_ScanF;
	 int m_o_map_TestNG;
	 int m_o_map_Red;
	 int m_o_map_Green;
	 int m_o_map_Yellow;
	 int m_o_map_Beep;
	 int m_i_map_IsTest;
	 int m_i_map_IsOut;
	 int m_i_map_IsScan;
	 int m_i_map_IsReset;

	 int m_error_grade;
     int m_alarm_time;

	 void SetIOPin(unsigned char mgrade,bool mIsTestOK,bool mIsTestNG,bool mIsScanFinish,IO_LED mLedColor,bool mIsBeep);
	 void GetIOPutIn(unsigned char &x1,unsigned char &x2,unsigned char &x3,unsigned char &x4);
	 bool GetGlassIO(int &port);
	 void Reset(void);
public:
	~CSmartIOControl(void);
};
