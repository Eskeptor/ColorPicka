#include "pch.h"
#include "ColorPicka.h"
#include "ColorRect.h"


IMPLEMENT_DYNAMIC(CColorRect, CStatic)

CColorRect::CColorRect()
	: m_clrBack(RGB(240, 240, 240))
	, m_clrBorder(RGB(0, 0, 0))
	, m_bBorderEnable(true)
	, m_nBorderThickness(1)
{

}

CColorRect::~CColorRect()
{
}


BEGIN_MESSAGE_MAP(CColorRect, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


/**
OnPaint
*/
void CColorRect::OnPaint()
{
	CPaintDC dc(this); 				   
	CDC* pDC = &dc;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, m_rtSize.Width(), m_rtSize.Height());

	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);

	memDC.PatBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), WHITENESS);

	// Draw Border
	if (m_bBorderEnable)
	{
		CPen pnBorder(PS_SOLID, m_nBorderThickness, m_clrBorder);
		CPen* pnOld = memDC.SelectObject(&pnBorder);
		int nThicknessHalf = m_nBorderThickness / 2;
		memDC.Rectangle(m_rtSize.left + nThicknessHalf, m_rtSize.top + nThicknessHalf, m_rtSize.right - nThicknessHalf, m_rtSize.bottom - nThicknessHalf);
		//memDC.Rectangle(m_rtSize);
		memDC.SelectObject(pnOld);
		pnBorder.DeleteObject();
	}
	// Draw Background
	{
		CBrush brBack(m_clrBack);
		CRect rtRect(m_rtSize.left + m_nBorderThickness, m_rtSize.top + m_nBorderThickness, m_rtSize.right - m_nBorderThickness, m_rtSize.bottom - m_nBorderThickness);
		memDC.FillRect(rtRect, &brBack);
		
		brBack.DeleteObject();
	}

	pDC->BitBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


/**
Set Background Color
@access		public
@param		clrBack		Set Color
@return
*/
void CColorRect::SetBackgroundColor(COLORREF clrBack)
{
	m_clrBack = clrBack;

	Invalidate();
}


/**
Set Background Color
@access		public
@param		nR			Red
@param		nG			Green
@param		nB			Blue
@return
*/
void CColorRect::SetBackgroundColor(int nR, int nG, int nB)
{
	if (nR >= 0 && nR <= 255 &&
		nG >= 0 && nG <= 255 &&
		nB >= 0 && nB <= 255)
	{
		m_clrBack = RGB(nR, nG, nB);

		Invalidate();
	}
}


/**
Set Border Color
@access		public
@param		clrBack		Set Color
@return
*/
void CColorRect::SetBorderColor(COLORREF clrBorder)
{
	m_clrBorder = clrBorder;

	Invalidate();
}


/**
Set Border Color
@access		public
@param		nR			Red
@param		nG			Green
@param		nB			Blue
@return
*/
void CColorRect::SetBorderColor(int nR, int nG, int nB)
{
	if (nR >= 0 && nR <= 255 &&
		nG >= 0 && nG <= 255 &&
		nB >= 0 && nB <= 255)
	{
		m_clrBorder = RGB(nR, nG, nB);

		Invalidate();
	}
}


/**
Set Border Enable
@access		public
@param		bIsEnable	Enable
@return
*/
void CColorRect::SetBorderEnable(bool bIsEnable, int nThickness)
{
	m_bBorderEnable = bIsEnable;
	m_nBorderThickness = nThickness > 0 ? nThickness : 1;

	Invalidate();
}


/**
PreCreateWindow
*/
BOOL CColorRect::PreCreateWindow(CREATESTRUCT& cs)
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	return CStatic::PreCreateWindow(cs);
}


/**
PreSubclassWindow
*/
void CColorRect::PreSubclassWindow()
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	CStatic::PreSubclassWindow();
}


/**
OnSize
*/
void CColorRect::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	Invalidate();
}
