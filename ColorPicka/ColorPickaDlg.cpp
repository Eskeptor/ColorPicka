﻿#include "pch.h"
#include "framework.h"
#include "ColorPicka.h"
#include "ColorPickaDlg.h"
#include "afxdialogex.h"
#include "CppUtil/CppUtil.h"
#include "OptionDlg.h"

#include <vector>

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


/**
Constructor
*/
CColorPickaDlg::CColorPickaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLORPICKA_DIALOG, pParent)
	, m_pOnMouseDlg(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


/**
Destructor
*/
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
	ON_MESSAGE(eEvent::eEVT_SetLogMousePosColor, &CColorPickaDlg::EvtSetLogMousePosColor)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPTION, &CColorPickaDlg::OnFileOption)
END_MESSAGE_MAP()


/**
OnInitDialog()
*/
BOOL CColorPickaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	// Menu Test
	if (false)
	{
		CMenu* ptrMenu = GetMenu();
		if (ptrMenu != nullptr)
		{
			MENUINFO stMenuInfo{ 0, };
			stMenuInfo.cbSize = sizeof(MENUINFO);

			if (ptrMenu->GetMenuInfo(&stMenuInfo))
			{
				stMenuInfo.hbrBack = ::CreateSolidBrush(RGB(240, 240, 240));
				stMenuInfo.fMask = MIM_BACKGROUND | MIM_STYLE;
				stMenuInfo.dwStyle = MNS_NOCHECK;
				ptrMenu->SetMenuInfo(&stMenuInfo);
			}
		}
	}

	InitControls();
	InitData();
	SetKeyboardHook();

	return TRUE;  
}


/**
OnPaint()
*/
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
	}
}


