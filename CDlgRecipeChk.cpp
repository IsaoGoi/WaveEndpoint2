// CDlgRecipeChk.cpp : 実装ファイル
//

#include "pch.h"
#include "vector"
#include "string"
#include "WaveEndPoint.h"
#include "CDlgAnalysis.h"
#include "CDlgRecipeSelect.h"
#include "CDlgRecipeChk.h"
#include "CDlgChkRcpEdit.h"
#include "CDlgAutoRcpChk.h"


extern void get_samplinglist(int iPM, std::vector<std::string>& pList);
extern int			m_iAutoRcpChkRequest;
extern	RECIPE			m_tSimRecipe;
extern	SET_WAVECFG		m_tSimStepWaveCfg[RCP_STEP_MAX];

extern	double			m_dRawDataForDisplay[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
extern	double			m_dFAverage3Data[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];

std::vector<std::string>	g_tPC1_WaveList(1000);
std::vector<std::string>	g_tPC2_WaveList(1000);
std::vector<std::string>	g_tPC3_WaveList(1000);
std::vector<std::string>	g_tPC4_WaveList(1000);
std::vector<std::string>	g_tPC5_WaveList(1000);

SAMPLE_WAVE					g_tSimDisplayWave[6000];

WRITE_WAVEDATA	m_tSimWaveHead;
SAMPLE_WAVE		m_tSimWave;

static int		m_bRcpSimSelectFile=FALSE;
static int		m_bRcpSimSelectRecipe = FALSE;


static UINT m_bmpOffOn[3] =
{
	/* 0 */IDB_GRAY_CIRCLE,		//Unknown
	/* 1 */IDB_GREEN_CIRCLE,	//ON
};
static UINT	m_uiRcpButtonID[PM_MAX] = { IDC_BTN_RCP1,IDC_BTN_RCP2,IDC_BTN_RCP3,IDC_BTN_RCP4,IDC_BTN_RCP5};



// CDlgRecipeChk ダイアログ

IMPLEMENT_DYNAMIC(CDlgRecipeChk, CDialogX)

CDlgRecipeChk::CDlgRecipeChk(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_RECIPE_CHK, pParent)
{
	m_pImage = NULL;
	m_pDlgWaveRef = NULL;
	m_pToolTip = NULL;

	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_RECIPE_CHK, pParent);

}

CDlgRecipeChk::~CDlgRecipeChk()
{
	if (m_pDlgWaveRef)
		delete m_pDlgWaveRef;

	if (m_pImage)
		delete m_pImage;

	if (m_pToolTip)
		delete m_pToolTip;
}

void CDlgRecipeChk::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_RECIPE, m_io_RcpName);
	DDX_Control(pDX, IDC_ED_WAVECFG, m_io_WaveCfg);
	DDX_Control(pDX, IDC_ED_RAWDATA, m_io_RawData);
	DDX_Control(pDX, IDC_ED_CALCDATA, m_io_CalcData);
	DDX_Control(pDX, IDC_ED_DEADTIME_SET, m_io_Set_DeadTime);
	DDX_Control(pDX, IDC_ED_DEADTIME_MON, m_io_DeadTime);
	DDX_Control(pDX, IDC_ED_STARTLVL_SET, m_io_Set_StartLevel);
	DDX_Control(pDX, IDC_ED_STARTLVL_MON, m_io_StartLevel);

	DDX_Control(pDX, IDC_ED_JUSTLVL_SET, m_io_Set_JustLevel);
	DDX_Control(pDX, IDC_ED_JUSTLVL_MON, m_io_JustLevel);
	DDX_Control(pDX, IDC_ED_PEAKDATA, m_io_PeakData);

	DDX_Control(pDX, IDC_ED_ENDLVL_SET, m_io_Set_EndLevel);
	DDX_Control(pDX, IDC_ED_ENDLVL_MON, m_io_EndLevel);
	DDX_Control(pDX, IDC_ED_OVERETCH_SET, m_io_Set_OverEtchTime);
	DDX_Control(pDX, IDC_ED_OVERETCH_MON, m_io_OverEtchTime);
	DDX_Control(pDX, IDC_ED_TOTALTIME, m_io_TotalTime);
	DDX_Control(pDX, IDC_ED_STEP, m_io_Step);
	DDX_Control(pDX, IDC_ED_JUSTTIME_LIMIT, m_io_JustLimitTime);
	DDX_Control(pDX, IDC_ED_JUSTTIME, m_io_JustTime);
	DDX_Control(pDX, IDC_ED_ENDTIME_LIMIT, m_io_EndLimitTime);
	DDX_Control(pDX, IDC_ED_ENDTIME, m_io_EndTime);
	DDX_Control(pDX, IDC_TREE_FILE, m_Tree_Sampling);
	DDX_Control(pDX, IDC_ED_DIFFDATA, m_io_Differential);
}


BEGIN_MESSAGE_MAP(CDlgRecipeChk, CDialogX)
	ON_BN_CLICKED(IDC_BTN_RCP1, &CDlgRecipeChk::OnBnClickedBtnRcp1)
	ON_BN_CLICKED(IDC_BTN_RCP2, &CDlgRecipeChk::OnBnClickedBtnRcp2)
	ON_BN_CLICKED(IDC_BTN_RCP3, &CDlgRecipeChk::OnBnClickedBtnRcp3)
	ON_BN_CLICKED(IDC_BTN_RCP4, &CDlgRecipeChk::OnBnClickedBtnRcp4)
	ON_BN_CLICKED(IDC_BTN_RCP5, &CDlgRecipeChk::OnBnClickedBtnRcp5)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CDlgRecipeChk::OnTvnSelchangedTreeFile)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_START, &CDlgRecipeChk::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgRecipeChk::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_CHK_RAWDATA, &CDlgRecipeChk::OnBnClickedChkRawdata)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_rcpedt, &CDlgRecipeChk::OnBnClickedBtnrcpedt)
	ON_BN_CLICKED(IDC_BTN_DRAWWAVE, &CDlgRecipeChk::OnBnClickedBtnDrawwave)
	ON_BN_CLICKED(IDC_BTN_GRAPHSEL, &CDlgRecipeChk::OnBnClickedBtnGraphsel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_AUTORCPCHK, &CDlgRecipeChk::OnBnClickedBtnAutorcpchk)
	ON_BN_CLICKED(IDC_BTN_AUTORCPSTOP, &CDlgRecipeChk::OnBnClickedBtnAutorcpstop)
END_MESSAGE_MAP()


// CDlgRecipeChk メッセージ ハンドラー


