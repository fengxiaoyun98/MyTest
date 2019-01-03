/*
 *
 *	ChartSerie.cpp
 *
 *	Written by C閐ric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *	History:
 *		- 11/08/2006: Management of the auto axis now done in the axis. Series Register
 *					  Unregister themselves to their respective axes	.
 *		- 29/02/2008: Taking into account that RefreshAutoAxis doesn't refresh the control.
 *		- 01/03/2008: RemovePointsFromBegin and RemovePointsFromEnd functions added.
 *		- 08/03/2008: AddPoints function added (thanks to Bruno Lavier).
 *		- 11/03/2008: Min and max values are now cached.
 *		- 14/03/2008: Series can be ordered. Speed improvement done in that case.
 *
 */

#include "stdafx.h"
#include "ChartSerie.h"
#include "ChartAxis.h"
#include "ChartView.h"

#include "Math.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartSerie::CChartSerie(CChartView* pParent, int Type) : CChartObject(pParent)
{
	m_iLastDrawnPoint = 0;

	m_pHorizontalAxis = pParent->GetBottomAxis();
	m_pHorizontalAxis->RegisterSeries(this);
	m_pVerticalAxis = pParent->GetLeftAxis();
	m_pVerticalAxis->RegisterSeries(this);

	m_strSerieName = _T("");
	m_iSerieType = Type;

	m_dXMinVal = m_dXMaxVal = m_dYMinVal = m_dYMaxVal = 0;
	m_Ordering = soNoOrdering;//缺省不排序
}

CChartSerie::~CChartSerie()
{
	m_pHorizontalAxis->UnregisterSeries(this);
	m_pVerticalAxis->UnregisterSeries(this);
	if(m_vPoints.size())
		m_vPoints.clear();
}

void CChartSerie::SetSeriesOrdering(SeriesOrdering newOrdering)
{
	m_Ordering = newOrdering;
	ReorderPoints();
}

void CChartSerie::SetName(CString NewName) 
{ 
	m_strSerieName = NewName; 
	
}

void CChartSerie::AddPoint(double X, double Y)
{
	bool bInsertPoint = true;
	if (m_vPoints.size() == 0)
	{
		m_dXMinVal = m_dXMaxVal = X;
		m_dYMinVal = m_dYMaxVal = Y;
		bInsertPoint = false;
	}
	else
	{
		// Check if it can be inserted at the end of the series
		switch (m_Ordering)
		{
		case soNoOrdering:
			bInsertPoint = false;
			break;
		case soXOrdering:
			if (X > m_vPoints[m_vPoints.size()-1].X)
				bInsertPoint = false;
			break;
		case soYOrdering:
			if (Y > m_vPoints[m_vPoints.size()-1].Y)
				bInsertPoint = false;
			break;
		}

		if (X > m_dXMaxVal)  m_dXMaxVal = X;
		if (X < m_dXMinVal)  m_dXMinVal = X;
		if (Y > m_dYMaxVal)  m_dYMaxVal = Y;
		if (Y < m_dYMinVal)  m_dYMinVal = Y;
	}

	SChartPoint NewPoint;
	NewPoint.X = X;
	NewPoint.Y = Y;
	if (!bInsertPoint)
		m_vPoints.push_back(NewPoint);
	else
		InsertNewPoint(NewPoint);

/*没必要在这里画屏幕,动画添加效果
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();
	CDC* pDC = m_pParent->GetDC();
	Draw(pDC);
	m_pParent->ReleaseDC(pDC);//释放GetDC（）
	*/
}

void CChartSerie::AddPoints(double *X, double *Y, UINT Count)
{
	if (m_vPoints.size() == 0)
	{
		m_dXMinVal = m_dXMaxVal = X[0];
		m_dYMinVal = m_dYMaxVal = Y[0];
	}

	m_vPoints.reserve(m_vPoints.size() + Count);
	for (UINT i=0;i<Count;i++)
	{
		AddPoint(X[i], Y[i]);
	}
}

