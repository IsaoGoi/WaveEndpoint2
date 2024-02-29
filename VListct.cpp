// CVListCt.cpp

#include "pch.h"
#include "VListCt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////
// CVListCtrl

// Constructor.
CVListCtrl::CVListCtrl() :
   m_bModeFirstColumn(FALSE), m_bModeAnyColumn(FALSE),
   m_nColumnSort(-1), m_bSortAscending(TRUE),
   m_bModeGridLineHorz(FALSE), m_bModeGridLineVert(FALSE)
{
}

// Destructor.
/* virtual */ CVListCtrl::~CVListCtrl()
{
}

BEGIN_MESSAGE_MAP(CVListCtrl, CListCtrl)
   //{{AFX_MSG_MAP(CVListCtrl)
   ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
   ON_WM_DESTROY()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
// CVListCtrl helper functions

// Does the sent item exist?
BOOL CVListCtrl::IsItem(int nItem) const
{
   return GetItemCount() > nItem;
}

// Does the sent column exist?
BOOL CVListCtrl::IsColumn(int nCol) const
{
   LV_COLUMN lvc;
   memset(&lvc, 0, sizeof(lvc));
   lvc.mask = LVCF_WIDTH;
   return GetColumn(nCol, &lvc);
}

// Returns the first item that is selected by default.
int CVListCtrl::GetSelectedItem(int nStartItem /* = -1 */ ) const
{
   return GetNextItem(nStartItem, LVNI_SELECTED);
}

// Selects the sent item.
BOOL CVListCtrl::SelectItem(int nItem) 
{
   return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

// Selects all the items in the control.
BOOL CVListCtrl::SelectAll()
{
   BOOL bReturn(TRUE);

   for (int nItem = 0; IsItem(nItem); nItem++) 
      if (! SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED)) {
         bReturn = FALSE;
         break;
      }

   return bReturn;
}

///////////////////////////////////////////////////////////////////
// CVListCtrl mode functions

#if defined CVLISTCTRL_FIRSTCOLUMNMODE
void CVListCtrl::SetModeFirstColumn(BOOL b /* = TRUE */ )
{
   m_bModeFirstColumn = b;
}
#endif

#if defined CVLISTCTRL_FIRSTCOLUMNMODE
void CVListCtrl::SetModeAnyColumn(BOOL b /* = TRUE */ )
{
   m_bModeAnyColumn = b;
}
#endif

// Handle clicks.
void CVListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   UNUSED_ALWAYS(pNMHDR);

   // Get the mouse position in screen coords, convert to client.
   POINT pt;
   ::GetCursorPos(&pt);
   ScreenToClient(&pt);

   // Do we want clicks anywhere in the first column to count,
   // and if so was the mouse over the first column?
   #if defined CVLISTCTRL_FIRSTCOLUMNMODE
   if (GetModeFirstColumn()) {
      UINT uiFlags(0U);
      HitTest(pt, &uiFlags);

      if (IsColumn(0)) {
         int nWidth(GetColumnWidth(0));

         // If outside of text but in first column, force the
         // selection of this item (if on a valid line).
         if (! (uiFlags & LVHT_ONITEM)  &&  pt.x <= nWidth) {
            pt.x = 2;
            int nItem(HitTest(pt, &uiFlags));

            if (-1 != nItem  &&  LVHT_ONITEM & uiFlags) 
               SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,
                  LVIS_FOCUSED | LVIS_SELECTED);
         }
      }
   }
   #endif

   #if defined CVLISTCTRL_FIRSTCOLUMNMODE
   // A click anywhere on the item selects.
   if (GetModeAnyColumn()) {
      UINT uiFlags(0U);
      HitTest(pt, &uiFlags);
   
      if (! (uiFlags & LVHT_ONITEM)) {
         pt.x = 2;
         int nItem(HitTest(pt, &uiFlags));

         if (-1 != nItem  &&  LVHT_ONITEM & uiFlags)
            SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,
               LVIS_FOCUSED | LVIS_SELECTED);
      }
   }
   #endif

   *pResult = 0;
}

