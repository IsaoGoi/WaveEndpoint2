// CDlgAnalysis.cpp : 実装ファイル
//

#include "pch.h"
#include "vector"
#include "string"
#include "WaveEndPoint.h"
#include "CDlgAnalysis.h"




// CDlgAnalysis ダイアログ

IMPLEMENT_DYNAMIC(CDlgAnalysis, CDialogX)

#define NUM_COLUMNS				3								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_WAVE			1
#define	SUB_ITEM_NAME			2

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("No."),
	_T("Wave"),
	_T("Name"),
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
	40,60,100
};



std::vector<std::string>	g_tPC1_List(1000);
std::vector<std::string>	g_tPC2_List(1000);
std::vector<std::string>	g_tPC3_List(1000);
std::vector<std::string>	g_tPC4_List(1000);
std::vector<std::string>	g_tPC5_List(1000);

std::vector<SAMPLE_WAVE>	g_tDisplayData(12000);


static	UINT		m_uidWaveLength[5+1] = { IDC_ED_WaveLen1 ,IDC_ED_WaveLen2,IDC_ED_WaveLen3,IDC_ED_WaveLen4,IDC_ED_WaveLen5};
static	UINT		m_uidWaveMax[5 + 1] = { IDC_ED_MAX1 ,IDC_ED_MAX2,IDC_ED_MAX3,IDC_ED_MAX4,IDC_ED_MAX5};
static	UINT		m_uidDiffWaveLength[5 + 1] = { IDC_ED_DiffWaveLen1 ,IDC_ED_DiffWaveLen2,IDC_ED_DiffWaveLen3,IDC_ED_DiffWaveLen4,IDC_ED_DiffWaveLen5};
static	UINT		m_uidWaveDiff[5 + 1] = { IDC_ED_DIFF1 ,IDC_ED_DIFF2,IDC_ED_DIFF3,IDC_ED_DIFF4,IDC_ED_DIFF5};

static	UINT		m_uidMaxChkBtn[5 + 1] = { IDC_CHK_MAX1,IDC_CHK_MAX2,IDC_CHK_MAX3,IDC_CHK_MAX4,IDC_CHK_MAX5};
static	UINT		m_uidDiffChkBtn[5 + 1] = { IDC_CHK_DIFF1 ,IDC_CHK_DIFF2,IDC_CHK_DIFF3,IDC_CHK_DIFF4,IDC_CHK_DIFF5};

void get_samplinglist(int iPM, std::vector<std::string>& pList)
{
	HANDLE				hFind;
	WIN32_FIND_DATA		win32fd;
	CString				szSerchPath('\0',32);
	CString				szFilePath('\0',32);
	
	pList.clear();

	szSerchPath.Format("RowWaveLog\\*.PM%d", (iPM + 1));
	hFind = FindFirstFile(szSerchPath, &win32fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		if ((win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			pList.push_back(win32fd.cFileName);
		}
	} while (FindNextFile(hFind, &win32fd));
	
	FindClose(hFind);
}

CDlgAnalysis::CDlgAnalysis(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_ANALYSIS, pParent)
	, m_szPC(_T(""))
	, m_szSamplingTM(_T(""))
	, m_szFileName(_T(""))
	, m_szWaveMin(_T(""))
	, m_szWaveMax(_T(""))
{
	m_iPMNo = -1;
	m_pImage = NULL;
	m_iSamplingTime = 0;
	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_ANALYSIS, pParent);
}

CDlgAnalysis::~CDlgAnalysis()
{
	if (m_pImage)
		delete m_pImage;
}

void CDlgAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_Tree_Sampling);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll_WaveLength);
	DDX_Text(pDX, IDC_ED_PC, m_szPC);
	DDX_Text(pDX, IDC_ED_SAMPLETIME, m_szSamplingTM);
	DDX_Text(pDX, IDC_ED_FILENAME, m_szFileName);
	DDX_Control(pDX, IDC_ED_SERCHSTART, m_io_SerchStartTime);
	DDX_Control(pDX, IDC_ED_SERCHEND, m_io_SerchEndTime);
	DDX_Control(pDX, IDC_LIST_WAVE, m_list_wave);
	DDX_Control(pDX, IDC_LIST_WAVE2, m_list_wave2);
	DDX_Control(pDX, IDC_ED_STARTWAVE, m_io_SerchStartWave);
	DDX_Control(pDX, IDC_ED_ENDWAVE, m_io_SerchEndWave);
	DDX_Text(pDX, IDC_ED_WAVE_MIN, m_szWaveMin);
	DDX_Text(pDX, IDC_ED_WAVE_MAX, m_szWaveMax);
}