BOOL CDlgRecipeChk::OnInitDialog()
{
	CRect		rc;
	int			iPM;

	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	m_pDlgWaveRef = new CDlgWaveRef(this);

	m_pToolTip = new CToolTipGraph(this);
	m_pToolTip->Deactive();


	// TODO: ここに初期化を追加してください
	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pWaveCfg = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pEPDSimulation = ((CWaveEndPointApp*)AfxGetApp())->GetEPDSimulation();

	for (iPM = 0; iPM < PM_MAX; iPM++) {
		if (m_pConfig->m_tEPDConfig.iPCEnable[iPM] == 0)
			GetDlgItem(m_uiRcpButtonID[iPM])->EnableWindow(FALSE);
	}/* for */

	DisplaySamplingList();

	m_io_RcpName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.RcpName"));

	m_io_WaveCfg.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.WaveCfgName"));

	m_io_RawData.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.RawData"));

	m_io_CalcData.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.CalcData"));

	m_io_Set_DeadTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.DeadTime.SP"));

	m_io_DeadTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.DeadTime"));

	m_io_Set_StartLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.StartLevel.SP"));

	m_io_StartLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.StartLevel"));

	m_io_JustLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustTmLimit.SP"));

	m_io_JustTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustTime"));

	m_io_Set_JustLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustLevel.SP"));

	m_io_JustLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustLevel"));
#if 0
	m_io_Set_JustDiff.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustDiff.SP"));

	m_io_JustDiff.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.JustDiff"));
#endif
	m_io_PeakData.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.Peak"));

	m_io_EndLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndTmLimit.SP"));

	m_io_EndTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndTime"));

	m_io_Set_EndLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndLevel.SP"));

	m_io_EndLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndLevel"));

#if 0
	m_io_Set_EndDiffJust.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndDiffJust.SP"));

	m_io_EndDiffJust.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndDiffJust"));

	m_io_Set_EndDiffPeak.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndDiffPeak.SP"));

	m_io_EndDiffPeak.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.EndDiffPeak"));
#endif

	m_io_Set_OverEtchTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.OverEtchTm.SP"));

	m_io_OverEtchTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.OverEtchTm"));

	m_io_Step.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.StepNo"));

	m_io_TotalTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.TotalTime"));

	m_io_Differential.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD.DiffData"));

	m_io_EPDStatus = m_pIODB->IOPointGetHandle("CTC.EPD.Status");

	m_io_RcpName.SetSpecialColor(1);
	m_io_WaveCfg.SetSpecialColor(1);
	m_io_RawData.SetSpecialColor(2);
	m_io_CalcData.SetSpecialColor(2);
	m_io_Differential.SetSpecialColor(2);
	m_io_Set_DeadTime.SetSpecialColor(1);
	m_io_DeadTime.SetSpecialColor(2);
	m_io_Set_StartLevel.SetSpecialColor(1);
	m_io_StartLevel.SetSpecialColor(2);
	m_io_JustLimitTime.SetSpecialColor(1);
	m_io_JustTime.SetSpecialColor(2);
	m_io_Set_JustLevel.SetSpecialColor(1);
	m_io_JustLevel.SetSpecialColor(2);
#if 0
	m_io_Set_JustDiff.SetSpecialColor(1);
	m_io_JustDiff.SetSpecialColor(2);
#endif
	m_io_PeakData.SetSpecialColor(2);
	m_io_EndLimitTime.SetSpecialColor(1);
	m_io_EndTime.SetSpecialColor(2);
	m_io_Set_EndLevel.SetSpecialColor(1);
	m_io_EndLevel.SetSpecialColor(2);
#if 0
	m_io_Set_EndDiffJust.SetSpecialColor(1);
	m_io_EndDiffJust.SetSpecialColor(2);
	m_io_Set_EndDiffPeak.SetSpecialColor(1);
	m_io_EndDiffPeak.SetSpecialColor(2);
#endif
	m_io_Set_OverEtchTime.SetSpecialColor(1);
	m_io_OverEtchTime.SetSpecialColor(2);
	m_io_TotalTime.SetSpecialColor(2);
	m_io_Step.SetSpecialColor(2);

	m_io_RcpName.UpdateDisplay();
	m_io_WaveCfg.UpdateDisplay();
	m_io_RawData.UpdateDisplay();
	m_io_CalcData.UpdateDisplay();
	m_io_Differential.UpdateDisplay();
	m_io_Set_DeadTime.UpdateDisplay();
	m_io_DeadTime.UpdateDisplay();
	m_io_Set_StartLevel.UpdateDisplay();
	m_io_StartLevel.UpdateDisplay();
	m_io_JustLimitTime.UpdateDisplay();
	m_io_JustTime.UpdateDisplay();
	m_io_Set_JustLevel.UpdateDisplay();
	m_io_JustLevel.UpdateDisplay();
#if 0
	m_io_Set_JustDiff.UpdateDisplay();
	m_io_JustDiff.UpdateDisplay();
#endif
	m_io_PeakData.UpdateDisplay();
	m_io_EndLimitTime.UpdateDisplay();
	m_io_EndTime.UpdateDisplay();
	m_io_Set_EndLevel.UpdateDisplay();
	m_io_EndLevel.UpdateDisplay();
#if 0
	m_io_Set_EndDiffJust.UpdateDisplay();
	m_io_EndDiffJust.UpdateDisplay();
	m_io_Set_EndDiffPeak.UpdateDisplay();
	m_io_EndDiffPeak.UpdateDisplay();
#endif
	m_io_Set_OverEtchTime.UpdateDisplay();
	m_io_OverEtchTime.UpdateDisplay();
	m_io_TotalTime.UpdateDisplay();
	m_io_Step.UpdateDisplay();

	GetDlgItem(IDC_GRAPH_RECT)->GetClientRect(m_pGraphRect);
	m_pGraphRect.OffsetRect(360, 135);

	GetDlgItem(IDC_BTN_rcpedt)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DRAWWAVE)->EnableWindow(FALSE);
	m_iSelectGraph = 0;
	GetDlgItem(IDC_BTN_GRAPHSEL)->EnableWindow(FALSE);
	m_bRectSelect = 0;

	m_iSelectPM = -1;
	m_iSelectRcpNo = -1;

	m_io_RefStartTime = m_pIODB->IOPointGetHandle(_T("CTC.Ref.StartTime"));
	m_io_RefStartLevel = m_pIODB->IOPointGetHandle(_T("CTC.Ref.StartLevel"));
	m_io_RefEndTime = m_pIODB->IOPointGetHandle(_T("CTC.Ref.EndTime"));
	m_io_RefEndLevel = m_pIODB->IOPointGetHandle(_T("CTC.Ref.EndLevel"));
	m_io_RefPeakTime = m_pIODB->IOPointGetHandle(_T("CTC.Ref.PeakTime"));
	m_io_RefPeakLevel = m_pIODB->IOPointGetHandle(_T("CTC.Ref.PeakLevel"));
	m_io_RefMinTime = m_pIODB->IOPointGetHandle(_T("CTC.Ref.MinimumTime"));
	m_io_RefMinLevel = m_pIODB->IOPointGetHandle(_T("CTC.Ref.MinimumLevel"));
	m_io_RefDiffLevel = m_pIODB->IOPointGetHandle(_T("CTC.Ref.Difference"));

	m_pDlgWaveRef->HideDlgWaveRef();

	SetTimer(1, 250, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgRecipeChk::DisplaySamplingList()
{
	int				iPM, i, iCnt;
	CString			szWork('\0', 512);
	HTREEITEM		hTreeHdl, hDataHdl;
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
			case PM1:	tList = g_tPC1_WaveList; break;
			case PM2:	tList = g_tPC2_WaveList; break;
			case PM3:	tList = g_tPC3_WaveList; break;
			case PM4:	tList = g_tPC4_WaveList; break;
			case PM5:	tList = g_tPC5_WaveList; break;
			}/* switch */

			tList.clear();
			get_samplinglist(iPM, tList);
			iCnt = (int)tList.size();

			switch (iPM) {
			case PM1:	g_tPC1_WaveList = tList; break;
			case PM2:	g_tPC2_WaveList = tList; break;
			case PM3:	g_tPC3_WaveList = tList; break;
			case PM4:	g_tPC4_WaveList = tList; break;
			case PM5:	g_tPC5_WaveList = tList; break;
			}/* switch */


			//モジュール内のデータをセット
			for (i = 0; i < iCnt; i++) {
				szWork = tList[i].c_str();
				tcItem.hParent = hTreeHdl;
				tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
				tcItem.item.lParam = ((iPM + 1) * 10000) + i;
				tcItem.item.iImage = 0;
				tcItem.item.iSelectedImage = 1;
				hDataHdl = m_Tree_Sampling.InsertItem(&tcItem);
			}/* for */
		}
	}
}


