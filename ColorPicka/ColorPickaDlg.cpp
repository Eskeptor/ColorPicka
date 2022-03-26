#include "pch.h"
#include "framework.h"
#include "ColorPicka.h"
#include "ColorPickaDlg.h"
#include "afxdialogex.h"
#include "CppUtil/CppUtil.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


CColorPickaDlg::CColorPickaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLORPICKA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CColorPickaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColorPickaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RGB_TO_HEX, &CColorPickaDlg::OnBnClickedBtnRgbToHex)
	ON_BN_CLICKED(IDC_BTN_HEX_TO_RGB, &CColorPickaDlg::OnBnClickedBtnHexToRgb)
	ON_BN_CLICKED(IDC_BTN_HSV_TO_HEX, &CColorPickaDlg::OnBnClickedBtnHsvToHex)
	ON_BN_CLICKED(IDC_BTN_HEX_TO_HSV, &CColorPickaDlg::OnBnClickedBtnHexToHsv)
	ON_BN_CLICKED(IDC_BTN_HSV_TO_RGB, &CColorPickaDlg::OnBnClickedBtnHsvToRgb)
	ON_BN_CLICKED(IDC_BTN_RGB_TO_HSV, &CColorPickaDlg::OnBnClickedBtnRgbToHsv)
	ON_STN_CLICKED(IDC_COLOR_HEX_1, &CColorPickaDlg::OnStnClickedColorHex1)
	ON_STN_CLICKED(IDC_COLOR_RGB_1, &CColorPickaDlg::OnStnClickedColorRgb1)
	ON_STN_CLICKED(IDC_COLOR_HEX_2, &CColorPickaDlg::OnStnClickedColorHex2)
	ON_STN_CLICKED(IDC_COLOR_HSV_1, &CColorPickaDlg::OnStnClickedColorHsv1)
	ON_STN_CLICKED(IDC_COLOR_RGB_2, &CColorPickaDlg::OnStnClickedColorRgb2)
	ON_STN_CLICKED(IDC_COLOR_HSV_2, &CColorPickaDlg::OnStnClickedColorHsv2)
	ON_MESSAGE(eEvent::eEVT_SetCurMousePosColor, &CColorPickaDlg::EvtSetCurMousePosColor)
	ON_MESSAGE(eEvent::eEVT_KeyCapture, &CColorPickaDlg::EvtKeyCapture)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CColorPickaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitControls();
	SetKeyboardHook();

	return TRUE;  
}


void CColorPickaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		// Color Rect
		//{
		//	CRect rect;
		//	for (int i = 0; i < eColorRectIdx::ColorRectIdxMax; i++)
		//	{
		//		m_arrRect[i].ctlRect.GetClientRect(&rect);
		//		CBrush brBack(RGB(m_arrRect[i].rgbColor.nR, m_arrRect[i].rgbColor.nG, m_arrRect[i].rgbColor.nB));

		//		CDC* pDC = m_arrRect[i].ctlRect.GetWindowDC();
		//		CBrush* pBrOld = pDC->SelectObject(&brBack);
		//		pDC->PatBlt(1, 1, rect.Width() - 2, rect.Height() - 2, PATCOPY);
		//		pDC->SelectObject(pBrOld);
		//		brBack.DeleteObject();
		//	}
		//}
	}
}


