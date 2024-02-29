// CDlgRecipeCopy.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgRecipeCopy.h"


// CDlgRecipeCopy ダイアログ
/*
** #define
*/
#define NUM_COLUMNS				4								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_NAME			1
#define	SUB_ITEM_COMMENT		2
#define	SUB_ITEM_DATE			3

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("Rcp No."),
	_T("Recipe Name"),
	_T("Comment"),
	_T("Update"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	100,300, 300, 220
};




IMPLEMENT_DYNAMIC(CDlgRecipeCopy, CDialogX)

CDlgRecipeCopy::CDlgRecipeCopy(CWnd* pParent, int iPCNo, int iRcpNo)
	: CDialogX(IDD_RCPCOPY, pParent)
	, m_szFromNo(_T(""))
	, m_szToNo(_T(""))
	, m_szFromName(_T(""))
{
	m_iFromPCNo = iPCNo;
	m_iFromRcpNo= iRcpNo;
	m_iToPCNo = iPCNo;
	m_iToRcpNo = iRcpNo;

	m_pImage = NULL;
}

CDlgRecipeCopy::~CDlgRecipeCopy()
{
	if (m_pImage)
		delete m_pImage;
}

void CDlgRecipeCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_FROM_NO, m_szFromNo);
	DDX_Text(pDX, IDC_ED_TO_NO, m_szToNo);
	DDX_Text(pDX, IDC_ED_FROM_NAME, m_szFromName);
	DDX_Control(pDX, IDC_ED_TO_NAME, m_io_ToName);
	DDX_Control(pDX, IDC_CMB_FROM, m_cmb_from_pc);
	DDX_Control(pDX, IDC_CMB_TO, m_cmb_to_pc);
	DDX_Control(pDX, IDC_LIST_FROM, m_list_from);
	DDX_Control(pDX, IDC_LIST_TO, m_list_to);
}


