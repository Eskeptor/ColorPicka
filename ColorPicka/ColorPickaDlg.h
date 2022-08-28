#pragma once
#include "MouseMagnifier/MouseMagnifier.h"
#include "ColorRect/ColorRect.h"
#include "ColorListCtrl/ColorListCtrl.h"

class CColorPickaDlg : public CDialogEx
{
// Enums and Structure ==================================================================================
private:
	enum eLabelIdx
	{
		Label_InfoR = 0,
		Label_InfoG,
		Label_InfoB,
		Label_InfoH,
		Label_InfoS,
		Label_InfoV,
		Label_InfoHex,
		Label_Title_InfoR1,
		Label_Title_InfoR2,
		Label_Title_InfoG1,
		Label_Title_InfoG2,
		Label_Title_InfoB1,
		Label_Title_InfoB2,
		LabelIdxMax
	};
	enum eEditIdx
	{
		Edit_Hex1 = 0,
		Edit_Hex2,
		Edit_Red1,
		Edit_Red2,
		Edit_Green1,
		Edit_Green2,
		Edit_Blue1,
		Edit_Blue2,
		Edit_Hue1,
		Edit_Hue2,
		Edit_Saturation1,
		Edit_Saturation2,
		Edit_Value1,
		Edit_Value2,
		EditIdxMax
	};
	enum eColorRectIdx
	{
		Rect_Hex1 = 0,
		Rect_Hex2,
		Rect_RGB1,
		Rect_RGB2,
		Rect_HSV1,
		Rect_HSV2,
		Rect_PickColor,
		ColorRectIdxMax
	};
	enum eWindowData
	{
		Width_NotUsedList = 454,
		Width_MiniMode = 310,
		Height_MiniMode = 230,
	};

	struct stRGB
	{
		int nR;
		int nG;
		int nB;
		stRGB(int _nR = 0, int _nG = 0, int _nB = 0) :
			nR(_nR), nG(_nG), nB(_nB) {}
	};

	struct stHSV
	{
		int nH;
		int nS;
		int nV;
		stHSV(int _nH = 0, int _nS = 0, int _nV = 0) :
			nH(_nH), nS(_nS), nV(_nV) {}
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLORPICKA_DIALOG };
#endif

	enum eEvent
	{
		eEVT_SetCurMousePosColor = WM_USER + 100,
		eEVT_KeyCapture,
		eEVT_SetLogMousePosColor,
	};

// ======================================================================================================


// Variables ============================================================================================
private:
	CStatic m_arrLabel[eLabelIdx::LabelIdxMax];				// Label Controls
	CRect m_arrLabelSize[eLabelIdx::LabelIdxMax];
	CEdit m_arrEdit[eEditIdx::EditIdxMax];					// Edit Controls
	CColorRect m_arrRect[eColorRectIdx::ColorRectIdxMax];	// Rect(Color) Controls
	CMouseMagnifier m_ctrlMag;								// Magnifier Control
	CColorListCtrl m_ctrlList;								// List Control
	CRect m_rcWindow;										// Window Rect


protected:
	HICON m_hIcon;
// ======================================================================================================

		
// Functions ============================================================================================
private:
	// Control Initialize
	void InitControls();

	// Label Initialize
	void InitLabel(CStatic* pCtlLabel, int nID, CString strText);
	void InitLabel(CStatic* pCtlLabel, int nID);
	// Edit Initialize
	void InitEdit(CEdit* pCtlEdit, int nID, bool bIsNumber, CString strText = _T(""));
	// ColorRect Initialize
	void InitColorRect(CColorRect* pCtlRect, int nID, int nR = 255, int nG = 255, int nB = 255);

	// Get Version
	CString GetVersion();

	// Set Color Rect's Color
	void SetRectColor(CColorRect* pCtlRect, stRGB rgbColor);
	void SetRectColor(CColorRect* pCtlRect, stHSV hsvColor);
	void SetRectColor(CColorRect* pCtlRect, CString strHexColor);

	// Set Color Box
	void SetColorBox(COLORREF clrBox);

	// Set Keyboard Hook (For Spacebar Click)
	bool SetKeyboardHook();
	// Keyboard Unhook
	void KeyboardUnhook();

	// Add Color Log
	void AddColorLog(COLORREF clrLog);
	// Load Color Log Data
	void LoadColorLogData();
	// Save Color Log Data
	void SaveColorLogData();
	// Show Color Picker Dialog
	bool ShowColorPicker(COLORREF& outClrSel);

	// Set Mini Mode
	void SetMiniMode(bool bSet);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// Set Color Of Current Mouse Position
	afx_msg LRESULT EvtSetCurMousePosColor(WPARAM, LPARAM lParam);
	// Key Capture Event
	afx_msg LRESULT EvtKeyCapture(WPARAM, LPARAM);
	// Set Color Of Log Color
	afx_msg LRESULT EvtSetLogMousePosColor(WPARAM, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CColorPickaDlg(CWnd* pParent = nullptr);

	afx_msg void OnBnClickedBtnRgbToHex();
	afx_msg void OnBnClickedBtnHexToRgb();
	afx_msg void OnBnClickedBtnHsvToHex();
	afx_msg void OnBnClickedBtnHexToHsv();
	afx_msg void OnBnClickedBtnHsvToRgb();
	afx_msg void OnBnClickedBtnRgbToHsv();
	afx_msg void OnStnClickedColorHex1();
	afx_msg void OnStnClickedColorRgb1();
	afx_msg void OnStnClickedColorHex2();
	afx_msg void OnStnClickedColorHsv1();
	afx_msg void OnStnClickedColorRgb2();
	afx_msg void OnStnClickedColorHsv2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnFileOption();

// ======================================================================================================

};
