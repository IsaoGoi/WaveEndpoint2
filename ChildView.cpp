﻿
// ChildView.cpp : CChildView クラスの実装
//

#include "pch.h"
#include "framework.h"
#include "WaveEndPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_pButtonPage = NULL;
	m_pDlgHeadPage = NULL;
}

CChildView::~CChildView()
{
	if (m_pButtonPage)
		delete m_pButtonPage;
	if (m_pDlgHeadPage)
		delete m_pDlgHeadPage;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView メッセージ ハンドラー

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。
}
void CChildView::CreateScreen()
{
	CRect	rc;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_DLGFRAME;

	m_pDlgHeadPage = new CDlgHeadPage(this);

	GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.top = rc.bottom - 110;
	m_pButtonPage = new CButtonPage(this, rc, dwStyle);

}
