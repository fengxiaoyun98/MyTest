/*
 *
 *	ChartLineSerie.h
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



class CChartLineSerie:
	public CChartSerie  
{
public:
	int  GetPenStyle() const        { return m_iPenStyle; }
	void SetPenStyle(int NewStyle);

	int	 GetWidth() const       { return m_iLineWidth; }
	void SetWidth(int NewValue);

	CChartLineSerie(CChartView* pParent);
	virtual ~CChartLineSerie();

private:
	void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	void Draw(CDC* pDC);
	void DrawAll(CDC *pDC);

	int m_iLineWidth;
	int m_iPenStyle;
};