#if defined CVLISTCTRL_SORTMODE
// handle column clicks.
void CVListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
   NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;

   // If a different column is being sorted from the last time we
   // sorted, we always start off ascending.
   if (pNMListView->iSubItem != m_nColumnSort)
      m_bSortAscending = TRUE;
   else
      m_bSortAscending = ! m_bSortAscending;

   m_nColumnSort = pNMListView->iSubItem;

   // Now, the only way the CListCtrl can know how to sort is by
   // each item's LPARAM, so we give it the item number.
#if 0
   for (int nItem = 0; IsItem(nItem); nItem++)
      SetItemData(nItem, nItem);
#endif
   // Call the sort routine.
   SortItems(CompareFunc, reinterpret_cast <INT64> (this));
   *pResult = 0;
}
#endif

#if defined CVLISTCTRL_SORTMODE
// This is the function that the base CListCtrl code calls whenever
// it needs to compare two items.
/* static */ int CALLBACK CVListCtrl::CompareFunc(LPARAM lParam1,
                                             LPARAM lParam2,
    INT64 lParamSort)
{
   CVListCtrl* pListCtrl =
      reinterpret_cast <CVListCtrl*> (lParamSort);

   LV_FINDINFO lvi;
   memset(&lvi, 0, sizeof(lvi));
   lvi.flags = LVFI_PARAM;

   lvi.lParam = lParam1;
   int nItem1(pListCtrl->FindItem(&lvi));

   lvi.lParam = lParam2;
   int nItem2(pListCtrl->FindItem(&lvi));

   CString s1(pListCtrl->GetItemText(nItem1,
      pListCtrl->m_nColumnSort));

   CString s2(pListCtrl->GetItemText(nItem2,
      pListCtrl->m_nColumnSort));

   int nReturn(s1.CompareNoCase(s2));

   if (! pListCtrl->m_bSortAscending)
      nReturn = -nReturn;

   return nReturn;
}
#endif

