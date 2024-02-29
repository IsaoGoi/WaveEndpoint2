// CDlgRcpEdit.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgRcpEdit.h"
#include "CDlgRecipeCopy.h"


/*
** #define
*/
#define NUM_COLUMNS				4								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_NAME			1
#define	SUB_ITEM_COMMENT		2
#define	SUB_ITEM_DATE			3

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("Rcp No."),
	_T("Recipe Name"),
	_T("Comment"),
	_T("Update"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	100,300, 300, 220
};
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


// CDlgRcpEdit ダイアログ
static int m_iCurrPC = -1;



IMPLEMENT_DYNAMIC(CDlgRcpEdit, CDialogX)

CDlgRcpEdit::CDlgRcpEdit(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_RCPEDIT, pParent)
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
	m_pImage = NULL;
	m_img_Recipe = NULL;
	m_hCurrSelItem = 0;
	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_RCPEDIT, pParent);

}

CDlgRcpEdit::~CDlgRcpEdit()
{
	if (m_pImage)
		delete m_pImage;
	if (m_img_Recipe)
		delete m_img_Recipe;

	delete m_pRecipe;
}

void CDlgRcpEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree_PC);
	DDX_Control(pDX, IDC_LIST_RECIPE, m_ListRecipe);
	DDX_Control(pDX, IDC_LIST_STEP, m_List_Step);
	DDX_Control(pDX, IDC_ED_RCPNAME, m_io_RcpName);
	DDX_Control(pDX, IDC_ED_RCPCOMMENT, m_io_RcpComment);
	DDX_Control(pDX, IDC_ED_STEPCOMMENT, m_io_StepComment);
	DDX_Control(pDX, IDC_ED_DEADTIME, m_io_DeadTime);
	DDX_Control(pDX, IDC_ED_STARTLEVEL, m_io_StartLevel);
	DDX_Control(pDX, IDC_ED_START_LIMIT, m_io_StartLimitTime);
	DDX_Control(pDX, IDC_CMB_WAVE, m_cmb_WaveCfg);
	DDX_Check(pDX, IDC_CHK_START_LEVEL, m_bStartLevel);
	DDX_Control(pDX, IDC_RDO_START_OVER, m_btn_Start_Over);
	DDX_Control(pDX, IDC_RDO_START_UNDER, m_btn_Start_Under);
	DDX_Check(pDX, IDC_CHK_START_LIMIT, m_bStartLimit);
	DDX_Check(pDX, IDC_CHK_AUTO_GAIN, m_bAutoGain);
	DDX_Control(pDX, IDC_ED_AUTO_GAIN, m_io_AutoGain);
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
	DDX_Control(pDX, IDC_ED_JUSTPEAKVAL, m_io_JustPeakVal);

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
	DDX_Control(pDX, IDC_ED_SAMPLING_TM, m_io_SamplingTime);
}


