// CDlgChkRcpEdit.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgChkRcpEdit.h"
#include "CDlgChkRcpWaveCfg.h"


// CDlgChkRcpEdit ダイアログ
/*
** Step List
*/
#define NUM_STEP_COLUMNS		3								/* Lot List表示項目数	*/

#define	SUB_STEP_ITEM_NO		0
#define	SUB_STEP_ITEM_WAVENO	1
#define	SUB_STEP_ITEM_COMMENT	2

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnStepLabel[NUM_STEP_COLUMNS] =
{
	_T("Step No."),
	_T("Use Wave"),
	_T("Comment"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnStepFmt[NUM_STEP_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnStepWidth[NUM_STEP_COLUMNS] =
{
	70,140, 160
};


IMPLEMENT_DYNAMIC(CDlgChkRcpEdit, CDialogX)

CDlgChkRcpEdit::CDlgChkRcpEdit(CWnd* pParent /*=nullptr*/, int iPM, int iRcpNo)
	: CDialogX(IDD_CHKRCPEDIT, pParent)
	, m_bStartLevel(FALSE)
	, m_bStartLimit(FALSE)
	, m_bEndLevel(FALSE)
	, m_bEndDiffJust(FALSE)
	, m_bEndDiffPeak(FALSE)
	, m_bEndFlat(FALSE)
	, m_bEndLimit(FALSE)
	, m_bJustLevel(FALSE)
	, m_bJustDiff(FALSE)
	, m_bJustLimit(FALSE)
	, m_bOverEtchTime(FALSE)
	, m_bOverEtchRate(FALSE)
	, m_iStepNo(0)
	, m_bJustPeak(FALSE)
	, m_bAutoGain(FALSE)
	, m_bJustBoxEnable(FALSE)
{
	m_iPMNo      = iPM;
	m_iRecipeNo = iRcpNo;

	SetBackColor(128, 128, 255);
}

CDlgChkRcpEdit::~CDlgChkRcpEdit()
{
	delete m_pRecipe;
}

void CDlgChkRcpEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP, m_List_Step);
	DDX_Control(pDX, IDC_ED_RCPNAME, m_io_RcpName);
	DDX_Control(pDX, IDC_ED_RCPCOMMENT, m_io_RcpComment);
	DDX_Control(pDX, IDC_ED_STEPCOMMENT, m_io_StepComment);
	DDX_Control(pDX, IDC_ED_DEADTIME, m_io_DeadTime);
	DDX_Control(pDX, IDC_ED_STARTLEVEL, m_io_StartLevel);
	DDX_Control(pDX, IDC_ED_START_LIMIT, m_io_StartLimitTime);
	DDX_Control(pDX, IDC_ED_AUTO_GAIN, m_io_AutoGain);
	DDX_Control(pDX, IDC_CMB_WAVE, m_cmb_WaveCfg);
	DDX_Check(pDX, IDC_CHK_START_LEVEL, m_bStartLevel);
	DDX_Control(pDX, IDC_RDO_START_OVER, m_btn_Start_Over);
	DDX_Control(pDX, IDC_RDO_START_UNDER, m_btn_Start_Under);
	DDX_Check(pDX, IDC_CHK_START_LIMIT, m_bStartLimit);
	DDX_Check(pDX, IDC_CHK_AUTO_GAIN, m_bAutoGain);
	DDX_Check(pDX, IDC_CHK_END_LEVEL, m_bEndLevel);
	DDX_Check(pDX, IDC_CHK_END_DIFF, m_bEndDiffJust);
	DDX_Check(pDX, IDC_CHK_END_DIFF2, m_bEndDiffPeak);
	DDX_Check(pDX, IDC_CHK_END_FLAT, m_bEndFlat);
	DDX_Check(pDX, IDC_CHK_END_LIMIT, m_bEndLimit);
	DDX_Control(pDX, IDC_RDO_END_OVER, m_btn_EneLevelOver);
	DDX_Control(pDX, IDC_RDO_END_UNDER, m_btn_EneLevelUnder);
	DDX_Check(pDX, IDC_CHK_JUST_LEVEL, m_bJustLevel);
	DDX_Control(pDX, IDC_RDO_JUST_OVER, m_btn_Just_Over);
	DDX_Control(pDX, IDC_RDO_JUST_UNDER, m_btn_Just_Under);
	DDX_Check(pDX, IDC_CHK_JUST_DIFF, m_bJustDiff);
	DDX_Check(pDX, IDC_CHK_JUST_LIMIT, m_bJustLimit);
	DDX_Check(pDX, IDC_CHK_OV_TIME, m_bOverEtchTime);
	DDX_Check(pDX, IDC_CHK_OV_RATE, m_bOverEtchRate);
	DDX_Control(pDX, IDC_ED_JUSTLEVEL, m_io_JustLevel);
	DDX_Control(pDX, IDC_ED_JUST_DIFF, m_io_JustDiff);
	DDX_Control(pDX, IDC_ED_JUST_LIMIT, m_io_JustLimitTime);
	DDX_Control(pDX, IDC_ED_JUSTPEAKCHGVAL, m_io_JustPeakLevel);
	DDX_Control(pDX, IDC_ED_ENDLEVEL, m_io_EndLevel);
	DDX_Control(pDX, IDC_ED_END_DIFF, m_io_EndDiffJust);
	DDX_Control(pDX, IDC_ED_END_DIFF2, m_io_EndDiffPeak);
	DDX_Control(pDX, IDC_ED_END_FLAT, m_io_EndFlatLevel);
	DDX_Control(pDX, IDC_ED_END_FLAT_TIME, m_io_EndFlatTime);
	DDX_Control(pDX, IDC_ED_END_LIMIT, m_io_EndLimitTime);
	DDX_Control(pDX, IDC_ED_OVER_ETCH_TIME, m_io_OverEtchTime);
	DDX_Control(pDX, IDC_ED_OVER_ETCH_RATE, m_io_OverEtchRate);
	DDX_Text(pDX, IDC_ED_STEP, m_iStepNo);
	DDX_Check(pDX, IDC_CHK_JUST_PEAK, m_bJustPeak);
	DDX_Control(pDX, IDC_RDO_JUST_PEAK_HIGH, m_btn_Just_Peak_H);
	DDX_Control(pDX, IDC_RDO_JUST_PEAK_LOW, m_btn_Just_Peak_L);
	DDX_Control(pDX, IDC_ED_JUSTPEAKVAL, m_io_JustPeakLevel);
	DDX_Control(pDX, IDC_ED_INTEGTIME, m_io_IntegrationTime);

	DDX_Check(pDX, IDC_CHK_JUST_BOX, m_bJustBoxEnable);
	DDX_Control(pDX, IDC_RDO_JUST_BOX_ASC, m_btn_Just_Box_Ascend);
	DDX_Control(pDX, IDC_RDO_JUST_BOX_DES, m_btn_Just_Box_Descend);
	DDX_Control(pDX, IDC_RDO_JUST_BOX_FLAT, m_btn_Just_Box_Flat);
	DDX_Control(pDX, IDC_ED_JUST_BOX_LEVEL, m_io_JustBoxLevel);
	DDX_Control(pDX, IDC_ED_JUST_BOX_TIME, m_io_JustBoxTime);
	DDX_Control(pDX, IDC_RDO_START_GAIN_TIME, m_btn_StartGainTime);
	DDX_Control(pDX, IDC_RDO_START_GAIN_PEAK, m_btn_StartGainPeak);
	DDX_Control(pDX, IDC_ED_JUSTPEAKSEL, m_io_JustPeakSelect);
	DDX_Control(pDX, IDC_ED_JUSTPEAKRATE, m_io_JustPeakRate);
	DDX_Control(pDX, IDC_ED_JUST_BOX_SEL, m_io_JustBoxSelect);
	DDX_Control(pDX, IDC_ED_JUST_BOX_RATE, m_io_JustBoxRate);
	DDX_Control(pDX, IDC_ED_END_DIFF_SEL, m_io_EndDIffJustSelect);
	DDX_Control(pDX, IDC_ED_END_DIFF_RATE, m_io_EndDIffJustRate);
	DDX_Control(pDX, IDC_ED_END_DIFF2_SEL, m_io_EndDIffPeakSelect);
	DDX_Control(pDX, IDC_ED_END_DIFF2_RATE, m_io_EndDIffPeakRate);
	DDX_Control(pDX, IDC_ED_END_FLAT2, m_io_EndFlatSelect);
	DDX_Control(pDX, IDC_ED_END_FLAT_RATE, m_io_EndFlatRate);
	DDX_Control(pDX, IDC_ED_DIFF_GAIN, m_io_DiffMax);
	DDX_Control(pDX, IDC_ED_JUST_BOX_WAVE, m_io_Just_UseWave);
	DDX_Control(pDX, IDC_ED_STARTLVL_WAVE, m_io_StartLevel_Wave);

}


BEGIN_MESSAGE_MAP(CDlgChkRcpEdit, CDialogX)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STEP, &CDlgChkRcpEdit::OnLvnItemchangedListStep)
	ON_BN_CLICKED(IDC_BTN_ADD_STEP, &CDlgChkRcpEdit::OnBnClickedBtnAddStep)
	ON_BN_CLICKED(IDC_CHK_START_LEVEL, &CDlgChkRcpEdit::OnBnClickedChkStartLevel)
	ON_BN_CLICKED(IDC_CHK_START_LIMIT, &CDlgChkRcpEdit::OnBnClickedChkStartLimit)
	ON_BN_CLICKED(IDC_CHK_AUTO_GAIN, &CDlgChkRcpEdit::OnBnClickedChkAutoGain)
	ON_BN_CLICKED(IDC_CHK_JUST_LEVEL, &CDlgChkRcpEdit::OnBnClickedChkJustLevel)
	ON_BN_CLICKED(IDC_CHK_JUST_DIFF, &CDlgChkRcpEdit::OnBnClickedChkJustDiff)
	ON_BN_CLICKED(IDC_CHK_JUST_LIMIT, &CDlgChkRcpEdit::OnBnClickedChkJustLimit)
	ON_BN_CLICKED(IDC_CHK_END_LEVEL, &CDlgChkRcpEdit::OnBnClickedChkEndLevel)
	ON_BN_CLICKED(IDC_CHK_END_DIFF, &CDlgChkRcpEdit::OnBnClickedChkEndDiff)
	ON_BN_CLICKED(IDC_CHK_END_DIFF2, &CDlgChkRcpEdit::OnBnClickedChkEndDiff2)
	ON_BN_CLICKED(IDC_CHK_END_FLAT, &CDlgChkRcpEdit::OnBnClickedChkEndFlat)
	ON_BN_CLICKED(IDC_CHK_END_LIMIT, &CDlgChkRcpEdit::OnBnClickedChkEndLimit)
	ON_BN_CLICKED(IDC_CHK_OV_TIME, &CDlgChkRcpEdit::OnBnClickedChkOvTime)
	ON_BN_CLICKED(IDC_CHK_OV_RATE, &CDlgChkRcpEdit::OnBnClickedChkOvRate)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgChkRcpEdit::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_DEL_STEP, &CDlgChkRcpEdit::OnBnClickedBtnDelStep)
	ON_BN_CLICKED(IDC_BTN_INSERT_STEP, &CDlgChkRcpEdit::OnBnClickedBtnInsertStep)
	ON_BN_CLICKED(IDC_BTN_WAVECFGEDT, &CDlgChkRcpEdit::OnBnClickedBtnWavecfgedt)
	ON_BN_CLICKED(IDC_CHK_JUST_PEAK, &CDlgChkRcpEdit::OnBnClickedChkJustPeak)
	ON_BN_CLICKED(IDC_CHK_JUST_BOX, &CDlgChkRcpEdit::OnBnClickedChkJustBox)
	ON_WM_TIMER()

