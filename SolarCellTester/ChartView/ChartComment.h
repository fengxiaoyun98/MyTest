/*
��̬���ע�ͣ���ʾһЩ��ͼ���йص�������Ϣ��
�ֵ�������Ϣ�������б���Ʒ��Ϣ��
*/

#pragma once

#include "ChartObject.h"

#include <string>


class CChartComment : public CChartObject  
{
	friend CChartView;

public:
	void SetFont(int iPointSize,CString strFaceName);

	CChartComment(CChartView* pParent);
	virtual ~CChartComment();

	void SetTransparent(bool bTransparent);  
	void SetComment(CString str);
	void SetPosStyle(int mode);
private:
	int m_mode;
	void SetRect(void);
	void Draw(CDC* pDC);
	void ClipArea(CRect& rcControl, CDC* pDC);

	CString m_comet;
	CRect m_rectpos;

	CString m_strFontName;
	int         m_iFontSize;

	bool m_bDocked;	// true if the legend is docked

	// If the legend is not docked:
	int m_iLeftPos;
	int m_iTopPos;

	bool m_bIsTransparent;
	bool m_bIsHorizontal;

	CSize m_BitmapSize;
};
