#include "StdAfx.h"
#include "AreaMapping.h"

AreaMapping::AreaMapping(void)
{
	m_amin=0,m_amax=1;
	m_bmin=0,m_bmax=1;
	m_scale=1;
}

AreaMapping::~AreaMapping(void)
{
}
AreaMapping::AreaMapping(double a0,double a1,double b0,double b1,double bdefalut)
{
	if(!SetPair(a0,a1,b0,b1,bdefalut))
		AreaMapping();
}
bool AreaMapping::SetPair(double a0,double a1,double b0,double b1,double bdefalut)
{
	if(a0>=a1||b0>=b1)
		return false;

	if(bdefalut<b0||bdefalut>b1)
		return false;
	m_amin=a0,m_amax=a1;
	m_bmin=b0,m_bmax=b1;
	m_scale=(m_amax-m_amin)/(m_bmax-m_bmin);
	m_defalut=bdefalut;
	return true;
}
bool AreaMapping::atob(double& a)
{
	if(a<m_amin||a>m_amax)
		false;
	a=(a-m_amin)/m_scale+m_bmin;
	return true;
}
bool AreaMapping::btoa(double& b)
{
	if(b<m_bmin||b>m_bmax)
		return false;
	b=(b-m_bmin)*m_scale+m_amin;
	return true;
}