void CChartSerie::AddPoints(std::vector<double>X, std::vector<double>Y ,UINT Count)
{
	if(X.size()==0||Y.size()==0||Count==0)
		return;
	if (m_vPoints.size() == 0)
	{
		m_dXMinVal = m_dXMaxVal = X[0];
		m_dYMinVal = m_dYMaxVal = Y[0];
	}

	//reserve时size可能比目标尺寸大，resize时size和目标尺寸一致
	m_vPoints.reserve(m_vPoints.size() + Count);

	std::vector<double>::iterator itery,iterx;
	iterx=X.begin ();
	itery=Y.begin ();

	for (UINT i=0;i<Count;i++)
	{
		AddPoint(*iterx, *itery);
		iterx++;
		itery++;
	}
}

void CChartSerie::SetPoints(double *X, double *Y, UINT Count)
{
	m_vPoints.clear();
	for (UINT i=0;i<Count;i++)
	{
		SChartPoint NewPoint;
		NewPoint.X = X[i];
		NewPoint.Y = Y[i];
		m_vPoints.push_back(NewPoint);
	}

	ReorderPoints();
	RefreshMinMax();
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();
	
}

void CChartSerie::RemovePointsFromBegin(unsigned Count)
{
	ASSERT (Count < m_vPoints.size());
	m_vPoints.erase(m_vPoints.begin(),m_vPoints.begin()+Count);

	RefreshMinMax();
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();
}

void CChartSerie::RemovePointsFromEnd(unsigned Count)
{
	ASSERT (Count < m_vPoints.size());
	m_vPoints.erase(m_vPoints.end()-Count,m_vPoints.end());

	RefreshMinMax();
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();
}

void CChartSerie::ClearSerie()
{
	// We don't care about the return of RefreshAutoAxis: 
	// we will always refresh the control afterwards.
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();

	m_dXMinVal = m_dXMaxVal = m_dYMinVal = m_dYMaxVal = 0;
	m_vPoints.clear();
}

void CChartSerie::InsertNewPoint(const SChartPoint& NewPoint)
{
	if (m_vPoints.size() == 0) 
	{
		m_vPoints.push_back(NewPoint);
		return;
	}

	bool bInserted = false;
	std::vector<SChartPoint>::iterator iter = m_vPoints.begin();
	if (m_Ordering == soXOrdering)
	{
		for (iter; iter!=m_vPoints.end(); iter++)
		{//效率insert不高
			if (NewPoint.X > iter->X)
			{
				m_vPoints.insert(iter,NewPoint);
				bInserted = true;
				break;
			}
		}
	}
	else if (m_Ordering == soYOrdering)
	{
		for (iter; iter!=m_vPoints.end(); iter++)
		{
			if (NewPoint.Y > iter->Y)
			{
				m_vPoints.insert(iter,NewPoint);
				bInserted = true;
				break;
			}
		}
	}

	if (!bInserted)
		m_vPoints.push_back(NewPoint);
}

void CChartSerie::ReorderPoints()
{
	SPointSort NewSort;
	switch(m_Ordering)
	{
	case soNoOrdering:
		//Do nothing
		break;
	case soXOrdering:
		NewSort.m_bXSort = true;
		std::sort(m_vPoints.begin(),m_vPoints.end(),NewSort);
		break;
	case soYOrdering:
		NewSort.m_bXSort = false;
		std::sort(m_vPoints.begin(),m_vPoints.end(),NewSort);
		break;
	}
}


double CChartSerie::GetXPointValue(int PointIndex) const
{
	if (PointIndex >= (int)m_vPoints.size() )
		return 0;

	return m_vPoints[PointIndex].X;
}

double CChartSerie::GetYPointValue(int PointIndex) const
{
	if (PointIndex >= (int)m_vPoints.size())
		return 0;

	return m_vPoints[PointIndex].Y;
}

