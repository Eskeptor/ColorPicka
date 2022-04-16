// ======================================================================================================
// File Name	: ColorListCtrl.h /.cpp
// Class		: CColorListCtrl
// Super Class	: CListCtrl
// Last Update	: 2022.03.27 - Eskeptor
// ======================================================================================================

#pragma once
#include "ColorListCtrlHeader.h"

class CColorListCtrl : public CListCtrl
{
// Enums and Structure ==================================================================================
private:
	enum eDefaultData
	{
		Default_ItemSize = 12,
		Default_BackColor = RGB(240, 240, 240),
		Default_ForeColor = RGB(0, 0, 0),
	};
public:
	enum eListEvent
	{
		eEVT_GetColor = WM_USER + 400,
	};

// ======================================================================================================


// Variables ============================================================================================
private:
	CColorListCtrlHeader m_ctrlHeader;		// Header Control

	int m_nItemSize;						// Item Height Size
	CImageList m_imgList;					// Image List for Item Height
	UINT m_nEvtSendColorID;					// Send Color Event ID
	int m_nItemMax;							// Item Max Size

public:
	DECLARE_DYNAMIC(CColorListCtrl)

// ======================================================================================================


// Functions ============================================================================================
private:
	// Reset Item Max Size and Refresh Item
	void ReItemMaxSize();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CColorListCtrl();
	virtual ~CColorListCtrl();

	// Set Header Background Color
	void SetHeaderBgColor(COLORREF clrBack);
	// Set Header Foreground Color
	void SetHeaderFrColor(COLORREF clrFore);
	// Set Header Border Enable
	void SetHeaderBorder(bool bIsEnable);
	// Set Header Height Size
	void SetHeaderSize(int nSize);
	
	// Set Item Height Size
	void SetItemSize(int nSize);
	// Set Item Background Color
	void SetItemBgColor(int nIdx, COLORREF clrBack);
	// Set Item Foreground Color
	void SetItemFrColor(int nIdx, COLORREF clrFore);

	// Add Color Item
	void AddRGBColorItem(int nIdx, CString strColor);

	// Set Send Color Event ID To Parent
	void SetEventIDSendColor(UINT nID);

	// Set Item Max
	void SetItemMax(int nMax);

	virtual void PreSubclassWindow();
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);

// ======================================================================================================

};


