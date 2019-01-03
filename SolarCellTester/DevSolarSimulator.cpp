#include "StdAfx.h"
#include "DevSolarSimulator.h"
#include "Daq841.h"

DevSolarSimulator::DevSolarSimulator(void)
{
}

DevSolarSimulator::~DevSolarSimulator(void)
{
}

MultiDAQ::MultiDAQ(void)
{
	Daq841 *pDev=new Daq841();
	this->push_front(pDev);
	m_iter=begin();
}

MultiDAQ::~MultiDAQ(void)
{
	m_iter=begin();
	while (m_iter!=end())
	{
		delete *m_iter;
		m_iter++;
	}
}
void DevSolarSimulator::SurfTemp(CurveIV*const iv)
{
	if(m_irTemp.ReqTemp())
		iv->SetSurfTemp(m_irTemp.GetTemp());
}
//ARM�ɼ�����
INT_PTR DevSolarSimulator::GetARMDATA(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short sun,unsigned short cmd)//��ȡIV����
{
	SurfTemp(iv);
	return (*m_iter)->ARMTestingRun(iv,irradiance,/*suns_voc,*/0,cmd);
}
//���β���
INT_PTR DevSolarSimulator::GetIV(CurveIV*const iv)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,0);
}
//���β��԰�������ǿ
INT_PTR DevSolarSimulator::GetIVAtSun(CurveIV*const iv,unsigned short sun)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun);

}
//���β�������ѹ�����Σ�VOLΪ���ĵ�ѹ������С��ѹ��
INT_PTR DevSolarSimulator::GetIVAtPmax(CurveIV*const iv,unsigned short sun,double vol)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun,vol);
}
//����ѹ���Ե�����
INT_PTR DevSolarSimulator::GetISetV(CurveIV*const iv,unsigned short sun,double vol)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun,vol,0);	
}

//����ⲿ�����¼�
bool DevSolarSimulator::DevKeyTriggleCheck(void)
{
	if((*m_iter)->CheckExternEvent(DaqBase::KEYACTOR)==PUSHSTARTKEY)
		return true;
	return false;
}
