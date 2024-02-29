// DlgUserList.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgUserList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUM_COLUMNS				3								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_USERID			1
#define	SUB_ITEM_LEVEL			2

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."), 
	_T("User ID"), 
	_T("Level"), 
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] = 
{
	60, 200, 200
};

static char *_gszUserLevel[8] = 
{
	_T("None"),
	_T("Operator"),
	_T("Engineer"),
	NULL,
};


/////////////////////////////////////////////////////////////////////////////
// CDlgUserList ダイアログ


CDlgUserList::CDlgUserList(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgUserList::IDD, pParent)
	
{
	//{{AFX_DATA_INIT(CDlgUserList)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_img_Status = NULL;
	m_iEditItem = -1;
	CDialogX::Create(IDD, pParent);
}
CDlgUserList::~CDlgUserList()
{
	m_pIODB->IOPointRead(m_ioAutoLogOff.GetIOHdl(), &m_pAccessLevel->m_iAutoLogOffTime);

	if(m_img_Status){
		delete m_img_Status;
		m_img_Status = NULL;
	}
	CDialogX::DestroyWindow();
}


void CDlgUserList::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserList)
	DDX_Control(pDX, IDC_USELIST, m_list_user);
	DDX_Control(pDX, IDC_CMB_LEVEL, m_cmb_level);
	DDX_Control(pDX, IDC_ED_PASSWORD, m_io_Password);
	DDX_Control(pDX, IDC_ED_CONFIRM, m_io_ConfirmPassword);
	DDX_Control(pDX, IDC_ED_AUTOLOGOFF, m_ioAutoLogOff);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ED_USERID, m_io_UserName);
}

BEGIN_MESSAGE_MAP(CDlgUserList, CDialogX)
	//{{AFX_MSG_MAP(CDlgUserList)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
//	ON_NOTIFY(NM_CLICK, IDC_USELIST, &CDlgUserList::OnNMClickUselist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USELIST, &CDlgUserList::OnLvnItemchangedUselist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserList メッセージ ハンドラ

int CDlgUserList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogX::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc;
	GetParent()->GetClientRect(rc);
	rc.DeflateRect(160, 30);
	rc.OffsetRect(140, 0);
	MoveWindow(rc);
	return 0;
}

void CDlgUserList::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(m_img_Status){
		delete m_img_Status;
		m_img_Status = NULL;
	}
	CDialogX::OnClose();
	CDialogX::DestroyWindow();
}

BOOL CDlgUserList::OnInitDialog() 
{
	int	i;

	CDialogX::OnInitDialog();

	m_pAccessLevel = ((CWaveEndPointApp*)AfxGetApp())->GetAccessLevelClass();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	for(i=0;_gszUserLevel[i+1] != NULL ;i++){
		m_cmb_level.InsertString(i,_gszUserLevel[i+1]);
		m_cmb_level.SetItemData(i,i);
	}
	m_io_UserName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.UserID"));
	m_io_Password.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.Password"));
	m_io_ConfirmPassword.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.ConfPass"));

	m_io_UserName.SetSpecialColor(1);
	m_io_Password.SetSpecialColor(1);
	m_io_ConfirmPassword.SetSpecialColor(1);

	m_io_Password.SetPasswordFld();
	m_io_ConfirmPassword.SetPasswordFld();

	m_ioAutoLogOff.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.AutoLogOffTime"));

	m_pIODB->IOPointWrite(m_ioAutoLogOff.GetIOHdl(),&m_pAccessLevel->m_iAutoLogOffTime);

	// TODO: この位置に初期化の補足処理を追加してください
	m_img_Status = new CImageList();

	m_img_Status->Create(IDB_ENABLE, 16, 2, RGB(255, 255, 255));
	m_list_user.SetImageList(m_img_Status, LVSIL_SMALL);

	
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_user.InsertColumn(i,&lvc);
	}/* for */
	m_list_user.ModifyStyle(LVS_TYPEMASK,((LVS_REPORT & LVS_TYPEMASK)
							 | LVS_SHOWSELALWAYS  | LVS_OWNERDRAWFIXED ));
	

	m_list_user.SetModeGridLine();

	SetUserList();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
