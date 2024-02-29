#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgAlarm.h"
// CDlgAlarm.cpp : 実装ファイル
//

// CDlgAlarm ダイアログ
#define NUM_COLUMNS				5								/* Lot List表示項目数	*/

#define	SUB_ITEM_ALID			0
#define	SUB_ITEM_LEVEL			1
#define	SUB_ITEM_MODULE			2
#define	SUB_ITEM_DATE			3
#define	SUB_ITEM_MSG			4

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("Alarm"),
	_T("Level"),
	_T("Module"),
	_T("Date"),
	_T("Message"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	80,80, 60,180,550
};


IMPLEMENT_DYNAMIC(CDlgAlarm, CDialogX)

CDlgAlarm::CDlgAlarm(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_ALARM, pParent)
{
	SetBackColor(255, 0, 0);
	CDialogX::Create(IDD_ALARM, pParent);
}

CDlgAlarm::~CDlgAlarm()
{
}

void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARM, m_list_alarm);
}
BEGIN_MESSAGE_MAP(CDlgAlarm, CDialogX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgAlarm::OnBnClickedAllClear)
END_MESSAGE_MAP()


// CDlgAlarm メッセージ ハンドラー


BOOL CDlgAlarm::OnInitDialog()
{
	CDialogX::OnInitDialog();

	int			i;
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	m_img_List.Create(IDB_ALMLEVEL, 16, 2, RGB(255, 255, 255));
	m_list_alarm.SetImageList(&m_img_List, LVSIL_SMALL);

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_alarm.InsertColumn(i, &lvc);
	}/* for */
	m_list_alarm.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED));

	m_list_alarm.SetModeGridLine(TRUE, TRUE);

	m_pAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetAlarmClass();
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

static char* m_pszLevel[3] = {_T("Light"),_T("Heavy")};
static char* m_pszModule[7] = { _T("PC1"),_T("PC2"),_T("PC3"),_T("PC4"),_T("PC5"),_T("Control") };

void CDlgAlarm::ShowAlarm()
{
	LV_ITEM lvi;
	CString		szStr('\0', 64);
	char		szBuf[32];
	int	i;

	m_list_alarm.DeleteAllItems();

	if (m_pAlarm->m_iCurrAlmTotal == 0) {
		HideAlarm();
		return;
	}

	for (i = 0; i < m_pAlarm->m_iCurrAlmTotal; i++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;

		lvi.iItem = 0;
		lvi.iSubItem = SUB_ITEM_ALID;
		lvi.iImage = m_pAlarm->m_tCurrAlarm[i].iAlarmLevel;
		sprintf(szBuf,"%04.4d", m_pAlarm->m_tCurrAlarm[i].iAlarmNo);
		lvi.pszText = szBuf;
		m_list_alarm.InsertItem(&lvi);
		m_list_alarm.SetItemData(lvi.iItem, m_pAlarm->m_tCurrAlarm[i].iAlarmNo);

		szStr = m_pszLevel[m_pAlarm->m_tCurrAlarm[i].iAlarmLevel];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_LEVEL, (LPCSTR)szStr);

		szStr = m_pszModule[m_pAlarm->m_tCurrAlarm[i].iModule];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MODULE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tCurrAlarm[i].szDate;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_DATE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tCurrAlarm[i].szMeassage;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MSG, szStr);
	}/* for */

	if (IsWindowVisible() == FALSE)
		ShowWindow(SW_SHOW);
}
void CDlgAlarm::UpdateAlarm()
{
	LV_ITEM		lvi;
	CString		szStr('\0', 64);
	char		szBuf[32];
	int	i;

	m_list_alarm.DeleteAllItems();

	if (m_pAlarm->m_iCurrAlmTotal == 0) {
		HideAlarm();
		return;
	}

	for (i = 0; i < m_pAlarm->m_iCurrAlmTotal; i++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;

		lvi.iItem = 0;
		lvi.iSubItem = SUB_ITEM_ALID;
		lvi.iImage = m_pAlarm->m_tCurrAlarm[i].iAlarmLevel;
		sprintf(szBuf,"%04.4d", m_pAlarm->m_tCurrAlarm[i].iAlarmNo);
		lvi.pszText = szBuf;
		m_list_alarm.InsertItem(&lvi);
		m_list_alarm.SetItemData(lvi.iItem, m_pAlarm->m_tCurrAlarm[i].iAlarmNo);

		szStr = m_pszLevel[m_pAlarm->m_tCurrAlarm[i].iAlarmLevel];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_LEVEL, (LPCSTR)szStr);

		szStr = m_pszModule[m_pAlarm->m_tCurrAlarm[i].iModule];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MODULE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tCurrAlarm[i].szDate;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_DATE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tCurrAlarm[i].szMeassage;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MSG, szStr);
	}/* for */
}
void CDlgAlarm::HideAlarm()
{
	if (IsWindowVisible() == TRUE)
		ShowWindow(SW_HIDE);

}


void CDlgAlarm::OnBnClickedAllClear()
{
	m_pAlarm->AllReset();
	m_list_alarm.DeleteAllItems();
	HideAlarm();
}


void CDlgAlarm::OnCancel()
{
	ShowWindow(SW_HIDE);
}
