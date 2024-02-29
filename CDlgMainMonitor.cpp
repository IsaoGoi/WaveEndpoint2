// CDlgMainMonitor.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgMainMonitor.h"
#include "CDlgRecipeSelect.h"

extern EPD_WAVEDATA2	m_tEPDData[PM_MAX];
extern	RECIPE			m_tRecipe[PM_MAX];
extern	SET_WAVECFG		m_tMainStepWaveCfg[PM_MAX][RCP_STEP_MAX];
extern	double			m_dRawDataForMainDisplay[PM_MAX][EPD_WAVE_DATA_MAX + 1];
extern	double			m_dCurrFAverage3Data[PM_MAX][RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];


// CDlgMainMonitor ダイアログ
static UINT m_bmpOffOn[3] =
{
	/* 0 */IDB_GRAY_CIRCLE,		//Unknown
	/* 1 */IDB_PINK_CIRCLE,		//Off
};

static int  m_iWindowDetail[PM_MAX + 1] = { FALSE, FALSE, FALSE, FALSE, FALSE };

static int	m_bOriginalRectInit[PM_MAX] = { FALSE, FALSE, FALSE, FALSE, FALSE };
CRect		m_OriginalRect[PM_MAX];


extern CRect		m_rect_Window[PM_MAX];


IMPLEMENT_DYNAMIC(CDlgMainMonitor, CDialogX)

CDlgMainMonitor::CDlgMainMonitor(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_MAIN_PC, pParent)
{
	m_iPMNo = iPM;
	CDialogX::Create(IDD_MAIN_PC, pParent);

}

CDlgMainMonitor::~CDlgMainMonitor()
{
}

void CDlgMainMonitor::DoDataExchange(CDataExchange* pDX)
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
#if 0
	DDX_Control(pDX, IDC_ED_JUSTDIFF_SET, m_io_Set_JustDiff);
	DDX_Control(pDX, IDC_ED_JUSTDIFF_MON, m_io_JustDiff);
#endif
	DDX_Control(pDX, IDC_ED_PEAKDATA, m_io_PeakData);

	DDX_Control(pDX, IDC_ED_ENDLVL_SET, m_io_Set_EndLevel);
	DDX_Control(pDX, IDC_ED_ENDLVL_MON, m_io_EndLevel);
#if 0
	DDX_Control(pDX, IDC_ED_ENDDIFF_J_SET, m_io_Set_EndDiffJust);
	DDX_Control(pDX, IDC_ED_ENDDIFF_J_MON, m_io_EndDiffJust);
	DDX_Control(pDX, IDC_ED_ENDDIFF_P_SET, m_io_Set_EndDiffPeak);
	DDX_Control(pDX, IDC_ED_ENDDIFF_P_MON, m_io_EndDiffPeak);
#endif
	DDX_Control(pDX, IDC_ED_OVERETCH_SET, m_io_Set_OverEtchTime);
	DDX_Control(pDX, IDC_ED_OVERETCH_MON, m_io_OverEtchTime);
	DDX_Control(pDX, IDC_ED_TOTALTIME, m_io_TotalTime);
	DDX_Control(pDX, IDC_ED_STEP, m_io_Step);
	DDX_Control(pDX, IDC_ED_JUSTTIME_LIMIT, m_io_JustLimitTime);
	DDX_Control(pDX, IDC_ED_JUSTTIME, m_io_JustTime);
	DDX_Control(pDX, IDC_ED_ENDTIME_LIMIT, m_io_EndLimitTime);
	DDX_Control(pDX, IDC_ED_ENDTIME, m_io_EndTime);
	DDX_Control(pDX, IDC_ED_LOTID, m_io_InfoLotID);
	DDX_Control(pDX, IDC_ED_CSTID, m_io_InfoCstID);
	DDX_Control(pDX, IDC_ED_GLSID, m_io_InfoGlsID);
	DDX_Control(pDX, IDC_ED_SLOTNO, m_io_InfoSlotNo);
	DDX_Control(pDX, IDC_CHK_DETAIL, m_Chk_Detail);
	DDX_Control(pDX, IDC_ED_DIFFDATA, m_io_DiffData);
}


