// CDlgPCMonitor.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgPCMonitor.h"


// CDlgPCMonitor ダイアログ

IMPLEMENT_DYNAMIC(CDlgPCMonitor, CDialogX)

#define NUM_COLUMNS				2								/* Lot List表示項目数	*/
#define	SUB_ITEM_NO				0
#define	SUB_ITEM_NAME			1

static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."),
	_T("Wave"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	40,120
};




static	int		m_iSelectNo[PM_MAX][10 + 1] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
};
static	double		m_dYMin[PM_MAX] = {0,0,0,0,0};
static	double		m_dYMax[PM_MAX] = {65535,65535,65535,65535,65535 };
static	double		m_dXMax[PM_MAX] = {120,120,120,120,120 };

static	int			m_iColorTbl[10][3] = {
	{  0,  0,  0},
	{255, 64, 64},
	{  0,  0,255},
	{  0,128,128},
	{  0,192,  0},
	{  0,  0,255},
	{255,  0,255},
	{ 64,  0, 64},
	{128,128,255},
	{128,  0,128},
};


static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };

CDlgPCMonitor::CDlgPCMonitor(CWnd* pParent /*=nullptr*/, int iPM)
	: CDialogX(IDD_PCMONITOR, pParent)
{
	m_iPMNo = iPM;

	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_PCMONITOR, pParent);

}

CDlgPCMonitor::~CDlgPCMonitor()
{
}

void CDlgPCMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_Y_MAX, m_io_Y_Max);
	DDX_Control(pDX, IDC_ED_Y_MIN, m_io_Y_Min);
	DDX_Control(pDX, IDC_ED_X_MAX, m_io_X_Max);
	DDX_Control(pDX, IDC_LIST_WAVE1, m_list_wave1);
	DDX_Control(pDX, IDC_LIST_WAVE2, m_list_wave2);
	DDX_Control(pDX, IDC_LIST_WAVE3, m_list_wave3);
	DDX_Control(pDX, IDC_LIST_WAVE4, m_list_wave4);
	DDX_Control(pDX, IDC_LIST_WAVE5, m_list_wave5);
	DDX_Control(pDX, IDC_ED_DATA1, m_io_Data1);
	DDX_Control(pDX, IDC_ED_DATA2, m_io_Data2);
	DDX_Control(pDX, IDC_ED_DATA3, m_io_Data3);
	DDX_Control(pDX, IDC_ED_DATA4, m_io_Data4);
	DDX_Control(pDX, IDC_ED_DATA5, m_io_Data5);
}


