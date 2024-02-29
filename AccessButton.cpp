// AccessButton.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "AccessButton.h"
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessButton

CAccessButton::CAccessButton()
{
	strUserID.Empty();
	strUserFullName.Empty();
	iUserLevel = 0;

}

CAccessButton::~CAccessButton()
{
	delete m_passString;
}


BEGIN_MESSAGE_MAP(CAccessButton, CBitmapButton)
	//{{AFX_MSG_MAP(CAccessButton)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessButton ���b�Z�[�W �n���h��


int CAccessButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBitmapButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	CRect rc;
	
	/*
	** �߽ܰ������
	*/
	LoadBitmaps(IDB_PASSWORD_FOCUS,IDB_PASSWORD_PUSH,IDB_PASSWORD_FOCUS,IDB_PASSWORD_PUSH);
	
	/*
	** �߽ܰ�ޕ\���ر
	*/
	rc.SetRect(17,32,143,52);
	m_passString = new CStatic();
	m_passString->Create(strUserID,WS_CHILD | WS_VISIBLE | SS_LEFT , rc, this);
	return 0;
}

void CAccessButton::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CBitmapButton::OnClose();
}

void CAccessButton::PasswordDraw() 
{
	// TODO: �w�肳�ꂽ�A�C�e����`�悷�邽�߂̃R�[�h��ǉ����Ă�������
	CAccessLevel	*m_pAccessLevel;
	char	szUserName[32];
	
	m_pAccessLevel = ((CWaveEndPointApp*)AfxGetApp())->GetAccessLevelClass();
	m_pAccessLevel->GetCurrUser(szUserName);
	strUserID = szUserName;
	strUserID.TrimRight();
	m_passString->SetWindowText(strUserID);	
}

void CAccessButton::OnClicked() 
{
	InputLogOn();
}
void CAccessButton::InputLogOn() 
{
	CDlgPassword dlg;
	INT_PTR		iRet;

	iRet = dlg.DoModal();
	if(iRet == IDOK){
		PasswordDraw();
	}
	((CWaveEndPointApp *)AfxGetApp())->ButtonPageUpdate();
}