void CDlgRecipeChk::OnBnClickedBtnRcp(int iPM)
{
	int		iRcpNo;

	CDlgRecipeSelect* pDlg;

	pDlg = new CDlgRecipeSelect(this, iPM);

	if (pDlg->DoModal() == IDOK) {
		iRcpNo = pDlg->GetSelectRcpNo();
		m_pEPDSimulation->LoadRecipeRequest(iPM,iRcpNo);
		m_bRcpSimSelectRecipe = TRUE;

//		if (m_bRcpSimSelectFile == TRUE)
//			m_pEPDSimulation->EPDStartRequest();

		GetDlgItem(IDC_BTN_rcpedt)->EnableWindow(TRUE);
		m_iSelectPM = iPM;
		m_iSelectRcpNo = iRcpNo;

	}
	delete pDlg;
}


void CDlgRecipeChk::OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
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

	hTreeHdl = m_Tree_Sampling.GetSelectedItem();
	if (hTreeHdl) {
		index = m_Tree_Sampling.GetItemData(hTreeHdl);

		if (index < 10000)
			return;

		iPCNo = (int)(index / 10000);
		--iPCNo;

		iSelectNo = index % 10000;

		switch (iPCNo) {
		case PM1:	szFileName.Format("RowWaveLog\\%s", g_tPC1_WaveList[iSelectNo].c_str()); break;
		case PM2:	szFileName.Format("RowWaveLog\\%s", g_tPC2_WaveList[iSelectNo].c_str()); break;
		case PM3:	szFileName.Format("RowWaveLog\\%s", g_tPC3_WaveList[iSelectNo].c_str()); break;
		case PM4:	szFileName.Format("RowWaveLog\\%s", g_tPC4_WaveList[iSelectNo].c_str()); break;
		case PM5:	szFileName.Format("RowWaveLog\\%s", g_tPC5_WaveList[iSelectNo].c_str()); break;
		}

//		g_tDisplayWave.clear();
		if (fp.Open(szFileName, CFile::modeRead)) {
			fp.Read(&m_tSimWaveHead, sizeof(WRITE_WAVEDATA));

			m_tSimWaveHead.iSamplingCount = 0;
			while (m_tSimWaveHead.iSamplingCount<6000) {
				if (fp.Read(&m_tSimWave, sizeof(SAMPLE_WAVE)) == 0)
					break;
				g_tSimDisplayWave[m_tSimWaveHead.iSamplingCount++] = m_tSimWave;
			}/* while */
			fp.Close();
			m_bRcpSimSelectFile = TRUE;
			//			if(m_bRcpSimSelectRecipe == TRUE)
//				m_pEPDSimulation->EPDStartRequest();

		}
	}
//	m_iSamplingTime = m_tSimWaveHead.iSampling100msec;

#if 0
	m_tGraph_Scale[0].dMin_X_Val = 0.0f;
	m_tGraph_Scale[0].dMax_X_Val = (double)g_tDisplayWave.size();
	m_tGraph_Scale[0].d_X_Scale = m_tGraph_Scale[0].dMax_X_Val - m_tGraph_Scale[0].dMin_X_Val;

	m_tGraph_Scale[0].dMin_Y_Val = 0;
	m_tGraph_Scale[0].dMax_Y_Val = 1000.00f;
	m_tGraph_Scale[0].d_Y_Scale = 65535.00f;
	if (m_bRcpSimSelectFile == FALSE) {
		return;
	}
//	CalcuWaveData(0);
#endif
	*pResult = 0;
}