void CDlgUserList::SetUserList()
{
	int			i;
	LV_ITEM		lvi;
	CString		szStr;
	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	ACCESSLEVEL AccessLevel;
	DWORD_PTR	dwData;

	m_list_user.DeleteAllItems();

	for(i=0;i<USER_MAX;i++){
		m_pAccessLevel->GetUser(i,szUserName,szPassword,&AccessLevel);
		
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;
		if(AccessLevel ==NONELEVEL)
			lvi.iImage = 0;
		else
			lvi.iImage = 1;

		szStr.Format("%02.02d",(i+1));
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_user.InsertItem(&lvi);


		szStr.Format("%s",szUserName);
		m_list_user.SetItemText(i,SUB_ITEM_USERID,(LPCSTR)szStr);

		szStr.Format("%s",_gszUserLevel[AccessLevel]);
		m_list_user.SetItemText(i,SUB_ITEM_LEVEL,(LPCSTR)szStr);


		dwData = i;

		if(AccessLevel ==NONELEVEL)
			dwData += 10000;
		else if(AccessLevel <=OPERATOR)
			dwData += 40000;
		else if(AccessLevel <=ENGINEER)
			dwData += 30000;
		else 	
			dwData += 40000;

		m_list_user.SetItemData(i,dwData);
	}/* for */

	GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

}


void CDlgUserList::OnBtnDelete() 
{
	int	iItem;
	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	ACCESSLEVEL AccessLevel;
	LV_ITEM		lvi;
	CString		szStr('\0',64);
	DWORD_PTR	dwData;
	int			i;

	iItem = m_list_user.GetSelectedItem();
	if(iItem < 0)
		return;

	m_pAccessLevel->GetUser(iItem,szUserName,szPassword,&AccessLevel);
	if(AccessLevel == NONELEVEL)
		return;

	szStr.Format("%s Delete OK?",szUserName);
	if(MessageBox(szStr,NULL,MB_YESNO) == IDYES){
		m_pAccessLevel->DeleteUser(iItem);

		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = iItem;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 0;

		m_pAccessLevel->GetUser(iItem,szUserName,szPassword,&AccessLevel);

		szStr.Format("%02.02d",(iItem+1));
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_user.SetItem(&lvi);

		szStr.Format("%s",szUserName);
		m_list_user.SetItemText(iItem,SUB_ITEM_USERID,(LPCSTR)szStr);

		szStr.Format("%s",_gszUserLevel[AccessLevel]);
		m_list_user.SetItemText(iItem,SUB_ITEM_LEVEL,(LPCSTR)szStr);

		dwData = iItem;
		dwData += 10000;
		m_list_user.SetItemData(iItem,dwData);


		m_pIODB->IOPointWrite(m_io_UserName.GetIOHdl(),szUserName);
		m_pIODB->IOPointWrite(m_io_Password.GetIOHdl(),szPassword);
		m_pIODB->IOPointWrite(m_io_ConfirmPassword.GetIOHdl(),szPassword);
		m_io_UserName.UpdateDisplay();
		m_io_Password.UpdateDisplay();
		m_io_ConfirmPassword.UpdateDisplay();

		m_cmb_level.SetCurSel(0);

		if(AccessLevel == NONELEVEL)
			GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);

		GetDlgItem(IDOK)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
	}
}


BOOL CDlgUserList::ModifyCheck()
{
	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	char		szConfirmPassword[PASSWORD_LEN+1];
	int			iLevel;
	ACCESSLEVEL AccessLevel;

	char		szOldUserName[USER_NAME_LEN+1];
	char		szOldPassword[PASSWORD_LEN+1];
	ACCESSLEVEL OldAccessLevel;

	int			i;
	int			bChange = FALSE;


	if(m_iEditItem == -1){
		return TRUE;
	}
	CAccessLevel *pAccessLevel = ((CWaveEndPointApp *)AfxGetApp())->GetAccessLevelClass();
	pAccessLevel->GetUser(m_iEditItem,szOldUserName,szOldPassword,&OldAccessLevel);

	m_pIODB->IOPointRead(m_io_UserName.GetIOHdl(),szUserName);
	m_pIODB->IOPointRead(m_io_Password.GetIOHdl(),szPassword);
	m_pIODB->IOPointRead(m_io_ConfirmPassword.GetIOHdl(),szConfirmPassword);

	iLevel = m_cmb_level.GetCurSel();
	if(iLevel < 0){
		iLevel = 0;
	}
	else
		iLevel++;

	AccessLevel = (ACCESSLEVEL)iLevel;



	if(strcmp(szUserName,szOldUserName))
		bChange = TRUE;
	else if(strcmp(szPassword,szOldPassword))
		bChange = TRUE;
	else if(AccessLevel != OldAccessLevel && OldAccessLevel != NONELEVEL)
		bChange = TRUE;

	if(bChange){
		if(MessageBox("Some Parameters are changed.\nDo you wannt to Save?",NULL,MB_YESNO) == IDYES)
			OnOK();
	}
	return TRUE;
}