BEGIN_MESSAGE_MAP(CDlgAnalysis, CDialogX)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CDlgAnalysis::OnTvnSelchangedTreeFile)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SERCH, &CDlgAnalysis::OnBnClickedBtnSerch)
	ON_BN_CLICKED(IDC_CHK_MAX1, &CDlgAnalysis::OnBnClickedChkMax1)
	ON_BN_CLICKED(IDC_CHK_MAX2, &CDlgAnalysis::OnBnClickedChkMax2)
	ON_BN_CLICKED(IDC_CHK_MAX3, &CDlgAnalysis::OnBnClickedChkMax3)
	ON_BN_CLICKED(IDC_CHK_MAX4, &CDlgAnalysis::OnBnClickedChkMax4)
	ON_BN_CLICKED(IDC_CHK_MAX5, &CDlgAnalysis::OnBnClickedChkMax5)
	ON_BN_CLICKED(IDC_CHK_DIFF1, &CDlgAnalysis::OnBnClickedChkDiff1)
	ON_BN_CLICKED(IDC_CHK_DIFF2, &CDlgAnalysis::OnBnClickedChkDiff2)
	ON_BN_CLICKED(IDC_CHK_DIFF3, &CDlgAnalysis::OnBnClickedChkDiff3)
	ON_BN_CLICKED(IDC_CHK_DIFF4, &CDlgAnalysis::OnBnClickedChkDiff4)
	ON_BN_CLICKED(IDC_CHK_DIFF5, &CDlgAnalysis::OnBnClickedChkDiff5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE, &CDlgAnalysis::OnLvnItemchangedListWave)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WAVE2, &CDlgAnalysis::OnLvnItemchangedListWave2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CSV, &CDlgAnalysis::OnBnClickedBtnCsv)
END_MESSAGE_MAP()


// CDlgAnalysis メッセージ ハンドラー


BOOL CDlgAnalysis::OnInitDialog()
{
	CRect		rc;
	int			i;
	LV_COLUMN	lvc;
	CString		szStr('\0', 128);
	float		fVal;

	CDialogX::OnInitDialog();

	m_iSelectWaveNo = -1;
	m_iSelectWaveNo2 = -1;

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	// Wave Graph Rect
	rc.DeflateRect(10, 10);
	rc.OffsetRect(80, 80);
	GetDlgItem(IDC_WAVELENGTH_RECT)->GetClientRect(m_Rect_WaveGraph);
	m_Rect_WaveGraph.OffsetRect(rc.left, rc.top);
	GetDlgItem(IDC_WAVELENGTH_RECT)->MoveWindow(m_Rect_WaveGraph);
	m_Rect_WaveGraph.DeflateRect(2, 2);

	GetDlgItem(IDC_RECT_SELGRAPH)->GetClientRect(m_Rect_MaxDiffGraph);
	m_Rect_MaxDiffGraph.OffsetRect(rc.left, rc.top+ m_Rect_WaveGraph.Height()+100);
	GetDlgItem(IDC_RECT_SELGRAPH)->MoveWindow(m_Rect_MaxDiffGraph);
	m_Rect_MaxDiffGraph.DeflateRect(2, 2);


	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	m_io_SerchStartTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.SerchStartTM"));
	m_io_SerchEndTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.SerchEndTM"));
	m_io_SerchStartTime.SetSpecialColor(2);
	m_io_SerchEndTime.SetSpecialColor(2);
	m_io_SerchStartWave.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.SerchStartWave"));
	m_io_SerchEndWave.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.SerchEndWave"));
	m_io_SerchStartWave.SetSpecialColor(2);
	m_io_SerchEndWave.SetSpecialColor(2);
	fVal = 200.0f;
	m_pIODB->IOPointWrite(m_io_SerchStartWave.GetIOHdl(), &fVal);
	fVal = 900.0f;
	m_pIODB->IOPointWrite(m_io_SerchEndWave.GetIOHdl(), &fVal);

	for (i = 0; i < 5; i++) {
		GetDlgItem(m_uidMaxChkBtn[i])->EnableWindow(FALSE);
		GetDlgItem(m_uidDiffChkBtn[i])->EnableWindow(FALSE);
	}

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_wave.InsertColumn(i, &lvc);
		m_list_wave2.InsertColumn(i, &lvc);
	}/* for */
	m_list_wave.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave.SetModeGridLine(TRUE, TRUE);
	m_list_wave2.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK) | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));
	m_list_wave2.SetModeGridLine(TRUE, TRUE);