BEGIN_MESSAGE_MAP(CDlgMainMonitor, CDialogX)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_RCP, &CDlgMainMonitor::OnBnClickedBtnRcp)
	ON_BN_CLICKED(IDC_BTN_START, &CDlgMainMonitor::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgMainMonitor::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_CHK_RAWDATA, &CDlgMainMonitor::OnBnClickedChkRawdata)
	ON_BN_CLICKED(IDC_CHK_RAWDATA_SAVE, &CDlgMainMonitor::OnBnClickedChkRawdataSave)
	ON_BN_CLICKED(IDC_CHK_DETAIL, &CDlgMainMonitor::OnBnClickedChkDetail)
END_MESSAGE_MAP()


// CDlgMainMonitor メッセージ ハンドラー

BOOL CDlgMainMonitor::OnInitDialog()
{
	CString	szStr('\0', 64);
	unsigned	unsVal;
	HICON		icon;
	CRect		rc;

	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pWaveCfg = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pEPDThread = ((CWaveEndPointApp*)AfxGetApp())->GetEPDThread(m_iPMNo);

	szStr.Format("PC%d End Point Detect", (m_iPMNo + 1));
	SetWindowText(szStr);


	switch (m_iPMNo) {
	case PM1:	icon = AfxGetApp()->LoadIcon(IDI_ICON_PC1); break;
	case PM2:	icon = AfxGetApp()->LoadIcon(IDI_ICON_PC2); break;
	case PM3:	icon = AfxGetApp()->LoadIcon(IDI_ICON_PC3); break;
	case PM4:	icon = AfxGetApp()->LoadIcon(IDI_ICON_PC4); break;
	default:	icon = AfxGetApp()->LoadIcon(IDI_ICON_PC5); break;
	}/* switch */

	SetIcon(icon, TRUE);

	((CButton*)GetDlgItem(IDC_CHK_RAWDATA_SAVE))->SetCheck(((CWaveEndPointApp*)AfxGetApp())->m_iRowDataSaveMode[m_iPMNo]);


	szStr.Format("PM%d.EPD.RcpName", (m_iPMNo + 1));
	m_io_RcpName.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.WaveCfgName", (m_iPMNo + 1));
	m_io_WaveCfg.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.RawData", (m_iPMNo + 1));
	m_io_RawData.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.CalcData", (m_iPMNo + 1));
	m_io_CalcData.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.DeadTime.SP", (m_iPMNo + 1));
	m_io_Set_DeadTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.DeadTime", (m_iPMNo + 1));
	m_io_DeadTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.StartLevel.SP", (m_iPMNo + 1));
	m_io_Set_StartLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.StartLevel", (m_iPMNo + 1));
	m_io_StartLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.JustTmLimit.SP", (m_iPMNo + 1));
	m_io_JustLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.JustTime", (m_iPMNo + 1));
	m_io_JustTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.JustLevel.SP", (m_iPMNo + 1));
	m_io_Set_JustLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.JustLevel", (m_iPMNo + 1));
	m_io_JustLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

#if 0
	szStr.Format("PM%d.EPD.JustDiff.SP", (m_iPMNo + 1));
	m_io_Set_JustDiff.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.JustDiff", (m_iPMNo + 1));
	m_io_JustDiff.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
#endif

	szStr.Format("PM%d.EPD.Peak", (m_iPMNo + 1));
	m_io_PeakData.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndTmLimit.SP", (m_iPMNo + 1));
	m_io_EndLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndTime", (m_iPMNo + 1));
	m_io_EndTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndLevel.SP", (m_iPMNo + 1));
	m_io_Set_EndLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndLevel", (m_iPMNo + 1));
	m_io_EndLevel.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

#if 0
	szStr.Format("PM%d.EPD.EndDiffJust.SP", (m_iPMNo + 1));
	m_io_Set_EndDiffJust.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndDiffJust", (m_iPMNo + 1));
	m_io_EndDiffJust.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndDiffPeak.SP", (m_iPMNo + 1));
	m_io_Set_EndDiffPeak.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.EndDiffPeak", (m_iPMNo + 1));
	m_io_EndDiffPeak.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
#endif

	szStr.Format("PM%d.EPD.OverEtchTm.SP", (m_iPMNo + 1));
	m_io_Set_OverEtchTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.OverEtchTm", (m_iPMNo + 1));
	m_io_OverEtchTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.StepNo", (m_iPMNo + 1));
	m_io_Step.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.TotalTime", (m_iPMNo + 1));
	m_io_TotalTime.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.Status", (m_iPMNo + 1));
	m_io_EPDStatus = m_pIODB->IOPointGetHandle(szStr);

	szStr.Format("PM%d.LotID", (m_iPMNo + 1));
	m_io_InfoLotID.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.CstID", (m_iPMNo + 1));
	m_io_InfoCstID.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.GlsID", (m_iPMNo + 1));
	m_io_InfoGlsID.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.SlotNo", (m_iPMNo + 1));
	m_io_InfoSlotNo.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	szStr.Format("PM%d.EPD.Mode", (m_iPMNo + 1));
	m_io_EPDMode = m_pIODB->IOPointGetHandle(szStr);

	szStr.Format("PM%d.EPD.DiffData", (m_iPMNo + 1));
	m_io_DiffData.SetIOHdl(m_pIODB->IOPointGetHandle(szStr));

	

	m_io_RcpName.SetSpecialColor(1);
	m_io_WaveCfg.SetSpecialColor(1);
	m_io_RawData.SetSpecialColor(2);
	m_io_CalcData.SetSpecialColor(2);
	m_io_DiffData.SetSpecialColor(2);
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
	m_io_InfoLotID.SetSpecialColor(7);
	m_io_InfoCstID.SetSpecialColor(7);
	m_io_InfoGlsID.SetSpecialColor(7);
	m_io_InfoSlotNo.SetSpecialColor(7);

	m_io_RcpName.UpdateDisplay();
	m_io_WaveCfg.UpdateDisplay();
	m_io_RawData.UpdateDisplay();
	m_io_CalcData.UpdateDisplay();
	m_io_DiffData.UpdateDisplay();
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
	m_io_InfoLotID.UpdateDisplay();
	m_io_InfoCstID.UpdateDisplay();
	m_io_InfoGlsID.UpdateDisplay();
	m_io_InfoSlotNo.UpdateDisplay();

	m_pIODB->IOPointRead(m_io_EPDStatus, &unsVal);
	if (unsVal == EPD_STS_IDLE || unsVal == EPD_STS_COMPLETE || unsVal == EPD_STS_ERROREND) {
		GetDlgItem(IDC_BTN_RCP)->EnableWindow(TRUE);
		if(((CWaveEndPointApp*)AfxGetApp())->m_iCurrRecipe[m_iPMNo])
			GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BTN_RCP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(FALSE);
	}
	if (unsVal >= EPD_STS_DEADTIME && unsVal <= EPD_STS_OVERETCH) {
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}


	m_Chk_Detail.SetCheck(m_iWindowDetail[m_iPMNo]);
	if (m_iWindowDetail[m_iPMNo] == FALSE) {
		GetWindowRect(rc);
		if (m_bOriginalRectInit[m_iPMNo] == FALSE) {
			m_OriginalRect[m_iPMNo] = rc;
			m_bOriginalRectInit[m_iPMNo] = TRUE;
		}
		rc.right -= 265;
		rc.bottom -= 42;
#if 0
		if (m_iPMNo == PM1)
			rc.OffsetRect(rc.left * -1, rc.top * -1);
		else if (m_iPMNo == PM2)
			rc.OffsetRect(rc.left * -1+rc.Width()+10, rc.top * -1);
		else if (m_iPMNo == PM3)
			rc.OffsetRect(rc.left * -1 , rc.top * -1+rc.Height()-80);
#endif
		m_CurrRect = rc;
		MoveWindow(rc);
	}

	GetDlgItem(IDC_GRAPH_RECT)->GetClientRect(m_pGraphRect);
	m_pGraphRect.OffsetRect(62, 16);


	m_unsEPDStatus = 0;
	SetTimer(1, 250, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgMainMonitor::OnTimer(UINT_PTR nIDEvent)
{
	unsigned	unsVal;
	CRect		rc;
	unsigned	unsMode;

	m_io_RcpName.ChkValueChange();
	m_io_WaveCfg.ChkValueChange();
	m_io_RawData.ChkValueChange();
	m_io_CalcData.ChkValueChange();
	m_io_DiffData.ChkValueChange();
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
	m_io_InfoLotID.ChkValueChange();
	m_io_InfoCstID.ChkValueChange();
	m_io_InfoGlsID.ChkValueChange();
	m_io_InfoSlotNo.ChkValueChange();


	m_pIODB->IOPointRead(m_io_EPDMode, &unsMode);
	m_pIODB->IOPointRead(m_io_EPDStatus, &unsVal);
	// 1:Local Mode
	if (unsMode == 1) {
		if (unsVal == EPD_STS_IDLE || unsVal == EPD_STS_COMPLETE || unsVal == EPD_STS_ERROREND) {
			if (GetDlgItem(IDC_BTN_RCP)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_RCP)->EnableWindow(TRUE);

			if (((CWaveEndPointApp*)AfxGetApp())->m_iCurrRecipe[m_iPMNo]) {
				if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == FALSE)
					GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
			}
			else {
				if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == TRUE)
					GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
			}

			if (GetDlgItem(IDC_CHK_RAWDATA_SAVE)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_BTN_RCP)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_RCP)->EnableWindow(FALSE);
			if (GetDlgItem(IDC_BTN_START)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
			if (GetDlgItem(IDC_CHK_RAWDATA_SAVE)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(FALSE);
		}
		if (unsVal >= EPD_STS_DEADTIME && unsVal <= EPD_STS_OVERETCH) {
			if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		}
	}
	// 2:Remote Mode
	else {
		GetDlgItem(IDC_BTN_RCP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);

		if (unsVal >= EPD_STS_DEADTIME && unsVal <= EPD_STS_OVERETCH) {
			if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		}
		if (unsVal == EPD_STS_IDLE || unsVal == EPD_STS_COMPLETE || unsVal == EPD_STS_ERROREND) {
			if (GetDlgItem(IDC_CHK_RAWDATA_SAVE)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(TRUE);
		}
		else {
			if (GetDlgItem(IDC_CHK_RAWDATA_SAVE)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_CHK_RAWDATA_SAVE)->EnableWindow(FALSE);
		}
	
	}
	if (unsVal != m_unsEPDStatus) {
		m_unsEPDStatus = unsVal;

		rc = m_pGraphRect;
		rc.DeflateRect(0, -40);
		rc.right += 30;
		InvalidateRect(rc,FALSE);
	}
	else if (unsVal > EPD_STS_IDLE && unsVal < EPD_STS_COMPLETE) {
		rc = m_pGraphRect;
		rc.DeflateRect(0, -40);
		rc.right += 30;
		InvalidateRect(rc, FALSE);
	}

	CDialogX::OnTimer(nIDEvent);
}


void CDlgMainMonitor::OnPaint()
{
	CRect	rc;
	CRect	rc_diff;
	CPen	DotPen;

	int		i, iStep;
	POINT	LinePoint[2];
	double	dWidth_per_100ms;
	double	dWidth_per_500ms;
	double	dHeight_per_1;
	double	dHeight_per_Diff;
	double	dWidth;
	double	dHeight;
	int		iTotal;
	CPen	RawGraphPen;
	CPen	CalcGraphPen[RCP_STEP_MAX];
	CPen	BlackPen;
	CPen	RedPen;
	CPen	BluePen;
	unsigned unsVal;
	int		iDrawCnt;
	int		bSkip;

	CPen* oldPen;
	CPaintDC dc(this); // device context for painting
	CString	szStr('\0', 32);
	int		i2ndDiffDrawCnt;


	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));
	RawGraphPen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
	CalcGraphPen[0].CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CalcGraphPen[1].CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CalcGraphPen[2].CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	CalcGraphPen[3].CreatePen(PS_SOLID, 2, RGB(0,   0, 255));
	CalcGraphPen[4].CreatePen(PS_SOLID, 2, RGB(255, 128, 255));
	BlackPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	RedPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	BluePen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));


	rc = m_pGraphRect;
	rc.DeflateRect(-1, -1);
	dc.Draw3dRect(rc, RGB(0, 0, 0), RGB(255, 255, 255));
	dc.FillSolidRect(m_pGraphRect, RGB(212, 255, 255));

	// rect fot Diff Wave
	rc_diff = m_pGraphRect;
	rc_diff.DeflateRect(-1, -1);
	rc_diff.bottom -= (rc_diff.Height() / 2);

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

	m_iDrawCount = m_tEPDData[m_iPMNo].index;
	if (m_iDrawCount > 1) {
		iTotal = ((m_iDrawCount / 600) + 1) * 600;
		dWidth_per_100ms = (double)m_pGraphRect.Width();
		dWidth_per_100ms /= (double)iTotal;

		dWidth_per_500ms = (double)m_pGraphRect.Width();
		dWidth_per_500ms /= (double)(iTotal / 5);

		dHeight = (double)m_pGraphRect.Height();
		dHeight_per_1 = dHeight / (double)1000.0f;

		rc.top = m_pGraphRect.bottom + 3;
		rc.bottom = rc.top + 25;
		for (i = 1; i < 4; i++) {
			rc.left = m_pGraphRect.left + (m_pGraphRect.Width() / 3 * i) - 50;
			rc.right = rc.left + 100;
			if((iTotal / 10 / 3 * i) < 100)
				szStr.Format("  %dSec", (iTotal / 10 / 3 * i));
			else
				szStr.Format("%dSec", (iTotal / 10 / 3 * i));
			if (i == 3) {
				rc.left -= 30;
				rc.right = rc.left + 100;
				dc.DrawText(szStr, rc, DT_RIGHT);
			}
			else
				dc.DrawText(szStr, rc, DT_CENTER);
		}

		for (iStep = 0; iStep < m_tRecipe[m_iPMNo].iStepCnt; iStep++) {
#if 0
			bSkip = FALSE;
			if (iStep) {
				for (i = 0; i < iStep; i++) {
					if (m_tRecipe[m_iPMNo].Step[i].iUseWave == m_tRecipe[m_iPMNo].Step[iStep].iUseWave) {
						bSkip = TRUE;
						break;
					}
				}
			}
			if (bSkip)
				continue;
#endif
			if (m_tRecipe[m_iPMNo].dDiffDisplayMax != 0.000000f)
				dHeight_per_Diff = dHeight / (double)(m_tRecipe[m_iPMNo].dDiffDisplayMax * 2);
			else
				dHeight_per_Diff = dHeight / 20.0f;

			for (i = iDrawCnt = i2ndDiffDrawCnt = 0; i < m_iDrawCount; i++) {
				if (iDrawCnt == 0) {
					m_CalcGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);
					m_RawGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					if (m_tEPDData[m_iPMNo].dCalcData[iStep][i] >= 0) {
						if (m_tEPDData[m_iPMNo].dCalcData[iStep][i] >= 1000.0f)
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDData[m_iPMNo].dCalcData[iStep][i] * dHeight_per_1);
					}
					else
						m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;


					if (m_tEPDData[m_iPMNo].dDiffData[iStep][i] >= 0) {
						if (m_tEPDData[m_iPMNo].dDiffData[iStep][i] >= 1000.0f)
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDData[m_iPMNo].dDiffData[iStep][i] * dHeight_per_1);
					}
					else
						m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;
					iDrawCnt++;
				}
				else if (m_CalcGraphPoint[iStep][iDrawCnt - 1].x != m_pGraphRect.left + (int)(dWidth_per_100ms * i)) {

					m_CalcGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					// Row Data
					m_RawGraphPoint[iStep][iDrawCnt].x = m_pGraphRect.left + (int)(dWidth_per_100ms * i);

					if (m_tEPDData[m_iPMNo].dCalcData[iStep][i] >= 0) {
						if (m_tEPDData[m_iPMNo].dCalcData[iStep][i] >= 1000.0f)
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDData[m_iPMNo].dCalcData[iStep][i] * dHeight_per_1);
					}
					else
						m_RawGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;

					if (m_tEPDData[m_iPMNo].dDiffData[iStep][i] >= 0) {
						if (m_tEPDData[m_iPMNo].dDiffData[iStep][i] >= 1000.0f)
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(1000.0f * dHeight_per_1);
						else
							m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom - (int)(m_tEPDData[m_iPMNo].dDiffData[iStep][i] * dHeight_per_1);
					}
					else
						m_CalcGraphPoint[iStep][iDrawCnt].y = m_pGraphRect.bottom;

					iDrawCnt++;
				}

			}/* for */
