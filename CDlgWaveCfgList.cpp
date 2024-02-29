// CDlgWaveCfgList.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgWaveCfgList.h"

/*
** #define
*/
#define NUM_COLUMNS				7								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_NAME			1
#define	SUB_ITEM_COMMENT		2
#define	SUB_ITEM_DATE			3
#define	SUB_ITEM_BASEWAVE1		4
#define	SUB_ITEM_BASEWAVE2		5
#define	SUB_ITEM_BASEWAVE3		6

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."),
	_T("Wave Config Name"),
	_T("Comment"),
	_T("Update"),
	_T("Wave-1(A)"),
	_T("Wave-2(B1)"),
	_T("Wave-3(B2)"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	60,160, 160, 120,100,100,100
};

static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };

// CDlgWaveCfgList ダイアログ

IMPLEMENT_DYNAMIC(CDlgWaveCfgList, CDialogX)

CDlgWaveCfgList::CDlgWaveCfgList(CWnd* pParent /*=nullptr*/,int iPM,int bSave,int iWaveCfgNo)
	: CDialogX(IDD_WAVECFG_LIST, pParent)
{
	m_iPMNo = iPM;
	m_iSave = bSave;
	m_iWaveCfgNo = iWaveCfgNo;
	m_pImage = NULL;

}

CDlgWaveCfgList::~CDlgWaveCfgList()
{
	if (m_pImage)
		delete m_pImage;
}

void CDlgWaveCfgList::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WAVECFG, m_list_WaveCfg);
	DDX_Control(pDX, IDC_ED_NAME, m_io_CfgName);
	DDX_Control(pDX, IDC_ED_COMMENT, m_io_CfgComment);
}


BEGIN_MESSAGE_MAP(CDlgWaveCfgList, CDialogX)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVECFG, &CDlgWaveCfgList::OnLvnItemchangedListWavecfg)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CDlgWaveCfgList::OnBnClickedBtnRemove)
END_MESSAGE_MAP()


// CDlgWaveCfgList メッセージ ハンドラー