#if 0
	LV_ITEM		lvi;
	for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
		szStr.Format("%04.04d", (i + 1));
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_wave.InsertItem(&lvi);
		m_list_wave.SetItemData(i, i);
		m_list_wave2.InsertItem(&lvi);
		m_list_wave2.SetItemData(i, i);

		szStr.Format("%6.2fum", m_pConfig->m_tWaveConfig.fWaveLength[i]);
		m_list_wave.SetItemText(i, SUB_ITEM_WAVE, (LPCSTR)szStr);
		m_list_wave2.SetItemText(i, SUB_ITEM_WAVE, (LPCSTR)szStr);

		szStr = m_pConfig->m_tWaveConfig.szWaveLenName[i];
		m_list_wave.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_list_wave2.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
	}
#endif

	DisplaySamplingList();

	GetDlgItem(IDC_BTN_CSV)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SERCH)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgAnalysis::DisplaySamplingList()
{
	int				iPM,i,iCnt;
	CString			szWork;
	HTREEITEM		hTreeHdl, hDataHdl, hSubHdl;
	HTREEITEM		hSelNoHdl = NULL;
	std::vector<std::string> tList;

	long lState = GetWindowLong(m_Tree_Sampling.m_hWnd, GWL_STYLE);
	lState |= (TVS_HASLINES);
	SetWindowLong(m_Tree_Sampling.m_hWnd, GWL_STYLE, lState);
	m_pImage = new CImageList();
	m_pImage->Create(IDB_CFG_TREE, 24, 2, RGB(255, 255, 255));
	m_Tree_Sampling.SetImageList(m_pImage, TVSIL_NORMAL);

	TV_INSERTSTRUCT	tcItem;

	tcItem.hInsertAfter = TVI_LAST;
	tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	for (iPM = 0; iPM < PM_MAX; iPM++) {
		if (m_pConfig->m_tEPDConfig.iPCEnable[iPM]) {
			tcItem.hParent = TVI_ROOT;
			szWork.Format("PC%d", (iPM + 1));
			tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
			tcItem.item.lParam = (iPM + 1);
			tcItem.item.iImage = 0;
			tcItem.item.iSelectedImage = 1;
			hTreeHdl = m_Tree_Sampling.InsertItem(&tcItem);

			switch (iPM) {
			case PM1:	tList = g_tPC1_List; break;
			case PM2:	tList = g_tPC2_List; break;
			case PM3:	tList = g_tPC3_List; break;
			case PM4:	tList = g_tPC4_List; break;
			case PM5:	tList = g_tPC5_List; break;
			}/* switch */

			tList.clear();
			get_samplinglist(iPM, tList);
			iCnt = tList.size();

			switch (iPM) {
			case PM1:	g_tPC1_List = tList; break;
			case PM2:	g_tPC2_List = tList; break;
			case PM3:	g_tPC3_List = tList; break;
			case PM4:	g_tPC4_List = tList; break;
			case PM5:	g_tPC5_List = tList; break;
			}/* switch */


			//モジュール内のデータをセット
			for (i = 0; i < iCnt; i++) {
				szWork = tList[i].c_str();
				tcItem.hParent = hTreeHdl;
				tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
				tcItem.item.lParam = ((iPM+1)*10000) + i;
				tcItem.item.iImage = 0;
				tcItem.item.iSelectedImage = 1;
				hDataHdl = m_Tree_Sampling.InsertItem(&tcItem);
			}/* for */
		}
	}
}