void CChartSerie::SetYPointValue(int PointIndex, double NewVal)
{
	if (PointIndex >= (int)m_vPoints.size())
		return;

	m_vPoints[PointIndex].Y = NewVal;

	m_dYMinVal = m_dYMaxVal = m_vPoints[0].Y;
	std::vector<SChartPoint>::iterator iter = m_vPoints.begin();
	for (iter; iter!=m_vPoints.end(); iter++)
	{
		if (iter->Y > m_dYMaxVal)  m_dYMaxVal = iter->Y;
		if (iter->Y < m_dYMinVal)  m_dYMinVal = iter->Y;
	}

	if (m_Ordering == soYOrdering)
		ReorderPoints();

	// We don't care about the return of RefreshAutoAxis: 
	// we will always refresh the control afterwards.
	m_pVerticalAxis->RefreshAutoAxis();
}
	
void CChartSerie::SetXPointValue(int PointIndex, double NewVal)
{
	if (PointIndex >= (int)m_vPoints.size())
		return;

	m_vPoints[PointIndex].X = NewVal;

	m_dXMinVal = m_dXMaxVal = m_vPoints[0].X;
	std::vector<SChartPoint>::iterator iter = m_vPoints.begin();
	for (iter; iter!=m_vPoints.end(); iter++)
	{
		if (iter->X > m_dXMaxVal)  m_dXMaxVal = iter->X;
		if (iter->X < m_dXMinVal)  m_dXMinVal = iter->X;
	}

	if (m_Ordering == soYOrdering)
		ReorderPoints();
}

void CChartSerie::RefreshMinMax()
{
	if (m_vPoints.size() == 0)
	{
		m_dXMinVal = m_dXMaxVal = m_dYMinVal = m_dYMaxVal = 0;
		return;
	}

	m_dXMinVal = m_dXMaxVal = m_vPoints[0].X;
	m_dYMinVal = m_dYMaxVal = m_vPoints[0].Y;

	std::vector<SChartPoint>::iterator iter = m_vPoints.begin();
	for (iter; iter!=m_vPoints.end(); iter++)
	{
		if (iter->X > m_dXMaxVal)  m_dXMaxVal = iter->X;
		if (iter->X < m_dXMinVal)  m_dXMinVal = iter->X;
		if (iter->Y > m_dYMaxVal)  m_dYMaxVal = iter->Y;
		if (iter->Y < m_dYMinVal)  m_dYMinVal = iter->Y;
	}
}


bool CChartSerie::GetSerieXMinMax(double &Min, double &Max) const
{
	if ( (m_vPoints.size()==0) || !IsVisible() ) 
		return false;

	Min = m_dXMinVal;
	Max = m_dXMaxVal;
	return true;
}

bool CChartSerie::GetSerieYMinMax(double &Min, double &Max) const
{
	if (m_vPoints.size() == 0)
		return false;

	Min = m_dYMinVal;
	Max = m_dYMaxVal;
	return true;
}

bool CChartSerie::SetVerticalAxis(bool bSecond)
{
	if (bSecond)
	{		
		CChartAxis* pAxis = m_pParent->GetRightAxis();
		if (pAxis)
		{
			m_pVerticalAxis->UnregisterSeries(this);
			m_pVerticalAxis->RefreshAutoAxis();

			m_pVerticalAxis = pAxis;
			m_pVerticalAxis->RegisterSeries(this);
			m_pVerticalAxis->RefreshAutoAxis();
			return true;
		}
		else
			return false;
	}
	else
	{
		CChartAxis* pAxis = m_pParent->GetLeftAxis();
		if (pAxis)
		{
			m_pVerticalAxis->UnregisterSeries(this);
			m_pVerticalAxis->RefreshAutoAxis();

			m_pVerticalAxis = pAxis;
			m_pVerticalAxis->RegisterSeries(this);
			m_pVerticalAxis->RefreshAutoAxis();
			return true;
		}
		else
			return false;
	}
}

