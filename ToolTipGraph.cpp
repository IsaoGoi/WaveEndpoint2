#include "pch.h"
#include "vector"
#include "string"
#include "WaveEndPoint.h"

#include "ToolTipGraph.h"


/////////////////////////////////////////////////////////////////////////////
// CToolTipGraph

CToolTipGraph::CToolTipGraph(CWnd *pParent)
{
	memset(szTime,'\0',sizeof(szTime));
	memset(szData,'\0',sizeof(szData));
	memset(szRow, '\0', sizeof(szRow));
	memset(szDiff, '\0', sizeof(szDiff));

	CRect	rc;
	pParent->GetWindowRect(rc);

	rc.right = rc.left+160;
	rc.bottom = rc.top+90;
	m_WinRect = rc;

	CString szClassName = ::AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_INFOBK));
	DWORD dwStyle,dwStyleEx;

	dwStyle = WS_POPUP;
	dwStyleEx = WS_EX_CLIENTEDGE;
	CreateEx(dwStyleEx,(LPCSTR)szClassName, "", dwStyle, rc, pParent,0);
}
CToolTipGraph::~CToolTipGraph()
{
	m_Font.DeleteObject(); 
}


BEGIN_MESSAGE_MAP(CToolTipGraph, CWnd)
	//{{AFX_MSG_MAP(CToolTipGraph)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipGraph メッセージ ハンドラ


void CToolTipGraph::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CFont* def_font;
	def_font = dc.SelectObject(&m_Font);

	dc.SetBkColor(GetSysColor(COLOR_INFOBK));
	dc.TextOut(10,5,szTime,(int)strlen(szTime));
	dc.TextOut(10,25,szData,(int)strlen(szData));
	dc.TextOut(10,45, szRow, (int)strlen(szRow));
	dc.TextOut(10, 65, szDiff, (int)strlen(szDiff));
	dc.SelectObject(def_font);

	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}
void CToolTipGraph::Active(CPoint point) 
{

	CRect	rc;
	rc = m_WinRect;

	if(point.y > rc.Height())
		rc.OffsetRect(point.x,(point.y - (rc.Height() + 5)));
	else
		rc.OffsetRect(point.x,1);

	if(IsWindowVisible() == FALSE)
		ShowWindow(SW_SHOW); 

	MoveWindow(rc);
	Invalidate();

}
void CToolTipGraph::Deactive() 
{
	if(IsWindowVisible() == TRUE)
		ShowWindow(SW_HIDE); 
}

int CToolTipGraph::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_Font.CreateFont(
	   19,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_BOLD,                  // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   "Times New Roman"));       // lpszFacename

	return 0;
}
