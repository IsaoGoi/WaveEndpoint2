// CDlgEPDLog.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgEPDLog.h"
#include "CDlgRcpStepView.h"

/*
** #define
*/
#define NUM_COLUMNS				3								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_GLSID			1
#define	SUB_ITEM_DATE			2

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."),
	_T("GLSID"),
	_T("Date"),
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
	40,92, 160
};


// CDlgEPDLog ダイアログ
static int m_iCurrPC = -1;

static UINT	m_ui_StartTime[RCP_STEP_MAX] = { IDC_ED_STARTTIME1 ,IDC_ED_STARTTIME2 ,IDC_ED_STARTTIME3 ,IDC_ED_STARTTIME4 ,IDC_ED_STARTTIME5 };
static UINT	m_ui_DeadTime[RCP_STEP_MAX] = { IDC_ED_DEADTIME1 ,IDC_ED_DEADTIME2 ,IDC_ED_DEADTIME3 ,IDC_ED_DEADTIME4 ,IDC_ED_DEADTIME5 };
static UINT	m_ui_JustTime[RCP_STEP_MAX] = { IDC_ED_JUSTTIME1 ,IDC_ED_JUSTTIME2 ,IDC_ED_JUSTTIME3 ,IDC_ED_JUSTTIME4 ,IDC_ED_JUSTTIME5 };
static UINT	m_ui_EndTime[RCP_STEP_MAX] = { IDC_ED_ENDTIME1 ,IDC_ED_ENDTIME2 ,IDC_ED_ENDTIME3 ,IDC_ED_ENDTIME4 ,IDC_ED_ENDTIME5 };
static UINT	m_ui_OverEtchTime[RCP_STEP_MAX] = { IDC_ED_OVERETCHTIME1 ,IDC_ED_OVERETCHTIME2 ,IDC_ED_OVERETCHTIME3 ,IDC_ED_OVERETCHTIME4 ,IDC_ED_OVERETCHTIME5 };
static UINT	m_ui_StartLevel[RCP_STEP_MAX] = { IDC_ED_STARTLEVEL1 ,IDC_ED_STARTLEVEL2 ,IDC_ED_STARTLEVEL3 ,IDC_ED_STARTLEVEL4 ,IDC_ED_STARTLEVEL5 };
static UINT	m_ui_JustLevel[RCP_STEP_MAX] = { IDC_ED_JUSTLEVEL1 ,IDC_ED_JUSTLEVEL2 ,IDC_ED_JUSTLEVEL3 ,IDC_ED_JUSTLEVEL4 ,IDC_ED_JUSTLEVEL5 };
static UINT	m_ui_PeakLevel[RCP_STEP_MAX] = { IDC_ED_PEAKLEVEL1 ,IDC_ED_PEAKLEVEL2 ,IDC_ED_PEAKLEVEL3 ,IDC_ED_PEAKLEVEL4 ,IDC_ED_PEAKLEVEL5 };
static UINT	m_ui_EndLevel[RCP_STEP_MAX] = { IDC_ED_ENDLEVEL1 ,IDC_ED_ENDLEVEL2 ,IDC_ED_ENDLEVEL3 ,IDC_ED_ENDLEVEL4 ,IDC_ED_ENDLEVEL5 };

static UINT	m_ui_RcpBtn[RCP_STEP_MAX] = { IDC_BTN_RCP1,IDC_BTN_RCP2,IDC_BTN_RCP3,IDC_BTN_RCP4,IDC_BTN_RCP5 };

IMPLEMENT_DYNAMIC(CDlgEPDLog, CDialogX)


static	int m_iSelect_PCNo = PM1;


CDlgEPDLog::CDlgEPDLog(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_EPDLOG, pParent)
{
	int	i;

	m_pImage = NULL;
	m_pConfig = NULL;
	m_pToolTip = NULL;
	m_hCurrSelItem = (HTREEITEM)-1;
	m_iToolTipDataNo = 0;

	memset(&m_tEPDData,'\0',sizeof(EPD_WAVEDATA2));
	memset(&m_tRecipe, '\0', sizeof(RECIPE));
	for(i=0;i< RCP_STEP_MAX;i++)
		memset(&m_tStepWaveCfg[i], '\0', sizeof(SET_WAVECFG));

	//	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_EPDLOG, pParent);

}

