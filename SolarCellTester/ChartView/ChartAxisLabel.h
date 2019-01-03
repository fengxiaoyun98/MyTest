/*
 *
 *	ChartAxisLabel.h
 *
 *	Written by Cédric Moonen (cedric_moonen@hotmail.com)
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



class CChartAxis;

class CChartAxisLabel : public CChartObject  
{
	friend CChartAxis;

public:
	void SetText(CString NewText);
	CString GetText() const        { return m_strLabelText;    }

	void SetFont(int nPointSize,CString strFaceName);

	CChartAxisLabel(CChartView* pParent, bool bHorizontal);
	virtual ~CChartAxisLabel();

private:
	void SetPosition(int LeftBorder, int TopBorder, CDC *pDC);
	void Draw(CDC* pDC);
	CSize GetSize(CDC* pDC) const;

	bool m_bIsHorizontal;      // Specifies if the axis is horizontal or not
	int  m_iFontSize;
	CString m_strFontName;
	CString m_strLabelText;
};