void CDlgRecipeChk::OnTimer(UINT_PTR nIDEvent)
{
	unsigned	unsVal;
	int			iStep;
	CString		szStr('\0', 16);
	CRect		rc;

	m_io_RcpName.ChkValueChange();
	m_io_WaveCfg.ChkValueChange();
	m_io_RawData.ChkValueChange();
	m_io_CalcData.ChkValueChange();
	m_io_Differential.ChkValueChange();
	m_io_Set_DeadTime.ChkValueChange();
	m_io_DeadTime.ChkValueChange();
	m_io_Set_StartLevel.ChkValueChange();
	m_io_StartLevel.ChkValueChange();
	m_io_JustLimitTime.ChkValueChange();
	m_io_JustTime.ChkValueChange();
	m_io_Set_JustLevel.ChkValueChange();
	m_io_JustLevel.ChkValueChange();
#if 0
	m_io_Set_JustDiff.ChkValueChange();
	m_io_JustDiff.ChkValueChange();
#endif
	m_io_PeakData.ChkValueChange();
	m_io_EndLimitTime.ChkValueChange();
	m_io_EndTime.ChkValueChange();
	m_io_Set_EndLevel.ChkValueChange();
	m_io_EndLevel.ChkValueChange();
#if 0
	m_io_Set_EndDiffJust.ChkValueChange();
	m_io_EndDiffJust.ChkValueChange();
	m_io_Set_EndDiffPeak.ChkValueChange();
	m_io_EndDiffPeak.ChkValueChange();
#endif
	m_io_Set_OverEtchTime.ChkValueChange();
	m_io_OverEtchTime.ChkValueChange();
	m_io_TotalTime.ChkValueChange();
	m_io_Step.ChkValueChange();

	m_pIODB->IOPointRead(m_io_EPDStatus, &unsVal);
	if (unsVal == EPD_STS_IDLE || unsVal == EPD_STS_COMPLETE || unsVal == EPD_STS_ERROREND) {
	
		if (GetDlgItem(IDC_BTN_RCP1)->IsWindowEnabled() == FALSE && m_pConfig->m_tEPDConfig.iPCEnable[PM1])
			GetDlgItem(IDC_BTN_RCP1)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_BTN_RCP2)->IsWindowEnabled() == FALSE && m_pConfig->m_tEPDConfig.iPCEnable[PM2])
			GetDlgItem(IDC_BTN_RCP2)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_BTN_RCP3)->IsWindowEnabled() == FALSE && m_pConfig->m_tEPDConfig.iPCEnable[PM3])
			GetDlgItem(IDC_BTN_RCP3)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_BTN_RCP4)->IsWindowEnabled() == FALSE && m_pConfig->m_tEPDConfig.iPCEnable[PM4])
			GetDlgItem(IDC_BTN_RCP4)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_BTN_RCP5)->IsWindowEnabled() == FALSE && m_pConfig->m_tEPDConfig.iPCEnable[PM5])
			GetDlgItem(IDC_BTN_RCP5)->EnableWindow(TRUE);
		
		if (m_bRcpSimSelectFile == TRUE && m_bRcpSimSelectRecipe == TRUE){
			if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
			if (GetDlgItem(IDC_BTN_DRAWWAVE)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_DRAWWAVE)->EnableWindow(TRUE);
			if (GetDlgItem(IDC_BTN_GRAPHSEL)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_GRAPHSEL)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
			if (GetDlgItem(IDC_BTN_DRAWWAVE)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_DRAWWAVE)->EnableWindow(FALSE);
			if (GetDlgItem(IDC_BTN_GRAPHSEL)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_GRAPHSEL)->EnableWindow(FALSE);
		}
		if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

		if (GetDlgItem(IDC_BTN_AUTORCPSTOP)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_AUTORCPSTOP)->EnableWindow(FALSE);

		if(GetDlgItem(IDC_TREE_FILE)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_TREE_FILE)->EnableWindow(TRUE);

		if (GetDlgItem(IDC_BTN_AUTORCPCHK)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_BTN_AUTORCPCHK)->EnableWindow(TRUE);
	}
	else {
		if (GetDlgItem(IDC_BTN_RCP1)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_RCP1)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_BTN_RCP2)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_RCP2)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_BTN_RCP3)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_RCP3)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_BTN_RCP4)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_RCP4)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_BTN_RCP5)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_RCP5)->EnableWindow(FALSE);

		if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);

		if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);

		if (GetDlgItem(IDC_TREE_FILE)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_TREE_FILE)->EnableWindow(FALSE);
	
		if (GetDlgItem(IDC_BTN_AUTORCPCHK)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_AUTORCPCHK)->EnableWindow(FALSE);

		if (m_iAutoRcpChkRequest) {
			if (GetDlgItem(IDC_BTN_AUTORCPSTOP)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_AUTORCPSTOP)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_BTN_AUTORCPSTOP)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_AUTORCPSTOP)->EnableWindow(FALSE);
		}
	}

	if (unsVal != m_unsEPDStatus) {
		m_unsEPDStatus = unsVal;

		rc = m_pGraphRect;
		rc.DeflateRect(0, -40);
		rc.right += 30;
		InvalidateRect(rc, FALSE);
	}
	else if (unsVal > EPD_STS_IDLE && unsVal < EPD_STS_COMPLETE) {
		rc = m_pGraphRect;
		rc.DeflateRect(0, -40);
		rc.right += 30;
		InvalidateRect(rc, FALSE);
	}
	else if (m_pEPDSimulation->EPDGraphDrawing() == TRUE) {
		rc = m_pGraphRect;
		rc.DeflateRect(0, -40);
		rc.right += 30;
		InvalidateRect(rc, FALSE);

	}
	m_pIODB->IOPointRead(m_io_Step.GetIOHdl(), &iStep);
	if (m_iStep != iStep) {
#if _TEST_	
		if (m_tSimRecipe.dDiffDisplayMax != 0.0f)
			szStr.Format("%3.2f", m_tSimRecipe.dDiffDisplayMax);
		else
			szStr = _T("10.00");
		GetDlgItem(IDC_ED_DIFF_MAX)->SetWindowText(szStr);

		szStr = _T("0.00");
		GetDlgItem(IDC_ED_DIFF_MID)->SetWindowText(szStr);

		if (m_tSimRecipe.dDiffDisplayMax != 0.0f)
			szStr.Format("%3.2f", (m_tSimRecipe.dDiffDisplayMax * -1));
		else
			szStr = _T("-10.00");
		GetDlgItem(IDC_ED_DIFF_MIN)->SetWindowText(szStr);
#endif
		m_iStep = iStep;
	}

	CDialogX::OnTimer(nIDEvent);
}

extern	EPD_WAVEDATA2	m_tEPDSimData;
extern	RECIPE			m_tSimRecipe;