BEGIN_MESSAGE_MAP(CDlgRecipeCopy, CDialogX)
	ON_CBN_SELCHANGE(IDC_CMB_FROM, &CDlgRecipeCopy::OnCbnSelchangeCmbFrom)
	ON_CBN_SELCHANGE(IDC_CMB_TO, &CDlgRecipeCopy::OnCbnSelchangeCmbTo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FROM, &CDlgRecipeCopy::OnLvnItemchangedListFrom)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TO, &CDlgRecipeCopy::OnLvnItemchangedListTo)
	ON_BN_CLICKED(IDOK, &CDlgRecipeCopy::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRecipeCopy メッセージ ハンドラー


BOOL CDlgRecipeCopy::OnInitDialog()
{
	int		i;
	int		iCnt;
	int		iCurrPC;
	LV_COLUMN	lvc;
	CString	szStr('\0', 32);

	CDialogX::OnInitDialog();

	m_pRcpCtrl = ((CWaveEndPointApp *)AfxGetApp())->GetPCRcpCtrl();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	m_io_ToName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CopyRcpName"));
	m_io_ToName.SetSpecialColor(2);


	m_pImage = new CImageList();
	m_pImage->Create(IDB_FILE, 16, 1, RGB(255, 255, 255));
	m_list_to.SetImageList(m_pImage, LVSIL_SMALL);

	// from/to Recipe List 初期化
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_from.InsertColumn(i, &lvc);
		m_list_to.InsertColumn(i, &lvc);
	}/* for */
	m_list_from.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_from.SetModeGridLine(TRUE, TRUE);
	m_list_to.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_to.SetModeGridLine(TRUE, TRUE);

	// Combo Box 初期化
	iCurrPC = -1;
	for (iCnt = i = 0; i < PM_MAX; i++) {
		if (m_pConfig->m_tEPDConfig.iPCEnable[i]) {
			szStr.Format("PC%d", (i + 1));
			m_cmb_from_pc.InsertString(iCnt, szStr);
			m_cmb_to_pc.InsertString(iCnt, szStr);
			m_cmb_from_pc.SetItemData(iCnt, i);
			m_cmb_to_pc.SetItemData(iCnt, i);
			if (i == m_iFromPCNo)
				iCurrPC = i;
			iCnt++;
		}
	}/* for */
	if (iCurrPC != -1) {
		m_cmb_from_pc.SetCurSel(iCurrPC);
		m_cmb_to_pc.SetCurSel(iCurrPC);
		OnCbnSelchangeCmbFrom();
		OnCbnSelchangeCmbTo();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgRecipeCopy::OnCbnSelchangeCmbFrom()
{
	int	item;
	int	iPC;

	item = m_cmb_from_pc.GetCurSel();

	if (item == -1)
		return;

	iPC = m_cmb_from_pc.GetItemData(item);
	m_iFromPCNo = iPC;
	SetFromRecipeList(m_iFromPCNo);
}
void CDlgRecipeCopy::OnCbnSelchangeCmbTo()
{
	int	item;
	int	iPC;

	item = m_cmb_to_pc.GetCurSel();

	if (item == -1)
		return;

	iPC = m_cmb_to_pc.GetItemData(item);
	m_iToPCNo = iPC;
	SetToRecipeList(m_iToPCNo);
}
void CDlgRecipeCopy::SetFromRecipeList(int iPC)
{
	int				i;
	int				iRcpNo;
	char			szRecipeName[RCP_LEN_NAME + 1];
	char			szComment[RCP_LEN_COMMENT];
	char			szDateTime[32];
	int				iCnt;
	LV_ITEM			lvi;
	CString			szStr('\0', 64);
	int				iCurrRcpItem=-1;

	m_list_from.DeleteAllItems();

	for (iCnt = i = 0; i < RCP_FILE_MAX; i++) {
		if (i == 0) {
			if (m_pRcpCtrl->GetInfoFirst(iPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}
		else {
			if (m_pRcpCtrl->GetInfoNext(iPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}

		lvi.mask = LVIF_TEXT;
		lvi.iItem = iCnt;
		lvi.iSubItem = SUB_ITEM_NO;

		szStr.Format("%03.03d", iRcpNo);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_from.InsertItem(&lvi);

		m_list_from.SetItemData(iCnt, iRcpNo);

		szStr = szRecipeName;
		m_list_from.SetItemText(iCnt, SUB_ITEM_NAME, (LPCSTR)szStr);

		szStr = szComment;
		m_list_from.SetItemText(iCnt, SUB_ITEM_COMMENT, (LPCSTR)szStr);

		szStr = szDateTime;
		m_list_from.SetItemText(iCnt, SUB_ITEM_DATE, (LPCSTR)szStr);

		if (iRcpNo == m_iFromRcpNo)
			iCurrRcpItem = iCnt;
		iCnt++;
	}/* for */
	if (iCurrRcpItem != -1)
		m_list_from.SelectItem(iCurrRcpItem);
}
void CDlgRecipeCopy::SetToRecipeList(int iPC)
{
	int				i;
	int				iRcpNo;
	char			szRecipeName[RCP_LEN_NAME + 1];
	char			szComment[RCP_LEN_COMMENT];
	char			szDateTime[32];
	LV_ITEM			lvi;
	CString			szStr('\0', 64);
	int				iCurrRcpItem = -1;

	m_list_to.DeleteAllItems();

	for (i = 0; i < RCP_FILE_MAX; i++) {
		if (i == 0) {
			if (m_pRcpCtrl->GetInfoFirst(iPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}
		else {
			if (m_pRcpCtrl->GetInfoNext(iPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}
		for (; i < (iRcpNo - 1); i++) {
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = i;
			lvi.iSubItem = SUB_ITEM_NO;
			lvi.iImage = 1;

			szStr.Format("%03.03d", i + 1);
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			m_list_to.InsertItem(&lvi);

			szStr = _T("");
			m_list_to.SetItemData(i, (i + (int)1));
			m_list_to.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
			m_list_to.SetItemText(i, SUB_ITEM_COMMENT, (LPCSTR)szStr);
			m_list_to.SetItemText(i, SUB_ITEM_DATE, (LPCSTR)szStr);
		}

		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;

		szStr.Format("%03.03d", iRcpNo);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_to.InsertItem(&lvi);

		m_list_to.SetItemData(i, iRcpNo);

		szStr = szRecipeName;
		m_list_to.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);

		szStr = szComment;
		m_list_to.SetItemText(i, SUB_ITEM_COMMENT, (LPCSTR)szStr);

		szStr = szDateTime;
		m_list_to.SetItemText(i, SUB_ITEM_DATE, (LPCSTR)szStr);

		if (iRcpNo == m_iToRcpNo)
			iCurrRcpItem = i;


	}/* for */
	for (; i < RCP_FILE_MAX; i++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 1;

		szStr.Format("%03.03d", i + 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_to.InsertItem(&lvi);

		szStr = _T("");
		m_list_to.SetItemData(i, (i + (int)1));
		m_list_to.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_to.SetItemText(i, SUB_ITEM_COMMENT, (LPCSTR)szStr);
		m_list_to.SetItemText(i, SUB_ITEM_DATE, (LPCSTR)szStr);
	}
	if (iCurrRcpItem != -1)
		m_list_to.SelectItem(iCurrRcpItem);
	else {
		memset(szRecipeName, '\0', sizeof(szRecipeName));
		m_pIODB->IOPointWrite(m_io_ToName.GetIOHdl(), szRecipeName);
		m_io_ToName.UpdateDisplay();
	}

}


void CDlgRecipeCopy::OnLvnItemchangedListFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	int	item;

	if (pNMLV->uNewState & LVIS_SELECTED) {
		item = pNMLV->iItem;
		m_iFromRcpNo = (int)m_list_from.GetItemData(item);
		m_szFromName = m_list_from.GetItemText(item, SUB_ITEM_NAME);
		m_szFromNo.Format("%03.03d", m_iFromRcpNo);
		UpdateData(FALSE);
	}
}


void CDlgRecipeCopy::OnLvnItemchangedListTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	int		item;
	char	szRcpName[40];

	if (pNMLV->uNewState & LVIS_SELECTED) {
		item = pNMLV->iItem;
		m_iToRcpNo = (int)m_list_to.GetItemData(item);
		m_list_to.GetItemText(item, SUB_ITEM_NAME, szRcpName,sizeof(szRcpName));
		m_szToNo.Format("%03.03d", m_iToRcpNo);
		m_pIODB->IOPointWrite(m_io_ToName.GetIOHdl(), szRcpName);
		m_io_ToName.UpdateDisplay();

		UpdateData(FALSE);
	}
}


void CDlgRecipeCopy::OnBnClickedOk()
{
	CRecipe* pRcp;
	char		szRcpName[40];

	if (m_iToRcpNo) {
		m_pIODB->IOPointRead(m_io_ToName.GetIOHdl(), szRcpName);
		if (strchr(szRcpName, ' '))
			*(strchr(szRcpName, ' ')) = '\0';
		if (strlen(szRcpName) == 0) {
			MessageBox(_T("Recipe Name is Empty!"), _T("Recipe Copy"), MB_ICONERROR);
			return;
		}
		if (m_pRcpCtrl->CheckRcpNameExists(m_iToPCNo, m_iToRcpNo, szRcpName) == FALSE) {
			MessageBox(_T("Dupulicate Recipe Name!"), _T("Recipe Copy"), MB_ICONERROR);
			return;
		}


		if (m_iFromRcpNo) {
			pRcp = new CRecipe();

			if (pRcp->RecipeFileLoad(m_iFromPCNo, m_iFromRcpNo) != RCP_FILE_SUCCESS) {
				MessageBox(_T("Source Recipe Load Error!"), _T("Recipe Copy"), MB_ICONERROR);
				delete pRcp;
				return;
			}
			strcpy(pRcp->m_tRecipe.szRecipeName, szRcpName);
			if (pRcp->RecipeFileSave(m_iToPCNo, m_iToRcpNo) != RCP_FILE_SUCCESS) {
				MessageBox(_T("Destination Recipe Save Error!"), _T("Recipe Copy"), MB_ICONERROR);
				delete pRcp;
				return;
			}
			delete pRcp;
		}
		else {
			MessageBox(_T("Invalid Source Recipe No.!"), _T("Recipe Copy"), MB_ICONERROR);
			return;
		}
	}
	else {
		MessageBox(_T("Invalid Destination Recipe No.!"), _T("Recipe Copy"), MB_ICONERROR);
		return;
	}
	OnOK();
}
