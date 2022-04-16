#include "pch.h"
#include "ColorPicka.h"
#include "ColorListCtrlHeader.h"


IMPLEMENT_DYNAMIC(CColorListCtrlHeader, CHeaderCtrl)

CColorListCtrlHeader::CColorListCtrlHeader()
    : m_bBorderEnable(true)
    , m_clrBack(eDefaultData::Default_BackColor)
    , m_clrFore(eDefaultData::Default_ForeColor)
    , m_nHeaderSize(eDefaultData::Default_HeaderSize)
{

}

CColorListCtrlHeader::~CColorListCtrlHeader()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrlHeader, CHeaderCtrl)
	ON_WM_PAINT()
    ON_MESSAGE(HDM_LAYOUT, &CColorListCtrlHeader::OnLayout)
END_MESSAGE_MAP()


/**
OnPaint
*/
void CColorListCtrlHeader::OnPaint()
{
	CPaintDC dc(this); 
	//CDC* pDC = &dc;

	//CDC memDC;
	//memDC.CreateCompatibleDC(pDC);

	//CBitmap bitmap;
	//bitmap.CreateCompatibleBitmap(pDC, m_rtSize.Width(), m_rtSize.Height());

	//CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);

	//memDC.PatBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), WHITENESS);

	DrawHeader(&dc);

	//pDC->BitBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), &memDC, 0, 0, SRCCOPY);

	//memDC.SelectObject(pOldBitmap);
	//memDC.DeleteDC();
	//bitmap.DeleteObject();
}


/**
Draw Header
@access		private
@param		pDC			DC
@return
*/
void CColorListCtrlHeader::DrawHeader(CDC* pDC)
{
    if (pDC != nullptr)
    {
        int nItemCount = GetItemCount();
        int nStart = 0;
        const int BUF_SIZE = 256;

        for (int i = 0; i < nItemCount; i++)
        {
            CRect rt;
            HDITEM item;

            memset(&item, 0, sizeof(item));

            TCHAR szBuffer[BUF_SIZE] = { 0, };

            item.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER | HDI_WIDTH;
            item.pszText = szBuffer;
            item.cchTextMax = BUF_SIZE - 1;

            GetItemRect(i, rt);
            GetItem(i, &item);

            rt.left = nStart;
            rt.right = rt.left + item.cxy;
            nStart = rt.right;

            // Background
            {
                if (m_bBorderEnable)
                {
                    pDC->Rectangle(rt);
                }

                CBrush brBack(m_clrBack);
                CRect rtFill(rt.left + 1, rt.top + 1, rt.right - 1, rt.bottom - 1);
                pDC->FillRect(rtFill, &brBack);
                brBack.DeleteObject();
            }

            // Text
            {
                CFont* pOldFont = pDC->SelectObject(GetFont());
                pDC->SetBkMode(TRANSPARENT);
                pDC->SetTextColor(m_clrFore);
                //pDC->MoveTo(rt.right - 1, rt.top);
                //pDC->LineTo(rt.right - 1, rt.bottom);
                if (item.pszText)
                {
                    CString strText = _T("");
                    strText.Format(_T(" %s"), item.pszText);
                    pDC->DrawText(strText, rt, DT_SINGLELINE | DT_VCENTER);
                }
                pDC->SelectObject(pOldFont);
            }
        }
    }
}


/**
PreCreateWindow
*/
BOOL CColorListCtrlHeader::PreCreateWindow(CREATESTRUCT& cs)
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	return CHeaderCtrl::PreCreateWindow(cs);
}


/**
PreSubclassWindow
*/
void CColorListCtrlHeader::PreSubclassWindow()
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	CHeaderCtrl::PreSubclassWindow();
}


/**
Set Header Background Color
@access     public
@param      clrBack     Background Color
@return
*/
void CColorListCtrlHeader::SetHeaderBgColor(COLORREF clrBack)
{
    m_clrBack = clrBack;

    Invalidate();
}


/**
Set Header Foreground Color
@access     public
@param      clrFore     Foreground Color
@return
*/
void CColorListCtrlHeader::SetHeaderFrColor(COLORREF clrFore)
{
    m_clrFore = clrFore;

    Invalidate();
}


/**
Set Header Border Enable
@access     public
@param      bIsEnable   Enable
@return
*/
void CColorListCtrlHeader::SetHeaderBorder(bool bIsEnable)
{
    m_bBorderEnable = bIsEnable;

    Invalidate();
}


/**
Set Header Height Size
@access     public
@param      nSize       Size
@return
*/
void CColorListCtrlHeader::SetHeaderSize(int nSize)
{
    m_nHeaderSize = nSize;

    Invalidate();
}


/**
Change Header Height
@event      HDM_LAYOUT
@param      wParam
@param      lParam
@return     
*/
LRESULT CColorListCtrlHeader::OnLayout(WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

    HD_LAYOUT& hdl = *(HD_LAYOUT*)lParam;
    RECT* pRect = hdl.prc;
    WINDOWPOS* pWinPos = hdl.pwpos;

    pWinPos->cy = m_nHeaderSize;
    pRect->top = m_nHeaderSize;

    return lResult;
}