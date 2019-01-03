/*
 *  ����ͼ��
 *	ChartLegend.h
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

#include "ChartObject.h"

#include <string>


class CChartSerie;

class CChartLegend : public CChartObject  
{
	friend CChartView;

public:
	void SetFont(int iPointSize,CString strFaceName);

	CChartLegend(CChartView* pParent);
	virtual ~CChartLegend();

	enum DockSide
	{
		dsDockRight,
		dsDockLeft,
		dsDockTop,
		dsDockBottom
	};

	void DockLegend(DockSide dsSide);
	void UndockLegend(int iLeftPos, int iTopPos);

	void SetTransparent(bool bTransparent);  
	void SetHorizontalMode(bool bHorizontal);

private:
	void Draw(CDC* pDC);
	void ClipArea(CRect& rcControl, CDC* pDC);
	void UpdatePosition(CDC* pDC, const CRect& rcControl);

	CString m_strFontName;
	int         m_iFontSize;

	bool m_bDocked;	// true if the legend is docked
	DockSide m_DockSide;

	// If the legend is not docked:
	int m_iLeftPos;
	int m_iTopPos;

	bool m_bIsTransparent;
	bool m_bIsHorizontal;

	CSize m_BitmapSize;
};