HCURSOR CColorPickaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
Controls Initialize
@access		private
@param
@return
*/
void CColorPickaDlg::InitControls()
{
	// Global
	{
		g_hwndMain = m_hWnd;
	}

	// Main Window
	{
		// Title
		{
			CString strVersion = _T("");
			strVersion.Format(_T("ColorPicka %s (Made by Eskeptor)"), GetVersion().GetString());
			SetWindowText(strVersion);
		}

#ifndef _USE_LIST
		// Window Size
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		MoveWindow(0, 0, 454, rcWindow.Height());
#endif
	}

	// Label
	{
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoR], IDC_LBL_COLOR_INFO_RED);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoG], IDC_LBL_COLOR_INFO_GREEN);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoB], IDC_LBL_COLOR_INFO_BLUE);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoH], IDC_LBL_COLOR_INFO_H);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoS], IDC_LBL_COLOR_INFO_S);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoV], IDC_LBL_COLOR_INFO_V);
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoHex], IDC_LBL_COLOR_INFO_HEX);
	}

	// Edit
	{
		InitEdit(&m_arrEdit[eEditIdx::Edit_Red1], IDC_EDIT_H_R_RED, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Red2], IDC_EDIT_R_H_RED, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Green1], IDC_EDIT_H_R_GREEN, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Green2], IDC_EDIT_R_H_GREEN, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Blue1], IDC_EDIT_H_R_BLUE, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Blue2], IDC_EDIT_R_H_BLUE, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hex1], IDC_EDIT_H_R_HEX, false);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hex2], IDC_EDIT_H_H_HEX, false);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hue1], IDC_EDIT_H_H_H, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hue2], IDC_EDIT_R_H_H, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Saturation1], IDC_EDIT_H_H_S, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Saturation2], IDC_EDIT_R_H_S, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Value1], IDC_EDIT_H_H_V, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Value2], IDC_EDIT_R_H_V, true);
	}

	// Color Rect
	{
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_Hex1], IDC_COLOR_HEX_1);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_Hex2], IDC_COLOR_HEX_2);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_RGB1], IDC_COLOR_RGB_1);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_RGB2], IDC_COLOR_RGB_2);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_HSV1], IDC_COLOR_HSV_1);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_HSV2], IDC_COLOR_HSV_2);
		InitColorRect(&m_arrRect[eColorRectIdx::Rect_PickColor], IDC_PIC_MOUSE_COLOR);
	}

	// Mouse Magnifier
	{
		m_ctrlMag.SubclassDlgItem(IDC_PIC_MOUSE_VIEW, this);
		m_ctrlMag.SetEventIDSendColor(eEvent::eEVT_SetCurMousePosColor);
	}

	// List Control
	{
		m_ctrlList.SubclassDlgItem(IDC_LIST_COLOR, this);

#ifndef _USE_LIST
		m_ctrlList.ShowWindow(SW_HIDE);
#endif
	}
}


/**
Label Initialize
@access		private
@param		pCtlLabel		Label Control(CStatic)
@param		nID				Label's ID
@param		strText			Label's Text
@return
*/
void CColorPickaDlg::InitLabel(CStatic* pCtlLabel, int nID, CString strText)
{
	if (pCtlLabel != nullptr &&
		nID > 0)
	{
		pCtlLabel->SubclassDlgItem(nID, this);
		pCtlLabel->SetWindowText(strText);
	}
}


/**
Edit Initialize
@access		private
@param		pCtlEdit		Edit Control(CEdit)
@param		nID				Edit's ID
@param		bIsNumber		Edit Option(Number)
@param		strText			Edit Text
@return
*/
void CColorPickaDlg::InitEdit(CEdit* pCtlEdit, int nID, bool bIsNumber, CString strText)
{
	if (pCtlEdit != nullptr &&
		nID > 0)
	{
		pCtlEdit->SubclassDlgItem(nID, this);
		pCtlEdit->SetWindowText(strText);
		LONG lStyle = GetWindowLong(pCtlEdit->m_hWnd, GWL_STYLE);
		if (bIsNumber)
			SetWindowLong(pCtlEdit->m_hWnd, GWL_STYLE, lStyle | ES_NUMBER);
		else
			SetWindowLong(pCtlEdit->m_hWnd, GWL_STYLE, lStyle & ~ES_NUMBER);
	}
}


/**
ColorRect Initialize
@access		private
@param		pCtlRect		Color Rect Control
@param		nID				Color Rect's ID
@param		nR				Default Color (Red)
@param		nG				Default Color (Green)
@param		nB				Default Color (Blue)
@return
*/
void CColorPickaDlg::InitColorRect(CColorRect* pCtlRect, int nID, int nR, int nG, int nB)
{
	if (pCtlRect != nullptr &&
		nID > 0)
	{
		pCtlRect->SubclassDlgItem(nID, this);
		pCtlRect->SetBackgroundColor(nR, nG, nB);
	}
}


/**
Get Version
@access		private
@param
@return		Version (ex: 1.0.0.0)
*/
CString CColorPickaDlg::GetVersion()
{
	CString strVersion = _T("");

	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hRsrc != NULL)
	{
		HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);
		if (hGlobalMemory != NULL)
		{
			CString strOrgVersion = _T("");
			LPVOID pVersionResouece = LockResource(hGlobalMemory);
			LPVOID pVersion = NULL;
			DWORD dwLength = 0;
			DWORD dwLang = 0;
			LPCTSTR lpSubBlock = _T("\\VarFileInfo\\Translation");

			BOOL bReturn = VerQueryValue(pVersionResouece, lpSubBlock, (LPVOID*)&pVersion, (UINT*)&dwLength);
			if (bReturn && 
				dwLength == 4) 
			{
				memcpy(&dwLang, pVersion, 4);
				strOrgVersion.Format(_T("\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion"),
					(dwLang & 0xff00) >> 8, dwLang & 0xff, (dwLang & 0xff000000) >> 24,
					(dwLang & 0xff0000) >> 16);
			}
			else 
			{
				strOrgVersion.Format(_T("\\StringFileInfo\\%04X04B0\\FileVersion"), GetUserDefaultLangID());
			}

			if (VerQueryValue(pVersionResouece, (LPCTSTR)strOrgVersion, (LPVOID*)&pVersion, (UINT*)&dwLength) != 0)
			{
				strVersion.Format(_T("%s"), (LPCTSTR)pVersion);
			}

			FreeResource(hGlobalMemory);
		}
	}

	return strVersion;
}


