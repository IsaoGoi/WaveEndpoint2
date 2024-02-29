// CmdButton.cpp : �����t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "ButtonPage.h"
//#include "CmdButton.h"


// CCmdButton

IMPLEMENT_DYNAMIC(CCmdButton, CBitmapButton)

CCmdButton::CCmdButton()
{

}

CCmdButton::~CCmdButton()
{
}


BEGIN_MESSAGE_MAP(CCmdButton, CBitmapButton)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// CCmdButton ���b�Z�[�W �n���h��



LRESULT CCmdButton::OnNcHitTest(CPoint point)
{
	CButtonPage	*pPage =(CButtonPage *)m_pParentWnd;
	pPage->ChkMouseMove(point);
	return CBitmapButton::OnNcHitTest(point);
}

BOOL CCmdButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pParentWnd = pParentWnd;

	return CBitmapButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}