BEGIN_MESSAGE_MAP(CDlgPCMonitor, CDialogX)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE1, &CDlgPCMonitor::OnLvnItemchangedListWave1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE2, &CDlgPCMonitor::OnLvnItemchangedListWave2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE3, &CDlgPCMonitor::OnLvnItemchangedListWave3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE4, &CDlgPCMonitor::OnLvnItemchangedListWave4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE5, &CDlgPCMonitor::OnLvnItemchangedListWave5)
	ON_BN_CLICKED(IDC_BTN_CLR1, &CDlgPCMonitor::OnBnClickedBtnClr1)
	ON_BN_CLICKED(IDC_BTN_CLR2, &CDlgPCMonitor::OnBnClickedBtnClr2)
	ON_BN_CLICKED(IDC_BTN_CLR3, &CDlgPCMonitor::OnBnClickedBtnClr3)
	ON_BN_CLICKED(IDC_BTN_CLR4, &CDlgPCMonitor::OnBnClickedBtnClr4)
	ON_BN_CLICKED(IDC_BTN_CLR5, &CDlgPCMonitor::OnBnClickedBtnClr5)
	ON_BN_CLICKED(IDC_BTN_START, &CDlgPCMonitor::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgPCMonitor::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CDlgPCMonitor メッセージ ハンドラー


BOOL CDlgPCMonitor::OnInitDialog()
{
	CDialogX::OnInitDialog();

	int		i,j;
	CRect	rc;
	CString	szStr('\0', 128);
	int		iVal;
	LV_COLUMN	lvc;
	LV_ITEM		lvi;

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	m_bmpPC.LoadBitmapA(m_uidPCBmp[m_iPMNo]);
	((CStatic*)GetDlgItem(IDC_BMP_PC))->SetBitmap(m_bmpPC);

	GetClientRect(rc);
	rc.DeflateRect(160, 40);
	rc.OffsetRect(70, 0);
	GetDlgItem(IDC_RECT_GRAPH)->MoveWindow(rc);
	rc.DeflateRect(4, 4);
	m_rcGraph = rc;
	m_dHeight = (double)rc.Height();
	m_dWidth = (double)rc.Width();


	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	m_pWaveData_Eth = NULL;
	m_pWaveData_USB = NULL;

	if (m_pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
		m_pWaveData_Eth = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_Eth();
	else
		m_pWaveData_USB = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_USB();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	szStr.Format("CTC.Graph.YMin_PC%d", (m_iPMNo + 1));
	m_io_Y_Min.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
	szStr.Format("CTC.Graph.YMax_PC%d", (m_iPMNo + 1));
	m_io_Y_Max.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
	szStr.Format("CTC.Graph.XMax_PC%d", (m_iPMNo + 1));
	m_io_X_Max.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
	iVal = (int)m_dYMin[m_iPMNo];
	m_pIODB->IOPointWrite(m_io_Y_Min.GetIOHdl(), &iVal);
	iVal = (int)m_dYMax[m_iPMNo];
	m_pIODB->IOPointWrite(m_io_Y_Max.GetIOHdl(), &iVal);
	iVal = (int)m_dXMax[m_iPMNo];
	m_pIODB->IOPointWrite(m_io_X_Max.GetIOHdl(), &iVal);

	m_io_Y_Min.SetSpecialColor(2);
	m_io_Y_Max.SetSpecialColor(2);
	m_io_X_Max.SetSpecialColor(2);
	m_io_Y_Min.UpdateDisplay();
	m_io_Y_Max.UpdateDisplay();
	m_io_X_Max.UpdateDisplay();

	m_io_Data1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PCMonitor1"));
	m_io_Data2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PCMonitor2"));
	m_io_Data3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PCMonitor3"));
	m_io_Data4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PCMonitor4"));
	m_io_Data5.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PCMonitor5"));
	m_io_Data1.SetSpecialColor(1);
	m_io_Data2.SetSpecialColor(1);
	m_io_Data3.SetSpecialColor(1);
	m_io_Data4.SetSpecialColor(1);
	m_io_Data5.SetSpecialColor(1);


	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_wave1.InsertColumn(i, &lvc);
		m_list_wave2.InsertColumn(i, &lvc);
		m_list_wave3.InsertColumn(i, &lvc);
		m_list_wave4.InsertColumn(i, &lvc);
		m_list_wave5.InsertColumn(i, &lvc);
	}/* for */
	m_list_wave1.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave1.SetModeGridLine(TRUE, TRUE);
	m_list_wave2.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave2.SetModeGridLine(TRUE, TRUE);
	m_list_wave3.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave3.SetModeGridLine(TRUE, TRUE);
	m_list_wave4.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave4.SetModeGridLine(TRUE, TRUE);
	m_list_wave5.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave5.SetModeGridLine(TRUE, TRUE);

	for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
		szStr.Format("%04.04d", (i + 1));
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_wave1.InsertItem(&lvi);
		m_list_wave1.SetItemData(i, i);
		m_list_wave2.InsertItem(&lvi);
		m_list_wave2.SetItemData(i, i);
		m_list_wave3.InsertItem(&lvi);
		m_list_wave3.SetItemData(i, i);
		m_list_wave4.InsertItem(&lvi);
		m_list_wave4.SetItemData(i, i);
		m_list_wave5.InsertItem(&lvi);
		m_list_wave5.SetItemData(i, i);

		szStr= m_pConfig->m_tWaveConfig2[m_iPMNo].szWaveLenName[i];
		m_list_wave1.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_wave2.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_wave3.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_wave4.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_wave5.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
	}

	for (i = 0; i < 5; i++) {
		if (m_iSelectNo[m_iPMNo][i] > 0) {
			switch (i) {
			case 0: m_list_wave1.SelectItem((m_iSelectNo[m_iPMNo][i] - 1));
				GetDlgItem(IDC_BTN_CLR1)->EnableWindow(TRUE); 
				break;
			case 1: m_list_wave2.SelectItem((m_iSelectNo[m_iPMNo][i] - 1)); 
				GetDlgItem(IDC_BTN_CLR2)->EnableWindow(TRUE);
				break;
			case 2: m_list_wave3.SelectItem((m_iSelectNo[m_iPMNo][i] - 1)); 
				GetDlgItem(IDC_BTN_CLR3)->EnableWindow(TRUE);
				break;
			case 3: m_list_wave4.SelectItem((m_iSelectNo[m_iPMNo][i] - 1)); 
				GetDlgItem(IDC_BTN_CLR4)->EnableWindow(TRUE);
				break;
			case 4: m_list_wave5.SelectItem((m_iSelectNo[m_iPMNo][i] - 1)); 
				GetDlgItem(IDC_BTN_CLR5)->EnableWindow(TRUE);
				break;
			}/* switch */
		}
		else {
			switch (i) {
			case 0:GetDlgItem(IDC_BTN_CLR1)->EnableWindow(FALSE); break;
			case 1:GetDlgItem(IDC_BTN_CLR2)->EnableWindow(FALSE); break;
			case 2:GetDlgItem(IDC_BTN_CLR3)->EnableWindow(FALSE); break;
			case 3:GetDlgItem(IDC_BTN_CLR4)->EnableWindow(FALSE); break;
			case 4:GetDlgItem(IDC_BTN_CLR5)->EnableWindow(FALSE); break;
			}/* switch */

		}
	}
	for (i = 0; i < 5; i++)
		m_usLastData[i] = 0;

	rc = m_rcGraph;
	rc.left -= 80;
	InvalidateRect(rc, TRUE);

	SetTimer(1, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgPCMonitor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
					   // 描画メッセージで CDialogX::OnPaint() を呼び出さないでください。
	dc.FillSolidRect(&m_rcGraph, RGB(200, 250, 250));

	POINT	LinePoint[2];
	double	dWidth;
	double	dHeight;
	double	dVal;
	CRect	rc;
	int		i;
	int		iTM;
	int		iTotal;
	int		iCnt;
//	unsigned short		usLastData[10 + 1];
	CString	szStr('\0',32);
	int		iVal;

	/* Pen ... 点線/緑 */
	CPen	DotPen;
	CPen	GraphPen;

	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));

	CPen* oldPen = dc.SelectObject(&DotPen);

	LinePoint[0].x = m_rcGraph.left;
	LinePoint[1].x = m_rcGraph.right;
	for (i = 1; i < 4; i++) {
		LinePoint[0].y = m_rcGraph.bottom - (m_dHeight / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);
	}

	LinePoint[0].y = m_rcGraph.top;
	LinePoint[1].y = m_rcGraph.bottom;
	for (i = 1; i < 4; i++) {
		LinePoint[0].x = m_rcGraph.left + (m_dWidth / 4 * i);
		LinePoint[1].x = LinePoint[0].x;
		dc.Polyline(&LinePoint[0], 2);
	}

	if (m_dXMax[m_iPMNo] <= (m_tPC_Monitor[m_iPMNo].index / 10 * m_tPC_Monitor[m_iPMNo].iSamplingCycle) || m_tPC_Monitor[m_iPMNo].iSamplingCycle != m_iSamplingCycle) {
		m_iSamplingCycle = m_tPC_Monitor[m_iPMNo].iSamplingCycle;

		if (m_dXMax[m_iPMNo] < 3600) {
			if (m_dXMax[m_iPMNo] < 600)
				m_dXMax[m_iPMNo] += 60;
			else
				m_dXMax[m_iPMNo] += 600;
		}
	}
	for (iCnt = 0; iCnt < m_tPC_Monitor[m_iPMNo].index; iCnt++) {
		for (i = 0; i < 5; i++) {
			if (m_iSelectNo[m_iPMNo][i] == 0)
				continue;

			if(m_tPC_Monitor[m_iPMNo].iSamplingCycle==1)
				m_ptGraph[i][iCnt].x = (m_dWidth / (m_dXMax[m_iPMNo]*10)) * iCnt + m_rcGraph.left;
			else
				m_ptGraph[i][iCnt].x = (m_dWidth / 6000.0f) * iCnt + m_rcGraph.left;
			
			dVal = (double)m_tPC_Monitor[m_iPMNo].usMonitorData[iCnt][m_iSelectNo[m_iPMNo][i] - 1];
			m_usLastData[i] = m_tPC_Monitor[m_iPMNo].usMonitorData[iCnt][m_iSelectNo[m_iPMNo][i] - 1];

			if (dVal < m_dYMin[m_iPMNo])
				dVal = m_dYMin[m_iPMNo];
			else if (dVal > m_dYMax[m_iPMNo])
				dVal = m_dYMax[m_iPMNo];

			m_ptGraph[i][iCnt].y = m_rcGraph.bottom - (int)((m_dHeight / (m_dYMax[m_iPMNo] - m_dYMin[m_iPMNo])) * (dVal - m_dYMin[m_iPMNo]));
		}/* for */
	}/* for */

	for (i = 0; i < 5; i++) {
		if (m_iSelectNo[m_iPMNo][i] == 0)
			continue;

		GraphPen.CreatePen(PS_SOLID, 2, RGB(m_iColorTbl[i][0], m_iColorTbl[i][1], m_iColorTbl[i][2]));
		dc.SelectObject(&GraphPen);
		dc.Polyline(m_ptGraph[i], iCnt);
		GraphPen.DeleteObject();
	}
