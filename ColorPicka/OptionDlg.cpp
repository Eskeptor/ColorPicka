#include "pch.h"
#include "ColorPicka.h"
#include "afxdialogex.h"
#include "OptionDlg.h"


IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_OPTIONS, pParent)
{

}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PGRID_OPTION, m_ctrlGrid);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COptionDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


/**
PreTranslateMessage
*/
BOOL COptionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


/**
OnInitDialog
*/
BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();

	return TRUE; 
}


/**
Controls Initialize
@access		private
@param
@return
*/
void COptionDlg::InitControls()
{
	// BBungGrid
	{
		CBBungGrid::stProperty GRID_STUFF[] =
		{
			P_ROOT(_T("TEXT:Options;")),					
				P_SUBROOT(_T("TEXT:Magnifier;")),			
					P_VAR(g_stOptions.stOpMag.bIsIncludeWindow, _T("TEXT:Include Window;")),
					P_VAR(g_stOptions.stOpMag.bIsDrawCross, _T("TEXT:Draw Cross;")),
				P_SUBROOT_END(),							
				P_SUBROOT(_T("TEXT:Log Color List;")),				
					P_VAR(g_stOptions.stOpList.bIsUseList, _T("TEXT:Use;")),
					P_VAR(g_stOptions.stOpList.nLogMax, _T("TEXT:Log Max;")),
					P_VAR(g_stOptions.stOpList.bIsSaveLog, _T("TEXT:Log Save;")),
				P_SUBROOT_END(),							
			P_ROOT_END(),									
		};
		int nSize = sizeof(GRID_STUFF) / sizeof(CBBungGrid::stProperty);

		m_ctrlGrid.Initial();
		m_ctrlGrid.AppendProperty(GRID_STUFF, nSize);
	}
}


/**
Button Click : Save
@access		public
@param
@return
*/
void COptionDlg::OnBnClickedBtnSave()
{
	m_ctrlGrid.SavePropertyData();
	m_ctrlGrid.DataRefresh(true);

	OnOK();
}
