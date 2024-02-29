// UpButton.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "UpButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpButton

CUpButton::CUpButton()
{
}

CUpButton::~CUpButton()
{
}


BEGIN_MESSAGE_MAP(CUpButton, CButton)
	//{{AFX_MSG_MAP(CUpButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpButton ���b�Z�[�W �n���h��

void CUpButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StartUpDown(1);
	CButton::OnLButtonDown(nFlags, point);
}

void CUpButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StopUpDown();
	CButton::OnLButtonUp(nFlags, point);
}

void CUpButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StartUpDown(1);
	CButton::OnLButtonDblClk(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// CDownButton

CDownButton::CDownButton()
{
}

CDownButton::~CDownButton()
{
}


BEGIN_MESSAGE_MAP(CDownButton, CButton)
	//{{AFX_MSG_MAP(CDownButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownButton ���b�Z�[�W �n���h��

void CDownButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StartUpDown(2);
	CButton::OnLButtonDown(nFlags, point);
}

void CDownButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StopUpDown();
	CButton::OnLButtonUp(nFlags, point);
}


void CDownButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	((CDlgNumEdit *)GetParent())->StartUpDown(2);
	CButton::OnLButtonDblClk(nFlags, point);
}