void CDlgAnalysis::OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	HTREEITEM		hTreeHdl;
	DWORD_PTR		index, iSelectNo;
	int				iPCNo = -1;
	CString			szFileName;
	CString			szStr;
	CFile			fp;
	WRITE_WAVEDATA	m_tWaveHead;
	SAMPLE_WAVE		m_tWave;
	double			dVal;
	unsigned short	usVal;
	int				i;
	LV_ITEM		lvi;

	hTreeHdl = m_Tree_Sampling.GetSelectedItem();
	if (hTreeHdl) {
		index = m_Tree_Sampling.GetItemData(hTreeHdl);

		if (index < 10000)
			return;

		iPCNo = index / 10000;
		--iPCNo;

		iSelectNo = index % 10000;

		switch (iPCNo) {
		case PM1:	szFileName.Format("RowWaveLog\\%s", g_tPC1_List[iSelectNo].c_str()); break;
		case PM2:	szFileName.Format("RowWaveLog\\%s", g_tPC2_List[iSelectNo].c_str()); break;
		case PM3:	szFileName.Format("RowWaveLog\\%s", g_tPC3_List[iSelectNo].c_str()); break;
		case PM4:	szFileName.Format("RowWaveLog\\%s", g_tPC4_List[iSelectNo].c_str()); break;
		case PM5:	szFileName.Format("RowWaveLog\\%s", g_tPC5_List[iSelectNo].c_str()); break;
		}

		g_tDisplayData.clear();
		if (fp.Open(szFileName, CFile::modeRead)) {
			fp.Read(&m_tWaveHead, sizeof(WRITE_WAVEDATA));

			while (1) {
				if (fp.Read(&m_tWave, sizeof(SAMPLE_WAVE)) == 0)
					break;
				g_tDisplayData.push_back(m_tWave);

			}/* while */
			fp.Close();
		}
	}

	m_iSamplingTime = m_tWaveHead.iSampling100msec;
	m_szPC.Format("PC%d",(iPCNo+1));
	m_szSamplingTM.Format("%d", m_iSamplingTime * 100);
	m_szFileName = szFileName;

	if (g_tDisplayData.size()) {
		dVal = g_tDisplayData.size() - 1;
		dVal *= m_iSamplingTime;
		dVal /= 10.0f;
	}
	else
		dVal = 0.0f;

	szStr.Format("%5.2f", dVal);
	GetDlgItem(IDC_ED_ENDTIME)->SetWindowText(szStr);

	usVal = 0;
	m_pIODB->IOPointWrite(m_io_SerchStartTime.GetIOHdl(),&usVal);
	usVal = (unsigned short)dVal;
	m_pIODB->IOPointWrite(m_io_SerchEndTime.GetIOHdl(), &usVal);
	m_io_SerchStartTime.UpdateDisplay();
	m_io_SerchEndTime.UpdateDisplay();

	m_tScrollInfo.cbSize =sizeof(SCROLLINFO);
	m_tScrollInfo.fMask = SIF_ALL;
	m_tScrollInfo.nMin = 0;
	m_tScrollInfo.nMax = g_tDisplayData.size();
	if (m_tScrollInfo.nMax)
		m_tScrollInfo.nMax--;
	m_tScrollInfo.nPage = 1;
	m_tScrollInfo.nPos = 0;
	m_tScrollInfo.nTrackPos = 0;
	m_Scroll_WaveLength.SetScrollInfo(&m_tScrollInfo);


	if (iPCNo != m_iPMNo) {
		m_iPMNo = iPCNo;
		m_list_wave.DeleteAllItems();
		m_list_wave2.DeleteAllItems();

		for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
			szStr.Format("%04.04d", (i + 1));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = i;
			lvi.iSubItem = SUB_ITEM_NO;
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			m_list_wave.InsertItem(&lvi);
			m_list_wave.SetItemData(i, i);
			m_list_wave2.InsertItem(&lvi);
			m_list_wave2.SetItemData(i, i);

			szStr.Format("%6.2fum", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i]);
			m_list_wave.SetItemText(i, SUB_ITEM_WAVE, (LPCSTR)szStr);
			m_list_wave2.SetItemText(i, SUB_ITEM_WAVE, (LPCSTR)szStr);

			szStr = m_pConfig->m_tWaveConfig2[m_iPMNo].szWaveLenName[i];
			m_list_wave.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
			m_list_wave2.SetItemText(i, SUB_ITEM_NAME, (LPCSTR)szStr);
		}

	}
	m_szWaveMin.Format("%6.2fum", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[0]);
	m_szWaveMax.Format("%6.2fum", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_pConfig->m_tEPDConfig.iWaveMax-1]);


	UpdateData(FALSE);
	InvalidateRect(m_Rect_WaveGraph);

	GetDlgItem(IDC_BTN_CSV)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SERCH)->EnableWindow(TRUE);
	OnBnClickedBtnSerch();
}