BEGIN_MESSAGE_MAP(CDlgRcpEdit, CDialogX)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgRcpEdit::OnTvnSelchangedTree1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RECIPE, &CDlgRcpEdit::OnLvnItemchangedListRecipe)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STEP, &CDlgRcpEdit::OnLvnItemchangedListStep)
	ON_BN_CLICKED(IDC_BTN_ADD_STEP, &CDlgRcpEdit::OnBnClickedBtnAddStep)
	ON_BN_CLICKED(IDC_CHK_START_LEVEL, &CDlgRcpEdit::OnBnClickedChkStartLevel)
	ON_BN_CLICKED(IDC_CHK_START_LIMIT, &CDlgRcpEdit::OnBnClickedChkStartLimit)
	ON_BN_CLICKED(IDC_CHK_AUTO_GAIN, &CDlgRcpEdit::OnBnClickedChkAutoGain)
	ON_BN_CLICKED(IDC_CHK_JUST_LEVEL, &CDlgRcpEdit::OnBnClickedChkJustLevel)
	ON_BN_CLICKED(IDC_CHK_JUST_DIFF, &CDlgRcpEdit::OnBnClickedChkJustDiff)
	ON_BN_CLICKED(IDC_CHK_JUST_LIMIT, &CDlgRcpEdit::OnBnClickedChkJustLimit)
	ON_BN_CLICKED(IDC_CHK_END_LEVEL, &CDlgRcpEdit::OnBnClickedChkEndLevel)
	ON_BN_CLICKED(IDC_CHK_END_DIFF, &CDlgRcpEdit::OnBnClickedChkEndDiff)
	ON_BN_CLICKED(IDC_CHK_END_DIFF2, &CDlgRcpEdit::OnBnClickedChkEndDiff2)
	ON_BN_CLICKED(IDC_CHK_END_FLAT, &CDlgRcpEdit::OnBnClickedChkEndFlat)
	ON_BN_CLICKED(IDC_CHK_END_LIMIT, &CDlgRcpEdit::OnBnClickedChkEndLimit)
	ON_BN_CLICKED(IDC_CHK_OV_TIME, &CDlgRcpEdit::OnBnClickedChkOvTime)
	ON_BN_CLICKED(IDC_CHK_OV_RATE, &CDlgRcpEdit::OnBnClickedChkOvRate)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgRcpEdit::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_DEL_STEP, &CDlgRcpEdit::OnBnClickedBtnDelStep)
	ON_BN_CLICKED(IDC_BTN_INSERT_STEP, &CDlgRcpEdit::OnBnClickedBtnInsertStep)
	ON_BN_CLICKED(IDC_BTN_COPY, &CDlgRcpEdit::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CDlgRcpEdit::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_CHK_JUST_PEAK, &CDlgRcpEdit::OnBnClickedChkJustPeak)
	ON_BN_CLICKED(IDC_CHK_JUST_BOX, &CDlgRcpEdit::OnBnClickedChkJustBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgRcpEdit メッセージ ハンドラー


BOOL CDlgRcpEdit::OnInitDialog()
{
	int			iPM;
	CString		szWork('\0', 64);
	HTREEITEM	hTreeHdl, hSubHdl;
	HTREEITEM	hCurrPCHdl=0;
	int			i;
	LV_COLUMN	lvc;
	long		lState;
	TV_INSERTSTRUCT	tcItem;
	CRect		rc;

	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

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
	m_io_JustPeakVal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustPeakVal"));
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
	m_io_SamplingTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.SamplingTime"));

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
	m_io_JustPeakVal.SetSpecialColor(2);
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
	m_io_SamplingTime.SetSpecialColor(2);

	m_img_Recipe = new CImageList();
	m_img_Recipe->Create(IDB_FILE, 16, 1, RGB(255, 255, 255));
	m_ListRecipe.SetImageList(m_img_Recipe, LVSIL_SMALL);

	/*
	** Recipeリスト初期化
	*/
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_ListRecipe.InsertColumn(i, &lvc);
	}/* for */
	m_ListRecipe.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));

	m_ListRecipe.SetModeGridLine(TRUE, TRUE);

	
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
	/*
	** PC Tree表示
	*/

	lState = GetWindowLong(m_Tree_PC.m_hWnd, GWL_STYLE);
	lState |= (TVS_HASLINES);
	SetWindowLong(m_Tree_PC.m_hWnd, GWL_STYLE, lState);
	m_pImage = new CImageList();
	m_pImage->Create(IDB_CFG_TREE, 24, 2, RGB(255, 255, 255));
	m_Tree_PC.SetImageList(m_pImage, TVSIL_NORMAL);


	tcItem.hInsertAfter = TVI_LAST;
	tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	tcItem.hParent = TVI_ROOT;
	szWork=_T("Recipe");
	tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
	tcItem.item.lParam = 0;
	tcItem.item.iImage = 0;
	tcItem.item.iSelectedImage = 2;
	hTreeHdl = m_Tree_PC.InsertItem(&tcItem);

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
			hSubHdl = m_Tree_PC.InsertItem(&tcItem);
			if (m_iCurrPC == iPM)
				hCurrPCHdl = hSubHdl;
		}
	}
	m_Tree_PC.Expand(hTreeHdl,TVE_EXPAND);

	if (hCurrPCHdl)
		m_Tree_PC.SelectItem(hCurrPCHdl);

	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgRcpEdit::ModifyCheck()
{
	int		item;
	CString	szMsg('\0', 128);
	int		rv;
	CPCRcpCtrl* pRcpCtrl;

	pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();

	if (m_iCurrEditRcpNo == 0 || m_iCurrStepNo==0)
		return;

	if(m_pRecipe->m_tRecipe.iStepCnt==0 && m_pRecipe->m_tOldRecipe.iStepCnt==0)
		return;

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
	m_pIODB->IOPointRead(m_io_JustPeakVal.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
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
	else if (m_btn_Just_Box_Descend.GetCheck())
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
	m_pIODB->IOPointRead(m_io_SamplingTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iSamplingTime);

	if(memcmp((char *)&m_pRecipe->m_tRecipe, (char*)&m_pRecipe->m_tOldRecipe,sizeof(RECIPE))){
		szMsg.Format("Do you want save Recipe(No.%d:%s)?", m_iCurrEditRcpNo, m_pRecipe->m_tRecipe.szRecipeName);
		if (MessageBox(szMsg, "Recipe Edit", MB_YESNO) == IDYES) {

			if (strlen(m_pRecipe->m_tRecipe.szRecipeName) == 0) {
				MessageBox("Invalid Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
				return;
			}

			if (pRcpCtrl->CheckRcpNameExists(m_iCurrPC, m_iCurrEditRcpNo, m_pRecipe->m_tRecipe.szRecipeName) == FALSE) {
				MessageBox("Duplicate Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
				return;
			}

			rv = m_pRecipe->RecipeFileSave(m_iCurrPC, m_iCurrEditRcpNo);
			if(rv != RCP_FILE_SUCCESS)
				MessageBox(_T("Recipe Save Error!"), _T("Recipe Save"), MB_ICONERROR);

		}
	}

}
void CDlgRcpEdit::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM		hTreeHdl;

	*pResult = 0;

	hTreeHdl = m_Tree_PC.GetSelectedItem();
	if (m_hCurrSelItem == hTreeHdl)
		return;
	m_hCurrSelItem = hTreeHdl;

	if (hTreeHdl) {
		m_iCurrPC = (int)m_Tree_PC.GetItemData(hTreeHdl);
		ShowRecipeList(m_iCurrPC,FALSE);

		GetDlgItem(IDC_BTN_COPY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);
		m_iCurrEditRcpNo = 0;
	}
}
void CDlgRcpEdit::ShowRecipeList(int iPM,int bUpdate)
{
	int				i;
	int				iRcpNo;
	char			szRecipeName[RCP_LEN_NAME + 1];
	char			szComment[RCP_LEN_COMMENT];
	char			szDateTime[32];
	int				iCurrRcpNo=0;
	LV_ITEM			lvi;
	CString			szStr('\0', 64);
	CTime			f_time;
	int		iWaveCfgNo;
	char	szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char	szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	char	szWaveCfgUpdate[32];
	int		iBaseWave[5];


	m_iWaveCfgCnt = 0;
	m_cmb_WaveCfg.ResetContent();
	if (bUpdate == FALSE)
		m_ListRecipe.DeleteAllItems();

	if (m_pWaveCfgCtrl->GetInfoFirst(iPM,&iWaveCfgNo, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate) == TRUE) {
		szStr.Format("%03.03d:%s", iWaveCfgNo, szWaveCfgName);
		m_cmb_WaveCfg.InsertString(m_iWaveCfgCnt, szStr);
		m_cmb_WaveCfg.SetItemData(m_iWaveCfgCnt, iWaveCfgNo);
		m_iCfgTable[m_iWaveCfgCnt] = iWaveCfgNo;
		m_iWaveCfgCnt++;
		while (m_pWaveCfgCtrl->GetInfoNext(iPM,&iWaveCfgNo, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate)) {
			szStr.Format("%03.03d:%s", iWaveCfgNo, szWaveCfgName);
			m_cmb_WaveCfg.InsertString(m_iWaveCfgCnt, szStr);
			m_cmb_WaveCfg.SetItemData(m_iWaveCfgCnt, iWaveCfgNo);
			m_iCfgTable[m_iWaveCfgCnt] = iWaveCfgNo;
			m_iWaveCfgCnt++;
		}
	}
	
	for (i = 0; i < RCP_FILE_MAX; i++) {
		if (i == 0) {
			if (m_pPCRcpCtrl->GetInfoFirst(m_iCurrPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}
		else {
			if (m_pPCRcpCtrl->GetInfoNext(m_iCurrPC, &iRcpNo, szRecipeName, szComment, szDateTime) == FALSE)
				break;
		}
		
		for (; iCurrRcpNo < (iRcpNo-1); iCurrRcpNo++) {
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = iCurrRcpNo;
			lvi.iSubItem = SUB_ITEM_NO;
			lvi.iImage = 1;

			szStr.Format("%03.03d", iCurrRcpNo+1);
			lvi.pszText = (LPSTR)((LPCSTR)szStr);
			if (bUpdate == FALSE)
				m_ListRecipe.InsertItem(&lvi);
			else
				m_ListRecipe.SetItem(&lvi);

			szStr = _T("");
			m_ListRecipe.SetItemData(iCurrRcpNo, (iCurrRcpNo + (int)1));
			m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_NAME, (LPCSTR)szStr);
			m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);
			m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_DATE, (LPCSTR)szStr);
		}
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = iCurrRcpNo;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 0;

		szStr.Format("%03.03d", iRcpNo);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		if (bUpdate == FALSE)
			m_ListRecipe.InsertItem(&lvi);
		else
			m_ListRecipe.SetItem(&lvi);

		m_ListRecipe.SetItemData(iCurrRcpNo, (iRcpNo+(int)1000));

		szStr = szRecipeName;
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_NAME, (LPCSTR)szStr);

		szStr = szComment;
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);

		szStr = szDateTime;
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_DATE, (LPCSTR)szStr);
		iCurrRcpNo++;
	}/* for */

	for (; iCurrRcpNo < RCP_FILE_MAX; iCurrRcpNo++) {
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = iCurrRcpNo;
		lvi.iSubItem = SUB_ITEM_NO;
		lvi.iImage = 1;

		szStr.Format("%03.03d", iCurrRcpNo + 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		if (bUpdate == FALSE)
			m_ListRecipe.InsertItem(&lvi);
		else
			m_ListRecipe.SetItem(&lvi);

		szStr = _T("");
		m_ListRecipe.SetItemData(iCurrRcpNo, (iCurrRcpNo + (int)1));
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_NAME, (LPCSTR)szStr);
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_COMMENT, (LPCSTR)szStr);
		m_ListRecipe.SetItemText(iCurrRcpNo, SUB_ITEM_DATE, (LPCSTR)szStr);


	}
}
void CDlgRcpEdit::OnLvnItemchangedListRecipe(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	int	item;
	int	iRcpNo;

	if (pNMLV->uNewState & LVIS_SELECTED) {
		item = pNMLV->iItem;
		iRcpNo = (int)m_ListRecipe.GetItemData(item);
		if (iRcpNo > 1000) {
			GetDlgItem(IDC_BTN_COPY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(TRUE);
		}
		else {
			GetDlgItem(IDC_BTN_COPY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);
		}
		iRcpNo %= 1000;
		if (iRcpNo) {
			if (m_iCurrEditRcpNo && m_iCurrEditRcpNo != iRcpNo) {
				ModifyCheck();
			}
			SetEditFields(m_iCurrPC, iRcpNo);
			m_iCurrEditRcpNo = iRcpNo;
		}
	}

}
void CDlgRcpEdit::SetEditFields(int iPC, int iRcpNo)
{
	int				i;
	CString			szStr('\0',64);
	LV_ITEM			lvi;
	char			szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char			szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	char			szWaveCfgUpdate[32];
	int				iBaseWave[5];

	m_pRecipe->RecipeFileLoad(iPC, iRcpNo);

	szStr.Format("PC%d", (iPC + 1));
	GetDlgItem(IDC_ED_PCNAME)->SetWindowTextA(szStr);

	szStr.Format("%d", iRcpNo);
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
	m_pIODB->IOPointWrite(m_io_SamplingTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iSamplingTime);
	m_io_RcpName.UpdateDisplay();
	m_io_RcpComment.UpdateDisplay();
	m_io_IntegrationTime.UpdateDisplay();
	m_io_DiffMax.UpdateDisplay();
	m_io_SamplingTime.UpdateDisplay();

	ShowEditStep(m_iCurrStepNo);
	ShowStepList();

}
void CDlgRcpEdit::ShowStepList()
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

		if (m_pWaveCfgCtrl->GetWaveCfgInfo(m_iCurrPC,m_pRecipe->m_tRecipe.Step[i].iUseWave, szWaveCfgName, szWaveCfgComment, &iBaseWave[0], &iBaseWave[1], &iBaseWave[2], &iBaseWave[3], szWaveCfgUpdate) == FALSE)
			szStr = _T("");
		else
			szStr.Format("%03.03d:%s", m_pRecipe->m_tRecipe.Step[i].iUseWave, szWaveCfgName);

		m_List_Step.SetItemText(i, SUB_STEP_ITEM_WAVENO, (LPCSTR)szStr);

		szStr = m_pRecipe->m_tRecipe.Step[i].szComment;
		m_List_Step.SetItemText(i, SUB_STEP_ITEM_COMMENT, (LPCSTR)szStr);
	}
	if (m_pRecipe->m_tRecipe.iStepCnt) {
		m_List_Step.SelectItem(m_iCurrStepNo -1);
	}
	else
		m_List_Step.SelectItem(-1);


}
void CDlgRcpEdit::OnLvnItemchangedListStep(NMHDR* pNMHDR, LRESULT* pResult)
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

			if(m_btn_StartGainTime.GetCheck())
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
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bBoxDetect = m_bJustBoxEnable;
			if (m_btn_Just_Peak_H.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = TRUE;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = FALSE;

			if (m_btn_Just_Box_Ascend.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_ASCEND;
			else if (m_btn_Just_Box_Descend.GetCheck())
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_DESCEND;
			else
				m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxType = BOX_FLAT;

			m_pIODB->IOPointRead(m_io_JustLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustLevel);
			m_pIODB->IOPointRead(m_io_JustDiff.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustDiffFromStart);
			m_pIODB->IOPointRead(m_io_JustLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fJustLimitTime);
			m_pIODB->IOPointRead(m_io_JustPeakVal.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
			m_pIODB->IOPointRead(m_io_JustBoxLevel.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxLevel);
			m_pIODB->IOPointRead(m_io_JustBoxTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.fBoxTime);
			m_pIODB->IOPointRead(m_io_JustPeakSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.ePeakDetectType);
			m_pIODB->IOPointRead(m_io_JustPeakRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgRate);
			m_pIODB->IOPointRead(m_io_JustBoxSelect.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.eBoxDetectType);
			m_pIODB->IOPointRead(m_io_JustBoxRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dBoxRate);
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
			m_pIODB->IOPointRead(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndFlatTime);
			m_pIODB->IOPointRead(m_io_EndFlatRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.dEndFlatRate);
			m_pIODB->IOPointRead(m_io_EndLimitTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tEnd.fEndLimitTime);

			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchTimeEnable = m_bOverEtchTime;
			m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.bOverEtchRateEnable = m_bOverEtchRate;
			m_pIODB->IOPointRead(m_io_OverEtchTime.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchTime);
			m_pIODB->IOPointRead(m_io_OverEtchRate.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tOverEtch.fOverEtchRate);

		}
		m_iCurrStepNo = iStep;
		ShowEditStep(m_iCurrStepNo);
	}
	*pResult = 0;
}
void CDlgRcpEdit::ShowEditStep(int iStep)
{
	int		i,j;

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
		if(j == m_iWaveCfgCnt)
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
	if (m_bAutoGain) {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(FALSE);
	}

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
	if (m_pRecipe->m_tRecipe.Step[i].tJust.eBoxType== BOX_ASCEND) {
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
	m_pIODB->IOPointWrite(m_io_JustPeakVal.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[i].tJust.dJustPeakChgValue);
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
	m_io_JustPeakVal.UpdateDisplay();
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
	UpdateData(FALSE);
}



void CDlgRcpEdit::OnBnClickedChkStartLevel()
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
		else{
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


void CDlgRcpEdit::OnBnClickedChkStartLimit()
{
	UpdateData(TRUE);

	if (m_bStartLimit) {
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_START_LIMIT)->EnableWindow(FALSE);
	}
}
void CDlgRcpEdit::OnBnClickedChkAutoGain()
{
	UpdateData(TRUE);

	if (m_bAutoGain) {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_AUTO_GAIN)->EnableWindow(FALSE);
	}
}
void CDlgRcpEdit::OnBnClickedChkJustLevel()
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


