#pragma once

#include "SunAds\CurveIV.h"

//class CDevLamp;//ģ�����Ŀ��ƣ�����CDevRefCell�ο����] 
//class CDevLoad;//���Ӹ��صĿ���] 
//class CDevTemp;�������������¶Ȳ���װ��] 
//Ӳ�������
//�๦�����ݲɼ�����Ӳ���ӿ�
//������DAQ������Ƭ��(F005/841)������ͨ�ŷ�ʽ�Ĳɼ�����

class DaqBase
{
public:
	DaqBase(void);
	virtual ~DaqBase(void);
public:
	//exmsg��Ҫ�������ݶ��壬������Ӧ��
	enum EXTERNEVENT{KEYACTOR=0,ONLINE,LOGIN};

	virtual UINT CheckExternEvent(UINT exmsg)=0;
	//���β���IV���ߣ�����(I,V)����
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq=0)=0;
	//���β���IV���ߣ����߶���vol����
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol)=0;
	//���β���IV���ߣ����߶���vol����
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol,UINT step)=0;
	//�������õĶ������Թ���
	//ARM�ɼ�����

	virtual INT_PTR ARMTestingRun(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short gq,unsigned short cmd) = 0;
public:
	CString GetModel(){return m_model;}
	CString GetName(){return m_name;};
	void SetName(CString n){ m_name=n;};
private:
	void SetModel(CString m){m_model=m;}
	//�豸��Ϣ���豸����//��������//�豸���//�豸���ܱ�
	COleDateTime m_date;
	CString m_model;
	CString m_name;
};

