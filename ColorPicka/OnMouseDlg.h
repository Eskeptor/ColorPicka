// ======================================================================================================
// File Name	: OnMouseDlg.h /.cpp
// Class		: COnMouseDlg
// Super Class	: CDialog
// Last Update	: 2022.12.13 - Eskeptor
// ======================================================================================================

#pragma once
#include "afxdialogex.h"


class COnMouseDlg : public CDialog
{
// Enums and Structure ==================================================================================
private:
	struct stThr
	{
		CWinThread* thrRefresh;	// Thread
		bool bExitFlag;

		stThr()
			: thrRefresh(nullptr), bExitFlag(false) {}
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ON_MOUSE };
#endif

	enum eEvent
	{
		eEVT_ChangeColor = WM_USER + 500,
	};
// ======================================================================================================


// Variables ============================================================================================
private:
	COLORREF m_clrBack;

	CStatic* m_pCtrlStaticR;
	CStatic* m_pCtrlStaticG;
	CStatic* m_pCtrlStaticB;

public:
	DECLARE_DYNAMIC(COnMouseDlg)
// ======================================================================================================


// Functions ============================================================================================
protected:
	// Change Color Event : eEvent::eEVT_ChangeColor
	afx_msg LRESULT EvtChangeColor(WPARAM, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:
	COnMouseDlg(CWnd* pParent = nullptr);   
	virtual ~COnMouseDlg();

	// Set Text Visible
	void SetTextVisible(bool bVisible);

	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();

// ======================================================================================================
	
};