void CDlgRcpEdit::OnBnClickedChkJustDiff()
{
	UpdateData(TRUE);

	if (m_bJustDiff) {
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_JUST_DIFF)->EnableWindow(FALSE);
	}
}


void CDlgRcpEdit::OnBnClickedChkJustLimit()
{
	UpdateData(TRUE);

	if (m_bJustLimit) {
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_JUST_LIMIT)->EnableWindow(FALSE);
	}
}
void CDlgRcpEdit::OnBnClickedChkJustPeak()
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

void CDlgRcpEdit::OnBnClickedChkEndLevel()
{

	UpdateData(TRUE);

	if (m_bEndLevel) {
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(TRUE);
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


void CDlgRcpEdit::OnBnClickedChkEndDiff()
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


void CDlgRcpEdit::OnBnClickedChkEndDiff2()
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


void CDlgRcpEdit::OnBnClickedChkEndFlat()
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


void CDlgRcpEdit::OnBnClickedChkEndLimit()
{
	UpdateData(TRUE);

	if (m_bEndLimit) {
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_END_LIMIT)->EnableWindow(FALSE);
	}
}


void CDlgRcpEdit::OnBnClickedChkOvTime()
{
	UpdateData(TRUE);

	if (m_bOverEtchTime) {
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_OVER_ETCH_TIME)->EnableWindow(FALSE);
	}
}