CDlgEPDLog::~CDlgEPDLog()
{
	if (m_pToolTip)
		delete m_pToolTip;
}

void CDlgEPDLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_list_log);
}


BEGIN_MESSAGE_MAP(CDlgEPDLog, CDialogX)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LOG, &CDlgEPDLog::OnTvnSelchangedTreeLog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_ROWDATA, &CDlgEPDLog::OnBnClickedChkRowdata)
	ON_BN_CLICKED(IDC_BTN_RCP1, &CDlgEPDLog::OnBnClickedBtnRcp1)
	ON_BN_CLICKED(IDC_BTN_RCP2, &CDlgEPDLog::OnBnClickedBtnRcp2)
	ON_BN_CLICKED(IDC_BTN_RCP3, &CDlgEPDLog::OnBnClickedBtnRcp3)
	ON_BN_CLICKED(IDC_BTN_RCP4, &CDlgEPDLog::OnBnClickedBtnRcp4)
	ON_BN_CLICKED(IDC_BTN_RCP5, &CDlgEPDLog::OnBnClickedBtnRcp5)
	ON_BN_CLICKED(IDC_RADIO_PC1, &CDlgEPDLog::OnBnClickedRadioPc1)
	ON_BN_CLICKED(IDC_RADIO_PC2, &CDlgEPDLog::OnBnClickedRadioPc2)
	ON_BN_CLICKED(IDC_RADIO_PC3, &CDlgEPDLog::OnBnClickedRadioPc3)
	ON_BN_CLICKED(IDC_RADIO_PC4, &CDlgEPDLog::OnBnClickedRadioPc4)
	ON_BN_CLICKED(IDC_RADIO_PC5, &CDlgEPDLog::OnBnClickedRadioPc5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOG, &CDlgEPDLog::OnLvnItemchangedListLog)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CDlgEPDLog メッセージ ハンドラー


BOOL CDlgEPDLog::OnInitDialog()
{
	long			lState;
	TV_INSERTSTRUCT	tcItem;
	CString			szWork('0',32);
	HTREEITEM		hTreeHdl;
	HTREEITEM		hCurrPCHdl = NULL;
	HTREEITEM		hPCHdl[PM_MAX] = { NULL,NULL,NULL,NULL,NULL };
	int				iPM;
	CRect			rc;
	int				i;
	int				iCnt;
	LV_COLUMN	lvc;


	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pEPDLog = ((CWaveEndPointApp*)AfxGetApp())->GetEPDLogClass();

	m_pEPDLog->LoadEPDLogList();

	for(i=0;i<RCP_STEP_MAX;i++)
		GetDlgItem(m_ui_RcpBtn[i])->EnableWindow(FALSE);

	m_pToolTip = new CToolTipGraph(this);
	m_pToolTip->Deactive();

#if 0
	/*
	** PC Tree表示
	*/
	lState = GetWindowLong(m_Tree_Log.m_hWnd, GWL_STYLE);
	lState |= (TVS_HASLINES);
	SetWindowLong(m_Tree_Log.m_hWnd, GWL_STYLE, lState);
	m_pImage = new CImageList();
	m_pImage->Create(IDB_CFG_TREE, 24, 2, RGB(255, 255, 255));
	m_Tree_Log.SetImageList(m_pImage, TVSIL_NORMAL);


	tcItem.hInsertAfter = TVI_LAST;
	tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	tcItem.hParent = TVI_ROOT;
	szWork = _T("EPD Log");
	tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
	tcItem.item.lParam = 0;
	tcItem.item.iImage = 0;
	tcItem.item.iSelectedImage = 2;
	hTreeHdl = m_Tree_Log.InsertItem(&tcItem);

	for (iPM = 0; iPM < PM_MAX; iPM++) {
		if (m_pConfig->m_tEPDConfig.iPCEnable[iPM]) {

			if (m_iCurrPC == -1)
				m_iCurrPC = iPM;

			tcItem.hParent = hTreeHdl;
			szWork.Format("PC%d", (iPM + 1));
			tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
			tcItem.item.lParam = iPM;
			tcItem.item.iImage = 0;
			tcItem.item.iSelectedImage = 1;
			hPCHdl[iPM] = m_Tree_Log.InsertItem(&tcItem);
			if (m_iCurrPC == iPM)
				hCurrPCHdl = hPCHdl[iPM];
		}
	}
	m_Tree_Log.Expand(hTreeHdl, TVE_EXPAND);

	if (hCurrPCHdl)
		m_Tree_Log.SelectItem(hCurrPCHdl);

	for (i = 0; i < m_pEPDLog->m_tDisplayLogTable.iLogCount; i++) {
		iPM = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].iPCNo;

		if (hPCHdl[iPM]) {
			tcItem.hParent = hPCHdl[iPM];
			tcItem.item.pszText = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szDateTime;
			tcItem.item.lParam = 1000000+i;
			tcItem.item.iImage = 0;
			tcItem.item.iSelectedImage = 1;
			m_Tree_Log.InsertItem(&tcItem);
		}

	}/* for */
