// DlgAlarmLog.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgAlarmLog.h"


// CDlgAlarmLog ダイアログ
// CDlgAlarm ダイアログ
#define NUM_COLUMNS				6								/* Lot List表示項目数	*/

#define	SUB_ITEM_ALID			0
#define	SUB_ITEM_ONOFF			1
#define	SUB_ITEM_LEVEL			2
#define	SUB_ITEM_MODULE			3
#define	SUB_ITEM_DATE			4
#define	SUB_ITEM_MSG			5

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("Alarm"),
	_T("Occurred"),
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
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	80,80, 80,80,200,650
};

static char* m_pszOccured[2 + 1] = {_T("Clear"),_T("Occurred"),NULL};
static char* m_pszLevel[2 + 1] = { _T("Light"),_T("Heavy"),NULL };
static char* m_pszModule[6 + 1] = { _T("PC1"),_T("PC2"),_T("PC3"),_T("PC4"),_T("PC5"),_T("System"),NULL };

IMPLEMENT_DYNAMIC(CDlgAlarmLog, CDialogX)

CDlgAlarmLog::CDlgAlarmLog(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_ALARMLOG, pParent)
{
//	SetBackColor(255, 0, 0);
	CDialogX::Create(IDD_ALARMLOG, pParent);
}

CDlgAlarmLog::~CDlgAlarmLog()
{
}

void CDlgAlarmLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALM, m_list_alarm);
}


BEGIN_MESSAGE_MAP(CDlgAlarmLog, CDialogX)
END_MESSAGE_MAP()


// CDlgAlarmLog メッセージ ハンドラー


BOOL CDlgAlarmLog::OnInitDialog()
{
	CRect	rc;
	CString		szStr('\0', 64);
	char		szBuf[32];
	int	i;
	LV_ITEM lvi;
	LV_COLUMN lvc;


	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	m_img_List.Create(IDB_ALMLEVEL, 16, 2, RGB(255, 255, 255));
	m_list_alarm.SetImageList(&m_img_List, LVSIL_SMALL);

	GetClientRect(rc);
	rc.DeflateRect(120, 80);
	rc.OffsetRect(50,30);
	m_list_alarm.MoveWindow(rc);

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_alarm.InsertColumn(i, &lvc);
	}/* for */
	m_list_alarm.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED));
	m_list_alarm.SetModeGridLine(TRUE, TRUE);

	m_pAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetAlarmClass();

	for (i = 0; i < m_pAlarm->m_iAlmLogTotal; i++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;

		lvi.iItem = 0;
		lvi.iSubItem = SUB_ITEM_ALID;
		lvi.iImage = m_pAlarm->m_tAlarmLogTbl[i].iAlarmLevel;
		sprintf(szBuf, "%04.4d", m_pAlarm->m_tAlarmLogTbl[i].iAlarmNo);
		lvi.pszText = szBuf;
		m_list_alarm.InsertItem(&lvi);
		if(m_pAlarm->m_tAlarmLogTbl[i].iOnOff)
			m_list_alarm.SetItemData(lvi.iItem, (m_pAlarm->m_tAlarmLogTbl[i].iAlarmNo + 50000));
		else
			m_list_alarm.SetItemData(lvi.iItem, m_pAlarm->m_tAlarmLogTbl[i].iAlarmNo);

		szStr = m_pszOccured[m_pAlarm->m_tAlarmLogTbl[i].iOnOff];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_ONOFF, (LPCSTR)szStr);

		szStr = m_pszLevel[m_pAlarm->m_tAlarmLogTbl[i].iAlarmLevel];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_LEVEL, (LPCSTR)szStr);

		szStr = m_pszModule[m_pAlarm->m_tAlarmLogTbl[i].iModule];
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MODULE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tAlarmLogTbl[i].szDate;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_DATE, (LPCSTR)szStr);

		szStr = m_pAlarm->m_tAlarmLogTbl[i].szMeassage;
		m_list_alarm.SetItemText(lvi.iItem, SUB_ITEM_MSG, szStr);
	}/* for */

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