#if 0																																															
			if (((CButton*)(GetDlgItem(IDC_CHK_RAWDATA)))->GetCheck()) {
				dc.SelectObject(&RawGraphPen);
				dc.Polyline(m_RawGraphPoint, m_iDrawCount);
			}
#endif
			dc.SelectObject(&RawGraphPen);
			dc.Polyline(m_RawGraphPoint[iStep], iDrawCnt);

			dc.SelectObject(&CalcGraphPen[iStep]);
			dc.Polyline(m_CalcGraphPoint[iStep], iDrawCnt);
		}

		rc = m_pGraphRect;
		rc.DeflateRect(0, -1);
		LinePoint[0].y = rc.top;
		LinePoint[1].y = rc.bottom-1;

		for (iStep = 0; iStep < RCP_STEP_MAX; iStep++) {
			if (iStep && m_tEPDData[m_iPMNo].dStartTime[iStep] == 0.0f)
				break;

			if (m_tEPDData[m_iPMNo].dDeadTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData[m_iPMNo].dStartTime[iStep] + m_tEPDData[m_iPMNo].dDeadTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData[m_iPMNo].dJustTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData[m_iPMNo].dStartTime[iStep] + m_tEPDData[m_iPMNo].dDeadTime[iStep] + m_tEPDData[m_iPMNo].dJustTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&RedPen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData[m_iPMNo].dEndTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData[m_iPMNo].dStartTime[iStep] + m_tEPDData[m_iPMNo].dDeadTime[iStep] + m_tEPDData[m_iPMNo].dJustTime[iStep] + m_tEPDData[m_iPMNo].dEndTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BluePen);
				dc.Polyline(&LinePoint[0], 2);
			}
			if (m_tEPDData[m_iPMNo].dOverEtchTime[iStep] != 0.0f) {
				LinePoint[0].x = rc.left + (int)(dWidth_per_100ms * (m_tEPDData[m_iPMNo].dStartTime[iStep] + m_tEPDData[m_iPMNo].dDeadTime[iStep] + m_tEPDData[m_iPMNo].dJustTime[iStep] + m_tEPDData[m_iPMNo].dEndTime[iStep] + m_tEPDData[m_iPMNo].dOverEtchTime[iStep]) * 10.0f);
				LinePoint[1].x = LinePoint[0].x;
				dc.SelectObject(&BlackPen);
				dc.Polyline(&LinePoint[0], 2);
			}
		}
	}
	dc.SelectObject(oldPen);

	DotPen.DeleteObject();
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