void CDlgRecipeChk::OnPaint()
{
	CRect	rc;
	CPen	DotPen;
	CPen	DotRedPen;

	int		i,iStep;
	POINT	LinePoint[5];
	double	dWidth_per_100ms;
	double	dHeight_per_1;
	double	dHeight;
	int		iTotal;
	CPen	RawGraphPen;
	CPen	CalcGraphPen[RCP_STEP_MAX];
	CPen	BlackPen;
	CPen	RedPen;
	CPen	BluePen;
	unsigned unsVal;
	int		iDrawCnt;


	CPen* oldPen;
	CPaintDC dc(this); // device context for painting
	CString	szStr('\0', 32);

	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));
	DotRedPen.CreatePen(PS_DOT, 1, RGB(200,10,10));
	RawGraphPen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));

	CalcGraphPen[0].CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CalcGraphPen[1].CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CalcGraphPen[2].CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	CalcGraphPen[3].CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	CalcGraphPen[4].CreatePen(PS_SOLID, 2, RGB(255, 128, 255));

	BlackPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	RedPen.CreatePen(PS_SOLID, 2, RGB(255, 128, 128));
	BluePen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));


	rc = m_pGraphRect;
	rc.DeflateRect(-1, -1);
	dc.Draw3dRect(rc, RGB(0, 0, 0), RGB(255, 255, 255));
	dc.FillSolidRect(m_pGraphRect, RGB(212, 255, 255));


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

	m_iDrawCount = m_tEPDSimData.index;
	if (m_iDrawCount > 1) {
		iTotal = ((m_iDrawCount / 600) + 1) * 600;
		dWidth_per_100ms = (double)m_pGraphRect.Width();
		dWidth_per_100ms /= (double)iTotal;


		dHeight = (double)m_pGraphRect.Height();
		dHeight_per_1 = dHeight / (double)1000.0f;

		rc.top = m_pGraphRect.bottom + 3;
		rc.bottom = rc.top + 25;
		for (i = 1; i < 4; i++) {
			rc.left = m_pGraphRect.left + (m_pGraphRect.Width() / 3 * i) - 50;
			rc.right = rc.left + 100;

			if(m_tSimWaveHead.iSampling100msec)
				szStr.Format("%4dSec", ((iTotal* m_tSimWaveHead.iSampling100msec) / 10  / 3 * i));
			else
				szStr.Format("%4dSec", (iTotal / 10 / 3 * i));
			if (i == 3) {
				rc.left -= 50;
				rc.right = rc.left + 100;
				dc.DrawText(szStr, rc, DT_RIGHT);
			}
			else
				dc.DrawText(szStr, rc, DT_CENTER);
		}



		for (iStep = 0; iStep < m_tSimRecipe.iStepCnt; iStep++) {

			for (i = iDrawCnt = 0; i < m_iDrawCount; i++) {

				if (iDrawCnt == 0) {
					m_CalcGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					// Row Data
					m_RawGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					if (m_tEPDSimData.dCalcData[iStep][i] >= 0) {
						if (m_tEPDSimData.dCalcData[iStep][i] >= 1000.0f)
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDSimData.dCalcData[iStep][i] * dHeight_per_1);
					}
					else
						m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;


					// for Too Tip
					m_GraphPoint_All_x[iStep][i] = m_pGraphRect.left + (int)(dWidth_per_100ms * i);
					// Diff
					if (m_tEPDSimData.dDiffData[iStep][i] >= 0) {
						if (m_tEPDSimData.dDiffData[iStep][i] >= 1000.0f)
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDSimData.dDiffData[iStep][i] * dHeight_per_1);
					}
					else
						m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;

					iDrawCnt++;
				}
				else {
					// for Too Tip
					m_GraphPoint_All_x[iStep][i] = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					if (m_CalcGraphPoint[iStep][iDrawCnt - 1].x != (int)(m_pGraphRect.left + (int)(dWidth_per_100ms * i))) {
						m_CalcGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

						// Row Data
						m_RawGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

						if (m_tEPDSimData.dCalcData[iStep][i] >= 0) {
							if (m_tEPDSimData.dCalcData[iStep][i] >= 1000.0f)
								m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
							else
								m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDSimData.dCalcData[iStep][i] * dHeight_per_1);
						}
						else
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;

						if (m_tEPDSimData.dDiffData[iStep][i] >= 0) {
							if (m_tEPDSimData.dDiffData[iStep][i] >= 1000.0f)
								m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
							else
								m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDSimData.dDiffData[iStep][i] * dHeight_per_1);
						}
						else
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;

						iDrawCnt++;
					}
				}
				if (m_iSelectGraph) {
					if (m_iSelectGraph != (iStep + 1))
						continue;
				}
			}/* for */
			// Raw Data
			dc.SelectObject(&RawGraphPen);
			dc.Polyline(m_RawGraphPoint[iStep], iDrawCnt);

			dc.SelectObject(&CalcGraphPen[iStep]);
			dc.Polyline(m_CalcGraphPoint[iStep], iDrawCnt);

		}
#if 0
		if (((CButton*)(GetDlgItem(IDC_CHK_RAWDATA)))->GetCheck()) {
			dc.SelectObject(&RawGraphPen);
			dc.Polyline(m_RawGraphPoint, m_iDrawCount);
		}
#endif

		rc = m_pGraphRect;
		rc.DeflateRect(0, -1);
		LinePoint[0].y = rc.top;
		LinePoint[1].y = rc.bottom-1;

		for (iStep = 0; iStep < RCP_STEP_MAX; iStep++) {
			if (iStep && m_tEPDSimData.dStartTime[iStep] == 0.0f)
				break;
			if (m_tSimWaveHead.iSampling100msec < 1)
				m_tSimWaveHead.iSampling100msec = 1;

			if (m_tEPDSimData.dDeadTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDSimData.dStartTime[iStep] + m_tEPDSimData.dDeadTime[iStep]) * (10.0f/ m_tSimWaveHead.iSampling100msec));
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDSimData.dJustTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDSimData.dStartTime[iStep] + m_tEPDSimData.dDeadTime[iStep] + m_tEPDSimData.dJustTime[iStep]) * (10.0f / m_tSimWaveHead.iSampling100msec));
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&RedPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDSimData.dEndTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDSimData.dStartTime[iStep] + m_tEPDSimData.dDeadTime[iStep] + m_tEPDSimData.dJustTime[iStep] + m_tEPDSimData.dEndTime[iStep]) * (10.0f / m_tSimWaveHead.iSampling100msec));
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BluePen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDSimData.dOverEtchTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDSimData.dStartTime[iStep] + m_tEPDSimData.dDeadTime[iStep] + m_tEPDSimData.dJustTime[iStep] + m_tEPDSimData.dEndTime[iStep] + m_tEPDSimData.dOverEtchTime[iStep]) * (10.0f / m_tSimWaveHead.iSampling100msec));
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
		}
	}