void CDlgAnalysis::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	int		nID;
	int		iPos;
	int		bChange = FALSE;

	nID = pScrollBar->GetDlgCtrlID();
	if (IDC_SCROLLBAR1 == nID) {
		iPos = m_Scroll_WaveLength.GetScrollPos();

		switch (nSBCode) {
		case SB_LINELEFT:
			if (iPos) {
				--iPos;
				bChange = TRUE;
			}
			break;
		case SB_LINERIGHT:
			if (iPos < m_tScrollInfo.nMax) {
				++iPos;
				bChange = TRUE;
			}
			break;
		case SB_THUMBPOSITION:
			iPos = nPos;
			bChange = TRUE;
			break;
		case SB_PAGELEFT:
			if (iPos) {
				if (iPos > 5)
					iPos -= 5;
				else
					iPos = 0;
				bChange = TRUE;
			}
			break;
		case SB_PAGERIGHT:
			if (iPos < m_tScrollInfo.nMax) {
				if ((iPos+5) < m_tScrollInfo.nMax)
					iPos += 5;
				else
					iPos = m_tScrollInfo.nMax;
				bChange = TRUE;
			}
			break;

		}/*switch*/
		if (bChange) {
			SCROLLINFO		tInfo;
			ZeroMemory(&tInfo, sizeof(SCROLLINFO));
			m_Scroll_WaveLength.GetScrollInfo(&tInfo);
			tInfo.nPos = iPos;
			tInfo.fMask= SIF_POS;
			m_Scroll_WaveLength.SetScrollInfo(&tInfo);
			InvalidateRect(m_Rect_WaveGraph);
		}
	}
	else
		CDialogX::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgAnalysis::OnPaint()
{
	int		iPos;
	int		i,j;
	POINT	LinePoint[2];
	int		iWidth;
	int		iHeight;
	double	dWidth;
	double	dHeight;
	double	dVal;
	CRect	rc;
	/* Pen ... 点線/緑 */
	CPen	DotPen;
	CPen	GraphPen;
	CBrush	GraphBrush;
	CString	szStr;
	unsigned short usStartVal, usEndVal;
	int				iStartPt;
	int				iEndPt;
	double			dMax;


	CPaintDC dc(this); // device context for painting

	iPos = m_Scroll_WaveLength.GetScrollPos();
	dVal = (iPos+1) * m_iSamplingTime;
	dVal /= 10.0f;
	szStr.Format("%5.2f", dVal);
	GetDlgItem(IDC_ED_CURRTIME)->SetWindowText(szStr);


	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));
	GraphPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	GraphBrush.CreateSolidBrush(RGB(255, 240, 240));

	dc.FillSolidRect(m_Rect_WaveGraph, RGB(200, 250, 250));
	iHeight = m_Rect_WaveGraph.Height();
	iWidth = m_Rect_WaveGraph.Width();

	CPen* oldPen = dc.SelectObject(&DotPen);

	LinePoint[0].x = m_Rect_WaveGraph.left;
	LinePoint[1].x = m_Rect_WaveGraph.right;

	for (i = 1; i < 4; i++) {
		LinePoint[0].y = m_Rect_WaveGraph.bottom - (iHeight / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);
	}

	// Wave 表示
	dWidth = (double)iWidth;
	dWidth /= (double)(EPD_WAVE_TOTAL + 2);

	dHeight = (double)iHeight;

	dc.SelectObject(&GraphPen);
	CBrush* oldBrush = dc.SelectObject(&GraphBrush);

	dMax = (double)(1024 * 16);