void CDlgUserList::OnLvnItemchangedUselist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int			iItem;
	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	ACCESSLEVEL AccessLevel;
	int			i;


	iItem = m_list_user.GetSelectedItem();

	if(pNMLV->uNewState > 2 && iItem >= 0){
		if(m_iEditItem != iItem && m_iEditItem != -1)
			ModifyCheck();


		m_pAccessLevel->GetUser(iItem,szUserName,szPassword,&AccessLevel);

		m_pIODB->IOPointWrite(m_io_UserName.GetIOHdl(),szUserName);
		m_pIODB->IOPointWrite(m_io_Password.GetIOHdl(),szPassword);
		m_pIODB->IOPointWrite(m_io_ConfirmPassword.GetIOHdl(),szPassword);
		m_io_UserName.UpdateDisplay();
		m_io_Password.UpdateDisplay();
		m_io_ConfirmPassword.UpdateDisplay();

		m_cmb_level.SetCurSel((AccessLevel-1));

		if(AccessLevel == NONELEVEL)
			GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_iEditItem = iItem;	
	}
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;
}
void CDlgUserList::OnOK()
{

	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	char		szConfirmPassword[PASSWORD_LEN+1];
	int			iLevel;
	int			i;
	int			rv;
	LV_ITEM		lvi;
	CString		szStr('\0',64);
	ACCESSLEVEL AccessLevel;
	DWORD_PTR	dwData;

	m_pIODB->IOPointRead(m_io_UserName.GetIOHdl(),szUserName);
	m_pIODB->IOPointRead(m_io_Password.GetIOHdl(),szPassword);
	m_pIODB->IOPointRead(m_io_ConfirmPassword.GetIOHdl(),szConfirmPassword);

	if(strlen(szUserName) == 0){
		MessageBox("Invalid User Name.");
		return;
	}
	if(strlen(szPassword) == 0){
		MessageBox("Invalid Pasword.");
		return;
	}
	if(strlen(szConfirmPassword) == 0){
		MessageBox("Invalid Confirm Pasword.");
		return;
	}
	if(strcmp(szPassword,szConfirmPassword)){
		MessageBox("Invalid Confirm Pasword.");
		return;
	}
	iLevel = m_cmb_level.GetCurSel();
	if(iLevel < 0){
		MessageBox("Invalid Access Level");
		return;
	}
	iLevel++;
	AccessLevel = (ACCESSLEVEL)iLevel;

	CAccessLevel *pAccessLevel = ((CWaveEndPointApp *)AfxGetApp())->GetAccessLevelClass();

	rv = pAccessLevel->EditUser(m_iEditItem,szUserName,szPassword,AccessLevel);
	if(rv){
		switch(rv){
		case USER_ERR_USERNAME:
			MessageBox("Invalid User Name");
			break;
		case USER_ERR_PASSWORD:
			MessageBox("Invalid Password");
			break;
		case USER_ERR_DUPLICATE:
			MessageBox("Dupulicate User Name.");
			break;
		case USER_ERR_USERID:
			MessageBox("Invalid User ID.");
			break;
		case USER_ERR_USERID_DUPLICATE:
			MessageBox("Dupulicate User ID.");
			break;
		}/* switch */
		return;
	}


	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = m_iEditItem;
	lvi.iSubItem = SUB_ITEM_NO;
	lvi.iImage = 1;

	m_pAccessLevel->GetUser(m_iEditItem,szUserName,szPassword,&AccessLevel);

	szStr.Format("%02.02d",(m_iEditItem+1));
	lvi.pszText = (LPSTR)((LPCSTR)szStr);
	m_list_user.SetItem(&lvi);

	szStr.Format("%s",szUserName);
	m_list_user.SetItemText(m_iEditItem,SUB_ITEM_USERID,(LPCSTR)szStr);

	szStr.Format("%s",_gszUserLevel[AccessLevel]);
	m_list_user.SetItemText(m_iEditItem,SUB_ITEM_LEVEL,(LPCSTR)szStr);

	dwData = m_iEditItem;

		if(AccessLevel ==NONELEVEL)
			dwData += 10000;
		else if(AccessLevel <=OPERATOR)
			dwData += 20000;
		else if(AccessLevel <=ENGINEER)
			dwData += 30000;
		else 	
			dwData += 40000;

	m_list_user.SetItemData(m_iEditItem,dwData);
	GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);

}