END_MESSAGE_MAP()


// CDlgChkRcpEdit メッセージ ハンドラー
BOOL CDlgChkRcpEdit::OnInitDialog()
{
	int			iPM;
	CString		szWork('\0', 64);
	HTREEITEM	hTreeHdl, hSubHdl;
	HTREEITEM	hCurrPCHdl = 0;
	int			i;
	LV_COLUMN	lvc;
	long		lState;
	TV_INSERTSTRUCT	tcItem;
	int		iWaveCfgNo;
	char	szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char	szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	char	szWaveCfgUpdate[32];
	int		iBaseWave[5];

	CDialogX::OnInitDialog();

	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pPCRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();
	m_pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pRecipe = new CRecipe();

	GetDlgItem(IDC_CMB_WAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DEL_STEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_COPY_STEP)->EnableWindow(FALSE);


	m_io_RcpName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpName"));
	m_io_RcpComment.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpComment"));
	m_io_IntegrationTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.IntegTime"));
	m_io_StepComment.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StepComment"));
	m_io_DeadTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.DeadTime"));
	m_io_StartLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StartLevel"));
	m_io_StartLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StartLimitTm"));
	m_io_AutoGain.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.AutoGain"));
	m_io_StartLevel_Wave.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StartLevelWave"));

	m_io_JustLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLevel"));

	m_io_JustDiff.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustDiff"));
	m_io_JustLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLimitTm"));
	m_io_JustPeakLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustPeakVal"));
	m_io_JustBoxLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustBoxLevel"));
	m_io_JustBoxTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustBoxTime"));
	m_io_JustPeakSelect.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustPeakType"));
	m_io_JustPeakRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustPeakRate"));
	m_io_JustBoxSelect.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustBoxType"));
	m_io_JustBoxRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustBoxRate"));
	m_io_Just_UseWave.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustBoxWave"));

	m_io_EndLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLevel"));
	m_io_EndDIffJustSelect.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffJustType"));
	m_io_EndDiffJust.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffJust"));
	m_io_EndDIffJustRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffJustRate"));
	m_io_EndDIffPeakSelect.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffPeakType"));
	m_io_EndDiffPeak.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffPeak"));
	m_io_EndDIffPeakRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffPeakRate"));
	m_io_EndFlatSelect.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatType"));
	m_io_EndFlatLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatLevel"));
	m_io_EndFlatRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatRate"));
	m_io_EndFlatTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatTime"));
	m_io_EndLimitTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLimitTm"));

	m_io_OverEtchTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchTime"));
	m_io_OverEtchRate.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchRate"));

	m_io_DiffMax.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.DiffDispMax"));

	m_io_RcpName.SetSpecialColor(2);
	m_io_RcpComment.SetSpecialColor(2);
	m_io_IntegrationTime.SetSpecialColor(2);
	m_io_StepComment.SetSpecialColor(2);
	m_io_DeadTime.SetSpecialColor(2);
	m_io_StartLevel.SetSpecialColor(2);
	m_io_StartLevel_Wave.SetSpecialColor(2);
	m_io_StartLimitTime.SetSpecialColor(2);
	m_io_AutoGain.SetSpecialColor(2);

	m_io_JustLevel.SetSpecialColor(2);
	m_io_JustDiff.SetSpecialColor(2);
	m_io_JustLimitTime.SetSpecialColor(2);
	m_io_JustPeakLevel.SetSpecialColor(2);
	m_io_JustBoxLevel.SetSpecialColor(2);
	m_io_JustBoxTime.SetSpecialColor(2);
	m_io_JustPeakSelect.SetSpecialColor(2);
	m_io_JustPeakRate.SetSpecialColor(2);
	m_io_JustBoxSelect.SetSpecialColor(2);
	m_io_JustBoxRate.SetSpecialColor(2);
	m_io_Just_UseWave.SetSpecialColor(2);

	m_io_EndLevel.SetSpecialColor(2);
	m_io_EndDiffJust.SetSpecialColor(2);
	m_io_EndDiffPeak.SetSpecialColor(2);
	m_io_EndFlatLevel.SetSpecialColor(2);
	m_io_EndFlatTime.SetSpecialColor(2);
	m_io_EndLimitTime.SetSpecialColor(2);
	m_io_EndDIffJustSelect.SetSpecialColor(2);
	m_io_EndDIffJustRate.SetSpecialColor(2);
	m_io_EndDIffPeakSelect.SetSpecialColor(2);
	m_io_EndDIffPeakRate.SetSpecialColor(2);
	m_io_EndFlatSelect.SetSpecialColor(2);
	m_io_EndFlatRate.SetSpecialColor(2);

	m_io_OverEtchTime.SetSpecialColor(2);
	m_io_OverEtchRate.SetSpecialColor(2);

	m_io_DiffMax.SetSpecialColor(2);

	m_iWaveCfgCnt = 0;
	if (m_pWaveCfgCtrl->GetInfoFirst(m_iPMNo ,&iWaveCfgNo, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate) == TRUE) {
		szWork.Format("%03.03d:%s", iWaveCfgNo, szWaveCfgName);
		m_cmb_WaveCfg.InsertString(m_iWaveCfgCnt, szWork);
		m_cmb_WaveCfg.SetItemData(m_iWaveCfgCnt, iWaveCfgNo);
		m_iCfgTable[m_iWaveCfgCnt] = iWaveCfgNo;
		m_iWaveCfgCnt++;
		while (m_pWaveCfgCtrl->GetInfoNext(m_iPMNo ,&iWaveCfgNo, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate)) {
			szWork.Format("%03.03d:%s", iWaveCfgNo, szWaveCfgName);
			m_cmb_WaveCfg.InsertString(m_iWaveCfgCnt, szWork);
			m_cmb_WaveCfg.SetItemData(m_iWaveCfgCnt, iWaveCfgNo);
			m_iCfgTable[m_iWaveCfgCnt] = iWaveCfgNo;
			m_iWaveCfgCnt++;
		}
	}


	/*
	** Stepリスト初期化
	*/
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_STEP_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnStepLabel[i];
		lvc.cx = _gnColumnStepWidth[i];
		lvc.fmt = _gnColumnStepFmt[i];
		m_List_Step.InsertColumn(i, &lvc);
	}/* for */
	m_List_Step.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));

	m_List_Step.SetModeGridLine(TRUE, TRUE);

	SetEditFields();

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgChkRcpEdit::ModifyCheck()
{
	int		item;
	CString	szMsg('\0', 128);
	int		rv;
	CPCRcpCtrl* pRcpCtrl;

	pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();


	// IGoi
#if 0
	int		iChange=FALSE;
	if (memcmp((char*)&m_pRecipe->m_tRecipe, (char*)&m_pRecipe->m_tOldRecipe, sizeof(RECIPE))) {
		iChange = TRUE;
	}
#endif
#if 0
	if (m_iCurrStepNo == 0)
		return;

	if (m_pRecipe->m_tRecipe.iStepCnt == 0 && m_pRecipe->m_pOldRecipe->iStepCnt == 0)
		return;
#endif

	m_pIODB->IOPointRead(m_io_RcpName.GetIOHdl(), m_pRecipe->m_tRecipe.szRecipeName);
	m_pIODB->IOPointRead(m_io_RcpComment.GetIOHdl(), m_pRecipe->m_tRecipe.szComment);
	m_pIODB->IOPointRead(m_io_IntegrationTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iIntegrationTime);
	if (strchr(m_pRecipe->m_tRecipe.szRecipeName, ' '))
		*(strchr(m_pRecipe->m_tRecipe.szRecipeName, ' ')) = '\0';

	item = m_cmb_WaveCfg.GetCurSel();
	if (item != -1)
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = m_cmb_WaveCfg.GetItemData(item);
	m_pIODB->IOPointRead(m_io_StepComment.GetIOHdl(), m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].szComment);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLevelEnable = m_bStartLevel;
	if (m_btn_Start_Over.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = UNDER_LEVEL;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLimitEnable = m_bStartLimit;

	m_pIODB->IOPointRead(m_io_DeadTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fDeadTime);
	m_pIODB->IOPointRead(m_io_StartLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dStartLevel);
	m_pIODB->IOPointRead(m_io_StartLevel_Wave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eWaveType);
	m_pIODB->IOPointRead(m_io_StartLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fStartLimitTime);
	m_pIODB->IOPointRead(m_io_AutoGain.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dAutoGainLevel);


	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bAutoGain = m_bAutoGain;
	if (m_btn_StartGainTime.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_TIME;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_PEAK;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLevelEnable = m_bJustLevel;
	if (m_btn_Just_Over.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = UNDER_LEVEL;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustDiffEnable = m_bJustDiff;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLimitEnable = m_bJustLimit;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakDetect = m_bJustPeak;
	if (m_btn_Just_Peak_H.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = TRUE;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = FALSE;

	m_pIODB->IOPointRead(m_io_JustLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustLevel);
	m_pIODB->IOPointRead(m_io_JustDiff.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustDiffFromStart);
	m_pIODB->IOPointRead(m_io_JustLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fJustLimitTime);
	m_pIODB->IOPointRead(m_io_JustPeakLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
	m_pIODB->IOPointRead(m_io_JustBoxLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxLevel);
	m_pIODB->IOPointRead(m_io_JustBoxTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fBoxTime);
	m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.ePeakDetectType);
	m_pIODB->IOPointRead(m_io_JustPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgRate);
	m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxDetectType);
	m_pIODB->IOPointRead(m_io_JustBoxRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxRate);
	m_pIODB->IOPointRead(m_io_Just_UseWave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eWaveType);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bBoxDetect = m_bJustBoxEnable;
	if (m_btn_Just_Box_Ascend.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_ASCEND;
	else if (m_btn_Just_Box_Descend.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_DESCEND;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_FLAT;


	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLevelEnable = m_bEndLevel;
	if (m_btn_EneLevelOver.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = UNDER_LEVEL;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff1Enable = m_bEndDiffJust;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff2Enable = m_bEndDiffPeak;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndFlatEnable = m_bEndFlat;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLimitEnable = m_bEndLimit;

	m_pIODB->IOPointRead(m_io_EndLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndLevel);
	m_pIODB->IOPointRead(m_io_EndDiffJust.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJust);
	m_pIODB->IOPointRead(m_io_EndDiffPeak.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeak);
	m_pIODB->IOPointRead(m_io_EndFlatLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatWave);
	m_pIODB->IOPointRead(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndFlatTime);
	m_pIODB->IOPointRead(m_io_EndLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndLimitTime);
	m_pIODB->IOPointRead(m_io_EndDIffJustSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffJustSelect);
	m_pIODB->IOPointRead(m_io_EndDIffJustRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJustRate);
	m_pIODB->IOPointRead(m_io_EndDIffPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffPeakSelect);
	m_pIODB->IOPointRead(m_io_EndDIffPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeakRate);
	m_pIODB->IOPointRead(m_io_EndFlatSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffFlatSelect);
	m_pIODB->IOPointRead(m_io_EndFlatRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatRate);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchTimeEnable = m_bOverEtchTime;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchRateEnable = m_bOverEtchRate;
	m_pIODB->IOPointRead(m_io_OverEtchTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchTime);
	m_pIODB->IOPointRead(m_io_OverEtchRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchRate);


	m_pIODB->IOPointRead(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);

	if (memcmp((char*)&m_pRecipe->m_tRecipe, (char*)&m_pRecipe->m_tOldRecipe, sizeof(RECIPE))) {
		szMsg.Format("Do you want save Recipe(No.%d:%s)?", m_iRecipeNo, m_pRecipe->m_tRecipe.szRecipeName);
		if (MessageBox(szMsg, "Recipe Edit", MB_YESNO) == IDYES) {

			if (strlen(m_pRecipe->m_tRecipe.szRecipeName) == 0) {
				MessageBox("Invalid Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
				return;
			}

			if(pRcpCtrl->CheckRcpNameExists(m_iPMNo, m_iRecipeNo,m_pRecipe->m_tRecipe.szRecipeName)==FALSE){
				MessageBox("Duplicate Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
				return;
			}

			rv = m_pRecipe->RecipeFileSave(m_iPMNo, m_iRecipeNo);
			if (rv != RCP_FILE_SUCCESS)
				MessageBox(_T("Recipe Save Error!"), _T("Recipe Save"), MB_ICONERROR);

		}
	}

}

void CDlgChkRcpEdit::SetEditFields()
{
	int				i;
	CString			szStr('\0', 64);
	LV_ITEM			lvi;
	char			szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char			szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	char			szWaveCfgUpdate[32];
	int				iBaseWave[5];

	m_pRecipe->RecipeFileLoad(m_iPMNo, m_iRecipeNo);

	szStr.Format("PC%d", (m_iPMNo + 1));
	GetDlgItem(IDC_ED_PCNAME)->SetWindowTextA(szStr);

	szStr.Format("%d", m_iRecipeNo);
	GetDlgItem(IDC_ED_RCPNO)->SetWindowTextA(szStr);

	if (m_pRecipe->m_tRecipe.iStepCnt) {
		GetDlgItem(IDC_CMB_WAVE)->EnableWindow(TRUE);
		if (m_pRecipe->m_tRecipe.iStepCnt < RCP_STEP_MAX) {
			GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_BTN_DEL_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_COPY_STEP)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_CMB_WAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DEL_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COPY_STEP)->EnableWindow(FALSE);
	}


	// 初期値表示
	m_iCurrStepNo = 1;
	m_pIODB->IOPointWrite(m_io_RcpName.GetIOHdl(), m_pRecipe->m_tRecipe.szRecipeName);
	m_pIODB->IOPointWrite(m_io_RcpComment.GetIOHdl(), m_pRecipe->m_tRecipe.szComment);
	m_pIODB->IOPointWrite(m_io_IntegrationTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iIntegrationTime);
	m_pIODB->IOPointWrite(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);
	m_io_RcpName.UpdateDisplay();
	m_io_RcpComment.UpdateDisplay();
	m_io_IntegrationTime.UpdateDisplay();
	m_io_DiffMax.UpdateDisplay();

	ShowEditStep(m_iCurrStepNo);
	ShowStepList();

}
void CDlgChkRcpEdit::ShowStepList()
{
	int				i;
	CString			szStr('\0', 64);
	LV_ITEM			lvi;
	char			szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char			szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	char			szWaveCfgUpdate[32];
	int				iBaseWave[5];

	m_List_Step.DeleteAllItems();

	for (i = 0; i < m_pRecipe->m_tRecipe.iStepCnt; i++) {
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = SUB_STEP_ITEM_NO;

		szStr.Format("%03.03d", i + 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_List_Step.InsertItem(&lvi);
		m_List_Step.SetItemData(i, (i + 1));

		if (m_pWaveCfgCtrl->GetWaveCfgInfo(m_iPMNo,m_pRecipe->m_tRecipe.Step[i].iUseWave, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate) == FALSE)
			szStr = _T("");
		else
			szStr.Format("%03.03d:%s", m_pRecipe->m_tRecipe.Step[i].iUseWave, szWaveCfgName);

		m_List_Step.SetItemText(i, SUB_STEP_ITEM_WAVENO, (LPCSTR)szStr);

		szStr = m_pRecipe->m_tRecipe.Step[i].szComment;
		m_List_Step.SetItemText(i, SUB_STEP_ITEM_COMMENT, (LPCSTR)szStr);
	}
	if (m_pRecipe->m_tRecipe.iStepCnt) {
		m_List_Step.SelectItem(m_iCurrStepNo - 1);
	}
	else
		m_List_Step.SelectItem(-1);


}
void CDlgChkRcpEdit::OnLvnItemchangedListStep(NMHDR* pNMHDR, LRESULT* pResult)
{
	int	item;
	int	iStep;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	UpdateData(TRUE);
	if (pNMLV->uNewState & LVIS_SELECTED) {


		item = pNMLV->iItem;
		iStep = (int)m_List_Step.GetItemData(item);
		// 編集内容を退避
		if (iStep != m_iCurrStepNo) {
			item = m_cmb_WaveCfg.GetCurSel();
			if (item != -1)
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = m_cmb_WaveCfg.GetItemData(item);
			m_pIODB->IOPointRead(m_io_StepComment.GetIOHdl(), m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].szComment);

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLevelEnable = m_bStartLevel;
			if (m_btn_Start_Over.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = OVER_LEVEL;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = UNDER_LEVEL;
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLimitEnable = m_bStartLimit;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bAutoGain = m_bAutoGain;

			m_pIODB->IOPointRead(m_io_DeadTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fDeadTime);
			m_pIODB->IOPointRead(m_io_StartLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dStartLevel);
			m_pIODB->IOPointRead(m_io_StartLevel_Wave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eWaveType);
			m_pIODB->IOPointRead(m_io_StartLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fStartLimitTime);
			m_pIODB->IOPointRead(m_io_AutoGain.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dAutoGainLevel);
			if (m_btn_StartGainTime.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_TIME;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_PEAK;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLevelEnable = m_bJustLevel;
			if (m_btn_Just_Over.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = OVER_LEVEL;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = UNDER_LEVEL;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustDiffEnable = m_bJustDiff;
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLimitEnable = m_bJustLimit;
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakDetect = m_bJustPeak;
			if (m_btn_Just_Peak_H.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = TRUE;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = FALSE;

			m_pIODB->IOPointRead(m_io_JustLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustLevel);
			m_pIODB->IOPointRead(m_io_JustDiff.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustDiffFromStart);
			m_pIODB->IOPointRead(m_io_JustLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fJustLimitTime);
			m_pIODB->IOPointRead(m_io_JustPeakLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
			m_pIODB->IOPointRead(m_io_JustBoxLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxLevel);
			m_pIODB->IOPointRead(m_io_JustBoxTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fBoxTime);
			m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.ePeakDetectType);
			m_pIODB->IOPointRead(m_io_JustPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgRate);
			m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxDetectType);
			m_pIODB->IOPointRead(m_io_JustBoxRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxRate);
			m_pIODB->IOPointRead(m_io_Just_UseWave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eWaveType);

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bBoxDetect = m_bJustBoxEnable;
			if (m_btn_Just_Box_Ascend.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_ASCEND;
			else if (m_btn_Just_Box_Descend.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_DESCEND;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_FLAT;


			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLevelEnable = m_bEndLevel;
			if (m_btn_EneLevelOver.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = OVER_LEVEL;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = UNDER_LEVEL;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff1Enable = m_bEndDiffJust;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff2Enable = m_bEndDiffPeak;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndFlatEnable = m_bEndFlat;

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLimitEnable = m_bEndLimit;

			m_pIODB->IOPointRead(m_io_EndLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndLevel);
			m_pIODB->IOPointRead(m_io_EndDIffJustSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffJustSelect);
			m_pIODB->IOPointRead(m_io_EndDiffJust.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJust);
			m_pIODB->IOPointRead(m_io_EndDIffJustRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJustRate);
			m_pIODB->IOPointRead(m_io_EndDIffPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffPeakSelect);
			m_pIODB->IOPointRead(m_io_EndDiffPeak.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeak);
			m_pIODB->IOPointRead(m_io_EndDIffPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeakRate);
			m_pIODB->IOPointRead(m_io_EndFlatSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffFlatSelect);
			m_pIODB->IOPointRead(m_io_EndFlatLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatWave);
			m_pIODB->IOPointRead(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndFlatTime);
			m_pIODB->IOPointRead(m_io_EndFlatRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatRate);
			m_pIODB->IOPointRead(m_io_EndLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndLimitTime);

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchTimeEnable = m_bOverEtchTime;
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchRateEnable = m_bOverEtchRate;
			m_pIODB->IOPointRead(m_io_OverEtchTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchTime);
			m_pIODB->IOPointRead(m_io_OverEtchRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchRate);

		}
		m_pIODB->IOPointRead(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);

		m_iCurrStepNo = iStep;
		ShowEditStep(m_iCurrStepNo);
	}
	*pResult = 0;
}
void CDlgChkRcpEdit::ShowEditStep(int iStep)
{
	int		i, j;

	m_iStepNo = iStep;

	if (iStep)
		i = iStep - 1;
	else
		i = 0;

	if (m_pRecipe->m_tRecipe.iStepCnt) {
		GetDlgItem(IDC_CMB_WAVE)->EnableWindow(TRUE);
		if (m_pRecipe->m_tRecipe.iStepCnt < RCP_STEP_MAX) {
			GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_BTN_DEL_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_COPY_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_CMB_WAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DEL_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_INSERT_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COPY_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	}

	if (m_pRecipe->m_tRecipe.Step[i].iUseWave != 0 && m_iWaveCfgCnt) {
		for (j = 0; j < m_iWaveCfgCnt; j++) {
			if (m_pRecipe->m_tRecipe.Step[i].iUseWave == m_iCfgTable[j]) {
				m_cmb_WaveCfg.SetCurSel(j);
				break;
			}
		}/* for */
		if (j == m_iWaveCfgCnt)
			m_cmb_WaveCfg.SetCurSel(0);
	}
	else
		m_cmb_WaveCfg.SetCurSel(0);
	m_pIODB->IOPointWrite(m_io_StepComment.GetIOHdl(), m_pRecipe->m_tRecipe.Step[i].szComment);

	m_bStartLevel = m_pRecipe->m_tRecipe.Step[i].tStart.bStartLevelEnable;
	if (m_bStartLevel) {
		//		GetDlgItem(IDC_RDO_START_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_START_UNDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ED_STARTLEVEL)->EnableWindow(TRUE);
	}
	else {
		//		GetDlgItem(IDC_RDO_START_OVER)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_START_UNDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_STARTLEVEL)->EnableWindow(FALSE);
	}
	if (m_pRecipe->m_tRecipe.Step[i].tStart.eLevelType == OVER_LEVEL) {
		m_btn_Start_Over.SetCheck(TRUE);
		m_btn_Start_Under.SetCheck(FALSE);
	}
	else {
		m_btn_Start_Over.SetCheck(FALSE);
		m_btn_Start_Under.SetCheck(TRUE);
	}


	m_bStartLimit = m_pRecipe->m_tRecipe.Step[i].tStart.bStartLimitEnable;
	if (m_bStartLimit)
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(FALSE);

	m_bAutoGain = m_pRecipe->m_tRecipe.Step[i].tStart.bAutoGain;
	if (m_bAutoGain)
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(FALSE);

	if (m_pRecipe->m_tRecipe.Step[i].tStart.eAutoGainType == AUTOGAIN_TIME) {
		m_btn_StartGainTime.SetCheck(TRUE);
		m_btn_StartGainPeak.SetCheck(FALSE);
	}
	else {
		m_btn_StartGainTime.SetCheck(FALSE);
		m_btn_StartGainPeak.SetCheck(TRUE);
	}

	m_pIODB->IOPointWrite(m_io_DeadTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tStart.fDeadTime);
	m_pIODB->IOPointWrite(m_io_StartLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tStart.dStartLevel);
	m_pIODB->IOPointWrite(m_io_StartLevel_Wave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tStart.eWaveType);
	m_pIODB->IOPointWrite(m_io_StartLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tStart.fStartLimitTime);
	m_pIODB->IOPointWrite(m_io_AutoGain.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tStart.dAutoGainLevel);



	m_bJustLevel = m_pRecipe->m_tRecipe.Step[i].tJust.bJustLevelEnable;
	if (m_bJustLevel) {
		//		GetDlgItem(IDC_RDO_JUST_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_JUST_UNDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ED_JUSTLEVEL)->EnableWindow(TRUE);

	}
	else {
		//		GetDlgItem(IDC_RDO_JUST_OVER)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_JUST_UNDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUSTLEVEL)->EnableWindow(FALSE);
	}
	if (m_pRecipe->m_tRecipe.Step[i].tJust.eLevelType == OVER_LEVEL) {
		m_btn_Just_Over.SetCheck(TRUE);
		m_btn_Just_Under.SetCheck(FALSE);
	}
	else {
		m_btn_Just_Over.SetCheck(FALSE);
		m_btn_Just_Under.SetCheck(TRUE);
	}

	m_bJustDiff = m_pRecipe->m_tRecipe.Step[i].tJust.bJustDiffEnable;
	if (m_bJustDiff)
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(FALSE);

	m_bJustLimit = m_pRecipe->m_tRecipe.Step[i].tJust.bJustLimitEnable;
	if (m_bJustLimit)
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(FALSE);

	m_bJustPeak = m_pRecipe->m_tRecipe.Step[i].tJust.bJustPeakDetect;
	if (m_pRecipe->m_tRecipe.Step[i].tJust.bJustPeakHigh == TRUE) {
		m_btn_Just_Peak_H.SetCheck(TRUE);
		m_btn_Just_Peak_L.SetCheck(FALSE);
	}
	else {
		m_btn_Just_Peak_H.SetCheck(FALSE);
		m_btn_Just_Peak_L.SetCheck(TRUE);
	}
	if (m_bJustPeak) {
		GetDlgItem(IDC_ED_JUSTPEAKSEL)->EnableWindow(TRUE);

		if (m_pRecipe->m_tRecipe.Step[i].tJust.ePeakDetectType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[i].tJust.ePeakDetectType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_JUSTPEAKSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(FALSE);
	}

	if (m_pRecipe->m_tRecipe.Step[i].tJust.eBoxType == BOX_ASCEND) {
		m_btn_Just_Box_Ascend.SetCheck(TRUE);
		m_btn_Just_Box_Descend.SetCheck(FALSE);
		m_btn_Just_Box_Flat.SetCheck(FALSE);
	}
	else if (m_pRecipe->m_tRecipe.Step[i].tJust.eBoxType == BOX_DESCEND) {
		m_btn_Just_Box_Ascend.SetCheck(FALSE);
		m_btn_Just_Box_Descend.SetCheck(TRUE);
		m_btn_Just_Box_Flat.SetCheck(FALSE);
	}
	else {
		m_btn_Just_Box_Ascend.SetCheck(FALSE);
		m_btn_Just_Box_Descend.SetCheck(FALSE);
		m_btn_Just_Box_Flat.SetCheck(TRUE);
	}

	m_bJustBoxEnable = m_pRecipe->m_tRecipe.Step[i].tJust.bBoxDetect;
	if (m_bJustBoxEnable) {
		GetDlgItem(IDC_ED_JUST_BOX_SEL)->EnableWindow(TRUE);
		if (m_pRecipe->m_tRecipe.Step[i].tJust.eBoxDetectType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[i].tJust.eBoxDetectType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_ED_JUST_BOX_TIME)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_JUST_BOX_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(FALSE);
	}
	m_pIODB->IOPointWrite(m_io_JustLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dJustLevel);
	m_pIODB->IOPointWrite(m_io_JustDiff.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dJustDiffFromStart);
	m_pIODB->IOPointWrite(m_io_JustLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.fJustLimitTime);
	m_pIODB->IOPointWrite(m_io_JustPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.ePeakDetectType);
	m_pIODB->IOPointWrite(m_io_JustPeakLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dJustPeakChgValue);
	m_pIODB->IOPointWrite(m_io_JustPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dJustPeakChgRate);
	m_pIODB->IOPointWrite(m_io_JustBoxSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.eBoxDetectType);
	m_pIODB->IOPointWrite(m_io_JustBoxLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dBoxLevel);
	m_pIODB->IOPointWrite(m_io_JustBoxRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dBoxRate);
	m_pIODB->IOPointWrite(m_io_JustBoxTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.fBoxTime);
	m_pIODB->IOPointWrite(m_io_Just_UseWave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.eWaveType);

	m_bEndLevel = m_pRecipe->m_tRecipe.Step[i].tEnd.bEndLevelEnable;
	if (m_bEndLevel) {
		//		GetDlgItem(IDC_RDO_END_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_END_UNDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(TRUE);
	}
	else {
		//		GetDlgItem(IDC_RDO_END_OVER)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_END_UNDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(FALSE);
	}
	if (m_pRecipe->m_tRecipe.Step[i].tEnd.eLevelType == OVER_LEVEL) {
		m_btn_EneLevelOver.SetCheck(TRUE);
		m_btn_EneLevelUnder.SetCheck(FALSE);
	}
	else {
		m_btn_EneLevelOver.SetCheck(FALSE);
		m_btn_EneLevelUnder.SetCheck(TRUE);
	}

	m_bEndDiffJust = m_pRecipe->m_tRecipe.Step[i].tEnd.bEndDiff1Enable;
	if (m_bEndDiffJust) {
		GetDlgItem(IDC_ED_END_DIFF_SEL)->EnableWindow(TRUE);
		if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffJustSelect == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffJustSelect == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_END_DIFF_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(FALSE);
	}
	m_bEndDiffPeak = m_pRecipe->m_tRecipe.Step[i].tEnd.bEndDiff2Enable;
	if (m_bEndDiffPeak) {
		GetDlgItem(IDC_ED_END_DIFF2_SEL)->EnableWindow(TRUE);
		if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffPeakSelect == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffPeakSelect == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_END_DIFF2_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(FALSE);
	}
	m_bEndFlat = m_pRecipe->m_tRecipe.Step[i].tEnd.bEndFlatEnable;
	if (m_bEndFlat) {
		GetDlgItem(IDC_ED_END_FLAT2)->EnableWindow(TRUE);

		if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffFlatSelect == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffFlatSelect == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_ED_END_FLAT_TIME)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_END_FLAT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT_TIME)->EnableWindow(FALSE);
	}

	m_bEndLimit = m_pRecipe->m_tRecipe.Step[i].tEnd.bEndLimitEnable;
	if (m_bEndLimit)
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_EndLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndLevel);
	m_pIODB->IOPointWrite(m_io_EndDIffJustSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffJustSelect);
	m_pIODB->IOPointWrite(m_io_EndDiffJust.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndDiffFromJust);
	m_pIODB->IOPointWrite(m_io_EndDIffJustRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndDiffFromJustRate);
	m_pIODB->IOPointWrite(m_io_EndDIffPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffPeakSelect);
	m_pIODB->IOPointWrite(m_io_EndDiffPeak.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndDiffFromPeak);
	m_pIODB->IOPointWrite(m_io_EndDIffPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndDiffFromPeakRate);
	m_pIODB->IOPointWrite(m_io_EndFlatSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.eEndDiffFlatSelect);
	m_pIODB->IOPointWrite(m_io_EndFlatLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndFlatWave);
	m_pIODB->IOPointWrite(m_io_EndFlatRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.dEndFlatRate);
	m_pIODB->IOPointWrite(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.fEndFlatTime);
	m_pIODB->IOPointWrite(m_io_EndLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tEnd.fEndLimitTime);

	m_bOverEtchTime = m_pRecipe->m_tRecipe.Step[i].tOverEtch.bOverEtchTimeEnable;
	if (m_bOverEtchTime)
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(FALSE);

	m_bOverEtchRate = m_pRecipe->m_tRecipe.Step[i].tOverEtch.bOverEtchRateEnable;
	if (m_bOverEtchRate)
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_OverEtchTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tOverEtch.fOverEtchTime);
	m_pIODB->IOPointWrite(m_io_OverEtchRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tOverEtch.fOverEtchRate);

	m_pIODB->IOPointWrite(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);


	m_io_StepComment.UpdateDisplay();
	m_io_DeadTime.UpdateDisplay();
	m_io_StartLevel.UpdateDisplay();
	m_io_StartLevel_Wave.UpdateDisplay();
	m_io_StartLimitTime.UpdateDisplay();
	m_io_AutoGain.UpdateDisplay();

	m_io_JustLevel.UpdateDisplay();
	m_io_JustDiff.UpdateDisplay();
	m_io_JustLimitTime.UpdateDisplay();
	m_io_JustPeakSelect.UpdateDisplay();
	m_io_JustPeakLevel.UpdateDisplay();
	m_io_JustPeakRate.UpdateDisplay();
	m_io_JustBoxSelect.UpdateDisplay();
	m_io_JustBoxLevel.UpdateDisplay();
	m_io_JustBoxRate.UpdateDisplay();
	m_io_JustBoxTime.UpdateDisplay();
	m_io_Just_UseWave.UpdateDisplay();

	m_io_EndLevel.UpdateDisplay();
	m_io_EndDIffJustSelect.UpdateDisplay();
	m_io_EndDiffJust.UpdateDisplay();
	m_io_EndDIffJustRate.UpdateDisplay();
	m_io_EndDIffPeakSelect.UpdateDisplay();
	m_io_EndDiffPeak.UpdateDisplay();
	m_io_EndDIffPeakRate.UpdateDisplay();
	m_io_EndFlatSelect.UpdateDisplay();
	m_io_EndFlatLevel.UpdateDisplay();
	m_io_EndFlatRate.UpdateDisplay();
	m_io_EndFlatTime.UpdateDisplay();
	m_io_EndLimitTime.UpdateDisplay();

	m_io_OverEtchTime.UpdateDisplay();
	m_io_OverEtchRate.UpdateDisplay();

	m_io_DiffMax.UpdateDisplay();
	UpdateData(FALSE);
}



void CDlgChkRcpEdit::OnBnClickedChkStartLevel()
{
	UpdateData(TRUE);

	if (m_bStartLevel) {
		GetDlgItem(IDC_ED_STARTLEVEL)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_START_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_START_UNDER)->EnableWindow(TRUE);

		if (m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType == UNDER_LEVEL) {
			m_btn_Start_Over.SetCheck(FALSE);
			m_btn_Start_Under.SetCheck(TRUE);
		}
		else {
			m_btn_Start_Over.SetCheck(TRUE);
			m_btn_Start_Under.SetCheck(FALSE);
		}

	}
	else {
		GetDlgItem(IDC_ED_STARTLEVEL)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_START_OVER)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_START_UNDER)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkStartLimit()
{
	UpdateData(TRUE);

	if (m_bStartLimit) {
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkAutoGain()
{
	UpdateData(TRUE);

	if (m_bAutoGain) {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(FALSE);
	}
}

void CDlgChkRcpEdit::OnBnClickedChkJustLevel()
{
	UpdateData(TRUE);

	if (m_bJustLevel) {
		GetDlgItem(IDC_ED_JUSTLEVEL)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_JUST_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_JUST_UNDER)->EnableWindow(TRUE);

		if (m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType == UNDER_LEVEL) {
			m_btn_Just_Over.SetCheck(FALSE);
			m_btn_Just_Under.SetCheck(TRUE);
		}
		else {
			m_btn_Just_Over.SetCheck(TRUE);
			m_btn_Just_Under.SetCheck(FALSE);
		}
	}
	else {
		GetDlgItem(IDC_ED_JUSTLEVEL)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_JUST_OVER)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_RDO_JUST_UNDER)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkJustDiff()
{
	UpdateData(TRUE);

	if (m_bJustDiff) {
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkJustLimit()
{
	UpdateData(TRUE);

	if (m_bJustLimit) {
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkJustPeak()
{
	DETECT_TYPE		eType;

	UpdateData(TRUE);

	if (m_bJustPeak) {

		GetDlgItem(IDC_ED_JUSTPEAKSEL)->EnableWindow(TRUE);
		m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_JUSTPEAKSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkEndLevel()
{
	UpdateData(TRUE);

	if (m_bEndLevel) {
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_END_OVER)->EnableWindow(TRUE);
		//		GetDlgItem(IDC_RDO_END_UNDER)->EnableWindow(TRUE);
#if 0
		if (m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType == UNDER_LEVEL) {
			m_btn_EneLevelOver.SetCheck(FALSE);
			m_btn_EneLevelUnder.SetCheck(TRUE);
		}
		else {
			m_btn_EneLevelOver.SetCheck(TRUE);
			m_btn_EneLevelUnder.SetCheck(FALSE);
		}
#endif
	}
	else {
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkEndDiff()
{
	DETECT_TYPE	eType;

	UpdateData(TRUE);

	if (m_bEndDiffJust) {
		m_pIODB->IOPointRead(m_io_EndDIffJustSelect.GetIOHdl(), &eType);
		GetDlgItem(IDC_ED_END_DIFF_SEL)->EnableWindow(TRUE);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_END_DIFF_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkEndDiff2()
{
	DETECT_TYPE	eType;

	UpdateData(TRUE);

	if (m_bEndDiffPeak) {
		m_pIODB->IOPointRead(m_io_EndDIffPeakSelect.GetIOHdl(), &eType);
		GetDlgItem(IDC_ED_END_DIFF2_SEL)->EnableWindow(TRUE);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
	}
	else {
		GetDlgItem(IDC_ED_END_DIFF2_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkEndLimit()
{
	UpdateData(TRUE);

	if (m_bEndLimit) {
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkOvTime()
{
	UpdateData(TRUE);

	if (m_bOverEtchTime) {
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedChkOvRate()
{
	UpdateData(TRUE);

	if (m_bOverEtchRate) {
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnBnClickedBtnSave()
{
	int	item;
	int	 rv;
	CPCRcpCtrl* pRcpCtrl;

	pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();

	if (m_iRecipeNo== 0)
		return;

	if (MessageBox(_T("Do you want Save Recipe?"), _T("Recipe Edit"), MB_YESNO) == IDNO)
		return;


	m_pIODB->IOPointRead(m_io_RcpName.GetIOHdl(), m_pRecipe->m_tRecipe.szRecipeName);
	m_pIODB->IOPointRead(m_io_RcpComment.GetIOHdl(), m_pRecipe->m_tRecipe.szComment);
	m_pIODB->IOPointRead(m_io_IntegrationTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iIntegrationTime);

	if (strchr(m_pRecipe->m_tRecipe.szRecipeName, ' '))
		*(strchr(m_pRecipe->m_tRecipe.szRecipeName, ' ')) = '\0';

	if (strlen(m_pRecipe->m_tRecipe.szRecipeName) == 0) {
		MessageBox("Invalid Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
		return;
	}

	item = m_cmb_WaveCfg.GetCurSel();
	if (item != -1)
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = m_cmb_WaveCfg.GetItemData(item);

	m_pIODB->IOPointRead(m_io_StepComment.GetIOHdl(), m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].szComment);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLevelEnable = m_bStartLevel;
	if (m_btn_Start_Over.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eLevelType = UNDER_LEVEL;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bStartLimitEnable = m_bStartLimit;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.bAutoGain = m_bAutoGain;

	if (m_btn_StartGainTime.GetCheck()) {
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_TIME;
	}
	else {
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eAutoGainType = AUTOGAIN_PEAK;
	}

	m_pIODB->IOPointRead(m_io_DeadTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fDeadTime);
	m_pIODB->IOPointRead(m_io_StartLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dStartLevel);
	m_pIODB->IOPointRead(m_io_StartLevel_Wave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.eWaveType);
	m_pIODB->IOPointRead(m_io_StartLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.fStartLimitTime);
	m_pIODB->IOPointRead(m_io_AutoGain.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tStart.dAutoGainLevel);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLevelEnable = m_bJustLevel;
	if (m_btn_Just_Over.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eLevelType = UNDER_LEVEL;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustDiffEnable = m_bJustDiff;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustLimitEnable = m_bJustLimit;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakDetect = m_bJustPeak;
	if (m_btn_Just_Peak_H.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = TRUE;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = FALSE;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bBoxDetect = m_bJustBoxEnable;
	if (m_btn_Just_Box_Ascend.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_ASCEND;
	else if (m_btn_Just_Box_Descend.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_DESCEND;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_FLAT;

	m_pIODB->IOPointRead(m_io_JustLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustLevel);
	m_pIODB->IOPointRead(m_io_JustDiff.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustDiffFromStart);
	m_pIODB->IOPointRead(m_io_JustLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fJustLimitTime);
	m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.ePeakDetectType);
	m_pIODB->IOPointRead(m_io_JustPeakLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
	m_pIODB->IOPointRead(m_io_JustPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgRate);
	m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxDetectType);
	m_pIODB->IOPointRead(m_io_JustBoxLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxLevel);
	m_pIODB->IOPointRead(m_io_JustBoxRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxRate);
	m_pIODB->IOPointRead(m_io_JustBoxTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fBoxTime);
	m_pIODB->IOPointRead(m_io_Just_UseWave.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eWaveType);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLevelEnable = m_bEndLevel;
	if (m_btn_EneLevelOver.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = OVER_LEVEL;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eLevelType = UNDER_LEVEL;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff1Enable = m_bEndDiffJust;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndDiff2Enable = m_bEndDiffPeak;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndFlatEnable = m_bEndFlat;

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.bEndLimitEnable = m_bEndLimit;

	m_pIODB->IOPointRead(m_io_EndLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndLevel);
	m_pIODB->IOPointRead(m_io_EndDIffJustSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffJustSelect);
	m_pIODB->IOPointRead(m_io_EndDiffJust.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJust);
	m_pIODB->IOPointRead(m_io_EndDIffJustRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromJustRate);
	m_pIODB->IOPointRead(m_io_EndDIffPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffPeakSelect);
	m_pIODB->IOPointRead(m_io_EndDiffPeak.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeak);
	m_pIODB->IOPointRead(m_io_EndDIffPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndDiffFromPeakRate);
	m_pIODB->IOPointRead(m_io_EndFlatSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.eEndDiffFlatSelect);
	m_pIODB->IOPointRead(m_io_EndFlatLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatWave);
	m_pIODB->IOPointRead(m_io_EndFlatRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatRate);
	m_pIODB->IOPointRead(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndFlatTime);
	m_pIODB->IOPointRead(m_io_EndLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndLimitTime);

	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchTimeEnable = m_bOverEtchTime;
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchRateEnable = m_bOverEtchRate;
	m_pIODB->IOPointRead(m_io_OverEtchTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchTime);
	m_pIODB->IOPointRead(m_io_OverEtchRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchRate);

	m_pIODB->IOPointRead(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);

	if (pRcpCtrl->CheckRcpNameExists(m_iPMNo, m_iRecipeNo, m_pRecipe->m_tRecipe.szRecipeName) == FALSE) {
		MessageBox("Duplicate Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
		return;
	}

	rv = m_pRecipe->RecipeFileSave(m_iPMNo, m_iRecipeNo);
	if (rv != RCP_FILE_SUCCESS)
		MessageBox(_T("Recipe Save Error!"), _T("Recipe Save"), MB_ICONERROR);

	ShowStepList();

}
void CDlgChkRcpEdit::OnBnClickedBtnAddStep()
{

	if (m_pRecipe->m_tRecipe.iStepCnt == 0)
		m_iCurrStepNo = 1;
	else
		m_iCurrStepNo = m_pRecipe->m_tRecipe.iStepCnt + 1;

	++m_pRecipe->m_tRecipe.iStepCnt;

	if (m_iWaveCfgCnt)
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = m_iCfgTable[0];
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = 0;

	sprintf(m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].szComment, "Step%d", m_iCurrStepNo);

	ShowStepList();
}


void CDlgChkRcpEdit::OnBnClickedBtnDelStep()
{
	int		item;
	int		iStep;
	int		i;

	item = m_List_Step.GetSelectedItem(-1);
	if (item == -1)
		return;

	iStep = m_List_Step.GetItemData(item);
	if (iStep) {
		if (iStep < m_pRecipe->m_tRecipe.iStepCnt) {
			for (i = iStep - 1; i < m_pRecipe->m_tRecipe.iStepCnt - 1; i++) {
				m_pRecipe->m_tRecipe.Step[i] = m_pRecipe->m_tRecipe.Step[i + 1];
			}
		}
		memset(&m_pRecipe->m_tRecipe.Step[m_pRecipe->m_tRecipe.iStepCnt - 1], '\0', sizeof(RCP_STEP));
		m_pRecipe->m_tRecipe.iStepCnt--;

		if (m_iCurrStepNo > m_pRecipe->m_tRecipe.iStepCnt)
			m_iCurrStepNo = m_pRecipe->m_tRecipe.iStepCnt;
		ShowStepList();
	}
}
void CDlgChkRcpEdit::OnBnClickedBtnInsertStep()
{
	int		item;
	int		iStep;
	int		i;

	item = m_List_Step.GetSelectedItem(-1);
	if (item == -1)
		return;

	iStep = m_List_Step.GetItemData(item);
	if (iStep) {
		for (i = m_pRecipe->m_tRecipe.iStepCnt; i >= iStep; i--) {
			m_pRecipe->m_tRecipe.Step[i] = m_pRecipe->m_tRecipe.Step[i - 1];
		}
		memset(&m_pRecipe->m_tRecipe.Step[iStep - 1], '\0', sizeof(RCP_STEP));
		m_pRecipe->m_tRecipe.iStepCnt++;

		m_iCurrStepNo = iStep;

		if (m_iWaveCfgCnt)
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = m_iCfgTable[0];
		else
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].iUseWave = 0;

		sprintf(m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].szComment, "Step%d", m_iCurrStepNo);
		ShowStepList();
	}
}

void CDlgChkRcpEdit::OnCancel()
{
	UpdateData(TRUE);
	ModifyCheck();

	CDialogX::OnCancel();
}


void CDlgChkRcpEdit::OnBnClickedBtnWavecfgedt()
{
	int	item;
	int	iUseWave;

	CDlgChkRcpWaveCfg* pDlg;

	item = m_cmb_WaveCfg.GetCurSel();
	if (item != -1) {
		iUseWave = m_cmb_WaveCfg.GetItemData(item);
		if (iUseWave) {
			pDlg = new CDlgChkRcpWaveCfg(this, m_iPMNo,iUseWave);
			pDlg->DoModal();
			delete pDlg;
		}
	}
}
void CDlgChkRcpEdit::OnBnClickedChkEndFlat()
{
	DETECT_TYPE	eType;

	UpdateData(TRUE);

		if (m_bEndFlat) {
			m_pIODB->IOPointRead(m_io_EndFlatSelect.GetIOHdl(), &eType);
			GetDlgItem(IDC_ED_END_FLAT2)->EnableWindow(TRUE);
			if (eType == DETECT_LEVEL) {
				GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_ED_END_FLAT_TIME)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_END_FLAT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(FALSE);
	}
}


void CDlgChkRcpEdit::OnBnClickedChkJustBox()
{
	DETECT_TYPE		eType;

	UpdateData(TRUE);

	if (m_bJustBoxEnable) {
		m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &eType);
		GetDlgItem(IDC_ED_JUST_BOX_SEL)->EnableWindow(TRUE);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_ED_JUST_BOX_TIME)->EnableWindow(TRUE);
#if 0
		if (m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType == BOX_ASCEND) {
			m_btn_Just_Box_Ascend.SetCheck(TRUE);
			m_btn_Just_Box_Descend.SetCheck(FALSE);
			m_btn_Just_Box_Flat.SetCheck(FALSE);
		}
		else if (m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType == BOX_DESCEND) {
			m_btn_Just_Box_Ascend.SetCheck(FALSE);
			m_btn_Just_Box_Descend.SetCheck(TRUE);
			m_btn_Just_Box_Flat.SetCheck(FALSE);
		}
		else {
			m_btn_Just_Box_Ascend.SetCheck(FALSE);
			m_btn_Just_Box_Descend.SetCheck(FALSE);
			m_btn_Just_Box_Flat.SetCheck(TRUE);
		}
#endif
	}
	else {
		GetDlgItem(IDC_ED_JUST_BOX_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_JUST_BOX_TIME)->EnableWindow(FALSE);
	}
}
void CDlgChkRcpEdit::OnTimer(UINT_PTR nIDEvent)
{
	DETECT_TYPE		eType;

	if (m_io_JustPeakSelect.m_bUpdate) {
		m_io_JustPeakSelect.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUSTPEAKVAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUSTPEAKRATE)->EnableWindow(TRUE);
		}
	}
	if (m_io_JustBoxSelect.m_bUpdate) {
		m_io_JustBoxSelect.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_JUST_BOX_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_JUST_BOX_RATE)->EnableWindow(TRUE);
		}
	}
	if (m_io_EndDIffJustSelect.m_bUpdate) {
		m_io_EndDIffJustSelect.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_EndDIffJustSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF_RATE)->EnableWindow(TRUE);
		}
	}
	if (m_io_EndDIffPeakSelect.m_bUpdate) {
		m_io_EndDIffPeakSelect.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_EndDIffPeakSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_DIFF2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_DIFF2_RATE)->EnableWindow(TRUE);
		}
	}
	if (m_io_EndFlatSelect.m_bUpdate) {
		m_io_EndFlatSelect.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_EndFlatSelect.GetIOHdl(), &eType);
		if (eType == DETECT_LEVEL) {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(FALSE);
		}
		else if (eType == DETECT_RATE) {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(FALSE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_ED_END_FLAT)->EnableWindow(TRUE);
			GetDlgItem(IDC_ED_END_FLAT_RATE)->EnableWindow(TRUE);
		}
	}
	CDialogX::OnTimer(nIDEvent);
}