BOOL CDlgWaveCfgList::OnInitDialog()
{
	int			i;
	LV_COLUMN	lvc;

	CDialogX::OnInitDialog();

	m_pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	m_bmpPC.LoadBitmapA(m_uidPCBmp[m_iPMNo]);
	((CStatic*)GetDlgItem(IDC_BMP_PC))->SetBitmap(m_bmpPC);

	m_io_CfgName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgWaveName"));
	m_io_CfgComment.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgWaveComment"));
	m_io_CfgName.SetSpecialColor(2);
	m_io_CfgComment.SetSpecialColor(2);



	/*
	** Wave Configリスト初期化
	*/

	m_pImage = new CImageList();
	m_pImage->Create(IDB_FILE, 16, 1, RGB(255, 255, 255));
	m_list_WaveCfg.SetImageList(m_pImage, LVSIL_SMALL);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_WaveCfg.InsertColumn(i, &lvc);
	}/* for */
	m_list_WaveCfg.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));

	m_list_WaveCfg.SetModeGridLine(TRUE, TRUE);

	GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);

	SetWaveCfgList();

	if (m_iSave) {
		GetDlgItem(IDOK)->SetWindowTextA(_T("Save"));
		GetDlgItem(IDC_ED_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_ED_COMMENT)->EnableWindow(TRUE);
		m_pIODB->IOPointWrite(m_io_CfgName.GetIOHdl(), m_szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_CfgComment.GetIOHdl(), m_szWaveCfgComment);

	}
	else {
		GetDlgItem(IDOK)->SetWindowTextA(_T("Load"));
		GetDlgItem(IDC_ED_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_COMMENT)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgWaveCfgList::SetWaveCfgList()
{
	int				i;
	int				iWaveCfgNo;
	char			szWaveCfgName[RCP_LEN_NAME + 1];
	char			szComment[RCP_LEN_COMMENT];
	int				iBaseWave[5];
	char			szDateTime[32];
	int				iCurrWaveCfgNo = 0;
	LV_ITEM			lvi;
	CString			szStr('\0', 64);
	CTime			f_time;

	for (i = 0; i < WAVECFG_FILE_MAX; i++) {
		if (i == 0) {
			if (m_pWaveCfgCtrl->GetInfoFirst(m_iPMNo,&iWaveCfgNo, szWaveCfgName, szComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szDateTime) == FALSE)
				break;
		}
		else {
			if (m_pWaveCfgCtrl->GetInfoNext(m_iPMNo, &iWaveCfgNo, szWaveCfgName, szComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szDateTime) == FALSE)
				break;
		}

		for (; iCurrWaveCfgNo < (iWaveCfgNo - 1); iCurrWaveCfgNo++) {
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = iCurrWaveCfgNo;
			lvi.iSubItem = SUB_ITEM_NO;
			lvi.iImage = 1;

			szStr.Format("%03.03d", iCurrWaveCfgNo + 1);
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			m_list_WaveCfg.InsertItem(&lvi);


			szStr = _T("");
			m_list_WaveCfg.SetItemData(iCurrWaveCfgNo, (iCurrWaveCfgNo + (int)1));
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_NAME, (LPCSTR)szStr);
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_DATE, (LPCSTR)szStr);
			szStr = _T("None");
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE1, (LPCSTR)szStr);
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE2, (LPCSTR)szStr);
			m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE3, (LPCSTR)szStr);
		}
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = iCurrWaveCfgNo;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 0;

		szStr.Format("%03.03d", iWaveCfgNo);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_WaveCfg.InsertItem(&lvi);

		m_list_WaveCfg.SetItemData(iCurrWaveCfgNo, (iWaveCfgNo + (int)1000));

		szStr = szWaveCfgName;
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_NAME, (LPCSTR)szStr);

		szStr = szComment;
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);

		szStr = szDateTime;
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_DATE, (LPCSTR)szStr);

		if (iBaseWave[0])
			szStr.Format("%d:%6.2fum", iBaseWave[0], m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[iBaseWave[0] - 1]);
		else
			szStr = _T("None");
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE1, (LPCSTR)szStr);

		if (iBaseWave[1])
			szStr.Format("%d:%6.2fum", iBaseWave[1], m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[iBaseWave[1] - 1]);
		else
			szStr = _T("None");
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE2, (LPCSTR)szStr);

		if (iBaseWave[2])
			szStr.Format("%d:%6.2fum", iBaseWave[2], m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[iBaseWave[2] - 1]);
		else
			szStr = _T("None");
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE3, (LPCSTR)szStr);


		iCurrWaveCfgNo++;
	}/* for */

	for (; iCurrWaveCfgNo < WAVECFG_FILE_MAX; iCurrWaveCfgNo++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = iCurrWaveCfgNo;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 1;

		szStr.Format("%03.03d", iCurrWaveCfgNo + 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_WaveCfg.InsertItem(&lvi);

		szStr = _T("");
		m_list_WaveCfg.SetItemData(iCurrWaveCfgNo, (iCurrWaveCfgNo + (int)1));
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_DATE, (LPCSTR)szStr);
		szStr = _T("None");
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE1, (LPCSTR)szStr);
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE2, (LPCSTR)szStr);
		m_list_WaveCfg.SetItemText(iCurrWaveCfgNo, SUB_ITEM_BASEWAVE3, (LPCSTR)szStr);
	}
	if (m_iWaveCfgNo) {
		m_list_WaveCfg.SelectItem(m_iWaveCfgNo - 1);

		if (m_pWaveCfgCtrl->GetWaveCfgInfo(m_iPMNo, m_iWaveCfgNo, szWaveCfgName, szComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szDateTime) == 0) {
			memset(szWaveCfgName, '\0', sizeof(szWaveCfgName));
			memset(szComment, '\0', sizeof(szComment));

		}
		szStr.Format("%03.03d", m_iWaveCfgNo);
		GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

		m_pIODB->IOPointWrite(m_io_CfgName.GetIOHdl(), szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_CfgComment.GetIOHdl(), szComment);
	}
}
void CDlgWaveCfgList::OnOK()
{
	int		item;
	int		iCheckCfgNo;
	CString	szMessage('\0',64);

	// Load
	if (m_iSave == FALSE) {
		item = m_list_WaveCfg.GetSelectedItem();
		if (item < 0) {
			MessageBox(_T("File is not Select!"), "Wave Config File", MB_ICONERROR);
			return;
		}
		m_iWaveCfgNo = m_list_WaveCfg.GetItemData(item);
		m_iWaveCfgNo %= 1000;

		m_pIODB->IOPointRead(m_io_CfgName.GetIOHdl(), m_szWaveCfgName);
		m_pIODB->IOPointRead(m_io_CfgComment.GetIOHdl(), m_szWaveCfgComment);

	}
	// Save
	else {
		item = m_list_WaveCfg.GetSelectedItem();
		if (item < 0) {
			MessageBox(_T("File is not Select!"), "Wave Config File", MB_ICONERROR);
			return;
		}
		m_iWaveCfgNo = m_list_WaveCfg.GetItemData(item);
		m_iWaveCfgNo %= 1000;

		m_pIODB->IOPointRead(m_io_CfgName.GetIOHdl(), m_szWaveCfgName);
		m_pIODB->IOPointRead(m_io_CfgComment.GetIOHdl(), m_szWaveCfgComment);
		if (strchr(m_szWaveCfgName, ' '))
			*(strchr(m_szWaveCfgName, ' ')) = '\0';
		if (strlen(m_szWaveCfgName) == 0) {
			MessageBox(_T("Invalid Config Name!"), "Wave Config File", MB_ICONERROR);
			return;
		}
		iCheckCfgNo = m_pWaveCfgCtrl->FindWaveCfgbyName(m_iPMNo, m_szWaveCfgName);
		if (iCheckCfgNo) {
			if (iCheckCfgNo != m_iWaveCfgNo) {
				szMessage.Format("Dupulicate Confing Name! (No.%03.03d)", iCheckCfgNo);
				MessageBox(szMessage, "Wave Config File", MB_ICONERROR);
				return;
			}
		}
	}

	CDialogX::OnOK();
}


