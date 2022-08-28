// ======================================================================================================
// File Name	: MouseMagnifier.h /.cpp
// Class		: CMouseMagnifier
// Super Class	: CStatic
// Last Update	: 2022.03.26 - Eskeptor
// ======================================================================================================

#pragma once

class CMouseMagnifier : public CStatic
{
// Enums and Structure ==================================================================================
private:
	enum eDefaultData
	{
		Default_Zoom = 2,
		Default_ViewWidth = 76,
		Default_ViewHeight = 76,
		Default_LineThickness = 1,
		Default_CrossClrRed = 0,
		Default_CrossClrGreen = 0,
		Default_CrossClrBlue = 0,
	};

	struct stThr
	{
		CWinThread* thrRefresh;	// Thread
		bool bExitFlag;

		stThr()
			: thrRefresh(nullptr), bExitFlag(false) {}
	};
public:
	enum eMagEvent
	{
		eEVT_GetColor = WM_USER + 300,
	};

// ======================================================================================================


// Variables ============================================================================================
private:
	double m_dZoom;						// Zoom Scale
	bool m_bIsCrossDraw;				// Cross Line Draw Enable
	int m_nActViewWidth;				// Actual Mouse Viewport Width
	int m_nActViewHeight;				// Actual Mouse Viewport Height
	CRect m_rtSize;						// Control's Rect Size
	POINT m_ptMouse;					// Mouse Position
	COLORREF m_clrCrossColor;			// Cross Line Color
	UINT m_nEvtSendColorID;				// Send Color Event ID
	COLORREF m_clrCurCursorPosColor;	// Color of Current Mouse Position
	bool m_bIsIncludeWindow;			// Window Not Include Option
	bool m_bCapture;					// Zoom Capture Flag

	stThr m_stRefreshThread;

public:
	DECLARE_DYNAMIC(CMouseMagnifier)
// ======================================================================================================


// Functions ============================================================================================
private:
	// Draw Cross Line
	void DrawCross(CDC* pDC);
	// Draw Zoom Screen
	void DrawScreen(CDC* pDC);

protected:
	static UINT ThreadMousePosition(LPVOID pParam);

	// Get Color Event : eMagEvent::eEVT_GetColor
	afx_msg LRESULT EvtGetColor(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

public:
	CMouseMagnifier();
	virtual ~CMouseMagnifier();

	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	afx_msg void OnDestroy();

	// Set Cross Line Draw Enable
	void SetDrawCross(bool bEnable);
	// Set Cross Line Color
	void SetCrossColor(COLORREF clrCross);
	// Set Send Color Event ID To Parent
	void SetEventIDSendColor(UINT nID);
	// Set Window Include Option
	void SetIncludeWindow(bool bInclude);
	// Start Mouse Capture
	void StartCapture();
	// Stop Mouse Capture
	void StopCapture();

// ======================================================================================================
	
};


