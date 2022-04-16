// ======================================================================================================
// File Name	: ColorListCtrlHeader.h /.cpp
// Class		: CColorListCtrlHeader
// Super Class	: CHeaderCtrl
// Last Update	: 2022.03.27 - Eskeptor
// ======================================================================================================

#pragma once

class CColorListCtrlHeader : public CHeaderCtrl
{
// Enums and Structure ==================================================================================
private:
	enum eDefaultData
	{
		Default_HeaderSize = 30,
		Default_BackColor = RGB(240, 240, 240),
		Default_ForeColor = RGB(0, 0, 0),
	};

// ======================================================================================================


// Variables ============================================================================================
private:
	CRect m_rtSize;			// Header Control Size

	bool m_bBorderEnable;	// Border Enabler
	int m_nHeaderSize;		// Header Height Size

	COLORREF m_clrBack;		// Background Color
	COLORREF m_clrFore;		// Foreground Color

public:
	DECLARE_DYNAMIC(CColorListCtrlHeader)
// ======================================================================================================


// Functions ============================================================================================
private:
	// Draw Header
	void DrawHeader(CDC* pDC);

protected:
	// HDM_LAYOUT Capture - Change Header Height 
	afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CColorListCtrlHeader();
	virtual ~CColorListCtrlHeader();

	// Set Header Background Color
	void SetHeaderBgColor(COLORREF clrBack);
	// Set Header Foreground Color
	void SetHeaderFrColor(COLORREF clrFore);
	// Set Header Border Enable
	void SetHeaderBorder(bool bIsEnable);
	// Set Header Height Size
	void SetHeaderSize(int nSize);

	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

// ======================================================================================================

};
