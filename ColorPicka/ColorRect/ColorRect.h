// ======================================================================================================
// File Name	: ColorRect.h /.cpp
// Class		: CColorRect
// Super Class	: CStatic
// Last Update	: 2022.03.26 - Eskeptor
// ======================================================================================================

#pragma once

class CColorRect : public CStatic
{
// Enums and Structure ==================================================================================

// ======================================================================================================


// Variables ============================================================================================
private:
	COLORREF m_clrBack;					// Background Color
	COLORREF m_clrBorder;				// Border Color
	CRect m_rtSize;						// Control's Rect Size
	int m_nBorderThickness;				// Border Thickness

	bool m_bBorderEnable;				// Border Enabler

public:
	DECLARE_DYNAMIC(CColorRect)

// ======================================================================================================


// Functions ============================================================================================
private:

protected:
	DECLARE_MESSAGE_MAP()

public:
	CColorRect();
	virtual ~CColorRect();

	// Set Background Color
	void SetBackgroundColor(COLORREF clrBack);
	void SetBackgroundColor(int nR, int nG, int nB);
	// Set Border Color
	void SetBorderColor(COLORREF clrBorder);
	void SetBorderColor(int nR, int nG, int nB);

	// Set Border Enable
	void SetBorderEnable(bool bIsEnable, int nThickness = 1);

	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

// ======================================================================================================

	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