RECALC:
	for (i = 0; i < EPD_WAVE_TOTAL; i++) {
		if (i == 0) {
			m_pGraph[i].x = m_Rect_WaveGraph.left;
			m_pGraph[i].y = m_Rect_WaveGraph.bottom - 1;
			continue;
		}
		m_pGraph[i].x = m_Rect_WaveGraph.left + (long)((double)(i + 1) * dWidth);
		dVal = (double)g_tDisplayData[iPos].usData[i];
		if (dMax < dVal) {
			if (dMax == (double)(1024 * 64))
				break;
			else {
				dMax *= 2.0f;
				goto RECALC;
			}
		}
		dVal /= dMax;
		dVal *= dHeight;
		m_pGraph[i].y = m_Rect_WaveGraph.bottom - (long)(dVal);

	}/* for */
	m_pGraph[i].x = m_pGraph[i - 1].x;
	m_pGraph[i].y = m_Rect_WaveGraph.bottom - 1;

	dc.Polygon(m_pGraph, (EPD_WAVE_TOTAL + 1));
	dc.Polyline(m_pGraph, EPD_WAVE_TOTAL);

	szStr.Format("%d", (int)dMax);
	GetDlgItem(IDC_ED_GRAPH_MAX2)->SetWindowText(szStr);


	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);

	/* Max/Diff Select Graph*/
	dc.FillSolidRect(m_Rect_MaxDiffGraph, RGB(0, 0, 0));
	iHeight = m_Rect_MaxDiffGraph.Height();
	iWidth = m_Rect_MaxDiffGraph.Width();


	GraphPen.DeleteObject();
	DotPen.DeleteObject();
	DotPen.CreatePen(PS_DOT, 0, RGB(255, 255, 255));
	dc.SelectObject(&DotPen);

	LinePoint[0].x = m_Rect_MaxDiffGraph.left;
	LinePoint[1].x = m_Rect_MaxDiffGraph.right;

	for (i = 1; i < 4; i++) {
		LinePoint[0].y = m_Rect_MaxDiffGraph.bottom - (iHeight / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);
	}

	LinePoint[0].y = m_Rect_MaxDiffGraph.top;
	LinePoint[1].y = m_Rect_MaxDiffGraph.bottom;
	for (i = 1; i < 4; i++) {
		LinePoint[0].x = m_Rect_MaxDiffGraph.left + (iWidth / 4 * i);
		LinePoint[1].x = LinePoint[0].x;
		dc.Polyline(&LinePoint[0], 2);
	}

	m_pIODB->IOPointRead(m_io_SerchStartTime.GetIOHdl(), &usStartVal);
	if(g_tDisplayData.size()<=6000)
		m_pIODB->IOPointRead(m_io_SerchEndTime.GetIOHdl(), &usEndVal);
	else {
//		usEndVal = (unsigned short)g_tDisplayData.size() / 10;
		usStartVal = 500;
		usEndVal = 1100;
	}
	if (usEndVal != 0 && usStartVal != usEndVal && m_iSamplingTime) {
		iStartPt = usStartVal * 10 / m_iSamplingTime;
		iEndPt = usEndVal * 10 / m_iSamplingTime;

		dMax = (double)(1024 * 16);
RECALC2:
		// Wave 表示
		dWidth = (double)iWidth;
		dWidth /= (double)(iEndPt - iStartPt);
		dHeight = (double)iHeight;


		for (i = 0; i < 5; i++) {
			if (((CButton*)GetDlgItem(m_uidMaxChkBtn[i]))->GetCheck() == FALSE)
				continue;

			GraphPen.CreatePen(PS_SOLID, 2, RGB(255 - (i * 20), 255, 255 - (i * 20)));
			dc.SelectObject(&GraphPen);

//			for (j = iStartPt; j <= iEndPt; j++) {
			for (j = iStartPt; j <= iEndPt; j++) {
					m_pWaveGraph[j- iStartPt].x = m_Rect_MaxDiffGraph.left + (long)((double)(j - iStartPt) * dWidth);
				dVal = (double)g_tDisplayData[j].usData[m_iMax_Best10[i]];
				if (dVal > dMax) {
					if (dMax != 65535.0f) {
						dMax *= 2.0f;
						GraphPen.DeleteObject();
						goto RECALC2;
					}
				}
				dVal /= dMax;
				dVal *= dHeight;
				m_pWaveGraph[j - iStartPt].y = m_Rect_MaxDiffGraph.bottom - (long)(dVal);
			}
			dc.Polyline(m_pWaveGraph, (iEndPt - iStartPt+1));
			GraphPen.DeleteObject();

		}/* for */
		for (i = 0; i < 5; i++) {
			if (((CButton*)GetDlgItem(m_uidDiffChkBtn[i]))->GetCheck() == FALSE)
				continue;

			GraphPen.CreatePen(PS_SOLID, 2, RGB(255, 0+(i * 20), 0+(i * 20)));
			dc.SelectObject(&GraphPen);

			for (j = iStartPt; j <= iEndPt; j++) {
				m_pWaveGraph[j - iStartPt].x = m_Rect_MaxDiffGraph.left + (long)((double)(j - iStartPt) * dWidth);
				dVal = (double)g_tDisplayData[j].usData[m_iDiff_Best10[i]];
				if (dVal > dMax) {
					if (dMax != 65535.0f) {
						dMax *= 2.0f;
						GraphPen.DeleteObject();
						goto RECALC2;
					}
				}
				dVal /= dMax;
				dVal *= dHeight;
				m_pWaveGraph[j - iStartPt].y = m_Rect_MaxDiffGraph.bottom - (long)(dVal);
			}
			dc.Polyline(m_pWaveGraph, (iEndPt - iStartPt+1));
			GraphPen.DeleteObject();

		}/* for */
		if (m_iSelectWaveNo != -1) {

			GraphPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
			dc.SelectObject(&GraphPen);

			for (j = iStartPt; j < iEndPt; j++) {
				m_pWaveGraph[j - iStartPt].x = m_Rect_MaxDiffGraph.left + (long)((double)(j - iStartPt) * dWidth);
				dVal = (double)g_tDisplayData[j].usData[m_iSelectWaveNo];
				if (dVal > dMax) {
					if (dMax != 65535.0f) {
						dMax *= 2.0f;
						GraphPen.DeleteObject();
						goto RECALC2;
					}
				}
				dVal /= dMax;
				dVal *= dHeight;
				m_pWaveGraph[j - iStartPt].y = m_Rect_MaxDiffGraph.bottom - (long)(dVal);
			}
			dc.Polyline(m_pWaveGraph, (iEndPt - iStartPt));
			GraphPen.DeleteObject();

		}
		if (m_iSelectWaveNo2 != -1) {

			GraphPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			dc.SelectObject(&GraphPen);

			for (j = iStartPt; j < iEndPt; j++) {
				m_pWaveGraph[j - iStartPt].x = m_Rect_MaxDiffGraph.left + (long)((double)(j - iStartPt) * dWidth);
				dVal = (double)g_tDisplayData[j].usData[m_iSelectWaveNo2];
				if (dVal > dMax) {
					if (dMax != 65535.0f) {
						dMax *= 2.0f;
						GraphPen.DeleteObject();
						goto RECALC2;
					}
				}
				dVal /= dMax;
				dVal *= dHeight;
				m_pWaveGraph[j - iStartPt].y = m_Rect_MaxDiffGraph.bottom - (long)(dVal);
			}
			dc.Polyline(m_pWaveGraph, (iEndPt - iStartPt));
			GraphPen.DeleteObject();

		}
		szStr.Format("%d", (int)dMax);
		GetDlgItem(IDC_ED_GRAPH_YMAX)->SetWindowText(szStr);
	}

	DotPen.DeleteObject();
	GraphBrush.DeleteObject();
}


