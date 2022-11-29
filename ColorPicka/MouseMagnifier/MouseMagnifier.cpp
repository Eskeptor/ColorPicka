#include "pch.h"
#include "ColorPicka.h"
#include "MouseMagnifier.h"


IMPLEMENT_DYNAMIC(CMouseMagnifier, CStatic)

CMouseMagnifier::CMouseMagnifier()
	: m_dZoom((double)eDefaultData::Default_Zoom)
	, m_bIsCrossDraw(true)
	, m_nActViewWidth(eDefaultData::Default_ViewWidth), m_nActViewHeight(eDefaultData::Default_ViewHeight)
	, m_ptMouse {0, 0}
	, m_clrCrossColor(RGB(Default_CrossClrRed, Default_CrossClrGreen, Default_CrossClrBlue))
	, m_nEvtSendColorID(0)
	, m_clrCurCursorPosColor(0)
	, m_bIsIncludeWindow(true)
	, m_bCapture(true)
{

}

CMouseMagnifier::~CMouseMagnifier()
{
	
}


BEGIN_MESSAGE_MAP(CMouseMagnifier, CStatic)
	ON_WM_PAINT()
	ON_MESSAGE(eMagEvent::eEVT_GetColor, &CMouseMagnifier::EvtGetColor)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/**
OnPaint
*/
void CMouseMagnifier::OnPaint()
{
	CPaintDC dc(this);
	CDC* pDC = &dc;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, m_rtSize.Width(), m_rtSize.Height());

	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);

	memDC.PatBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), WHITENESS);

	DrawScreen(&memDC);
	if (m_bIsCrossDraw)
		DrawCross(&memDC);

	pDC->BitBlt(0, 0, m_rtSize.Width(), m_rtSize.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


/**
Draw Cross Line
@access		private
@param		pDC			CDC
@return
*/
void CMouseMagnifier::DrawCross(CDC* pDC)
{
	if (pDC != nullptr)
	{
		int nHalfWidth = m_rtSize.Width() / 2;
		int nHalfHeight = m_rtSize.Height() / 2;

		CPen pnLine(PS_SOLID, eDefaultData::Default_LineThickness, m_clrCrossColor);
		CPen* pnOld = pDC->SelectObject(&pnLine);

		pDC->MoveTo(0, nHalfHeight);
		pDC->LineTo(m_rtSize.Width(), nHalfHeight);
		pDC->MoveTo(nHalfWidth, 0);
		pDC->LineTo(nHalfWidth, m_rtSize.Height());
		pDC->SelectObject(pnOld);

		pnLine.DeleteObject();
	}
}


/**
Draw Zoom Screen
@access		private
@param		pDC			CDC
@return
*/
void CMouseMagnifier::DrawScreen(CDC* pDC)
{
	if (pDC != nullptr)
	{
		CWnd* pWin = GetDesktopWindow();
		CWindowDC winDC(pWin);

		pDC->Rectangle(m_rtSize);

		// Color of Current Mouse Position
		m_clrCurCursorPosColor = winDC.GetPixel(m_ptMouse.x, m_ptMouse.y);

		int nLineThick = eDefaultData::Default_LineThickness;
		int nHalfWidth = m_nActViewWidth / 2;
		int nHalfHeight = m_nActViewHeight / 2;
		pDC->StretchBlt(nLineThick, 
			nLineThick, 
			m_rtSize.Width() - (nLineThick * 2), 
			m_rtSize.Height() - (nLineThick * 2), 
			&winDC, 
			m_ptMouse.x - nHalfWidth, 
			m_ptMouse.y - nHalfHeight, 
			m_nActViewWidth, 
			m_nActViewHeight, 
			SRCCOPY);
	}
}


/**
Set Cross Line Draw Enable
@access		public
@param		bEnable			Draw Enable
@return
*/
void CMouseMagnifier::SetDrawCross(bool bEnable)
{
	m_bIsCrossDraw = bEnable;
}


/**
Set Cross Line Color
@access		public
@param		clrCross		Cross Line Color
@return
*/
void CMouseMagnifier::SetCrossColor(COLORREF clrCross)
{
	m_clrCrossColor = clrCross;
}


/**
PreCreateWindow
*/
BOOL CMouseMagnifier::PreCreateWindow(CREATESTRUCT& cs)
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	if (m_stRefreshThread.thrRefresh == nullptr)
	{
		m_stRefreshThread.thrRefresh = AfxBeginThread(ThreadMousePosition, this);

		if (m_stRefreshThread.thrRefresh == nullptr)
		{
			MessageBox(_T("Refresh Thread Create Fail"), _T("Error"));
		}
	}

	return CStatic::PreCreateWindow(cs);
}


