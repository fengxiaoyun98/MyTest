/*
�Ժ�841����Ľ������̷�װ
�ṩ�����豸����ط������ݲɼ���I/O����
�ṩ�豸��Ϣ
*/

#pragma once
#include "DaqBase.h"
//#include "device\Serial.h"
#include "Options\SetDev841.h"
#include "Options\Setbase.h"
#include "TimerUS.h"

#include "Encryption/Encryption.h"       //������Ȩ��
#include "EncrEdit.h"

class Daq841:public DaqBase
{
public:
	enum TESTMODE{TMODSET=0,TMODCUS=1};
	Daq841(void);
	virtual ~Daq841(void);
public:
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol);
	virtual INT_PTR TestingRun(CurveIV*const iv,unsigned short gq=0);
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol,UINT step);
	virtual UINT CheckExternEvent(UINT exmsg);

	virtual INT_PTR ARMTestingRun(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short gq,unsigned short cmd);
public:
	CSetBase* m_setbase;
	CSetDev841* m_set;
	CSetSunPara* m_setcal;
//	CSerial m_com;
	class ParaList
	{
	public:
		unsigned char dwi;
		unsigned char dwv;
		unsigned short gq;
		unsigned char mode;
		unsigned char step;
		unsigned short lowvol;
		unsigned short daqcnt;
		unsigned char dly;
	};
private:
	TimerUS m_PlusCtrl;
	ParaList m_ModeList[10];
	INT_PTR TestSingle(CurveIV *const iv,ParaList* pl);
	INT_PTR TestSingle(unsigned char dwi,unsigned char dwv,unsigned short gq,unsigned char mode,CurveIV *const iv,unsigned char step=0,unsigned short lowvol=0,unsigned short daqcnt=400,unsigned char dly=0);
	INT_PTR GetPmaxPoint(double &i,double &v,double prevol);
	INT_PTR portstate(unsigned char* p);
	INT_PTR __Test(void);
	UINT checkmcukeystate(void);
	INT_PTR ARMTestSingle(USHORT cmd,UCHAR dw,USHORT gqv,USHORT gqcnt,CurveIV *const iv,/*CDoubleXY *const suns_voc,*/CDoubleXY *const irradiance,byte dly,USHORT testcnt = 400);
	//////////////////////����ʹ��/////////////////////////////
public:
	CEncryption m_encr;
	CEncrEdit   m_encr_edit;
	int m_ifneedactive;
	std::vector<unsigned char>m_testdata;
	CString strvalue;
};