void CDlgAnalysis::OnBnClickedBtnSerch()
{
	unsigned short usStartVal, usEndVal;
	float	fStartWave, fEndWave;
	int				i,j,k,l;
	int				iStartPt;
	int				iEndPt;
	int				iVal;
	CString			szStr('\0', 32);
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	m_pIODB->IOPointRead(m_io_SerchStartTime.GetIOHdl(), &usStartVal);
	m_pIODB->IOPointRead(m_io_SerchEndTime.GetIOHdl(), &usEndVal);
	m_pIODB->IOPointRead(m_io_SerchStartWave.GetIOHdl(), &fStartWave);
	m_pIODB->IOPointRead(m_io_SerchEndWave.GetIOHdl(), &fEndWave);

	if (usStartVal >= usEndVal) {
		MessageBox("Invalid Start && End Time!", "Wave Serch", MB_ICONERROR);
		return;
	}
	szStr.Format("%dSec", usStartVal);
	GetDlgItem(IDC_ED_GRAPH_START_TM)->SetWindowTextA(szStr);
	szStr.Format("%dSec", usEndVal);
	GetDlgItem(IDC_ED_GRAPH_END_TM)->SetWindowTextA(szStr);


	ZeroMemory(m_usMaxData,sizeof(m_usMaxData));
	ZeroMemory(m_usDiffData, sizeof(m_usDiffData));
	ZeroMemory(m_iMax_Best10, sizeof(m_iMax_Best10));

	for(i=0;i< EPD_WAVE_TOTAL;i++)
		m_usMinData[i] = 0xFFFF;

	for (i = 0; i < 5; i++) {
		m_iDiff_Best10[i] = -1;
	}/* for */

	iStartPt = usStartVal * 10 / m_iSamplingTime;
	iEndPt   = usEndVal * 10 / m_iSamplingTime;
	m_iSerchMaxCnt=0;
	m_iSerchDiffCnt=0;

	for (i = iStartPt; i < iEndPt; i++) {
		for (j = 0; j < EPD_WAVE_TOTAL; j++) {
			if (pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[j] >= fStartWave && pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[j] <= fEndWave) {
				if (m_usMaxData[j] < g_tDisplayData[i].usData[j])
					m_usMaxData[j] = g_tDisplayData[i].usData[j];
				if (m_usMinData[j] > g_tDisplayData[i].usData[j])
					m_usMinData[j] = g_tDisplayData[i].usData[j];
			}
		}/* for */
	}/* for */

	for (i = 0; i < EPD_WAVE_TOTAL; i++) {
		if (pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] >= fStartWave && pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] <= fEndWave) {
			m_iMax_Best10[m_iSerchMaxCnt] = i;
			m_iSerchMaxCnt++;
		}
	}	
	for (i = 0; i < 5; i++) {
		for (j = i + 1; j < m_iSerchMaxCnt; j++) {
			if (m_usMaxData[m_iMax_Best10[i]] < m_usMaxData[m_iMax_Best10[j]]) {
				iVal = m_iMax_Best10[i];
				m_iMax_Best10[i] = m_iMax_Best10[j];
				m_iMax_Best10[j] = iVal;
			}
		}/*for*/
	}

	// 差分最大Best 10
	for (i = 0; i < EPD_WAVE_TOTAL; i++) {
		if (pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] >= fStartWave && pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] <= fEndWave) {
			if (m_usMaxData[i] > m_usMinData[i])
				m_usDiffData[i] = m_usMaxData[i] - m_usMinData[i];
			else
				m_usDiffData[i] = 0;
		}
	}/* for */


	for (i = 0; i < EPD_WAVE_TOTAL; i++) {
		if (pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] >= fStartWave && pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] <= fEndWave) {
			m_iDiff_Best10[m_iSerchDiffCnt] = i;
			m_iSerchDiffCnt++;
		}
	}
	for (i = 0; i < 5; i++) {
		for (j = i + 1; j < m_iSerchDiffCnt; j++) {
			if (m_usDiffData[m_iDiff_Best10[i]] < m_usDiffData[m_iDiff_Best10[j]]) {
				iVal = m_iDiff_Best10[i];
				m_iDiff_Best10[i] = m_iDiff_Best10[j];
				m_iDiff_Best10[j] = iVal;
			}
		}/*for*/
	}

	for (i = 0; i < 5 && i < m_iSerchMaxCnt; i++) {
		szStr.Format("%d:<%5.2fum>", (m_iMax_Best10[i] + 1), m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_iMax_Best10[i]]);
		GetDlgItem(m_uidWaveLength[i])->SetWindowTextA(szStr);

		szStr.Format("%u", m_usMaxData[m_iMax_Best10[i]]);
		GetDlgItem(m_uidWaveMax[i])->SetWindowTextA(szStr);

		GetDlgItem(m_uidMaxChkBtn[i])->EnableWindow(TRUE);
	}
	for(;i<5;i++)
		GetDlgItem(m_uidMaxChkBtn[i])->EnableWindow(FALSE);

	for (i = 0; i < 5 && i < m_iSerchDiffCnt; i++) {
		szStr.Format("%d:<%5.2fum>", (m_iDiff_Best10[i] + 1), m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_iDiff_Best10[i]]);
		GetDlgItem(m_uidDiffWaveLength[i])->SetWindowTextA(szStr);

		szStr.Format("%u", m_usDiffData[m_iDiff_Best10[i]]);
		GetDlgItem(m_uidWaveDiff[i])->SetWindowTextA(szStr);
		GetDlgItem(m_uidDiffChkBtn[i])->EnableWindow(TRUE);
	}
	for (; i < 5; i++)
		GetDlgItem(m_uidDiffChkBtn[i])->EnableWindow(FALSE);

	InvalidateRect(m_Rect_MaxDiffGraph);
}



