#include "pch.h"
#include "ColorPicka.h"
#include "ColorListCtrl.h"


IMPLEMENT_DYNAMIC(CColorListCtrl, CListCtrl)

CColorListCtrl::CColorListCtrl()
    : m_nItemSize(eDefaultData::Default_ItemSize)
    , m_nEvtSendColorID(0)
    , m_nItemMax(20)
    , m_nCurSel(-1)
    , m_arrClr{ RGB(210, 245, 245), RGB(210, 245, 245) }
{

}

CColorListCtrl::~CColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CColorListCtrl::OnNMCustomdraw)
    ON_NOTIFY_REFLECT(NM_CLICK, &CColorListCtrl::OnNMClick)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


/**
PreSubclassWindow
*/
void CColorListCtrl::PreSubclassWindow()
{
    ModifyStyle(LVS_OWNERDRAWFIXED, 0, SWP_FRAMECHANGED);

    m_imgList.Create(1, m_nItemSize, ILC_COLORDDB, 1, 0);
    SetImageList(&m_imgList, LVSIL_SMALL);

    CHeaderCtrl* pCtrl = GetHeaderCtrl();
    HWND hWnd = (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);

    pCtrl->ModifyStyle(0, LVS_OWNERDRAWFIXED, SWP_FRAMECHANGED);

    m_ctrlHeader.SubclassWindow(hWnd);
	CListCtrl::PreSubclassWindow();
}


/**
Set Header Background Color
@access     public
@param      clrBack     Background Color
@return
*/
void CColorListCtrl::SetHeaderBgColor(COLORREF clrBack)
{
    m_ctrlHeader.SetHeaderBgColor(clrBack);
}


/**
Set Header Foreground Color
@access     public
@param      clrFore     Foreground Color
@return
*/
void CColorListCtrl::SetHeaderFrColor(COLORREF clrFore)
{
    m_ctrlHeader.SetHeaderFrColor(clrFore);
}


/**
Set Header Border Enable
@access     public
@param      bIsEnable   Enable
@return
*/
void CColorListCtrl::SetHeaderBorder(bool bIsEnable)
{
    m_ctrlHeader.SetHeaderBorder(bIsEnable);
}


/**
Set Header Height Size
@access     public
@param      nSize       Height Size
@return
*/
void CColorListCtrl::SetHeaderSize(int nSize)
{
    m_ctrlHeader.SetHeaderSize(nSize);
}


/**
Set Item Height Size
@access     public
@param      nSize       Height Size
@return
*/
void CColorListCtrl::SetItemSize(int nSize)
{
    if (m_imgList.DeleteImageList())
    {
        m_nItemSize = nSize;
        m_imgList.Create(1, nSize, ILC_COLORDDB, 1, 0);
        SetImageList(&m_imgList, LVSIL_SMALL);

        Invalidate();
    }
}


/**
Set Selected Item Background Color
@access     public
@param      nIdx        Item Index (-1 : All Items)
@param      clrBack     Background Color
@return
*/
void CColorListCtrl::SetSelectedItemBgColor(COLORREF clrBack)
{
    m_arrClr[eColorIdx::Selected_BackColor] = clrBack;
}


/**
Set Selected Item Foreground Color
@access     public
@param      nIdx        Item Index (-1 : All Items)
@param      clrBack     Foreground Color
@return
*/
void CColorListCtrl::SetSelectedItemFrColor(COLORREF clrFore)
{
    m_arrClr[eColorIdx::Selected_ForeColor] = clrFore;
}


/**
OnNMCustomdraw
*/
void CColorListCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO 아이템 선택 후 Text 내용 안보이게 수정
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = CDRF_DODEFAULT;
    pLVCD->nmcd.uItemState = CDIS_DEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
        case CDDS_PREPAINT:
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            break;
        }
        case CDDS_ITEMPREPAINT:
        {
            *pResult = CDRF_NOTIFYSUBITEMDRAW;
            break;
        }
        case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
        {
            if (m_nCurSel == pLVCD->nmcd.dwItemSpec)
            {
                pLVCD->clrText = m_arrClr[eColorIdx::Selected_ForeColor];
                pLVCD->clrTextBk = m_arrClr[eColorIdx::Selected_BackColor];
            }
            else
            {
                CString strColor = GetItemText(pLVCD->nmcd.dwItemSpec, 0);
                if (strColor.IsEmpty() == false &&
                    strColor.GetAt(0) == _T('!'))
                {
                    CString strRGB = strColor.Mid(1, strColor.GetLength() - 1);
                    COLORREF clrRGB = _ttoi(strRGB);

                    pLVCD->clrText = clrRGB;
                    pLVCD->clrTextBk = clrRGB;
                }
            }
            
            *pResult = CDRF_NOTIFYPOSTPAINT;
            break;
        }
        default:
        {
            *pResult = CDRF_DODEFAULT;
            break;
        }
    }
}


/**
Add Color Item
@access     public
@param      nIdx        Index
@param      strColor    Color (RGB(R,G,B) or COLORREF)
@return
*/
void CColorListCtrl::AddRGBColorItem(int nIdx, CString strColor)
{
    CString strRGBColor = _T("");
    strRGBColor.Format(_T("!%s"), strColor.GetString());
    InsertItem(nIdx, strRGBColor);
    ReItemMaxSize();
}


/**
OnNMClick
*/
void CColorListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    if (pNMItemActivate->iItem >= 0)
    {
        CString strColor = GetItemText(pNMItemActivate->iItem, 0);
        int nColor = _ttoi(strColor.Mid(1, strColor.GetLength() - 1));
        m_nCurSel = pNMItemActivate->iItem;

        if (m_nEvtSendColorID > 0)
        {
            ::PostMessage(GetParent()->m_hWnd, m_nEvtSendColorID, 0, nColor);
        }

        Invalidate();
    }

    *pResult = 0;
}


/**
Set Send Color Event ID To Parent
@access		public
@param		nID			Event ID
@return
*/
void CColorListCtrl::SetEventIDSendColor(UINT nID)
{
    m_nEvtSendColorID = nID;
}


/**
Set Item Max
@access     public
@param      nMax        Item Max Size
@return
*/
void CColorListCtrl::SetItemMax(int nMax)
{
    if (nMax > 0)
    {
        m_nItemMax = nMax;
        ReItemMaxSize();
    }
}


/**
Reset Item Max Size and Refresh Item
@access     private
@param
@return
*/
void CColorListCtrl::ReItemMaxSize()
{
    int nCount = GetItemCount();

    if (nCount > m_nItemMax)
    {
        for (int i = nCount; i > m_nItemMax; i--)
        {
            DeleteItem(i - 1);
        }
        Invalidate();
    }
}


/**
OnKeyDown
*/
void CColorListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (m_nCurSel >= 0 &&
        m_nCurSel < GetItemCount())
    {
        if (nChar == VK_DELETE)
        {
            if (DeleteItem(m_nCurSel))
            {
                m_nCurSel = -1;

                Invalidate();
            }
        }
    }

    CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