/**
Set Color Rect's Color
@access		private
@param		pCtlRect		Rect
@param		rgbColor		RGB Color
@return
*/
void CColorPickaDlg::SetRectColor(CColorRect* pCtlRect, stRGB rgbColor)
{
	if (pCtlRect != nullptr &&
		pCtlRect->m_hWnd != NULL)
	{
		pCtlRect->SetBackgroundColor(rgbColor.nR, rgbColor.nG, rgbColor.nB);
	}
}


/**
Set Color Rect's Color
@access		private
@param		pCtlRect		Rect
@param		hsvColor		HSV Color
@return
*/
void CColorPickaDlg::SetRectColor(CColorRect* pCtlRect, stHSV hsvColor)
{
	if (pCtlRect != nullptr &&
		pCtlRect->m_hWnd != NULL)
	{
		stRGB rgbColor;
		CppUtil::HSVToRGB(hsvColor.nH, hsvColor.nS, hsvColor.nV, rgbColor.nR, rgbColor.nG, rgbColor.nB);
		pCtlRect->SetBackgroundColor(rgbColor.nR, rgbColor.nG, rgbColor.nB);
	}
}


/**
Set Color Rect's Color
@access		private
@param		pCtlRect		Rect
@param		strHexColor		Hex Color
@return
*/
void CColorPickaDlg::SetRectColor(CColorRect* pCtlRect, CString strHexColor)
{
	if (pCtlRect != nullptr &&
		pCtlRect->m_hWnd != NULL)
	{
		stRGB rgbColor;
		CppUtil::HexToRGB(strHexColor, rgbColor.nR, rgbColor.nG, rgbColor.nB);
		pCtlRect->SetBackgroundColor(rgbColor.nR, rgbColor.nG, rgbColor.nB);
	}
}


