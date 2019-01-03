#include "StdAfx.h"
#include "..\StdAfx.h"
#include "CalibrationFiltering.h"

CalibrationFiltering::CalibrationFiltering(void)
{
	m_setbase=&theApp.m_opt;
	m_setcal=m_setbase->GetSetSunPara();
	m_setiv=m_setbase->GetSetCurveIV();
}

CalibrationFiltering::~CalibrationFiltering(void)
{
}
void CalibrationFiltering::CalFilter(CDoubleXY*const obj)
{
}
