// CDlgAutoRcpChk.cpp : 実装ファイル
//

#include "pch.h"
#include "vector"
#include "string"
#include "WaveEndPoint.h"
#include "CDlgRecipeSelect.h"
#include "CDlgAutoRcpChk.h"


// CDlgAutoRcpChk ダイアログ
static int		m_bRcpSimSelectFile = FALSE;
static int		m_iSelectPM = 0;
static int		m_iFileCnt = 0;
static int		m_bRcpInterlock = FALSE;
static int		m_bWaveInterlock = FALSE;
static int		m_iRecipePM = PM1;
static int		m_iRecipeNo = 0;

std::vector<std::string>	g_tWaveList(1000);

extern void get_samplinglist(int iPM, std::vector<std::string>& pList);


RCP_CHK_DATA	g_tAutoChk[CHK_TBL_MAX+1];


/*
** #define
*/
#define NUM_COLUMNS				30								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_FILENAME		1
#define	SUB_ITEM_CHECK			2
#define	SUB_ITEM_RESULT			3
#define	SUB_ITEM_TOTAL			4
#define	SUB_ITEM1_NAME			5
#define	SUB_ITEM1_TIME			6
#define	SUB_ITEM1_LEVEL			7
#define	SUB_ITEM2_NAME			8
#define	SUB_ITEM2_TIME			9
#define	SUB_ITEM2_LEVEL			10
#define	SUB_ITEM3_NAME			11
#define	SUB_ITEM3_TIME			12
#define	SUB_ITEM3_LEVEL			13
#define	SUB_ITEM4_NAME			14
#define	SUB_ITEM4_TIME			15
#define	SUB_ITEM4_LEVEL			16
#define	SUB_ITEM5_NAME			17
#define	SUB_ITEM5_TIME			18
#define	SUB_ITEM5_LEVEL			19
#define	SUB_ITEM6_NAME			20
#define	SUB_ITEM6_TIME			21
#define	SUB_ITEM6_LEVEL			22
#define	SUB_ITEM7_NAME			23
#define	SUB_ITEM7_TIME			24
#define	SUB_ITEM7_LEVEL			25
#define	SUB_ITEM8_NAME			26
#define	SUB_ITEM8_TIME			27
#define	SUB_ITEM8_LEVEL			28
#define	SUB_ITEM8_LEVEL			29

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."),
	_T("File Name"),
	_T("Chk"),
	_T("Result"),
	_T("Total"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
	_T("Report"),
	_T("Time"),
	_T("Level"),
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
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
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
	35,200,35,60,60,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
};

static	char	szChkName[2][4] = {"x","o"};
static	char	szJudgeName[5][16] = { "-------","Normal","TimeOV","Abort","Error"};




IMPLEMENT_DYNAMIC(CDlgAutoRcpChk, CDialogX)

CDlgAutoRcpChk::CDlgAutoRcpChk(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_AUTO_RCPCHECK, pParent)
{

}

CDlgAutoRcpChk::~CDlgAutoRcpChk()
{
}

void CDlgAutoRcpChk::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_RECIPE, m_io_RcpName);
	DDX_Control(pDX, IDC_LIST1, m_list_RawData);
}


BEGIN_MESSAGE_MAP(CDlgAutoRcpChk, CDialogX)
	ON_BN_CLICKED(IDC_BTN_RCP1, &CDlgAutoRcpChk::OnBnClickedBtnRcp1)
	ON_BN_CLICKED(IDC_BTN_RCP2, &CDlgAutoRcpChk::OnBnClickedBtnRcp2)
	ON_BN_CLICKED(IDC_BTN_RCP3, &CDlgAutoRcpChk::OnBnClickedBtnRcp3)
	ON_BN_CLICKED(IDC_BTN_RCP4, &CDlgAutoRcpChk::OnBnClickedBtnRcp4)
	ON_BN_CLICKED(IDC_BTN_RCP5, &CDlgAutoRcpChk::OnBnClickedBtnRcp5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RD_PC1, &CDlgAutoRcpChk::OnBnClickedRdPc1)
	ON_BN_CLICKED(IDC_RD_PC2, &CDlgAutoRcpChk::OnBnClickedRdPc2)
	ON_BN_CLICKED(IDC_RD_PC3, &CDlgAutoRcpChk::OnBnClickedRdPc3)
	ON_BN_CLICKED(IDC_RD_PC4, &CDlgAutoRcpChk::OnBnClickedRdPc4)
	ON_BN_CLICKED(IDC_RD_PC5, &CDlgAutoRcpChk::OnBnClickedRdPc5)
	ON_BN_CLICKED(IDC_BTN_ALLSEL, &CDlgAutoRcpChk::OnBnClickedBtnAllsel)
	ON_BN_CLICKED(IDC_BTN_ALLUNSEL, &CDlgAutoRcpChk::OnBnClickedBtnAllunsel)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CDlgAutoRcpChk::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_UNSELECT, &CDlgAutoRcpChk::OnBnClickedBtnUnselect)