/**
Button Click : RGB To Hex Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnRgbToHex()
{
	CString strRed = _T("");
	CString strGreen = _T("");
	CString strBlue = _T("");

	m_arrEdit[eEditIdx::Edit_Red1].GetWindowText(strRed);
	m_arrEdit[eEditIdx::Edit_Green1].GetWindowText(strGreen);
	m_arrEdit[eEditIdx::Edit_Blue1].GetWindowText(strBlue);

	stRGB rgbData;	
	rgbData.nR = _ttoi(strRed);
	rgbData.nG = _ttoi(strGreen);
	rgbData.nB = _ttoi(strBlue);

	CString strHex = _T("");
	strHex.Format(_T("#%X"), CppUtil::RGBToHex(rgbData.nR, rgbData.nG, rgbData.nB));
	m_arrEdit[eEditIdx::Edit_Hex1].SetWindowTextW(strHex);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_Hex1], strHex);
}


/**
Button Click : Hex To RGB Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnHexToRgb()
{
	CString strHex = _T("");
	m_arrEdit[eEditIdx::Edit_Hex1].GetWindowText(strHex);

	strHex = strHex.Trim();
	strHex = strHex.Mid(strHex.ReverseFind(_T('#')) + 1);
	int nHex = CppUtil::HexToDec(strHex);

	stRGB rgbData;
	CppUtil::HexToRGB(nHex, rgbData.nR, rgbData.nG, rgbData.nB);

	CString strData = _T("");
	strData.Format(_T("%d"), rgbData.nR);
	m_arrEdit[eEditIdx::Edit_Red1].SetWindowText(strData);

	strData.Format(_T("%d"), rgbData.nG);
	m_arrEdit[eEditIdx::Edit_Green1].SetWindowText(strData);

	strData.Format(_T("%d"), rgbData.nB);
	m_arrEdit[eEditIdx::Edit_Blue1].SetWindowText(strData);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_RGB1], rgbData);
}


/**
Button Click : HSV To Hex Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnHsvToHex()
{
	CString strHue = _T("");
	CString strSaturation = _T("");
	CString strValue = _T("");

	m_arrEdit[eEditIdx::Edit_Hue1].GetWindowText(strHue);
	m_arrEdit[eEditIdx::Edit_Saturation1].GetWindowText(strSaturation);
	m_arrEdit[eEditIdx::Edit_Value1].GetWindowText(strValue);

	stHSV hsvData;
	hsvData.nH = _ttoi(strHue);
	hsvData.nS = _ttoi(strSaturation);
	hsvData.nV = _ttoi(strValue);

	CString strHex = _T("");
	strHex.Format(_T("#%X"), CppUtil::HSVToHex(hsvData.nH, hsvData.nS, hsvData.nV));

	m_arrEdit[eEditIdx::Edit_Hex2].SetWindowText(strHex);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_Hex2], strHex);
}


/**
Button Click : Hex To HSV Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnHexToHsv()
{
	CString strHex = _T("");
	m_arrEdit[eEditIdx::Edit_Hex2].GetWindowText(strHex);

	strHex = strHex.Trim();
	strHex = strHex.Mid(strHex.ReverseFind(_T('#')) + 1);
	int nHex = CppUtil::HexToDec(strHex);
	stHSV hsvData;

	CppUtil::HexToHSV(nHex, hsvData.nH, hsvData.nS, hsvData.nV);

	CString strData = _T("");
	strData.Format(_T("%d"), hsvData.nH);
	m_arrEdit[eEditIdx::Edit_Hue1].SetWindowText(strData);

	strData.Format(_T("%d"), hsvData.nS);
	m_arrEdit[eEditIdx::Edit_Saturation1].SetWindowText(strData);

	strData.Format(_T("%d"), hsvData.nV);
	m_arrEdit[eEditIdx::Edit_Value1].SetWindowText(strData);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_HSV1], hsvData);
}


/**
Button Click : HSV To RGB Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnHsvToRgb()
{
	CString strHue = _T("");
	CString strSaturation = _T("");
	CString strValue = _T("");

	m_arrEdit[eEditIdx::Edit_Hue2].GetWindowText(strHue);
	m_arrEdit[eEditIdx::Edit_Saturation2].GetWindowText(strSaturation);
	m_arrEdit[eEditIdx::Edit_Value2].GetWindowText(strValue);

	stHSV hsvData;
	hsvData.nH = _ttoi(strHue);
	hsvData.nS = _ttoi(strSaturation);
	hsvData.nV = _ttoi(strValue);
	stRGB rgbData;

	CppUtil::HSVToRGB(hsvData.nH, hsvData.nS, hsvData.nV, rgbData.nR, rgbData.nG, rgbData.nB);

	CString strData = _T("");
	strData.Format(_T("%d"), rgbData.nR);
	m_arrEdit[eEditIdx::Edit_Red2].SetWindowText(strData);

	strData.Format(_T("%d"), rgbData.nG);
	m_arrEdit[eEditIdx::Edit_Green2].SetWindowText(strData);

	strData.Format(_T("%d"), rgbData.nB);
	m_arrEdit[eEditIdx::Edit_Blue2].SetWindowText(strData);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_RGB2], rgbData);
}


/**
Button Click : RGB To HSV Convert
@access		public
@param
@return
*/
void CColorPickaDlg::OnBnClickedBtnRgbToHsv()
{
	CString strRed = _T("");
	CString strGreen = _T("");
	CString strBlue = _T("");

	m_arrEdit[eEditIdx::Edit_Red2].GetWindowText(strRed);
	m_arrEdit[eEditIdx::Edit_Green2].GetWindowText(strGreen);
	m_arrEdit[eEditIdx::Edit_Blue2].GetWindowText(strBlue);

	stRGB rgbData;
	rgbData.nR = _ttoi(strRed);
	rgbData.nG = _ttoi(strGreen);
	rgbData.nB = _ttoi(strBlue);
	stHSV hsvData;

	CppUtil::RGBToHSV(rgbData.nR, rgbData.nG, rgbData.nB, hsvData.nH, hsvData.nS, hsvData.nV);

	CString strData = _T("");
	strData.Format(_T("%d"), hsvData.nH);
	m_arrEdit[eEditIdx::Edit_Hue2].SetWindowText(strData);

	strData.Format(_T("%d"), hsvData.nS);
	m_arrEdit[eEditIdx::Edit_Saturation2].SetWindowText(strData);

	strData.Format(_T("%d"), hsvData.nV);
	m_arrEdit[eEditIdx::Edit_Value2].SetWindowText(strData);

	SetRectColor(&m_arrRect[eColorRectIdx::Rect_HSV2], hsvData);
}


