#pragma once
#include "afxdialogex.h"
#include "BBungGrid/BBungGrid.h"


class COptionDlg : public CDialog
{
// Enums and Structure ==================================================================================
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS };
#endif

// ======================================================================================================


// Variables ============================================================================================
private:
	CBBungGrid m_ctrlGrid;

public:
	DECLARE_DYNAMIC(COptionDlg)

// ======================================================================================================


// Functions ============================================================================================
private:
	// Control Initialize
	void InitControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:
	COptionDlg(CWnd* pParent = nullptr);   
	virtual ~COptionDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();

// ======================================================================================================

};