#endif

	if (m_iSelect_PCNo == PM1)
		((CButton*)GetDlgItem(IDC_RADIO_PC1))->SetCheck(TRUE);
	else if (m_iSelect_PCNo == PM2)
		((CButton*)GetDlgItem(IDC_RADIO_PC2))->SetCheck(TRUE);
	else if (m_iSelect_PCNo == PM3)
		((CButton*)GetDlgItem(IDC_RADIO_PC3))->SetCheck(TRUE);
	else if (m_iSelect_PCNo == PM4)
		((CButton*)GetDlgItem(IDC_RADIO_PC4))->SetCheck(TRUE);
	else if (m_iSelect_PCNo == PM5)
		((CButton*)GetDlgItem(IDC_RADIO_PC5))->SetCheck(TRUE);

	/*
	** Log リスト初期化
	*/
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_log.InsertColumn(i, &lvc);
	}/* for */
	m_list_log.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_log.SetModeGridLine(TRUE, TRUE);


	SelectPC(m_iSelect_PCNo);

	GetDlgItem(IDC_GRAPH_RECT)->GetClientRect(m_RectGraph);
	m_RectGraph.OffsetRect(395, 351);
	m_RectUpdate = m_RectGraph;
	m_RectGraph.DeflateRect(1, 1);
	m_RectUpdate.DeflateRect(-100, -50);

	InvalidateRect(m_RectUpdate);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}