#if 0
	GetClientRect(rc);
	rc.OffsetRect(90, 78);
	rc.right = rc.left + 132;
	rc.bottom = rc.top + 24;

	for (i = 0; i < 5; i++) {
		if (m_iSelectNo[m_iPMNo][i] == 0) {
			rc.OffsetRect(0, 180);
			continue;
		}
		GraphPen.CreatePen(PS_SOLID, 2, RGB(m_iColorTbl[i][0], m_iColorTbl[i][1], m_iColorTbl[i][2]));
		dc.SelectObject(&GraphPen);
#if 0
			dc.Polyline(m_ptGraph[i], iTotal);
#endif

		dc.SetTextColor(RGB(m_iColorTbl[i][0], m_iColorTbl[i][1], m_iColorTbl[i][2]));
		szStr.Format(" %5u", usLastData[i]);
		dc.DrawText(szStr, rc, DT_RIGHT);
		rc.OffsetRect(0,180);

		GraphPen.DeleteObject();
	}
#endif
	dc.SelectObject(oldPen);
	DotPen.DeleteObject();
}

void CDlgPCMonitor::OnTimer(UINT_PTR nIDEvent)
{
	int		iVal;
	CRect	rc;
	int		bUpdate=FALSE;

	m_pIODB->IOPointWrite(m_io_Data1.GetIOHdl(),&m_usLastData[0]);
	m_pIODB->IOPointWrite(m_io_Data2.GetIOHdl(), &m_usLastData[1]);
	m_pIODB->IOPointWrite(m_io_Data3.GetIOHdl(), &m_usLastData[2]);
	m_pIODB->IOPointWrite(m_io_Data4.GetIOHdl(), &m_usLastData[3]);
	m_pIODB->IOPointWrite(m_io_Data5.GetIOHdl(), &m_usLastData[4]);
	m_io_Data1.ChkValueChange();
	m_io_Data2.ChkValueChange();
	m_io_Data3.ChkValueChange();
	m_io_Data4.ChkValueChange();
	m_io_Data5.ChkValueChange();

	if (m_io_Y_Min.m_bUpdate) {
		m_pIODB->IOPointRead(m_io_Y_Min.GetIOHdl(), &iVal);
		m_dYMin[m_iPMNo] = (double)iVal;
		m_io_Y_Min.m_bUpdate = FALSE;
		bUpdate = TRUE;
	}
	if (m_io_Y_Max.m_bUpdate) {
		m_pIODB->IOPointRead(m_io_Y_Max.GetIOHdl(), &iVal);
		m_dYMax[m_iPMNo] = (double)iVal;
		m_io_Y_Max.m_bUpdate = FALSE;
		bUpdate = TRUE;
	}

	m_pIODB->IOPointRead(m_io_X_Max.GetIOHdl(), &iVal);
	if(iVal != (int)m_dXMax[m_iPMNo]){
		iVal = m_dXMax[m_iPMNo];
		m_pIODB->IOPointWrite(m_io_X_Max.GetIOHdl(), &iVal);
		m_io_X_Max.UpdateDisplay();
		bUpdate = TRUE;
	}

	if (m_tPC_Monitor[m_iPMNo].iStart) {
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);

		if (bUpdate) {
			rc = m_rcGraph;
			rc.left -= 80;
			InvalidateRect(rc, TRUE);
		}
		else {
			rc = m_rcGraph;
			rc.right = (m_dWidth / (m_dXMax[m_iPMNo] * 10/ m_tPC_Monitor[m_iPMNo].iSamplingCycle)) * m_tPC_Monitor[m_iPMNo].index + rc.left;
			rc.left = rc.right - 5;
			InvalidateRect(rc, FALSE);
#if 0
			rc = m_rcGraph;
				rc.right = rc.left;
				rc.left -= rc.right - 25;
				InvalidateRect(rc, TRUE);
#endif
		}
	}
	else {
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}



	CDialogX::OnTimer(nIDEvent);
}


