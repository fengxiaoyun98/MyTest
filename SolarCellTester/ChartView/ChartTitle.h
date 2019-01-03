/*
 *
 *	ChartTitle.h
 *
 *	Written by CÈdric Moonen (cedric_moonen@hotmail.com)
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

#include <vector>
#include <string>

//±ÍÃ‚¿∏

class CChartTitle : public CChartObject  
{
	friend CChartView;

public:
	size_t GetStringCount() const;
	CString GetString(size_t Index) const;
	void AddString(CString NewString);
	void RemoveAll();

	void SetFont(int iPointSize,CString strFaceName);

	CChartTitle(CChartView* pParent);
	virtual ~CChartTitle();

private:
	CSize GetSize(CDC* pDC);
	void Draw(CDC *pDC);

	std::vector<CString> m_StringArray;

	CString m_strFontName;
	int         m_iFontSize;
};