void CDlgEPDLog::OnTvnSelchangedTreeLog(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	HTREEITEM		hTreeHdl;
	int				iDataNo;
	CString			szStr('\0', 128);
	int				i;

	hTreeHdl = m_Tree_Log.GetSelectedItem();
	if (m_hCurrSelItem == hTreeHdl)
		return;
	m_hCurrSelItem = hTreeHdl;

	if (hTreeHdl) {
		iDataNo = (int)m_Tree_Log.GetItemData(hTreeHdl);
		if (1000000 > iDataNo)
			return;
		iDataNo %= 1000000;
		m_pEPDLog->ReadEPDLog(iDataNo, &m_tEPDData, &m_tRecipe, m_tStepWaveCfg);

		m_iPMNo = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[iDataNo].iPCNo;
		szStr.Format("%d", (m_iPMNo +1));
		GetDlgItem(IDC_ED_PC)->SetWindowText(szStr);

		if (m_tEPDData.iEndStatus == 1)
			szStr = _T("Normal End");
		else if (m_tEPDData.iEndStatus == 2)
			szStr = _T("Time Over");
		else if (m_tEPDData.iEndStatus == 3)
			szStr = _T("Aborted");
		else if (m_tEPDData.iEndStatus == 4)
			szStr = _T("Error End");
		else
			szStr = _T("Unknown");
		GetDlgItem(IDC_ED_ENDSTATUS)->SetWindowText(szStr);

		szStr.Format("%8.1f", m_tEPDData.dTotalTime);
		GetDlgItem(IDC_ED_TOTALTIME)->SetWindowText(szStr);

		GetDlgItem(IDC_ED_LOG_LOTID)->SetWindowText(m_tEPDData.szLotName);
		GetDlgItem(IDC_ED_LOG_CSTID)->SetWindowText(m_tEPDData.szCstName);
		GetDlgItem(IDC_ED_LOG_GLSID)->SetWindowText(m_tEPDData.szGlassName);
		szStr.Format("%d", m_tEPDData.usSlotNo);
		GetDlgItem(IDC_ED_LOG_SLOTNO)->SetWindowText(szStr);


		for (i = 0; i < RCP_STEP_MAX; i++) {
			if (i != 0 && m_tEPDData.dStartTime[i] == 0.0f) {
				szStr = _T("    ");
				GetDlgItem(m_ui_StartTime[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_DeadTime[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_JustTime[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_EndTime[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_OverEtchTime[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_StartLevel[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_JustLevel[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_PeakLevel[i])->SetWindowText(szStr);
				GetDlgItem(m_ui_EndLevel[i])->SetWindowText(szStr);

				GetDlgItem(m_ui_StartTime[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_DeadTime[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_JustTime[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_EndTime[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_OverEtchTime[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_StartLevel[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_JustLevel[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_PeakLevel[i])->EnableWindow(FALSE);
				GetDlgItem(m_ui_EndLevel[i])->EnableWindow(FALSE);

				GetDlgItem(m_ui_RcpBtn[i])->EnableWindow(FALSE);
			}
			else {
				GetDlgItem(m_ui_StartTime[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_DeadTime[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_JustTime[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_EndTime[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_OverEtchTime[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_StartLevel[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_JustLevel[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_PeakLevel[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_EndLevel[i])->EnableWindow(TRUE);
				GetDlgItem(m_ui_RcpBtn[i])->EnableWindow(TRUE);

				szStr.Format("%8.1f", m_tEPDData.dStartTime[i]);
				GetDlgItem(m_ui_StartTime[i])->SetWindowText(szStr);

				szStr.Format("%8.1f", m_tEPDData.dDeadTime[i]);
				GetDlgItem(m_ui_DeadTime[i])->SetWindowText(szStr);

				szStr.Format("%8.1f", m_tEPDData.dJustTime[i]);
				GetDlgItem(m_ui_JustTime[i])->SetWindowText(szStr);

				szStr.Format("%8.1f", m_tEPDData.dEndTime[i]);
				GetDlgItem(m_ui_EndTime[i])->SetWindowText(szStr);

				szStr.Format("%8.1f", m_tEPDData.dOverEtchTime[i]);
				GetDlgItem(m_ui_OverEtchTime[i])->SetWindowText(szStr);

				szStr.Format("%7.2f", m_tEPDData.dStartLevel[i]);
				GetDlgItem(m_ui_StartLevel[i])->SetWindowText(szStr);

				szStr.Format("%7.2f", m_tEPDData.dJustLevel[i]);
				GetDlgItem(m_ui_JustLevel[i])->SetWindowText(szStr);

				szStr.Format("%7.2f", m_tEPDData.dPeakLevel[i]);
				GetDlgItem(m_ui_PeakLevel[i])->SetWindowText(szStr);

				szStr.Format("%7.2f", m_tEPDData.dEndLevel[i]);
				GetDlgItem(m_ui_EndLevel[i])->SetWindowText(szStr);
			}
		}
		UpdateData(FALSE);
		InvalidateRect(m_RectUpdate);

	}
}


void CDlgEPDLog::OnPaint()
{
	CRect	rc;
	CPen	DotPen;

	int		i, iStep;
	POINT	LinePoint[2];
	double	dWidth_per_100ms;
	double	dHeight_per_1;
	double	dWidth;
	double	dHeight;
	int		iTotal;
	CPen	RawGraphPen;
	CPen	CalcGraphPen[RCP_STEP_MAX];
	CPen	BlackPen;
	CPen	RedPen;
	CPen	BluePen;
	int		iDrawCount;
	int		iDrawCnt;
	int		bSkip;
	int		bOverWrite;

	CPen* oldPen;
	CPaintDC dc(this); // device context for painting
	CString	szStr('\0', 32);

	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));
	RawGraphPen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
	CalcGraphPen[0].CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CalcGraphPen[1].CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CalcGraphPen[2].CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	CalcGraphPen[3].CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	CalcGraphPen[4].CreatePen(PS_SOLID, 2, RGB(255, 128, 255));
	BlackPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	RedPen.CreatePen(PS_SOLID, 2, RGB(255, 128, 128));
	BluePen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));


	rc = m_RectGraph;
	rc.DeflateRect(-1, -1);

	bOverWrite = ((CButton *)GetDlgItem(IDC_CHECK_OVERWRITE))->GetCheck();
	if (bOverWrite == FALSE) {
		dc.Draw3dRect(rc, RGB(0, 0, 0), RGB(255, 255, 255));
		dc.FillSolidRect(m_RectGraph, RGB(212, 255, 255));
	}

	// 縦軸目盛り
	oldPen = dc.SelectObject(&DotPen);

	LinePoint[0].x = rc.left;
	LinePoint[1].x = rc.right;

	for (i = 1; i < 4; i++) {
		LinePoint[0].y = rc.bottom - (rc.Height() / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);
	}
	// 横軸目盛り
	LinePoint[0].y = rc.top;
	LinePoint[1].y = rc.bottom;

	for (i = 1; i < 3; i++) {
		LinePoint[0].x = rc.left + (rc.Width() / 3 * i);
		LinePoint[1].x = LinePoint[0].x;
		dc.Polyline(&LinePoint[0], 2);
	}

	iDrawCount = m_tEPDData.index;
	if (iDrawCount > 1) {
		iTotal = ((iDrawCount / 600) + 1) * 600;
		dWidth_per_100ms = (double)m_RectGraph.Width();
		dWidth_per_100ms /= (double)iTotal;

		// 2024/01/30 Time Scale for Tool Chip
		m_dTimeScale = 1.0f;
		if (dWidth_per_100ms < 1.0) {
			m_dTimeScale /= dWidth_per_100ms;
		}

		dHeight = (double)m_RectGraph.Height();
		dHeight_per_1 = dHeight / (double)1000.0f;

		rc.top = m_RectGraph.bottom + 3;
		rc.bottom = rc.top + 25;

		for (i = 1; i < 4; i++) {
			rc.left = m_RectGraph.left + (m_RectGraph.Width() / 3 * i) - 50;
			rc.right = rc.left + 100;
			szStr.Format("%dSec", (iTotal / 10 / 3 * i));
			if (i == 3) {
				rc.left -= 50;
				rc.right = rc.left + 100;
				dc.DrawText(szStr, rc, DT_RIGHT);
			}
			else
				dc.DrawText(szStr, rc, DT_CENTER);
		}/* for */

		for (iStep = 0; iStep < m_tRecipe.iStepCnt; iStep++) {
#if 0
			bSkip = FALSE;
			if (iStep) {
				for (i = 0; i < iStep; i++) {
					if (m_tRecipe.Step[i].iUseWave == m_tRecipe.Step[iStep].iUseWave) {
						bSkip = TRUE;
						break;
					}
				}
			}
			if (bSkip == TRUE)
				continue;
#endif			
			for (i = iDrawCnt = 0; i < iDrawCount; i++) {
//				m_RawGraphPoint[i].x = m_RectGraph.left + (int)(dWidth_per_100ms * i);
//				m_RawGraphPoint[i].y = m_RectGraph.bottom - (int)(m_tEPDData.dData[i] * dHeight_per_1);

				if (iDrawCnt) {
					if(m_CalcGraphPoint[iDrawCnt-1].x == m_RectGraph.left + (int)(dWidth_per_100ms * i))
						continue;
				}
				
				m_CalcGraphPoint[iDrawCnt].x = m_RectGraph.left + (int)(dWidth_per_100ms * i);
				if (m_tEPDData.dCalcData[i] >= 0)
					m_CalcGraphPoint[iDrawCnt].y = m_RectGraph.bottom - (int)(m_tEPDData.dCalcData[iStep][i] * dHeight_per_1);
				else
					m_CalcGraphPoint[iDrawCnt].y = m_RectGraph.bottom;
				iDrawCnt++;
			}/* for */
#if 0
			if (((CButton*)(GetDlgItem(IDC_CHK_ROWDATA)))->GetCheck()) {
				dc.SelectObject(&RawGraphPen);
				dc.Polyline(m_RawGraphPoint, iDrawCount);
			}
#endif
			dc.SelectObject(&CalcGraphPen[iStep]);
			dc.Polyline(m_CalcGraphPoint, iDrawCnt);

			// 2024/01/31 凡例の表示
			szStr.Format("Step%d", (iStep + 1));
			rc.left = m_RectGraph.left + (iStep * 150);
			rc.right = rc.left+50;
			rc.top = m_RectGraph.bottom + 30;
			rc.bottom = rc.top + 20;
			dc.DrawText(szStr, rc, DT_CENTER);

			m_CalcGraphPoint[0].x = m_RectGraph.left + (iStep * 150) + 50;
			m_CalcGraphPoint[1].x = m_CalcGraphPoint[0].x + 70;
			m_CalcGraphPoint[0].y = m_RectGraph.bottom + 40;
			m_CalcGraphPoint[1].y = m_RectGraph.bottom + 40;
			dc.Polyline(m_CalcGraphPoint, 2);


		}

		rc = m_RectGraph;
		rc.DeflateRect(0, -1);
		LinePoint[0].y = rc.top;
		LinePoint[1].y = rc.bottom;

		for (iStep = 0; iStep < RCP_STEP_MAX; iStep++) {
			if (iStep && m_tEPDData.dStartTime[iStep] == 0.0f)
				break;

			if (m_tEPDData.dDeadTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData.dStartTime[iStep] + m_tEPDData.dDeadTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData.dJustTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData.dStartTime[iStep] + m_tEPDData.dDeadTime[iStep] + m_tEPDData.dJustTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&RedPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData.dEndTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData.dStartTime[iStep] + m_tEPDData.dDeadTime[iStep] + m_tEPDData.dJustTime[iStep] + m_tEPDData.dEndTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BluePen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData.dOverEtchTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData.dStartTime[iStep] + m_tEPDData.dDeadTime[iStep] + m_tEPDData.dJustTime[iStep] + m_tEPDData.dEndTime[iStep] + m_tEPDData.dOverEtchTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
		}
	}

	dc.SelectObject(oldPen);

	DotPen.DeleteObject();
	RawGraphPen.DeleteObject();
	for (i = 0; i < RCP_STEP_MAX; i++)
		CalcGraphPen[i].DeleteObject();
	BlackPen.DeleteObject();
	RedPen.DeleteObject();
	BluePen.DeleteObject();



}
void CDlgEPDLog::OnBnClickedChkRowdata()
{
	InvalidateRect(m_RectUpdate);
}


void CDlgEPDLog::DisplayRecipe(int iStep)
{
	CDlgRcpStepView* pDlg;

	pDlg = new CDlgRcpStepView(this, m_iPMNo, &m_tRecipe, &m_tStepWaveCfg[iStep], iStep);
	pDlg->DoModal();

	delete pDlg;
}


void CDlgEPDLog::SelectPC(int iPCNo)
{
	int				i;
	int				iCnt;
	int				iPM;

	LV_ITEM		lvi;
	CString		szStr('\0', 64);

	m_iSelect_PCNo = iPCNo;

	m_list_log.DeleteAllItems();
	if (m_pEPDLog->m_tDisplayLogTable.iLogCount == 0)
		return;

	for (iCnt = 0,i= m_pEPDLog->m_tDisplayLogTable.iLogCount-1; i >=0; i--) {

		iPM = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].iPCNo;

		if (m_iSelect_PCNo == iPM) {

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iCnt;
			lvi.iSubItem = SUB_ITEM_NO;
			szStr.Format("%d", (iCnt + 1));
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			m_list_log.InsertItem(&lvi);

			m_list_log.SetItemData(iCnt, i);


			szStr = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szGlassID;
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			m_list_log.SetItemText(iCnt, SUB_ITEM_GLSID, (LPCSTR)szStr);


			szStr.Format("%04.04s/%02.02s/%02.02s %02.02s:%02.02s:%02.02s",
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szYYYYMMDD[0],
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szYYYYMMDD[4],
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szYYYYMMDD[6],
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szhhmmss[0],
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szhhmmss[2],
				&m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[i].szhhmmss[4]);

			m_list_log.SetItemText(iCnt, SUB_ITEM_DATE, (LPCSTR)szStr);
			iCnt++;
		}

	}/* for */

}


void CDlgEPDLog::OnLvnItemchangedListLog(NMHDR* pNMHDR, LRESULT* pResult)
{
	int		iItem = -1;
	int		iTblNo;
	CString	szStr('\0', 256);
	int		i;


	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;


	if ((pNMLV->uNewState & LVIS_SELECTED) == 0)
		return;


	iItem = m_list_log.GetSelectedItem(iItem);
	if (iItem != -1)
		iTblNo = m_list_log.GetItemData(iItem);
	else
		return;

	m_pEPDLog->ReadEPDLog(iTblNo, &m_tEPDData, &m_tRecipe, m_tStepWaveCfg);

	m_iPMNo = m_pEPDLog->m_tDisplayLogTable.tEPDLogPath[iTblNo].iPCNo;
	szStr.Format("%d", (m_iPMNo + 1));
	GetDlgItem(IDC_ED_PC)->SetWindowText(szStr);

	if (m_tEPDData.iEndStatus == 1)
		szStr = _T("Normal End");
	else if (m_tEPDData.iEndStatus == 2)
		szStr = _T("Time Over");
	else if (m_tEPDData.iEndStatus == 3)
		szStr = _T("Aborted");
	else if (m_tEPDData.iEndStatus == 4)
		szStr = _T("Error End");
	else
		szStr = _T("Unknown");
	GetDlgItem(IDC_ED_ENDSTATUS)->SetWindowText(szStr);

	szStr.Format("%8.1f", m_tEPDData.dTotalTime);
	GetDlgItem(IDC_ED_TOTALTIME)->SetWindowText(szStr);

	GetDlgItem(IDC_ED_LOG_LOTID)->SetWindowText(m_tEPDData.szLotName);
	GetDlgItem(IDC_ED_LOG_CSTID)->SetWindowText(m_tEPDData.szCstName);
	GetDlgItem(IDC_ED_LOG_GLSID)->SetWindowText(m_tEPDData.szGlassName);
	szStr.Format("%d", m_tEPDData.usSlotNo);
	GetDlgItem(IDC_ED_LOG_SLOTNO)->SetWindowText(szStr);


	for (i = 0; i < RCP_STEP_MAX; i++) {
		if (i != 0 && m_tEPDData.dStartTime[i] == 0.0f) {
			szStr = _T("    ");
			GetDlgItem(m_ui_StartTime[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_DeadTime[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_JustTime[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_EndTime[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_OverEtchTime[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_StartLevel[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_JustLevel[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_PeakLevel[i])->SetWindowText(szStr);
			GetDlgItem(m_ui_EndLevel[i])->SetWindowText(szStr);

			GetDlgItem(m_ui_StartTime[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_DeadTime[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_JustTime[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_EndTime[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_OverEtchTime[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_StartLevel[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_JustLevel[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_PeakLevel[i])->EnableWindow(FALSE);
			GetDlgItem(m_ui_EndLevel[i])->EnableWindow(FALSE);

			GetDlgItem(m_ui_RcpBtn[i])->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(m_ui_StartTime[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_DeadTime[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_JustTime[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_EndTime[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_OverEtchTime[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_StartLevel[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_JustLevel[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_PeakLevel[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_EndLevel[i])->EnableWindow(TRUE);
			GetDlgItem(m_ui_RcpBtn[i])->EnableWindow(TRUE);

			szStr.Format("%8.1f", m_tEPDData.dStartTime[i]);
			GetDlgItem(m_ui_StartTime[i])->SetWindowText(szStr);

			szStr.Format("%8.1f", m_tEPDData.dDeadTime[i]);
			GetDlgItem(m_ui_DeadTime[i])->SetWindowText(szStr);

			szStr.Format("%8.1f", m_tEPDData.dJustTime[i]);
			GetDlgItem(m_ui_JustTime[i])->SetWindowText(szStr);

			szStr.Format("%8.1f", m_tEPDData.dEndTime[i]);
			GetDlgItem(m_ui_EndTime[i])->SetWindowText(szStr);

			szStr.Format("%8.1f", m_tEPDData.dOverEtchTime[i]);
			GetDlgItem(m_ui_OverEtchTime[i])->SetWindowText(szStr);

			szStr.Format("%7.2f", m_tEPDData.dStartLevel[i]);
			GetDlgItem(m_ui_StartLevel[i])->SetWindowText(szStr);

			szStr.Format("%7.2f", m_tEPDData.dJustLevel[i]);
			GetDlgItem(m_ui_JustLevel[i])->SetWindowText(szStr);

			szStr.Format("%7.2f", m_tEPDData.dPeakLevel[i]);
			GetDlgItem(m_ui_PeakLevel[i])->SetWindowText(szStr);

			szStr.Format("%7.2f", m_tEPDData.dEndLevel[i]);
			GetDlgItem(m_ui_EndLevel[i])->SetWindowText(szStr);
		}
	}
	UpdateData(FALSE);
	if(((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE))->GetCheck()==FALSE)
		InvalidateRect(m_RectUpdate);
	else
		InvalidateRect(m_RectUpdate,FALSE);

}


void CDlgEPDLog::OnMouseMove(UINT nFlags, CPoint point)
{
	int		iDrawCount;
	double	dTime;
	double	dStepData;
	double	dRawData;
	int		iWaveNo;
	int		i;

	iDrawCount = m_tEPDData.index;
	if (iDrawCount > 1) {

		if (m_RectGraph.top <= point.y && m_RectGraph.bottom >= point.y) {
			if (m_RectGraph.left <= point.x && m_RectGraph.right >= point.x) {
				for (i = 0; i < iDrawCount; i++) {
					if (m_CalcGraphPoint[i].x == point.x) {
						dTime = (double)i * m_dTimeScale;
						dTime /= 10.0f;

						iWaveNo = m_tRecipe.Step[m_iToolTipDataNo].iUseWave;
						dStepData = m_tEPDData.dCalcData[m_iToolTipDataNo][i];
						dRawData = m_tEPDData.dDiffData[m_iToolTipDataNo][i];
						sprintf(m_pToolTip->szTime, "% 5.1fSec", dTime);
						sprintf(m_pToolTip->szData, "Step%d : % 5.2fdg", (m_iToolTipDataNo+1), dStepData);
						sprintf(m_pToolTip->szRow, "(Row% 5.2fdg)", dRawData);
						m_pToolTip->Active(point);
						break;
					}/* for */
				}
			}
			else
				m_pToolTip->Deactive();
		}
		else
			m_pToolTip->Deactive();

	}
	else
		m_pToolTip->Deactive();

	CDialogX::OnMouseMove(nFlags, point);
}


void CDlgEPDLog::OnRButtonUp(UINT nFlags, CPoint point)
{
	int		iChg=FALSE;
	int		iDrawCount;
	double	dTime;
	double	dStepData;
	double	dRawData;
	int		iWaveNo;
	int		i;

	if (m_RectGraph.top <= point.y && m_RectGraph.bottom >= point.y &&
		m_RectGraph.left <= point.x && m_RectGraph.right >= point.x && m_tEPDData.index) {


		if (m_iToolTipDataNo < (m_tRecipe.iStepCnt - 1)) {
			++m_iToolTipDataNo;
			iChg = TRUE;
		}
		else if (m_iToolTipDataNo) {
			m_iToolTipDataNo = 0;
			iChg = TRUE;
		}
	}
	if (iChg) {
		iDrawCount = m_tEPDData.index;
		if (iDrawCount > 1) {

			if (m_RectGraph.top <= point.y && m_RectGraph.bottom >= point.y) {
				if (m_RectGraph.left <= point.x && m_RectGraph.right >= point.x) {
					for (i = 0; i < iDrawCount; i++) {
						if (m_CalcGraphPoint[i].x == point.x) {
							dTime = i;
							dTime /= 10.0f;

							iWaveNo = m_tRecipe.Step[m_iToolTipDataNo].iUseWave;
							dStepData = m_tEPDData.dCalcData[m_iToolTipDataNo][i];
							dRawData = m_tEPDData.dDiffData[m_iToolTipDataNo][i];
							sprintf(m_pToolTip->szTime, "% 5.1fSec", dTime);
							sprintf(m_pToolTip->szData, "Step%d : %5.2fdg", (m_iToolTipDataNo+1), dStepData);
							sprintf(m_pToolTip->szRow, "(Row% 5.2fdg)", dRawData);
							m_pToolTip->Active(point);
							break;
						}/* for */
					}
				}
				else
					m_pToolTip->Deactive();
			}
			else
				m_pToolTip->Deactive();

		}
		else
			m_pToolTip->Deactive();
	}

	CDialogX::OnRButtonUp(nFlags, point);
}