/**
OnQueryDragIcon()
*/
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
		GetWindowRect(&m_rcWindow);

		// Title
		{
			CString strVersion = _T("");
			strVersion.Format(_T("ColorPicka %s (Made by Eskeptor)"), GetVersion().GetString());
			SetWindowText(strVersion);
		}

		if (g_stOptions.stOpList.bIsUseList == false)
		{
			MoveWindow(0, 0, eWindowData::Width_NotUsedList, m_rcWindow.Height());
		}
	}

	// Label
	{
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoR], IDC_LBL_COLOR_INFO_RED, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoG], IDC_LBL_COLOR_INFO_GREEN, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoB], IDC_LBL_COLOR_INFO_BLUE, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoH], IDC_LBL_COLOR_INFO_H, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoS], IDC_LBL_COLOR_INFO_S, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoV], IDC_LBL_COLOR_INFO_V, _T(""));
		InitLabel(&m_arrLabel[eLabelIdx::Label_InfoHex], IDC_LBL_COLOR_INFO_HEX, _T(""));

		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoR1], IDC_LBL_COLOR_INFO_RED_1);
		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoR2], IDC_LBL_COLOR_INFO_RED_2);
		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoG1], IDC_LBL_COLOR_INFO_GREEN_1);
		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoG2], IDC_LBL_COLOR_INFO_GREEN_2);
		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoB1], IDC_LBL_COLOR_INFO_BLUE_1);
		InitLabel(&m_arrLabel[eLabelIdx::Label_Title_InfoB2], IDC_LBL_COLOR_INFO_BLUE_2);

		for (int i = 0; i < eLabelIdx::LabelIdxMax; i++)
		{
			m_arrLabel[i].GetWindowRect(&m_arrLabelSize[i]);
			ScreenToClient(&m_arrLabelSize[i]);
		}
	}

	// Edit
	{
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hex1], IDC_EDIT_H_R_HEX, false);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Hex2], IDC_EDIT_H_H_HEX, false);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Red1], IDC_EDIT_H_R_RED, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Red2], IDC_EDIT_R_H_RED, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Green1], IDC_EDIT_H_R_GREEN, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Green2], IDC_EDIT_R_H_GREEN, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Blue1], IDC_EDIT_H_R_BLUE, true);
		InitEdit(&m_arrEdit[eEditIdx::Edit_Blue2], IDC_EDIT_R_H_BLUE, true);	
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
		m_ctrlMag.SetIncludeWindow(g_stOptions.stOpMag.bIsIncludeWindow);
		m_ctrlMag.SetDrawCross(g_stOptions.stOpMag.bIsDrawCross);
	}

	// List Control
	{
		m_ctrlList.SubclassDlgItem(IDC_LIST_COLOR, this);
		if (g_stOptions.stOpList.bIsUseList)
		{
			m_ctrlList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
			m_ctrlList.SetEventIDSendColor(eEvent::eEVT_SetLogMousePosColor);

			CRect rtList;
			m_ctrlList.GetWindowRect(&rtList);

			m_ctrlList.InsertColumn(0, _T("Color"), LVCFMT_LEFT, rtList.Width() - 4);
			m_ctrlList.SetHeaderSize(30);
			m_ctrlList.SetHeaderBorder(false);
			m_ctrlList.SetItemSize(20);
			m_ctrlList.SetItemMax(g_stOptions.stOpList.nLogMax);

			if (g_stOptions.stOpList.bIsSaveLog)
			{
				LoadColorLogData();
			}
		}
		else
		{
			m_ctrlList.ShowWindow(SW_HIDE);
		}
	}

	// Mini Mode
	{
		SetMiniMode(g_stOptions.stOpSys.bIsMiniMode);
	}

	// On Mouse Mode Control
	{
		m_pOnMouseDlg = new COnMouseDlg;
		m_pOnMouseDlg->Create(IDD_ON_MOUSE, this);
		m_pOnMouseDlg->SetTextVisible(false);
		m_pOnMouseDlg->ShowWindow(g_stOptions.stOpSys.bIsOnMouseMode ? SW_SHOW : SW_HIDE);

		if (m_stOnMouseThread.thrRefresh == nullptr)
		{
			m_stOnMouseThread.thrRefresh = AfxBeginThread(ThreadMousePosition, this);

			if (m_stOnMouseThread.thrRefresh == nullptr)
			{
				MessageBox(_T("On Mouse Thread Create Fail"), _T("Error"));
			}
		}
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
Label Initialize
@access		private
@param		pCtlLabel		Label Control(CStatic)
@param		nID				Label's ID
@return
*/
void CColorPickaDlg::InitLabel(CStatic* pCtlLabel, int nID)
{
	if (pCtlLabel != nullptr &&
		nID > 0)
	{
		pCtlLabel->SubclassDlgItem(nID, this);
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
Initial Data Setting
@access		private
@param
@return
*/
void CColorPickaDlg::InitData()
{
	// Hex <-> RGB
	{
		// Hex
		{
			m_arrEdit[eEditIdx::Edit_Hex1].SetWindowText(_T("#FFFFFF"));
		}

		// RGB
		{
			m_arrEdit[eEditIdx::Edit_Red1].SetWindowText(_T("255"));
			m_arrEdit[eEditIdx::Edit_Green1].SetWindowText(_T("255"));
			m_arrEdit[eEditIdx::Edit_Blue1].SetWindowText(_T("255"));
		}
	}

	// Hex <-> HSV
	{
		// Hex
		{
			m_arrEdit[eEditIdx::Edit_Hex2].SetWindowText(_T("#FFFFFF"));
		}

		// HSV
		{
			m_arrEdit[eEditIdx::Edit_Hue1].SetWindowText(_T("0"));
			m_arrEdit[eEditIdx::Edit_Saturation1].SetWindowText(_T("0"));
			m_arrEdit[eEditIdx::Edit_Value1].SetWindowText(_T("100"));
		}
	}

	// RGB <-> HSV
	{
		// RGB
		{
			m_arrEdit[eEditIdx::Edit_Red2].SetWindowText(_T("255"));
			m_arrEdit[eEditIdx::Edit_Green2].SetWindowText(_T("255"));
			m_arrEdit[eEditIdx::Edit_Blue2].SetWindowText(_T("255"));
		}

		// HSV
		{
			m_arrEdit[eEditIdx::Edit_Hue2].SetWindowText(_T("0"));
			m_arrEdit[eEditIdx::Edit_Saturation2].SetWindowText(_T("0"));
			m_arrEdit[eEditIdx::Edit_Value2].SetWindowText(_T("100"));
		}
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
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		CString strHex = _T("");
		strHex.Format(_T("#%X"), CppUtil::RGBToHex(GetRValue(clrSel), GetGValue(clrSel), GetBValue(clrSel)));
		m_arrEdit[eEditIdx::Edit_Hex1].SetWindowTextW(strHex);

		m_arrRect[Rect_Hex1].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
}


/**
Click : RGB1 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorRgb1()
{
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		CString strData = _T("");
		strData.Format(_T("%d"), GetRValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Red1].SetWindowText(strData);

		strData.Format(_T("%d"), GetGValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Green1].SetWindowText(strData);

		strData.Format(_T("%d"), GetBValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Blue1].SetWindowText(strData);

		m_arrRect[Rect_RGB1].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
}


/**
Click : Hex2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHex2()
{
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		CString strHex = _T("");
		strHex.Format(_T("#%X"), CppUtil::RGBToHex(GetRValue(clrSel), GetGValue(clrSel), GetBValue(clrSel)));
		m_arrEdit[eEditIdx::Edit_Hex2].SetWindowTextW(strHex);

		m_arrRect[Rect_Hex2].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
}


/**
Click : HSV1 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHsv1()
{
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		stHSV hsvData;
		CppUtil::RGBToHSV(GetRValue(clrSel), GetGValue(clrSel), GetBValue(clrSel), hsvData.nH, hsvData.nS, hsvData.nV);

		CString strData = _T("");
		strData.Format(_T("%d"), hsvData.nH);
		m_arrEdit[eEditIdx::Edit_Hue1].SetWindowText(strData);

		strData.Format(_T("%d"), hsvData.nS);
		m_arrEdit[eEditIdx::Edit_Saturation1].SetWindowText(strData);

		strData.Format(_T("%d"), hsvData.nV);
		m_arrEdit[eEditIdx::Edit_Value1].SetWindowText(strData);

		m_arrRect[Rect_HSV1].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
}


/**
Click : RGB2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorRgb2()
{
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		CString strData = _T("");
		strData.Format(_T("%d"), GetRValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Red2].SetWindowText(strData);

		strData.Format(_T("%d"), GetGValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Green2].SetWindowText(strData);

		strData.Format(_T("%d"), GetBValue(clrSel));
		m_arrEdit[eEditIdx::Edit_Blue2].SetWindowText(strData);

		m_arrRect[Rect_RGB2].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
}


/**
Click : HSV2 Color
@access		public
@param
@return
*/
void CColorPickaDlg::OnStnClickedColorHsv2()
{
	m_ctrlMag.StopCapture();
	COLORREF clrSel = 0;

	if (ShowColorPicker(clrSel))
	{
		stHSV hsvData;
		CppUtil::RGBToHSV(GetRValue(clrSel), GetGValue(clrSel), GetBValue(clrSel), hsvData.nH, hsvData.nS, hsvData.nV);

		CString strData = _T("");
		strData.Format(_T("%d"), hsvData.nH);
		m_arrEdit[eEditIdx::Edit_Hue2].SetWindowText(strData);

		strData.Format(_T("%d"), hsvData.nS);
		m_arrEdit[eEditIdx::Edit_Saturation2].SetWindowText(strData);

		strData.Format(_T("%d"), hsvData.nV);
		m_arrEdit[eEditIdx::Edit_Value2].SetWindowText(strData);

		m_arrRect[Rect_HSV2].SetBackgroundColor(clrSel);
	}
	m_ctrlMag.StartCapture();
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
	AddColorLog(clrColor);

	if (g_stOptions.stOpSys.bIsOnMouseMode)
	{
		::PostMessage(m_pOnMouseDlg->m_hWnd, COnMouseDlg::eEvent::eEVT_ChangeColor, 0, lParam);
	}

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
Add Color Log
@access		private
@param		clrLog		Color
@return
*/
void CColorPickaDlg::AddColorLog(COLORREF clrLog)
{
	if (g_stOptions.stOpList.bIsUseList)
	{
		CString strColorLog = _T("");
		strColorLog.Format(_T("%d"), clrLog);
		m_ctrlList.AddRGBColorItem(0, strColorLog);
	}
}


/**
OnClose
*/
void CColorPickaDlg::OnClose()
{
	if (m_pOnMouseDlg != nullptr)
	{
		m_pOnMouseDlg->DestroyWindow();
		delete m_pOnMouseDlg;
		m_pOnMouseDlg = nullptr;
	}

	if (m_stOnMouseThread.thrRefresh != nullptr)
	{
		m_stOnMouseThread.bExitFlag = true;
		while (m_stOnMouseThread.thrRefresh != nullptr)
			Sleep(1);
	}

	if (g_stOptions.stOpList.bIsSaveLog)
	{
		SaveColorLogData();
	}
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
			if (pHookKey->vkCode == g_nHookKey &&
				wParam == WM_KEYDOWN)
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


/**
Set Color Of Log Color
@event		eEvent::eEVT_SetLogMousePosColor
@param		wParam		Not Use
@param		lParam		Color
@return		1L
*/
LRESULT CColorPickaDlg::EvtSetLogMousePosColor(WPARAM, LPARAM lParam)
{
	COLORREF clrColor = (COLORREF)lParam;

	SetColorBox(clrColor);

	return 1L;
}


/**
Get Color Log Data
@access		private
@param
@return
*/
void CColorPickaDlg::LoadColorLogData()
{
	CString strIniFileName = _T("");
	strIniFileName.Format(_T("%s%s.ini"), CppUtil::GetExePath().GetString(), CppUtil::GetExeName(false).GetString());

	if (CppUtil::FileCheck(strIniFileName))
	{
		CString strLog = CppUtil::INIReadString(STR_APP_LIST, STR_KEY_LIST_LOGDATA, strIniFileName);

		if (strLog.IsEmpty() == false)
		{
			std::vector<CString> vColors;

			int nTokenize = 0;
			CString strToken = _T("");

			strToken = strLog.Tokenize(_T("|"), nTokenize);
			while (strToken.IsEmpty() == false)
			{
				vColors.push_back(strToken);

				strToken = strLog.Tokenize(_T("|"), nTokenize);
			}

			int nIdx = 0;
			std::vector<CString>::reverse_iterator iter = vColors.rbegin();
			while (iter != vColors.rend())
			{
				m_ctrlList.AddRGBColorItem(0, *iter);
				iter++;
			}
		}
	}
}


/**
Save Color Log Data
@access		private
@param
@return
*/
void CColorPickaDlg::SaveColorLogData()
{
	CString strIniFileName = _T("");
	strIniFileName.Format(_T("%s%s.ini"), CppUtil::GetExePath().GetString(), CppUtil::GetExeName(false).GetString());

	int nCount = m_ctrlList.GetItemCount();
	CString strTemp = _T("");
	CString strColor = _T("");

	for (int i = 0; i < nCount; i++)
	{
		strTemp = m_ctrlList.GetItemText(i, 0);
		strColor.Append(strTemp.Mid(1, strTemp.GetLength()));

		if (i != nCount - 1)
			strColor.Append(_T("|"));
	}

	CppUtil::INIWriteString(STR_APP_LIST, STR_KEY_LIST_LOGDATA, strIniFileName, strColor);
}


/**
Show Color Picker Dialog
@access		private
@param		outClrSel	Selected Color
@return		Color Dialog Result == Ok
*/
bool CColorPickaDlg::ShowColorPicker(COLORREF& outClrSel)
{
	CMFCColorDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		outClrSel = dlg.GetColor();
		return true;
	}
	
	return false;
}


/**
Menu : File - Option
*/
void CColorPickaDlg::OnFileOption()
{
	int nLogMax = g_stOptions.stOpList.nLogMax;

	COptionDlg dlgOption;
	dlgOption.DoModal();

	if (nLogMax != g_stOptions.stOpList.nLogMax)
		m_ctrlList.SetItemMax(g_stOptions.stOpList.nLogMax);

	SetMiniMode(g_stOptions.stOpSys.bIsMiniMode);
	
	m_pOnMouseDlg->ShowWindow(g_stOptions.stOpSys.bIsOnMouseMode ? SW_SHOW : SW_HIDE);
}


/**
Set Mini Mode
@access		private
@param		bSet		Mini Mode Set
@return		
*/
void CColorPickaDlg::SetMiniMode(bool bSet)
{
	int nWindowWidth = 0;
	int nWindowHeight = 0;
	int nColorInfoX1 = 0;
	int nColorInfoX2 = 0;
	int nColorInfoX3 = 0;

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	CRect rcMag;
	m_ctrlMag.GetWindowRect(&rcMag);
	ScreenToClient(&rcMag);
	CRect rcPick;
	m_arrRect[eColorRectIdx::Rect_PickColor].GetWindowRect(&rcPick);
	ScreenToClient(&rcPick);

	if (bSet)
	{
		nWindowWidth = eWindowData::Width_MiniMode;
		nWindowHeight = eWindowData::Height_MiniMode;

		nColorInfoX1 = 194;
		nColorInfoX2 = 156;
		nColorInfoX3 = 180;

		m_arrRect[eColorRectIdx::Rect_PickColor].MoveWindow(rcPick.left, rcMag.top + (rcMag.Width() / 2), rcMag.Height(), rcMag.Width() / 2);
	}
	else
	{
		nWindowWidth = m_rcWindow.Width();
		nWindowHeight = m_rcWindow.Height();

		nColorInfoX1 = m_arrLabelSize[eLabelIdx::Label_InfoR].left;
		nColorInfoX2 = m_arrLabelSize[eLabelIdx::Label_Title_InfoR1].left;
		nColorInfoX3 = m_arrLabelSize[eLabelIdx::Label_Title_InfoR2].left;

		m_arrRect[eColorRectIdx::Rect_PickColor].MoveWindow(rcPick.left, rcMag.top, rcMag.Width(), rcMag.Width());
	}

	m_arrLabel[eLabelIdx::Label_InfoR].MoveWindow(nColorInfoX1, m_arrLabelSize[eLabelIdx::Label_InfoR].top, m_arrLabelSize[eLabelIdx::Label_InfoR].Width(), m_arrLabelSize[eLabelIdx::Label_InfoR].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoR1].MoveWindow(nColorInfoX2, m_arrLabelSize[eLabelIdx::Label_Title_InfoR1].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoR1].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoR1].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoR2].MoveWindow(nColorInfoX3, m_arrLabelSize[eLabelIdx::Label_Title_InfoR2].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoR2].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoR2].Height());

	m_arrLabel[eLabelIdx::Label_InfoG].MoveWindow(nColorInfoX1, m_arrLabelSize[eLabelIdx::Label_InfoG].top, m_arrLabelSize[eLabelIdx::Label_InfoG].Width(), m_arrLabelSize[eLabelIdx::Label_InfoG].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoG1].MoveWindow(nColorInfoX2, m_arrLabelSize[eLabelIdx::Label_Title_InfoG1].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoG1].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoG1].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoG2].MoveWindow(nColorInfoX3, m_arrLabelSize[eLabelIdx::Label_Title_InfoG2].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoG2].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoG2].Height());

	m_arrLabel[eLabelIdx::Label_InfoB].MoveWindow(nColorInfoX1, m_arrLabelSize[eLabelIdx::Label_InfoB].top, m_arrLabelSize[eLabelIdx::Label_InfoB].Width(), m_arrLabelSize[eLabelIdx::Label_InfoB].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoB1].MoveWindow(nColorInfoX2, m_arrLabelSize[eLabelIdx::Label_Title_InfoB1].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoB1].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoB1].Height());
	m_arrLabel[eLabelIdx::Label_Title_InfoB2].MoveWindow(nColorInfoX3, m_arrLabelSize[eLabelIdx::Label_Title_InfoB2].top, m_arrLabelSize[eLabelIdx::Label_Title_InfoB2].Width(), m_arrLabelSize[eLabelIdx::Label_Title_InfoB2].Height());

	MoveWindow(rcWindow.left, rcWindow.top, nWindowWidth, nWindowHeight);

}


