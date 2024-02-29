// DialogX.cpp : �����t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DialogX.h"


// CDialogX �_�C�A���O

IMPLEMENT_DYNAMIC(CDialogX, CDialog)

CDialogX::CDialogX(UINT id,CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent)
{
	m_iBack_R = 180;
	m_iBack_G = 200;
	m_iBack_B = 250;
	m_bInitial = FALSE;
}

CDialogX::~CDialogX()
{
}

void CDialogX::SetBackColor(int r,int g,int b)
{
	m_iBack_R = r;
	m_iBack_G = g;
	m_iBack_B = b;
}
void CDialogX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogX, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



BOOL CDialogX::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bInitial == FALSE)
		m_brBack.CreateSolidBrush(RGB(m_iBack_R,m_iBack_G,m_iBack_B));

	m_bInitial = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
// CDialogX ���b�Z�[�W �n���h��

HBRUSH CDialogX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
		case CTLCOLOR_DLG:
		case CTLCOLOR_STATIC:
			pDC->SetBkColor(RGB(m_iBack_R,m_iBack_G,m_iBack_B));
			return(HBRUSH)m_brBack;
			break;
		default:
			break;
	}
	switch(pWnd->GetDlgCtrlID()){
		case IDC_STATIC:
			pDC->SetBkColor(RGB(m_iBack_R,m_iBack_G,m_iBack_B));
			break;
		default:
			break;
	}/* switch */

	return hbr;
}