//	if (m_bRectSelect==5) {
	if ((m_SelRect.left || m_SelRect.right) && (m_SelRect.top || m_SelRect.bottom)) {
		dc.SelectObject(&DotRedPen);
		LinePoint[0].x = m_SelRect.left;
		LinePoint[0].y = m_SelRect.top;
		LinePoint[1].x = m_SelRect.right;
		LinePoint[1].y = m_SelRect.top;
		LinePoint[2].x = m_SelRect.right;
		LinePoint[2].y = m_SelRect.bottom;
		LinePoint[3].x = m_SelRect.left;
		LinePoint[3].y = m_SelRect.bottom;
		LinePoint[4].x = m_SelRect.left;
		LinePoint[4].y = m_SelRect.top;
		dc.Polyline(&LinePoint[0], 5);
	}

	dc.SelectObject(oldPen);

	DotPen.DeleteObject();
	DotRedPen.DeleteObject();
	RawGraphPen.DeleteObject();
	
	for(i=0;i<RCP_STEP_MAX;i++)
		CalcGraphPen[i].DeleteObject();

	BlackPen.DeleteObject();
	RedPen.DeleteObject();
	BluePen.DeleteObject();

	CBitmap	curr_bmp;
	BITMAP	bmp;
	CBitmap* pOldBitmap = NULL;
	CDC* pdc;
	CDC		cDC;

	if (m_unsEPDStatus == EPD_STS_DEADTIME)
		unsVal = 1;
	else
		unsVal = 0;

	pdc = GetDlgItem(IDC_BMP_START)->GetWindowDC();		// 描画用のデバイス コンテキスト
	if (pdc) {
		cDC.CreateCompatibleDC(pdc);

		curr_bmp.LoadBitmap(m_bmpOffOn[unsVal]);
		curr_bmp.GetBitmap(&bmp);
		pOldBitmap = cDC.SelectObject(&curr_bmp);

		pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

		GetDlgItem(IDC_BMP_START)->ReleaseDC(pdc);
		if (pOldBitmap)
			pOldBitmap->DeleteObject();

		curr_bmp.DeleteObject();
		cDC.DeleteDC();
	}

	if (m_unsEPDStatus == EPD_STS_JUSTWAIT)
		unsVal = 1;
	else
		unsVal = 0;

	pdc = GetDlgItem(IDC_BMP_JUST)->GetWindowDC();		// 描画用のデバイス コンテキスト
	if (pdc) {
		cDC.CreateCompatibleDC(pdc);

		curr_bmp.LoadBitmap(m_bmpOffOn[unsVal]);
		curr_bmp.GetBitmap(&bmp);
		pOldBitmap = cDC.SelectObject(&curr_bmp);

		pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

		GetDlgItem(IDC_BMP_JUST)->ReleaseDC(pdc);
		if (pOldBitmap)
			pOldBitmap->DeleteObject();

		curr_bmp.DeleteObject();
		cDC.DeleteDC();
	}
	if (m_unsEPDStatus == EPD_STS_ENDWAIT)
		unsVal = 1;
	else
		unsVal = 0;

	pdc = GetDlgItem(IDC_BMP_END)->GetWindowDC();		// 描画用のデバイス コンテキスト
	if (pdc) {
		cDC.CreateCompatibleDC(pdc);

		curr_bmp.LoadBitmap(m_bmpOffOn[unsVal]);
		curr_bmp.GetBitmap(&bmp);
		pOldBitmap = cDC.SelectObject(&curr_bmp);

		pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

		GetDlgItem(IDC_BMP_END)->ReleaseDC(pdc);
		if (pOldBitmap)
			pOldBitmap->DeleteObject();

		curr_bmp.DeleteObject();
		cDC.DeleteDC();
	}
	if (m_unsEPDStatus == EPD_STS_OVERETCH)
		unsVal = 1;
	else
		unsVal = 0;

	pdc = GetDlgItem(IDC_BMP_OVETCH)->GetWindowDC();		// 描画用のデバイス コンテキスト
	if (pdc) {
		cDC.CreateCompatibleDC(pdc);

		curr_bmp.LoadBitmap(m_bmpOffOn[unsVal]);
		curr_bmp.GetBitmap(&bmp);
		pOldBitmap = cDC.SelectObject(&curr_bmp);

		pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

		GetDlgItem(IDC_BMP_OVETCH)->ReleaseDC(pdc);
		if (pOldBitmap)
			pOldBitmap->DeleteObject();

		curr_bmp.DeleteObject();
		cDC.DeleteDC();
	}

}


void CDlgRecipeChk::OnBnClickedBtnStart()
{
	if (m_bRcpSimSelectFile == TRUE && m_bRcpSimSelectRecipe == TRUE) {
		m_SelRect.left = m_SelRect.right = m_SelRect.top = m_SelRect.bottom = 0;
		m_pDlgWaveRef->HideDlgWaveRef();
		m_pEPDSimulation->EPDStartRequest();

		m_iStep = 0;
	}
}


void CDlgRecipeChk::OnBnClickedBtnStop()
{
	m_pEPDSimulation->EPDStopRequest();
}


void CDlgRecipeChk::OnBnClickedChkRawdata()
{
	CRect		rc;

	rc = m_pGraphRect;
	rc.DeflateRect(0, -40);
	rc.right += 30;
	InvalidateRect(rc, FALSE);
}


void CDlgRecipeChk::OnClose()
{
	m_Tree_Sampling.SendMessage(WM_CLOSE);
	CDialogX::OnClose();
}


void CDlgRecipeChk::OnBnClickedBtnrcpedt()
{

	if (m_iSelectPM == -1 || m_iSelectRcpNo == -1)
		return;

	CDlgChkRcpEdit* pDlg = new CDlgChkRcpEdit(this, m_iSelectPM, m_iSelectRcpNo);
	pDlg->DoModal();
	m_pEPDSimulation->LoadRecipeRequest(m_iSelectPM, m_iSelectRcpNo);
	m_bRcpSimSelectRecipe = TRUE;

	delete pDlg;
}


void CDlgRecipeChk::OnBnClickedBtnDrawwave()
{
	if (m_bRcpSimSelectFile == TRUE && m_bRcpSimSelectRecipe == TRUE) {
		if (m_pEPDSimulation->EPDGraphDrawing() == FALSE) {
			m_pDlgWaveRef->HideDlgWaveRef();
			m_pEPDSimulation->EPDGraphRequest();
		}
		else
			m_pEPDSimulation->EPDGraphStop();
		}
}