END_MESSAGE_MAP()


// CDlgAutoRcpChk メッセージ ハンドラー


BOOL CDlgAutoRcpChk::OnInitDialog()
{
	int			i;
	LV_COLUMN	lvc;
	long		lState;


	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pEPDSimulation = ((CWaveEndPointApp*)AfxGetApp())->GetEPDSimulation();

	m_io_RcpName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.RcpName"));

	m_io_RcpName.SetSpecialColor(2);
	m_io_RcpName.UpdateDisplay();

	switch (m_iSelectPM) {
	case PM1:	((CButton*)GetDlgItem(IDC_RD_PC1))->SetCheck(TRUE);
				break;
	case PM2:	((CButton*)GetDlgItem(IDC_RD_PC2))->SetCheck(TRUE);
		break;
	case PM3:	((CButton*)GetDlgItem(IDC_RD_PC3))->SetCheck(TRUE);
		break;
	case PM4:	((CButton*)GetDlgItem(IDC_RD_PC4))->SetCheck(TRUE);
		break;
	case PM5:	((CButton*)GetDlgItem(IDC_RD_PC5))->SetCheck(TRUE);
		break;
	}

	/*
	** Samplingリスト初期化
	*/
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_RawData.InsertColumn(i, &lvc);
	}/* for */
	m_list_RawData.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));

	m_list_RawData.SetModeGridLine(TRUE, TRUE);

	if (m_iFileCnt == 0)
		SetFileList();
	else
		ChkListUpdate(TRUE);

	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgAutoRcpChk::SetFileList()
{
	int				i;
	LV_ITEM			lvi;
	CString			szStr('\0', 64);

	m_list_RawData.DeleteAllItems();

	g_tWaveList.clear();
	ZeroMemory(g_tAutoChk, sizeof(g_tAutoChk));


	get_samplinglist(m_iSelectPM, g_tWaveList);
	m_iFileCnt = g_tWaveList.size();

	//モジュール内のデータをセット
	for (i = 0; i < m_iFileCnt; i++)
		strncpy(g_tAutoChk[i].szFileName, g_tWaveList[i].c_str(), 63);

	ChkListUpdate(TRUE);
}

void CDlgAutoRcpChk::OnOK()
{
	char	szStr[64];

	// Recipe Check Start
	m_pEPDSimulation->EPDStartAutoCheck(m_iSelectPM, m_iRecipePM, m_iRecipeNo);

	CDialogX::OnOK();
}
void CDlgAutoRcpChk::OnBnClickedBtnRcp(int iPM)
{
	int		iRcpNo;

	CDlgRecipeSelect* pDlg;

	pDlg = new CDlgRecipeSelect(this, iPM);

	if (pDlg->DoModal() == IDOK) {
		iRcpNo = pDlg->GetSelectRcpNo();
		m_pEPDSimulation->LoadRecipeRequest(iPM, iRcpNo);
		m_bRcpInterlock = TRUE;
		m_iRecipePM = iPM;
		m_iRecipeNo = iRcpNo;
	}
	delete pDlg;
}


