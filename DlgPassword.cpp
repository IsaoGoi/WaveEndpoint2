// DlgPassword.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgPassword.h"


// CDlgPassword ダイアログ

IMPLEMENT_DYNAMIC(CDlgPassword, CDialogX)

CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPassword)
	m_strPassword = _T("");
	m_strUserID = _T("");
	//}}AFX_DATA_INIT
}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPassword)
	DDX_Control(pDX, IDC_BTNCLEAR, m_BtnClear);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 8);
	DDX_Text(pDX, IDC_USERID, m_strUserID);
	DDV_MaxChars(pDX, m_strUserID, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialogX)
	//{{AFX_MSG_MAP(CDlgPassword)
	ON_BN_CLICKED(IDC_BTNCLEAR, OnBtnclear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgPassword::OnBtnclear() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAccessLevel	*pAccessLevel = ((CWaveEndPointApp *)AfxGetApp())->GetAccessLevelClass();
	pAccessLevel->LogOff(FALSE);
	((CWaveEndPointApp*)AfxGetApp())->ChangeMainScreen();
	CDialogX::OnOK();
}


void CDlgPassword::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CDialogX::OnCancel();
}

void CDlgPassword::OnOK()
{
	UpdateData(TRUE);
	char	szUserID[50];
	char	szPassword[50];

	/*
	** 入力ｺｰﾄﾞのﾁｪｯｸ
	*/
	m_strUserID.TrimLeft();
	m_strUserID.TrimRight();
	if(m_strUserID.IsEmpty()){
		MessageBox("Invalid User ID",NULL,MB_ICONERROR);
		return;
	}
	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();
	if(m_strPassword.IsEmpty()){
		MessageBox("Invalid Password",NULL,MB_ICONERROR);
		return;
	}
	strcpy_s(szUserID,sizeof(szUserID),(LPCSTR)m_strUserID);
	strcpy_s(szPassword,sizeof(szPassword),(LPCSTR)m_strPassword);



	CAccessLevel	*pAccessLevel = ((CWaveEndPointApp*)AfxGetApp())->GetAccessLevelClass();
	int rv = pAccessLevel->LogOn(szUserID,szPassword);
	if(rv){
		if(rv ==USER_ERR_USERNAME)
			MessageBox("Invalid User ID",NULL,MB_ICONERROR);
		else
			MessageBox("Invalid Password",NULL,MB_ICONERROR);
		return;
	}

	CDialogX::OnOK();
}