// Override that draws the entire row as selected, instead of just
//the text in the zeroth column.
#if defined CVLISTCTRL_ROWMODE
/* virtual */ void CVListCtrl::DrawItem(LPDRAWITEMSTRUCT 
                                        lpDrawItemStruct)
{
											
	// Get the display context for the control.
   CDC* pCDC = CDC::FromHandle(lpDrawItemStruct->hDC);

   // Use mask if there is one.
   UINT uiFlags(ILD_TRANSPARENT);

   // Check to see if this item is selected.
   if (lpDrawItemStruct->itemState & ODS_SELECTED) {
//      pCDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
//      pCDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
      pCDC->SetBkColor(RGB(192,192,192));
      uiFlags |= ILD_BLEND50;
   }
   else {
//      pCDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
//    pCDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	  pCDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	  if(lpDrawItemStruct->itemData >= 60000)			// 60000 = Yellow
		pCDC->SetBkColor(RGB(254,255,141));
	  else if(lpDrawItemStruct->itemData >= 50000)			// 50000 = Red
		pCDC->SetBkColor(RGB(255,200,200));
	  else if(lpDrawItemStruct->itemData >= 40000)			// 40000 = Blue
		pCDC->SetBkColor(RGB(158,160,254));
	  else if(lpDrawItemStruct->itemData >= 30000)			// 30000 = Sky Blue
		pCDC->SetBkColor(RGB(227,255,255));
	  else if(lpDrawItemStruct->itemData >= 20000)			// 20000 = Gray
		pCDC->SetBkColor(RGB(150,150,150));
	  else if(lpDrawItemStruct->itemData >= 10000)		// 10000 = White
		pCDC->SetBkColor(RGB(255,255,255));
	  else if(lpDrawItemStruct->itemData >= 9000)		// 9000  = Red
		pCDC->SetBkColor(RGB(255,200,200));
	  else
		pCDC->SetBkColor(RGB(197,252,243));				// Sky Blue
   }

   // Get the image list and draw.
   CImageList* plistImages = GetImageList(LVSIL_SMALL);

   // Get the item image to be displayed.
   LV_ITEM lvi;
   memset(&lvi, 0, sizeof(lvi));
   lvi.mask = LVIF_IMAGE | LVIF_STATE;
   lvi.iItem = lpDrawItemStruct->itemID;
   GetItem(&lvi);

   // If valid, draw.
   if (plistImages) {
      CPoint ptAt(lpDrawItemStruct->rcItem.left,
         lpDrawItemStruct->rcItem.top);
      plistImages->Draw(pCDC, lvi.iImage, ptAt, uiFlags);
   }

   #if defined CVLISTCTRL_GRIDLINEMODE
   // Create the pen used to draw the gridlines.
   CPen pen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
   CPen* penOld = pCDC->SelectObject(&pen);
   #endif

   // Now walk through columns and draw text.
   char szText[512];     // Increase this if you have longer text.
   LV_COLUMN lvc;
   memset(&lvc, 0, sizeof(lvc));
   lvc.mask = LVCF_WIDTH;
   CRect rTextClip;
   for (int nColumn = 0; GetColumn(nColumn, &lvc); nColumn++) {


      // First time.
      if (0 == nColumn) {
         rTextClip.left   = lpDrawItemStruct->rcItem.left +
            (plistImages ? 16 : 0);
         rTextClip.top    = lpDrawItemStruct->rcItem.top;
         rTextClip.right  = lpDrawItemStruct->rcItem.left + lvc.cx;
         rTextClip.bottom = lpDrawItemStruct->rcItem.bottom;
      }
      else {
         // Just "move" the rect to the right.
         rTextClip.left  = rTextClip.right;
         rTextClip.right = rTextClip.left + lvc.cx;
      }

      // Get the text.
      lvi.iItem = lpDrawItemStruct->itemID;
      lvi.mask = LVIF_TEXT;
      lvi.iSubItem = nColumn;
      lvi.pszText = szText;
      lvi.cchTextMax = sizeof(szText);
      GetItem(&lvi);

      // Add ellipses if necessary.
      int nTextCount(lstrlen(lvi.pszText));

      CSize sizText(pCDC->GetOutputTextExtent(lvi.pszText,
         nTextCount));

      // Make the clipping rectangle a little smaller for a gap
      // effect between columns.
      rTextClip.right -= 4;

      if (sizText.cx >= rTextClip.Width()) {

         // Make a shorter string, including "..." that fits.
         CSize sizEllipse = pCDC->GetOutputTextExtent("...", 3);

         // Now start dropping characters at the tail until width
         // is correct.
         while (sizText.cx+sizEllipse.cx > (rTextClip.Width())  &&
           nTextCount > 1) {
            lvi.pszText[--nTextCount] = 0;
            sizText = pCDC->GetOutputTextExtent(lvi.pszText,
               nTextCount);
         }

         // Ok, append "...".
         lvi.pszText[nTextCount] = '.';
         lvi.pszText[nTextCount + 1] = '.';
         lvi.pszText[nTextCount + 2] = '.';
         lvi.pszText[nTextCount + 3] = 0;
      }

      // Restore full rect.
      rTextClip.right += 4;

      // Print the text.
      pCDC->ExtTextOut(rTextClip.left + 2, rTextClip.top + 1,
         ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
         lstrlen(lvi.pszText), NULL);

      #if defined CVLISTCTRL_GRIDLINEMODE
      // Draw the gridlines.
      if (GetModeGridLineHorz()) {
		pCDC->MoveTo(rTextClip.left , rTextClip.bottom - 1);
		pCDC->LineTo(rTextClip.right, rTextClip.bottom - 1);
//			pCDC->MoveTo(rTextClip.left + 1, rTextClip.bottom - 1);
//			pCDC->LineTo(rTextClip.right - 1, rTextClip.bottom - 1);
	  }

      if (GetModeGridLineVert()) {
		pCDC->MoveTo(rTextClip.right - 1, rTextClip.bottom -1);
		pCDC->LineTo(rTextClip.right - 1, rTextClip.top -1);
//			pCDC->MoveTo(rTextClip.right - 1, rTextClip.bottom - 1);
//			pCDC->LineTo(rTextClip.right - 1, rTextClip.top);
	  }
      #endif
   }

   // Reset colors if necessary.
   if (lpDrawItemStruct->itemState & ODS_SELECTED) {
      pCDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
      pCDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
   }

   // If focused draw focus rect.
   if (lpDrawItemStruct->itemState & ODS_FOCUS) {
      CRect rTextClip(lpDrawItemStruct->rcItem);
      rTextClip.left += (plistImages ? 16 : 0);
      pCDC->DrawFocusRect(&rTextClip);
   }

   // We must leave the display context as we found it.
   #if defined CVLISTCTRL_GRIDLINEMODE
   pCDC->SelectObject(penOld);
   #endif
}
#endif