void CDlgPCMonitor::OnLvnItemchangedListWave1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	SelchangeWave(0);
}
void CDlgPCMonitor::OnLvnItemchangedListWave2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	SelchangeWave(1);
}
void CDlgPCMonitor::OnLvnItemchangedListWave3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	SelchangeWave(2);
}
void CDlgPCMonitor::OnLvnItemchangedListWave4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	SelchangeWave(3);
}
void CDlgPCMonitor::OnLvnItemchangedListWave5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	SelchangeWave(4);
}


void CDlgPCMonitor::SelchangeWave(int iNo)
{
	CVListCtrl* pList = NULL;
	int			itemNo;
	int			iWaveNo;
	CRect		rc;

	switch (iNo) {
	case 0:pList = &m_list_wave1; break;
	case 1:pList = &m_list_wave2; break;
	case 2:pList = &m_list_wave3; break;
	case 3:pList = &m_list_wave4; break;
	case 4:pList = &m_list_wave5; break;
	}/* switch */

	if (pList) {
		itemNo = -1;
		itemNo = pList->GetSelectedItem(itemNo);
		if (itemNo == -1)
			return;

		iWaveNo = pList->GetItemData(itemNo);

		m_iSelectNo[m_iPMNo][iNo] = iWaveNo + 1;
		rc = m_rcGraph;
		rc.left -= 80;
		InvalidateRect(rc, FALSE);
	}
	switch (iNo) {
	case 0:GetDlgItem(IDC_BTN_CLR1)->EnableWindow(TRUE); break;
	case 1:GetDlgItem(IDC_BTN_CLR2)->EnableWindow(TRUE); break;
	case 2:GetDlgItem(IDC_BTN_CLR3)->EnableWindow(TRUE); break;
	case 3:GetDlgItem(IDC_BTN_CLR4)->EnableWindow(TRUE); break;
	case 4:GetDlgItem(IDC_BTN_CLR5)->EnableWindow(TRUE); break;
	}/* switch */

}