void CDlgAnalysis::OnLvnItemchangedListWave(NMHDR* pNMHDR, LRESULT* pResult)
{
	int		iItem=-1;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	iItem = m_list_wave.GetSelectedItem(iItem);
	if (iItem != -1)
		m_iSelectWaveNo = m_list_wave.GetItemData(iItem);
	else
		m_iSelectWaveNo = -1;

	InvalidateRect(m_Rect_MaxDiffGraph);
}


void CDlgAnalysis::OnLvnItemchangedListWave2(NMHDR* pNMHDR, LRESULT* pResult)
{
	int		iItem = -1;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
	iItem = m_list_wave2.GetSelectedItem(iItem);
	if (iItem != -1)
		m_iSelectWaveNo2 = m_list_wave2.GetItemData(iItem);
	else
		m_iSelectWaveNo2 = -1;

	InvalidateRect(m_Rect_MaxDiffGraph);
}


void CDlgAnalysis::OnClose()
{
	m_Tree_Sampling.SendMessage(WM_CLOSE);
	m_Scroll_WaveLength.SendMessage(WM_CLOSE);
	m_list_wave.SendMessage(WM_CLOSE);
	m_list_wave2.SendMessage(WM_CLOSE);

	CDialogX::OnClose();
}


void CDlgAnalysis::OnBnClickedBtnCsv()
{
	CString szStr('\0',1024*10);
	CString szWork('\0', 16);
	CString szFileName;
	CStdioFile	fp;
	int			i, j;
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	double		dVal;


	szStr.Format("Output to %s.csv.", m_szFileName);
	if (MessageBox(szStr, _T("Output CSV"), MB_YESNO) == IDYES) {
		szFileName = m_szFileName;
		szFileName += _T(".csv");

		if(!fp.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText)){
			MessageBox(_T("Unable to open file\n"),_T("Output CSV"), MB_ICONERROR);
			return;
		}

		for (i = 0; i < EPD_WAVE_TOTAL; i++) {

			szStr.Format("%4.1fum", pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i]);
			if (g_tDisplayData.size() <= 6000) {
				for (j = 0; j < g_tDisplayData.size(); j++) {
					dVal = (double)g_tDisplayData[j].usData[i];
					dVal = dVal / 65535.0f * 100.0f;

					szWork.Format(",%6.2f", dVal);
					szStr += szWork;
				}
			}
			else {
				for (j = 5000; j < g_tDisplayData.size(); j++) {
					dVal = (double)g_tDisplayData[j].usData[i];
					dVal = dVal / 65535.0f * 100.0f;

					szWork.Format(",%6.2f", dVal);
					szStr += szWork;
				}

			}
			szStr += _T("\n");
			fp.WriteString(szStr);
		}/* for */
		fp.Close();
	}
}