void CDlgRcpEdit::OnBnClickedChkOvRate()
{
	UpdateData(TRUE);

	if (m_bOverEtchRate) {
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_ED_OVER_ETCH_RATE)->EnableWindow(FALSE);
	}
}
void CDlgRcpEdit::OnBnClickedBtnSave()
{
	int	item;
	int	 rv;
	CPCRcpCtrl* pRcpCtrl;

	pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();


	if (m_iCurrEditRcpNo == 0)
		return;

	m_pIODB->IOPointRead(m_io_RcpName.GetIOHdl(), m_pRecipe->m_tRecipe.szRecipeName);
	m_pIODB->IOPointRead(m_io_RcpComment.GetIOHdl(), m_pRecipe->m_tRecipe.szComment);
	m_pIODB->IOPointRead(m_io_IntegrationTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iIntegrationTime);
	m_pIODB->IOPointRead(m_io_DiffMax.GetIOHdl(), &m_pRecipe->m_tRecipe.dDiffDisplayMax);
	m_pIODB->IOPointRead(m_io_SamplingTime.GetIOHdl(), &m_pRecipe->m_tRecipe.iSamplingTime);

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
	m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bBoxDetect = m_bJustBoxEnable;

	if (m_btn_Just_Peak_H.GetCheck())
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = TRUE;
	else
		m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.bJustPeakHigh = FALSE;

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
	m_pIODB->IOPointRead(m_io_JustPeakVal.GetIOHdl(), &m_pRecipe->m_tRecipe.Step[m_iCurrStepNo - 1].tJust.dJustPeakChgValue);
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

	if (pRcpCtrl->CheckRcpNameExists(m_iCurrPC, m_iCurrEditRcpNo, m_pRecipe->m_tRecipe.szRecipeName) == FALSE) {
		MessageBox("Duplicate Recipe Name!", _T("Recipe Edit"), MB_ICONERROR);
		return;
	}


	rv = m_pRecipe->RecipeFileSave(m_iCurrPC, m_iCurrEditRcpNo);
	if (rv != RCP_FILE_SUCCESS)
		MessageBox(_T("Recipe Save Error!"), _T("Recipe Save"), MB_ICONERROR);

	ShowRecipeList(m_iCurrPC, TRUE);
	ShowStepList();

}
void CDlgRcpEdit::OnBnClickedBtnAddStep()
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