void CDlgRecipeChk::OnBnClickedBtnGraphsel()
{
	CRect	rc;

	m_pDlgWaveRef->HideDlgWaveRef();
	m_SelRect.left = m_SelRect.right = m_SelRect.top = m_SelRect.bottom = 0;

	++m_iSelectGraph;
	if (m_iSelectGraph > m_tSimRecipe.iStepCnt)
			m_iSelectGraph = 0;
#if 0
	if (m_iSelectGraph) {
		for (iStep = m_iSelectGraph - 1; iStep < m_tSimRecipe.iStepCnt; iStep++) {

			bSkip = FALSE;
			for (j = 0; j < iStep; j++) {
				if (m_tSimRecipe.Step[iStep].iUseWave == m_tSimRecipe.Step[j].iUseWave) {
					bSkip = TRUE;
					break;
				}
			}
			if (bSkip == FALSE)
				break;
		}/* for */
		if (iStep == m_tSimRecipe.iStepCnt)
			iStep = 0;
		else
			iStep++;
		m_iSelectGraph = iStep;
	}
#endif
	switch (m_iSelectGraph) {
	case 0:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Wave All")); break;
	case 1:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Step1 Wave")); break;
	case 2:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Step2 Wave")); break;
	case 3:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Step3 Wave")); break;
	case 4:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Step4 Wave")); break;
	case 5:	GetDlgItem(IDC_BTN_GRAPHSEL)->SetWindowText(_T("Step5 Wave")); break;
	}
	rc = m_pGraphRect;
	rc.DeflateRect(0, -40);
	rc.right += 30;
	InvalidateRect(rc, FALSE);


}

void CDlgRecipeChk::OnLButtonDown(UINT nFlags, CPoint point)
{
	unsigned	unsVal;

	m_pIODB->IOPointRead(m_io_EPDStatus, &unsVal);
	if (unsVal != EPD_STS_IDLE)
		return;
	else if (m_pEPDSimulation->EPDGraphDrawing() == TRUE)
		return;

	else if(m_iSelectGraph==0)
		return;

	if (m_pDlgWaveRef->IsWindowVisible())
		m_pDlgWaveRef->ShowWindow(SW_HIDE);

	if (m_pGraphRect.top <= point.y && m_pGraphRect.bottom >= point.y) {
		if (m_pGraphRect.left <= point.x && m_pGraphRect.right >= point.x) {
			m_SelRect.left = m_SelRect.right = m_SelRect.top = m_SelRect.bottom = 0;
			InvalidateRect(m_pGraphRect, TRUE);
			m_bRectSelect = 1;
			m_SelStartPoint = point;
		}
	}
	CDialogX::OnLButtonDown(nFlags, point);
}




void CDlgRecipeChk::OnLButtonUp(UINT nFlags, CPoint point)
{
	int		iTotal;
	double	dWidth_per_100ms;
	double	dHeight, dHeight_per_1;
	int		i;
	int		iStart = FALSE;
	double	dVal;


	if (m_bRectSelect) {
		m_bRectSelect = 0;

		if (m_pGraphRect.top <= point.y && m_pGraphRect.bottom >= point.y) {
			if (m_pGraphRect.left <= point.x && m_pGraphRect.right >= point.x) {

				iTotal = ((m_tEPDSimData.index / 600) + 1) * 600;
				dWidth_per_100ms = (double)m_pGraphRect.Width();
				dWidth_per_100ms /= (double)iTotal;

				dHeight = (double)m_pGraphRect.Height();
				dHeight_per_1 = dHeight / (double)1000.0f;
				
//				iStartPt = (double)(m_SelRect.left - m_pGraphRect.left) / dWidth_per_100ms;
//				iEndPt = (double)(m_SelRect.right - m_pGraphRect.left) / dWidth_per_100ms;

				for(i= 0;i< m_tEPDSimData.index;i++){
					if (m_CalcGraphPoint[m_iSelectGraph - 1][i].x >= m_SelRect.right)
						break;

					if ((m_CalcGraphPoint[m_iSelectGraph - 1][i].x >= m_SelRect.left && m_CalcGraphPoint[m_iSelectGraph - 1][i].x <= m_SelRect.right) &&
						(m_CalcGraphPoint[m_iSelectGraph - 1][i].y >= m_SelRect.top && m_CalcGraphPoint[m_iSelectGraph - 1][i].y <= m_SelRect.bottom)) {
						if (iStart == FALSE) {
							dVal = m_CalcGraphPoint[m_iSelectGraph - 1][i].x - m_pGraphRect.left;
							dVal /= dWidth_per_100ms;
								
//							m_dSelStartTime = (i+1) * m_tSimWaveHead.iSampling100msec;
							m_dSelStartTime = dVal * m_tSimWaveHead.iSampling100msec;
							m_dSelStartTime /= 10.0f;
							m_dSelStartLevel = m_tEPDSimData.dCalcData[m_iSelectGraph - 1][i];

#if _GOI_
							m_dSelStartTime = (double)(m_CalcGraphPoint[m_iSelectGraph - 1][i].x- m_pGraphRect.left);
							m_dSelStartTime /= dWidth_per_100ms;
							if (m_tSimWaveHead.iSampling100msec) {
								m_dSelStartTime *= m_tSimWaveHead.iSampling100msec;
								m_dSelStartTime /= 10.0f;
							}
							else
								m_dSelStartTime /= 10.0f;

							m_dSelStartLevel = (double)(m_pGraphRect.bottom - m_CalcGraphPoint[m_iSelectGraph - 1][i].y);
							m_dSelStartLevel /= dHeight_per_1;
#endif
							m_dSelEndTime = m_dSelStartTime;
							m_dSelEndLevel = m_dSelStartLevel;
							m_dSelPeakTime = m_dSelStartTime;
							m_dSelPeakLevel = m_dSelStartLevel;
							m_dSelMinTime = m_dSelStartTime;
							m_dSelMinLevel = m_dSelStartLevel;
							iStart = TRUE;
						}
						else {
							dVal = m_CalcGraphPoint[m_iSelectGraph - 1][i].x - m_pGraphRect.left;
							dVal /= dWidth_per_100ms;

//							m_dSelEndTime = (i+1) * m_tSimWaveHead.iSampling100msec;
							m_dSelEndTime = dVal * m_tSimWaveHead.iSampling100msec;
							m_dSelEndTime /= 10.0f;
							m_dSelEndLevel = m_tEPDSimData.dCalcData[m_iSelectGraph - 1][i];

#if _GOI_
							m_dSelEndTime = (double)(m_CalcGraphPoint[m_iSelectGraph - 1][i].x - m_pGraphRect.left);
							m_dSelEndTime /= dWidth_per_100ms;
							if (m_tSimWaveHead.iSampling100msec) {
								m_dSelEndTime *= m_tSimWaveHead.iSampling100msec;
								m_dSelEndTime /= 10.0f;
							}
							else
								m_dSelEndTime /= 10.0f;
//							m_dSelEndTime = i;
//							m_dSelEndTime /= 10.0f;
							m_dSelEndLevel = (double)(m_pGraphRect.bottom - m_CalcGraphPoint[m_iSelectGraph - 1][i].y);
							m_dSelEndLevel /= dHeight_per_1;
//							m_dSelEndLevel = m_tEPDSimData.dCalcData[m_iSelectGraph - 1][i];
#endif
							if (m_dSelEndLevel > m_dSelPeakLevel) {
								m_dSelPeakTime = m_dSelEndTime;
								m_dSelPeakLevel = m_dSelEndLevel;
							}
							if (m_dSelEndLevel < m_dSelMinLevel) {
								m_dSelMinTime = m_dSelEndTime;
								m_dSelMinLevel = m_dSelEndLevel;
							}
						}
					}
				}
				m_dSelDiffLevel = m_dSelPeakLevel - m_dSelMinLevel;

				m_pIODB->IOPointWrite(m_io_RefStartTime,&m_dSelStartTime);
				m_pIODB->IOPointWrite(m_io_RefStartLevel, &m_dSelStartLevel);
				m_pIODB->IOPointWrite(m_io_RefEndTime, &m_dSelEndTime);
				m_pIODB->IOPointWrite(m_io_RefEndLevel, &m_dSelEndLevel);
				m_pIODB->IOPointWrite(m_io_RefPeakTime, &m_dSelPeakTime);
				m_pIODB->IOPointWrite(m_io_RefPeakLevel, &m_dSelPeakLevel);
				m_pIODB->IOPointWrite(m_io_RefMinTime, &m_dSelMinTime);
				m_pIODB->IOPointWrite(m_io_RefMinLevel, &m_dSelMinLevel);
				m_pIODB->IOPointWrite(m_io_RefDiffLevel, &m_dSelDiffLevel);
				if(iStart)
					m_pDlgWaveRef->ShowDlgWaveRef();
			}
		}
	}
	CDialogX::OnLButtonUp(nFlags, point);
}