void CDlgMainMonitor::OnBnClickedBtnRcp()
{
	int		iRcpNo;

	CDlgRecipeSelect* pDlg;

	pDlg = new CDlgRecipeSelect(this, m_iPMNo);

	if (pDlg->DoModal() == IDOK) {
		iRcpNo= pDlg->GetSelectRcpNo();
		m_pEPDThread->LoadRecipeRequest(iRcpNo);
	}

	delete pDlg;
}


void CDlgMainMonitor::OnBnClickedBtnStart()
{
	char			szBuff[32 + 1];
	unsigned short	usSlot;
	int				iVal;

	sprintf(szBuff, _T("LOCAL_LOT_PC%d"), (m_iPMNo + 1));
	m_pIODB->IOPointWrite(m_io_InfoLotID.GetIOHdl(), szBuff);

	sprintf(szBuff, _T("LOCAL_CST_PC%d"), (m_iPMNo + 1));
	m_pIODB->IOPointWrite(m_io_InfoCstID.GetIOHdl(), szBuff);

	sprintf(szBuff, _T("LOCAL_GLS_PC%d"), (m_iPMNo + 1));
	m_pIODB->IOPointWrite(m_io_InfoGlsID.GetIOHdl(), szBuff);

	usSlot = 99;
	m_pIODB->IOPointWrite(m_io_InfoSlotNo.GetIOHdl(), &usSlot);

	if (m_pEPDThread->EPDStartRequest()) {
		if (((CButton*)GetDlgItem(IDC_CHK_RAWDATA_SAVE))->GetCheck()) {
			CWaveSamplingSave *pWaveSampling = ((CWaveEndPointApp*)AfxGetApp())->GetWaveSamplingSave();

			// 2秒以上
			iVal = m_tRecipe->iSamplingTime;
			iVal++;
			if (iVal > 10)
				iVal = (iVal - 9) * 10;

			pWaveSampling->StartWaveSampling(m_iPMNo, iVal);	

#if 0
			if(m_tRecipe->iIntegrationTime <=100000)
				pWaveSampling->StartWaveSampling(m_iPMNo, 1);	// 1=100msec
			else
				pWaveSampling->StartWaveSampling(m_iPMNo, (m_tRecipe->iIntegrationTime / 100000));	// 1=100msec
#endif
		}
	}
}


