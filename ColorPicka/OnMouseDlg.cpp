#include "pch.h"
#include "ColorPicka.h"
#include "afxdialogex.h"
#include "OnMouseDlg.h"


IMPLEMENT_DYNAMIC(COnMouseDlg, CDialog)

COnMouseDlg::COnMouseDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ON_MOUSE, pParent)
	, m_clrBack(RGB(240, 240, 240))
{

}


COnMouseDlg::~COnMouseDlg()
{
}


void COnMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COnMouseDlg, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(eEvent::eEVT_ChangeColor, &COnMouseDlg::EvtChangeColor)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/**
OnDestroy()
*/
void COnMouseDlg::OnDestroy()
{
	CDialog::OnDestroy();
}


/**
Change Color Event
@event		eEvent::eEVT_ChangeColor
@param		lParam
@return
*/
LRESULT COnMouseDlg::EvtChangeColor(WPARAM, LPARAM lParam)
{
	m_clrBack = (COLORREF)lParam;
	Invalidate();

	return 1L;
}


/**
OnEraseBkgnd()
*/
BOOL COnMouseDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	pDC->FillSolidRect(rect, RGB(0, 0, 0));

	rect.top += 2;
	rect.left += 2;
	rect.right -= 2;
	rect.bottom -= 2;
	pDC->FillSolidRect(rect, m_clrBack);

	CString strText = _T("");
	strText.Format(_T(" R: %d"), GetRValue(m_clrBack));
	m_pCtrlStaticR->SetWindowText(strText);

	strText.Format(_T(" G: %d"), GetGValue(m_clrBack));
	m_pCtrlStaticG->SetWindowText(strText);

	strText.Format(_T(" B: %d"), GetBValue(m_clrBack));
	m_pCtrlStaticB->SetWindowText(strText);

	strText.Empty();
	return TRUE;
}


/**
OnInitDialog
*/
BOOL COnMouseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pCtrlStaticR = (CStatic*)GetDlgItem(IDC_ON_MOUSE_STATIC_R);
	m_pCtrlStaticG = (CStatic*)GetDlgItem(IDC_ON_MOUSE_STATIC_G);
	m_pCtrlStaticB = (CStatic*)GetDlgItem(IDC_ON_MOUSE_STATIC_B);

	return TRUE;  
}


/**
Set Text Visible
@access		public
@param		bVisible	Text Visible
@return
*/
void COnMouseDlg::SetTextVisible(bool bVisible)
{
	m_pCtrlStaticR->ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
	m_pCtrlStaticG->ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
	m_pCtrlStaticB->ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
}