void CDlgRecipeChk::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect	rc;


	if (m_bRectSelect) {

		if (m_pGraphRect.top <= point.y && m_pGraphRect.bottom >= point.y) {
			if (m_pGraphRect.left <= point.x && m_pGraphRect.right >= point.x) {
				if (m_SelStartPoint.x <= point.x) {
					m_SelRect.left = m_SelStartPoint.x;
					m_SelRect.right = point.x;
				}
				else {
					m_SelRect.left = point.x;
					m_SelRect.right = m_SelStartPoint.x;
				}
				if (m_SelStartPoint.y <= point.y) {
					m_SelRect.top = m_SelStartPoint.y;
					m_SelRect.bottom = point.y;
				}
				else {
					m_SelRect.top = point.y;
					m_SelRect.bottom = m_SelStartPoint.y;
				}

				rc = m_SelRect;
				rc.DeflateRect(-20, -20);
				InvalidateRect(rc, TRUE);
			}
			else
				m_bRectSelect = 0;
		}
		else
			m_bRectSelect = 0;

	}
	/*
	** Tooll Tip Window
	*/
	int		i;
	unsigned unsVal;
	double	dTime;
	double	dStepData;
	double	dRowData;
	double	dDiffData;

	m_pIODB->IOPointRead(m_io_EPDStatus, &unsVal);
	if (unsVal != EPD_STS_IDLE) {
		m_pToolTip->Deactive();
		return;
	}
	if(m_iSelectGraph && m_tEPDSimData.index && m_bRcpSimSelectFile){
		if (m_pGraphRect.top <= point.y && m_pGraphRect.bottom >= point.y) {
			if (m_pGraphRect.left <= point.x && m_pGraphRect.right >= point.x) {

				for (i = 0; i < m_tEPDSimData.index; i++) {
					// for Too Tip
					if (m_GraphPoint_All_x[m_iSelectGraph - 1][i] == point.x) {
						dTime = (i+1) * m_tSimWaveHead.iSampling100msec;
						dTime /= 10.0f;
						dStepData = m_tEPDSimData.dCalcData[m_iSelectGraph - 1][i];
						dRowData = m_tEPDSimData.dDiffData[m_iSelectGraph - 1][i];

						sprintf(m_pToolTip->szTime, "% 5.1fSec", dTime);
						sprintf(m_pToolTip->szData, "% 5.2fdg", dStepData);
						sprintf(m_pToolTip->szRow, "(Row% 6.2f)", dRowData);
						m_pToolTip->Active(point);
						break;
					}
				}/* for */
			}
			else {
				m_pToolTip->Deactive();
			}
		}
		else {
			m_pToolTip->Deactive();
		}
	}
	// 微分
	else if (m_iSelectGraph == 6 &&  m_tEPDSimData.index && m_bRcpSimSelectFile) {
		if (m_pGraphRect.top <= point.y && m_pGraphRect.bottom >= point.y) {
			if (m_pGraphRect.left <= point.x && m_pGraphRect.right >= point.x) {

				for (i = 0; i < m_tEPDSimData.index; i++) {
					// for Too Tip
					if (m_GraphPoint_All_x[m_iSelectGraph - 1][i] == point.x) {
						dTime = (i + 1) * m_tSimWaveHead.iSampling100msec;
						dTime /= 10.0f;
						dStepData = m_tEPDSimData.dCalcData[m_iSelectGraph - 1][i];
						dRowData = m_dRawDataForDisplay[m_iSelectGraph - 1][i];
						sprintf(m_pToolTip->szTime, "% 5.1fSec", dTime);
						sprintf(m_pToolTip->szData, "% 5.2fdg", dStepData);
						sprintf(m_pToolTip->szRow, "(Row% 5.2fdg)", dRowData);
						m_pToolTip->Active(point);
						break;
					}
				}/* for */
			}
			else {
				m_pToolTip->Deactive();
			}
		}
		else {
			m_pToolTip->Deactive();
		}
	}
	else {
		m_pToolTip->Deactive();
	}
	CDialogX::OnMouseMove(nFlags, point);
}


void CDlgRecipeChk::OnBnClickedBtnAutorcpchk()
{
	CDlgAutoRcpChk* pDlg;

	pDlg = new CDlgAutoRcpChk(this);

	pDlg->DoModal();

	delete pDlg;
}
void CDlgRecipeChk::OnBnClickedBtnAutorcpstop()
{
	m_pEPDSimulation->EPDAutoRcpChkStopRequest();
}