void CDlgRcpEdit::OnBnClickedBtnDelStep()
{
	int		item;
	int		iStep;
	int		i;

	item = m_List_Step.GetSelectedItem(-1);
	if (item == -1)
		return;

	iStep = m_List_Step.GetItemData(item);
	if(iStep){
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
void CDlgRcpEdit::OnBnClickedBtnInsertStep()
{
	int		item;
	int		iStep;
	int		i;

	item = m_List_Step.GetSelectedItem(-1);
	if (item == -1)
		return;

	iStep = m_List_Step.GetItemData(item);
	if (iStep) {
		for (i = m_pRecipe->m_tRecipe.iStepCnt; i >= iStep ; i--) {
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


void CDlgRcpEdit::OnBnClickedBtnCopy()
{
	CDlgRecipeCopy* pDlg;

	ModifyCheck();

	pDlg = new CDlgRecipeCopy(this, m_iCurrPC, m_iCurrEditRcpNo);
	if (pDlg->DoModal() == IDOK) {
		ShowRecipeList(m_iCurrPC, TRUE);
		m_ListRecipe.SelectItem(m_iCurrEditRcpNo - 1);
	}
	delete pDlg;
}


void CDlgRcpEdit::OnBnClickedBtnRemove()
{
	int			item;
	CString		szStr('\0', 64);
	CString		szRcpName('\0', 32);
	CRecipe		tRecipe;
	int			rv;


	item = m_ListRecipe.GetSelectedItem(-1);
	if (item == -1)
		return;
	
	szRcpName = m_ListRecipe.GetItemText(item, SUB_ITEM_NAME);
	szStr.Format("Do you want to delete %d:%s", m_iCurrEditRcpNo, szRcpName);
	if (MessageBox(szStr, _T("Recipe Delete"), MB_YESNO) == IDYES) {
		rv = tRecipe.RecipeFileDelete(m_iCurrPC, m_iCurrEditRcpNo);
		if (rv == RCP_FILE_SUCCESS) {
			ShowRecipeList(m_iCurrPC, TRUE);
			m_ListRecipe.SelectItem(m_iCurrEditRcpNo - 1);
		}
		else
			MessageBox(_T("Recipe Remove Error!"), _T("Recipe Remove"), MB_ICONERROR);
	}
}
void CDlgRcpEdit::OnBnClickedChkJustBox()
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


void CDlgRcpEdit::OnTimer(UINT_PTR nIDEvent)
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