/**
Set On Mouse Mode
@access		private
@param		bSet		On Mouse Mode Set
@return
*/
void CColorPickaDlg::SetOnMouseMode(bool bSet)
{

}


/**
m_stOnMouseThread's Thread Function
*/
UINT CColorPickaDlg::ThreadMousePosition(LPVOID pParam)
{
	CColorPickaDlg* pMain = (CColorPickaDlg*)pParam;

	if (pMain != nullptr)
	{
		POINT ptMouse;
		POINT ptPrevMouse{ 0, };

		while (pMain->m_stOnMouseThread.bExitFlag == false &&
			   pMain != nullptr)
		{
			if (g_stOptions.stOpSys.bIsOnMouseMode)
			{
				GetCursorPos(&ptMouse);

				if (ptMouse.x != ptPrevMouse.x ||
					ptMouse.y != ptPrevMouse.y)
				{
					ptPrevMouse.x = ptMouse.x;
					ptPrevMouse.y = ptMouse.y;

					if (pMain->m_pOnMouseDlg != nullptr)
					{
						pMain->m_pOnMouseDlg->MoveWindow(ptMouse.x + 5, ptMouse.y, 60, 60, TRUE);
					}
				}
			}

			Sleep(10);
		}

		pMain->m_stOnMouseThread.thrRefresh = nullptr;
	}

	return 0;
}