bool CChartSerie::SetHorizontalAxis(bool bSecond)
{
	if (bSecond)
	{
		CChartAxis* pAxis = m_pParent->GetTopAxis();
		if (pAxis)
		{
			m_pHorizontalAxis->UnregisterSeries(this);
			m_pHorizontalAxis->RefreshAutoAxis();

			m_pHorizontalAxis = pAxis;
			m_pHorizontalAxis->RegisterSeries(this);
			m_pHorizontalAxis->RefreshAutoAxis();
			return true;
		}
		else
			return false;
	}
	else
	{
		CChartAxis* pAxis = m_pParent->GetBottomAxis();
		if (pAxis)
		{
			m_pHorizontalAxis->UnregisterSeries(this);
			m_pHorizontalAxis->RefreshAutoAxis();

			m_pHorizontalAxis = pAxis;
			m_pHorizontalAxis->RegisterSeries(this);
			m_pHorizontalAxis->RefreshAutoAxis();
			return true;
		}
		else
			return false;
	}
}


double CChartSerie::XScreenToValue(long XScreenCoord) const
{
	return m_pHorizontalAxis->ScreenToValue(XScreenCoord);
}

double CChartSerie::YScreenToValue(long YScreenCoord) const
{
	return m_pVerticalAxis->ScreenToValue(YScreenCoord);
}

void CChartSerie::ValueToScreen(double XValue, double YValue, CPoint &ScreenPoint) const
{
	ScreenPoint.x = m_pHorizontalAxis->ValueToScreen(XValue);
	ScreenPoint.y = m_pVerticalAxis->ValueToScreen(YValue);
}

CChartLineSerie* CChartSerie::GetAsLine()
{
	ASSERT(m_iSerieType==stLineSerie);
	return reinterpret_cast<CChartLineSerie*>(this);
}

CChartPointsSerie* CChartSerie::GetAsPoints()
{
	ASSERT(m_iSerieType==stPointsSerie);
	return reinterpret_cast<CChartPointsSerie*>(this);
}

void CChartSerie::GetVisiblePoints(int& iFirst, int& iLast)
{
	// Init the values at largest range
	// TODO: optimize the function.
	iFirst = 0;
	iLast = (int)m_vPoints.size() - 1;
	if (m_Ordering == soNoOrdering)
		return;

	double XMin=0, XMax=0;
	double YMin=0, YMax=0;
	m_pHorizontalAxis->GetMinMax(XMin, XMax);
	m_pVerticalAxis->GetMinMax(YMin, YMax);

	int Count = 0;
	std::vector<SChartPoint>::iterator iter = m_vPoints.begin();
	for (iter; iter!=m_vPoints.end(); iter++)
	{
		if ( (m_Ordering==soXOrdering) && (iter->X >= XMin) )
		{
			iFirst = Count;
			break;
		}
		else if ( (m_Ordering==soYOrdering) && (iter->Y >= YMin) )
		{
			iFirst = Count;
			break;
		}
		Count++;
	}

	for (iter; iter!=m_vPoints.end(); iter++)
	{
		if ( (m_Ordering==soXOrdering) && (iter->X > XMax) )
		{
			iLast = Count;
			break;
		}
		else if ( (m_Ordering==soYOrdering) && (iter->Y > YMax) )
		{
			iLast = Count;
			break;
		}
		Count++;
	}

/*	Count = m_vPoints.size() - 1;
	std::vector<SChartPoint>::reverse_iterator revIter = m_vPoints.rbegin();
	for (revIter; revIter!=m_vPoints.rend(); revIter++)
	{
		if ( (m_Ordering==soXOrdering) && (revIter->X <= XMax) )
		{
			iLast = Count;
			break;
		}
		else if ( (m_Ordering==soYOrdering) && (revIter->Y <= YMax) )
		{
			iLast = Count;
			break;
		}
		Count--;
	}*/
}

