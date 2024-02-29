// AccessButton.cpp : インプリメンテーション ファイル
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
// CAccessButton メッセージ ハンドラ


int CAccessButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBitmapButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	CRect rc;
	
	/*
	** ﾊﾟｽﾜｰﾄﾞﾀｲﾄﾙ
	*/
	LoadBitmaps(IDB_PASSWORD_FOCUS,IDB_PASSWORD_PUSH,IDB_PASSWORD_FOCUS,IDB_PASSWORD_PUSH);
	
	/*
	** ﾊﾟｽﾜｰﾄﾞ表示ｴﾘｱ
	*/
	rc.SetRect(17,32,143,52);
	m_passString = new CStatic();
	m_passString->Create(strUserID,WS_CHILD | WS_VISIBLE | SS_LEFT , rc, this);
	return 0;
}

void CAccessButton::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CBitmapButton::OnClose();
}

void CAccessButton::PasswordDraw() 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください
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



