/*
�豸����Ϊϵͳ�ṩ���Է����ṩ��������
���������¶ȵ��豸
*/
#pragma once
#include "device\serial.h"
#include "sunads\CurveIV.h"
#include "device\IR485.h"
#include <list>
#include <vector>
#include "DaqBase.h"

typedef	std::list <DaqBase*> __DaqList;
class MultiDAQ:public __DaqList //���ݲɼ�������
{
public:
	iterator m_iter;
	//CSerial m_com;
	enum DEVTYPE{ M841A,M005A,PCI6250,USB6250,DIODEMODEL};
public:
	MultiDAQ(void);
	virtual ~MultiDAQ(void);
	void AddDev(DaqBase* pdev);
	void DeletDev(CString name);
	void ConfigDev(void);//�����豸����ʼ���������豸�б��趨��ǰ�豸
};

class DevSolarSimulator : 
	public MultiDAQ
{
public:
	void SurfTemp(CurveIV*const iv);
	IR485 m_irTemp;
	//�����ⲿ�¼�����
	enum ENVENT{
		PUSHSTARTKEY=1
	};
public:
	INT_PTR GetIV(CurveIV*const iv);//��ȡIV���У���Ire
	INT_PTR GetIVAtSun(CurveIV*const iv,unsigned short sun);
	INT_PTR GetIVAtPmax(CurveIV*const iv,unsigned short sun,double vol);
	INT_PTR GetISetV(CurveIV*const iv,unsigned short sun,double vol);//����ѹ���Ե���

	INT_PTR GetARMDATA(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short sun,unsigned short cmd);

	DevSolarSimulator(void);
	~DevSolarSimulator(void);
//������ԣ�����β���

/*
�����߲��ԣ����⣬�Ͷ�����ߵ��ӣ��ڱ������ݣ�IV��Para������Щ��ͬ
RS�����߲��ԣ�800��ǿ��1000��ǿ���ԣ����¼���RS
Pmax���Ե�һ�α�׼��ǿ���ڶ��ξ۽�����ʵ���ԣ����¼���Pmax��Im��Vm
*/
	bool DevKeyTriggleCheck(void);
};