/**
Click : Hex1 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHex1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Click : RGB1 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorRgb1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Click : Hex2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHex2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Click : HSV1 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHsv1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Click : RGB2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorRgb2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Click : HSV2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHsv2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
PreTranslateMessage
*/
BOOL CColorPickaDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialogEx::PreTranslateMessage(pMsg);
}


/**
Set Color Of Current Mouse Position
@event		eEvent::eEVT_SetCurMousePosColor
@param		wParam		Not Use
@param		lParam		Color
@return		1L
*/
LRESULT CColorPickaDlg::EvtSetCurMousePosColor(WPARAM, LPARAM lParam)
{
	COLORREF clrColor = (COLORREF)lParam;

	SetColorBox(clrColor);

	return 1L;
}


/**
Key Capture Event
@event		eEvent::eEVT_KeyCapture
@param		wParam		Not Use
@param		lParam		Not Use
@return		1L
*/
LRESULT CColorPickaDlg::EvtKeyCapture(WPARAM, LPARAM)
{
	::PostMessage(m_ctrlMag.m_hWnd, CMouseMagnifier::eMagEvent::eEVT_GetColor, 0, 0);
	return 1L;
}


/**
Set Color Box
@access		private
@param		clrBox		Set Color
@return
*/
void CColorPickaDlg::SetColorBox(COLORREF clrBox)
{
	m_arrRect[eColorRectIdx::Rect_PickColor].SetBackgroundColor(clrBox);

	CString strText = _T("");
	stRGB rgbData;
	rgbData.nR = GetRValue(clrBox);
	rgbData.nG = GetGValue(clrBox);
	rgbData.nB = GetBValue(clrBox);
	strText.Format(_T("%d"), rgbData.nR);
	m_arrLabel[Label_InfoR].SetWindowText(strText);
	strText.Format(_T("%d"), rgbData.nG);
	m_arrLabel[Label_InfoG].SetWindowText(strText);
	strText.Format(_T("%d"), rgbData.nB);
	m_arrLabel[Label_InfoB].SetWindowText(strText);

	stHSV hsvData;
	CppUtil::RGBToHSV(rgbData.nR, rgbData.nG, rgbData.nB, hsvData.nH, hsvData.nS, hsvData.nV);
	strText.Format(_T("%d"), hsvData.nH);
	m_arrLabel[Label_InfoH].SetWindowText(strText);
	strText.Format(_T("%d"), hsvData.nS);
	m_arrLabel[Label_InfoS].SetWindowText(strText);
	strText.Format(_T("%d"), hsvData.nV);
	m_arrLabel[Label_InfoV].SetWindowText(strText);

	CString strHex = CppUtil::DecToHex(CppUtil::RGBToHex(rgbData.nR, rgbData.nG, rgbData.nB));
	m_arrLabel[Label_InfoHex].SetWindowText(strHex);
}


/**
OnClose
*/
void CColorPickaDlg::OnClose()
{
	KeyboardUnhook();

	CDialogEx::OnClose();
}


/**
Keyboard Hooker Function
@access		Local
@param		nCode
@param		wParam
@param		lParam
@return
*/
LRESULT CALLBACK KeyboardHooker(int nCode, WPARAM wParam, LPARAM lParam)
{
	static bool bIsCtrlDown = false;

	if (nCode == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT pHookKey = (PKBDLLHOOKSTRUCT)lParam;

		switch (wParam)
		{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
				if (pHookKey->vkCode == VK_RCONTROL ||
					pHookKey->vkCode == VK_LCONTROL)
					bIsCtrlDown = true;
				break;
			case WM_SYSKEYUP:
				bIsCtrlDown = false;
				break;
		}

		if (bIsCtrlDown)
		{
			if (pHookKey->vkCode == g_nHookKey)
			{
				::PostMessage(g_hwndMain, CColorPickaDlg::eEvent::eEVT_KeyCapture, 0, 0);
			}
		}
	}

	return CallNextHookEx(g_hKeyboard, nCode, wParam, lParam);
}


/**
Set Keyboard Hook (For Spacebar Click)
@access		private
@param		
@return		true / false
*/
bool CColorPickaDlg::SetKeyboardHook()
{
	if (g_hKeyboard != NULL)
		KeyboardUnhook();

	g_hKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHooker, (HINSTANCE)theApp.m_hInstance, NULL);

	return g_hKeyboard != NULL;
}


/**
Keyboard Unhook
@access		private
@param
@return
*/
void CColorPickaDlg::KeyboardUnhook()
{
	if (g_hKeyboard != NULL)
	{
		UnhookWindowsHookEx(g_hKeyboard);
	}
}


