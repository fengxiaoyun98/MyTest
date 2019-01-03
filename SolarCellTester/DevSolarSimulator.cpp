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
//ARM采集函数
INT_PTR DevSolarSimulator::GetARMDATA(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short sun,unsigned short cmd)//获取IV序列
{
	SurfTemp(iv);
	return (*m_iter)->ARMTestingRun(iv,irradiance,/*suns_voc,*/0,cmd);
}
//单次测试
INT_PTR DevSolarSimulator::GetIV(CurveIV*const iv)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,0);
}
//单次测试按给定光强
INT_PTR DevSolarSimulator::GetIVAtSun(CurveIV*const iv,unsigned short sun)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun);

}
//单次测量定电压点区段（VOL为中心电压或者最小电压）
INT_PTR DevSolarSimulator::GetIVAtPmax(CurveIV*const iv,unsigned short sun,double vol)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun,vol);
}
//定电压测试电流线
INT_PTR DevSolarSimulator::GetISetV(CurveIV*const iv,unsigned short sun,double vol)
{
	SurfTemp(iv);
	return (*m_iter)->TestingRun(iv,sun,vol,0);	
}

//检测外部按键事件
bool DevSolarSimulator::DevKeyTriggleCheck(void)
{
	if((*m_iter)->CheckExternEvent(DaqBase::KEYACTOR)==PUSHSTARTKEY)
		return true;
	return false;
}
