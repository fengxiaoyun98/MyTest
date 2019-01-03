/*
 *
 *	ChartPointsSerie.h
 *
 *	Written by C�dric Moonen (cedric_moonen@hotmail.com)
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
 *
 */

#pragma once
#include "ChartSerie.h"


class CChartPointsSerie : 
	public CChartSerie  
{
public:
	enum PointType
	{
		ptEllipse=0,
		ptRectangle=1,
		ptTriangle=2
	};

	void SetPointSize(int XSize, int YSize);
	void GetPointSize(int& XSize, int& YSize) const
	{
        XSize = m_iXPointSize;
        YSize = m_iYPointSize;
    }
	void SetPointType(PointType Type);
	PointType GetPointType() const     { return m_iPointType; }

	CChartPointsSerie(CChartView* pParent);
	virtual ~CChartPointsSerie();

private:
	void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	void DrawAll(CDC *pDC);
	void Draw(CDC* pDC);


	int m_iXPointSize;		// Width of the points
	int m_iYPointSize;		// Height of the points
	PointType m_iPointType;

};