// Fix color "bug".
/* virtual */ void CVListCtrl::PreSubclassWindow() 
{
   #if ! defined CVLISTCTRL_ROWMODE

   if ((GetTextBkColor() == CLR_NONE  ||
      GetTextBkColor() == CLR_DEFAULT)  &&
     ::GetSysColor(COLOR_WINDOW) == ::GetSysColor(COLOR_3DFACE)) {

      // If the current background is lighter than average, we use
      // white (unless it's already white - then use light gray).
      if (GetRValue(::GetSysColor(COLOR_3DFACE)) +
        GetGValue(::GetSysColor(COLOR_3DFACE)) +
        GetBValue(::GetSysColor(COLOR_3DFACE)) > (255 * 3 / 2)) {

         if (::GetSysColor(COLOR_3DFACE) == RGB(255,255,255))
            SetTextBkColor(RGB(192,192,192));
         else
            SetTextBkColor(RGB(255,255,255));
      }
      else {
         // Current background is darker than average.  Use dark
         // gray (unless it's already dark gray, then use black).
         if (::GetSysColor(COLOR_3DFACE) == RGB(83,83,83))
            SetTextBkColor(RGB(0,0,0));
         else
            SetTextBkColor(RGB(83,83,83));
      }
   }
   #endif

   // Call the base class member.
   CListCtrl::PreSubclassWindow();
}

#if defined CVLISTCTRL_COLUMNWIDTHS
// Call this from your view's OnInitialUpdate() method.  Pass it
// something like "Software\\MyCompany\\MyApp\\CustomerColWidths".
void CVListCtrl::SetModeColumnWidths(const CString& s)
{
   m_sColumnWidthsKey = s;
 
   // Be sure we really want to do this.
   if (! m_sColumnWidthsKey.IsEmpty()) {
      HKEY hKey(NULL);
//      DWORD dwDisp(0L);

      // First open the key (if it's there).
      if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER,
        m_sColumnWidthsKey, NULL, KEY_READ, &hKey)) {

         // If key must there, open columns until you run out.
         LONG lnReturn(ERROR_SUCCESS);

         for (int nCol = 0; ERROR_SUCCESS == lnReturn; nCol++) {
            CString sValueName;
            sValueName.Format("Column%d", nCol);
            DWORD dwType(0L);
            DWORD dwData(0L);
            DWORD dwDataSize(sizeof(DWORD));
            BYTE* pData = reinterpret_cast<BYTE*> (&dwData);
            
            if (ERROR_SUCCESS == (lnReturn = ::RegQueryValueEx(
              hKey, sValueName, NULL, &dwType, pData,
              &dwDataSize))) 

               SetColumnWidth(nCol, dwData);
         }

         ::RegCloseKey(hKey);
         hKey = NULL;
      }
   }
}
#endif

// When the control is about to be destroyed, we write the column
// widths to the Registry key specified by the client.
void CVListCtrl::OnDestroy() 
{
   CListCtrl::OnDestroy();
   
   #if defined CVLISTCTRL_COLUMNWIDTHS

   if (! m_sColumnWidthsKey.IsEmpty()) {
      HKEY hKey(NULL);
      DWORD dwDisp(0L);

      ::RegCreateKeyEx(HKEY_CURRENT_USER, m_sColumnWidthsKey,
        NULL, "CVListCtrlClass", 0L, KEY_WRITE, NULL, &hKey,
        &dwDisp);

      for (int nCol = 0; IsColumn(nCol); nCol++) {
         CString sValueName;
         sValueName.Format("Column%d", nCol);
         int nData(GetColumnWidth(nCol));
         
         const BYTE* pData = 
            reinterpret_cast<const BYTE*> (&nData);

         ::RegSetValueEx(hKey, sValueName, NULL, REG_DWORD, pData,
            sizeof(DWORD));
      }

      ::RegCloseKey(hKey);
      hKey = NULL;
   }
   #endif
}