void CDlgPCMonitor::WaveClear(int iNo)
{
	CRect	rc;

	if (m_iSelectNo[m_iPMNo][iNo] != 0) {

		switch (iNo) {
		case 0:
			GetDlgItem(IDC_BTN_CLR1)->EnableWindow(FALSE); 
			m_list_wave1.SetItemState((m_iSelectNo[m_iPMNo][iNo]-1),0, LVIS_SELECTED);
			break;
		case 1:
			GetDlgItem(IDC_BTN_CLR2)->EnableWindow(FALSE); 
			m_list_wave2.SetItemState((m_iSelectNo[m_iPMNo][iNo] - 1), 0, LVIS_SELECTED);
			break;
		case 2:
			GetDlgItem(IDC_BTN_CLR3)->EnableWindow(FALSE); 
			m_list_wave3.SetItemState((m_iSelectNo[m_iPMNo][iNo] - 1), 0, LVIS_SELECTED);
			break;
		case 3:
			GetDlgItem(IDC_BTN_CLR4)->EnableWindow(FALSE); 
			m_list_wave4.SetItemState((m_iSelectNo[m_iPMNo][iNo] - 1), 0, LVIS_SELECTED);
			break;
		case 4:
			GetDlgItem(IDC_BTN_CLR5)->EnableWindow(FALSE); 
			m_list_wave5.SetItemState((m_iSelectNo[m_iPMNo][iNo] - 1), 0, LVIS_SELECTED);
			break;
		}/* switch */

		m_iSelectNo[m_iPMNo][iNo] = 0;
		rc = m_rcGraph;
		rc.left -= 80;
		InvalidateRect(rc, FALSE);
	}
}


void CDlgPCMonitor::OnBnClickedBtnStart()
{
	m_iSamplingCycle = 1;
	StartPCMonitor(m_iPMNo);
}


void CDlgPCMonitor::OnBnClickedBtnStop()
{
	StopPCMonitor(m_iPMNo);
}