void CDlgAutoRcpChk::OnTimer(UINT_PTR nIDEvent)
{
	m_io_RcpName.ChkValueChange();

	if (m_bRcpInterlock && m_bWaveInterlock) {
		if (GetDlgItem(IDOK)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else{
		if (GetDlgItem(IDOK)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	CDialogX::OnTimer(nIDEvent);
}


void CDlgAutoRcpChk::ChangePC(int iPM)
{
	if (m_iSelectPM == iPM)
		return;

	m_iSelectPM = iPM;
	SetFileList();
}

void CDlgAutoRcpChk::ChkListUpdate(int bCreate)
{
	int				i,j;
	LV_ITEM			lvi;
	CString			szStr('\0', 64);

	if(bCreate == TRUE)
		m_list_RawData.DeleteAllItems();

	m_bWaveInterlock = FALSE;

	//モジュール内のデータをセット
	for (i = 0; i < m_iFileCnt && i < 1000; i++) {
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;


		szStr.Format("%03.03d", i+ 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		if (bCreate == TRUE) {
			m_list_RawData.InsertItem(&lvi);
			m_list_RawData.SetItemData(i,i);
		}
		szStr = g_tAutoChk[i].szFileName;
		m_list_RawData.SetItemText(i, SUB_ITEM_FILENAME, (LPCSTR)szStr);

		szStr = szChkName[g_tAutoChk[i].bCheck];
		m_list_RawData.SetItemText(i, SUB_ITEM_CHECK, (LPCSTR)szStr);
		if (m_bWaveInterlock == FALSE && g_tAutoChk[i].bCheck == TRUE)
			m_bWaveInterlock = TRUE;

		szStr = szJudgeName[g_tAutoChk[i].iResult];
		m_list_RawData.SetItemText(i, SUB_ITEM_RESULT, (LPCSTR)szStr);
		if(g_tAutoChk[i].iResult > 1)		// Timeout/Abort/Error
			m_list_RawData.SetItemData(i, (i+60000));

		szStr.Format("%5.1fSec",g_tAutoChk[i].dTotalTime);
		m_list_RawData.SetItemText(i, SUB_ITEM_TOTAL, (LPCSTR)szStr);

		for (j = 0; j < g_tAutoChk[i].iItemCount; j++) {
			szStr = g_tAutoChk[i].szItemName[j];
			m_list_RawData.SetItemText(i, (SUB_ITEM1_NAME+j*3), (LPCSTR)szStr);

			szStr.Format("%5.2f" ,g_tAutoChk[i].dItemTime[j]);
			m_list_RawData.SetItemText(i, (SUB_ITEM1_TIME + j * 3), (LPCSTR)szStr);

			szStr.Format("%5.2f", g_tAutoChk[i].dItemLevel[j]);
			m_list_RawData.SetItemText(i, (SUB_ITEM1_LEVEL + j * 3), (LPCSTR)szStr);
		}/* for */
	}/* for */

}


void CDlgAutoRcpChk::OnBnClickedBtnAllsel()
{
	int	i;

	for (i = 0; i < m_iFileCnt; i++)
		g_tAutoChk[i].bCheck = TRUE;

	ChkListUpdate(FALSE);
}
void CDlgAutoRcpChk::OnBnClickedBtnAllunsel()
{
	int	i;

	for (i = 0; i < m_iFileCnt; i++)
		g_tAutoChk[i].bCheck = FALSE;

	ChkListUpdate(FALSE);
}

void CDlgAutoRcpChk::OnBnClickedBtnSelect()
{
	int iItem = -1;
	int	i;

	while (1) {
		iItem = m_list_RawData.GetSelectedItem(iItem);
		if (iItem == -1)
			break;

		i = m_list_RawData.GetItemData(iItem);
		i %= 60000;
		g_tAutoChk[i].bCheck = TRUE;

	}/* while */
	ChkListUpdate(FALSE);

}


void CDlgAutoRcpChk::OnBnClickedBtnUnselect()
{
	int iItem = -1;
	int	i;

	while (1) {
		iItem = m_list_RawData.GetSelectedItem(iItem);
		if (iItem == -1)
			break;

		i = m_list_RawData.GetItemData(iItem);
		i %= 60000;
		g_tAutoChk[i].bCheck = FALSE;

	}/* while */
	ChkListUpdate(FALSE);
}