/**
PreSubclassWindow
*/
void CMouseMagnifier::PreSubclassWindow()
{
	GetWindowRect(&m_rtSize);
	ScreenToClient(&m_rtSize);

	if (m_stRefreshThread.thrRefresh == nullptr)
	{
		m_stRefreshThread.thrRefresh = AfxBeginThread(ThreadMousePosition, this);

		if (m_stRefreshThread.thrRefresh == nullptr)
		{
			MessageBox(_T("Refresh Thread Create Fail"), _T("Error"));
		}
	}

	CStatic::PreSubclassWindow();
}


/**
m_pThrRefresh Function
*/
UINT CMouseMagnifier::ThreadMousePosition(LPVOID pParam)
{
	CMouseMagnifier* pMain = (CMouseMagnifier*)pParam;

	if (pMain != nullptr)
	{
		POINT ptMouse;
		CRect rtCurWindow;
		bool bIsIncludeWindow = false;
		while (pMain->m_stRefreshThread.bExitFlag == false &&
			   pMain != nullptr)
		{
			pMain->GetParent()->GetWindowRect(&rtCurWindow);
			GetCursorPos(&ptMouse);

			if (pMain->m_bIsIncludeWindow)
			{
				bIsIncludeWindow = true;
			}
			else
			{
				if (ptMouse.x < rtCurWindow.left ||
					ptMouse.x > rtCurWindow.right ||
					ptMouse.y < rtCurWindow.top ||
					ptMouse.y > rtCurWindow.bottom)
				{
					bIsIncludeWindow = true;
				}
				else
				{
					bIsIncludeWindow = false;
				}
			}

			if (bIsIncludeWindow &&
				pMain->m_bCapture)
			{
				if (ptMouse.x != pMain->m_ptMouse.x ||
					ptMouse.y != pMain->m_ptMouse.y)
				{
					pMain->m_ptMouse.x = ptMouse.x;
					pMain->m_ptMouse.y = ptMouse.y;
					pMain->Invalidate();
				}
			}

			Sleep(1);
		}

		pMain->m_stRefreshThread.thrRefresh = nullptr;
	}

	return 0;
}


/**
Get Color Event
@event		eMagEvent::eEVT_GetColor
@param		wParam		Not Used
@param		lParam		Not Used
@return		1
*/
LRESULT CMouseMagnifier::EvtGetColor(WPARAM, LPARAM)
{
	if (m_nEvtSendColorID > 0)
	{
		::PostMessage(GetParent()->m_hWnd, m_nEvtSendColorID, 0, m_clrCurCursorPosColor);
	}

	return 1L;
}


/**
Set Send Color Event ID To Parent
@access		public
@param		nID			Event ID
@return
*/
void CMouseMagnifier::SetEventIDSendColor(UINT nID)
{
	m_nEvtSendColorID = nID;
}


/**
OnDestroy
*/
void CMouseMagnifier::OnDestroy()
{
	CStatic::OnDestroy();

	if (m_stRefreshThread.thrRefresh != nullptr)
	{
		m_stRefreshThread.bExitFlag = true;
		while (m_stRefreshThread.thrRefresh != nullptr)
			Sleep(1);
	}
}


/**
Set Window Include Option
@access		public
@param		bInclude		Window Include
@return
*/
void CMouseMagnifier::SetIncludeWindow(bool bInclude)
{
	m_bIsIncludeWindow = bInclude;
}


/**
Start Mouse Capture
@access		public
@param
@return
*/
void CMouseMagnifier::StartCapture()
{
	m_bCapture = true;
}


/**
Stop Mouse Capture
@access		public
@param
@return
*/
void CMouseMagnifier::StopCapture()
{
	m_bCapture = false;
}