void CDlgMainMonitor::OnBnClickedBtnStop()
{
	m_pEPDThread->EPDStopRequest();
}


void CDlgMainMonitor::OnBnClickedChkRawdata()
{
	CRect	rc;

	rc = m_pGraphRect;
	rc.DeflateRect(0, -40);
	InvalidateRect(rc);
}


void CDlgMainMonitor::OnBnClickedChkRawdataSave()
{
	((CWaveEndPointApp *)AfxGetApp())->m_iRowDataSaveMode[m_iPMNo] = ((CButton*)GetDlgItem(IDC_CHK_RAWDATA_SAVE))->GetCheck();
}


void CDlgMainMonitor::OnBnClickedChkDetail()
{

	m_iWindowDetail[m_iPMNo] = m_Chk_Detail.GetCheck();
	if (m_iWindowDetail[m_iPMNo] == FALSE) {
		m_rect_Window[m_iPMNo].right -= 280;
		m_rect_Window[m_iPMNo].bottom -= 80;
	}
	else {
		m_rect_Window[m_iPMNo].right = m_rect_Window[m_iPMNo].left + m_OriginalRect[m_iPMNo].Width();
		m_rect_Window[m_iPMNo].bottom = m_rect_Window[m_iPMNo].top + m_OriginalRect[m_iPMNo].Height();
	}

	MoveWindow(m_rect_Window[m_iPMNo]);
}