void CDlgWaveCfgList::OnLvnItemchangedListWavecfg(NMHDR* pNMHDR, LRESULT* pResult)
{
	int				item;
	int				iWaveCfgNo;
	char			szWaveCfgName[RCP_LEN_NAME + 1];
	char			szComment[RCP_LEN_COMMENT];
	int				iBaseWave[5];
	char			szDateTime[32];
	CString			szStr('\0', 16);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->uNewState & LVIS_SELECTED) {
		item = pNMLV->iItem;
		iWaveCfgNo = m_list_WaveCfg.GetItemData(item);
		if(iWaveCfgNo > 1000)
			GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);
		iWaveCfgNo %= 1000;
		szStr.Format("%03.03d", iWaveCfgNo);
		GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

		if (m_pWaveCfgCtrl->GetWaveCfgInfo(m_iPMNo, iWaveCfgNo, szWaveCfgName, szComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szDateTime) == FALSE) {
			memset(szWaveCfgName, '\0', sizeof(szWaveCfgName));
			memset(szComment, '\0', sizeof(szComment));
		}
		m_pIODB->IOPointWrite(m_io_CfgName.GetIOHdl(), szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_CfgComment.GetIOHdl(), szComment);
		m_io_CfgName.UpdateDisplay();
		m_io_CfgComment.UpdateDisplay();

	}
	*pResult = 0;
}


void CDlgWaveCfgList::OnBnClickedBtnRemove()
{
	int		item;
	int		iCheckCfgNo;
	CString	szMessage('\0', 64);

	item = m_list_WaveCfg.GetSelectedItem();
	if (item < 0) {
		MessageBox(_T("File is not Select!"), "Wave Config File", MB_ICONERROR);
		return;
	}
	m_iWaveCfgNo = m_list_WaveCfg.GetItemData(item);
	if (m_iWaveCfgNo < 1000)
		return;

	m_iWaveCfgNo %= 1000;
	m_pIODB->IOPointRead(m_io_CfgName.GetIOHdl(), m_szWaveCfgName);
	m_pIODB->IOPointRead(m_io_CfgComment.GetIOHdl(), m_szWaveCfgComment);
	szMessage.Format("Do you want Delete WaveConfig? (No.%03.03d:%s)", m_iWaveCfgNo, m_szWaveCfgName);
	if (MessageBox(szMessage, _T("Wave Config File"), MB_YESNO) == IDYES) {
		CCalcWaveCfg* pWaveCfg = new CCalcWaveCfg();
		pWaveCfg->DeleteWaveCfg(m_iPMNo, m_iWaveCfgNo);

		memset(m_szWaveCfgName, '\0', sizeof(m_szWaveCfgName));
		memset(m_szWaveCfgComment, '\0', sizeof(m_szWaveCfgComment));
		m_pIODB->IOPointWrite(m_io_CfgName.GetIOHdl(), m_szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_CfgComment.GetIOHdl(), m_szWaveCfgComment);
		m_io_CfgName.UpdateDisplay();
		m_io_CfgComment.UpdateDisplay();
		SetWaveCfgList();
		delete pWaveCfg;
	}
}