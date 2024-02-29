// CDlgRcpStepView.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgRcpStepView.h"


// CDlgRcpStepView ダイアログ

IMPLEMENT_DYNAMIC(CDlgRcpStepView, CDialogX)

CDlgRcpStepView::CDlgRcpStepView(CWnd* pParent /*=nullptr*/,int iPM, RECIPE* pRecipe, SET_WAVECFG* pWaveCfg,int iStep)
	: CDialogX(IDD_RCP_STEPVIEW, pParent)
{
	m_iPMNo = iPM;
	m_pRecipe = pRecipe;
	m_pWaveCfg = pWaveCfg;
	m_iStepNo = iStep;

	SetBackColor(255, 192, 255);
}

CDlgRcpStepView::~CDlgRcpStepView()
{
}

void CDlgRcpStepView::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PCNO, m_io_PC);
	DDX_Control(pDX, IDC_ED_RCPNO, m_io_RcpNo);
	DDX_Control(pDX, IDC_ED_RCPNAME, m_io_RcpName);
	DDX_Control(pDX, IDC_ED_STEPNO, m_io_StepNo);
	DDX_Control(pDX, IDC_ED_DEADTIME, m_io_DeadTimeVew);
	DDX_Control(pDX, IDC_ED_STARTLEVEL_SW, m_io_StartLevelSW);
	DDX_Control(pDX, IDC_ED_STARTLEVEL, m_io_StartLevelView);
	DDX_Control(pDX, IDC_ED_JUSTLEVEL_SW, m_io_JustLevelSW);
	DDX_Control(pDX, IDC_ED_JUSTLEVEL, m_io_JustLevelView);
	DDX_Control(pDX, IDC_ED_JUSTDIFF_SW, m_io_JustDiffSW);
	DDX_Control(pDX, IDC_ED_JUSTDIFF, m_io_JustDiffView);
	DDX_Control(pDX, IDC_ED_JUSTLIMIT_SW, m_io_JustLimitSW);
	DDX_Control(pDX, IDC_ED_JUSTLIMIT, m_io_JustLimitView);
	DDX_Control(pDX, IDC_ED_ENDLEVEL_SW, m_io_EndLevelSW);
	DDX_Control(pDX, IDC_ED_ENDLEVEL, m_io_EndLevelView);
	DDX_Control(pDX, IDC_ED_ENDDIFF_SW1, m_io_EndDiff1SW);
	DDX_Control(pDX, IDC_ED_ENDDIFF1, m_io_EndDiff1View);
	DDX_Control(pDX, IDC_ED_ENDDIFF_SW2, m_io_EndDiff2SW);
	DDX_Control(pDX, IDC_ED_ENDDIFF2, m_io_EndDiff2View);
	DDX_Control(pDX, IDC_ED_FLATWAVE_SW, m_io_EndFlatSW);
	DDX_Control(pDX, IDC_ED_FLATLEVEL, m_io_EndFlatLevel);
	DDX_Control(pDX, IDC_ED_FLATTIME, m_io_EndFlatTime);
	DDX_Control(pDX, IDC_ED_ENDLIMIT_SW, m_io_EndLimitSW);
	DDX_Control(pDX, IDC_ED_ENDLIMIT, m_io_EndLimitView);
	DDX_Control(pDX, IDC_ED_OVETCHTIME_SW, m_io_OverEtchTimeSW);
	DDX_Control(pDX, IDC_ED_OVERETCHTIME, m_io_OverEtchTimeView);
	DDX_Control(pDX, IDC_ED_OVETCHRATE_SW, m_io_OverEtchRateSW);
	DDX_Control(pDX, IDC_ED_OVERETCHRATE, m_io_OverEtchRateView);
	DDX_Control(pDX, IDC_ED_WAVECFGNO, m_io_WaveCfgNo);
	DDX_Control(pDX, IDC_ED_WAVECFGNAME, m_io_WaveCfgName);
	DDX_Control(pDX, IDC_ED_WAVE_A, m_io_WaveA);
	DDX_Control(pDX, IDC_ED_WAVE_B1, m_io_WaveB1);
	DDX_Control(pDX, IDC_ED_WAVE_B2, m_io_WaveB2);
	DDX_Control(pDX, IDC_ED_WAVE_A_CNT, m_io_Wave_A_Cnt);
	DDX_Control(pDX, IDC_ED_WAVE_A_OFFGAIN1, m_io_A_OffGainType1);
	DDX_Control(pDX, IDC_ED_WAVE_A_OFFGAIN2, m_io_A_OffGainType2);
	DDX_Control(pDX, IDC_ED_WAVE_A_OFFGAIN_VAL1, m_io_A_OffGainVal1);
	DDX_Control(pDX, IDC_ED_WAVE_A_OFFGAIN_VAL2, m_io_A_OffGainVal2);
	DDX_Control(pDX, IDC_ED_WAVE_A_RECIPROCAL, m_io_A_Reciprocal);
	DDX_Control(pDX, IDC_ED_WAVE_B1_CNT, m_io_Wave_B1_Cnt);
	DDX_Control(pDX, IDC_ED_WAVE_B1_OFFGAIN1, m_io_B1_OffGainType1);
	DDX_Control(pDX, IDC_ED_WAVE_B1_OFFGAIN2, m_io_B1_OffGainType2);
	DDX_Control(pDX, IDC_ED_WAVE_B1_OFFGAIN_VAL1, m_io_B1_OffGainVal1);
	DDX_Control(pDX, IDC_ED_WAVE_B1_OFFGAIN_VAL2, m_io_B1_OffGainVal2);
	DDX_Control(pDX, IDC_ED_WAVE_B1_RECIPROCAL, m_io_B1_Reciprocal);
	DDX_Control(pDX, IDC_ED_WAVE_B2_CNT, m_io_Wave_B2_Cnt);
	DDX_Control(pDX, IDC_ED_WAVE_B2_OFFGAIN1, m_io_B2_OffGainType1);
	DDX_Control(pDX, IDC_ED_WAVE_B2_OFFGAIN2, m_io_B2_OffGainType2);
	DDX_Control(pDX, IDC_ED_WAVE_B2_OFFGAIN_VAL1, m_io_B2_OffGainVal1);
	DDX_Control(pDX, IDC_ED_WAVE_B2_OFFGAIN_VAL2, m_io_B2_OffGainVal2);
	DDX_Control(pDX, IDC_ED_WAVE_B2_RECIPROCAL, m_io_B2_Reciprocal);

	DDX_Control(pDX, IDC_ED_WAVE_SEL1, m_io_WaveSel1);
	DDX_Control(pDX, IDC_ED_WAVE_SEL2, m_io_WaveSel2);
	DDX_Control(pDX, IDC_ED_WAVE_SEL3, m_io_WaveSel3);
	DDX_Control(pDX, IDC_ED_OPERAND1, m_io_WaveOperand1);
	DDX_Control(pDX, IDC_ED_OPERAND2, m_io_WaveOperand2);
	DDX_Control(pDX, IDC_ED_WAVE_STEP2_AVECNT, m_io_Step2_AveCnt);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_RECIPROCAL, m_io_Step3_Reciprocal);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_OFFSET_TYPE, m_io_Step3_OffsetType);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_OFFSET_VAL, m_io_Step3_OffsetVal);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_MAXLVL_TYPE, m_io_Step3_MaxLvlType);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_MAXLVL_VAL, m_io_Step3_MaxLevel);
	DDX_Control(pDX, IDC_ED_WAVE_STEP3_AVECNT, m_io_Step3_AveCnt);
	DDX_Control(pDX, IDC_ED_STEP4_DIFF1, m_io_Step4_DiffEnable1);
	DDX_Control(pDX, IDC_ED_STEP4_DIFF1_RPTAVE, m_io_Step4_RepeatAveCnt1);
	DDX_Control(pDX, IDC_ED_STEP4_DIFF2, m_io_Step4_DiffEnable2);
	DDX_Control(pDX, IDC_ED_STEP4_DIFF2_RPTAVE, m_io_Step4_RepeatAveCnt2);
	DDX_Control(pDX, IDC_ED_WAVE_STEP4_AVECNT, m_io_Step4_AveCnt);
	DDX_Control(pDX, IDC_ED_WAVE_STEP4_CORRECT, m_io_Step4_CorrType);
	DDX_Control(pDX, IDC_ED_WAVE_STEP4_CORR_VAL, m_io_Step4_CorrVal);

	DDX_Control(pDX, IDC_ED_INTEGTIME, m_io_IntegTime);
}


BEGIN_MESSAGE_MAP(CDlgRcpStepView, CDialogX)
END_MESSAGE_MAP()


// CDlgRcpStepView メッセージ ハンドラー



BOOL CDlgRcpStepView::OnInitDialog()
{
	int		iVal;
	char	szStr[64];

	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();
	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();


	m_io_PC.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.PCName"));
	m_io_RcpNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpNo"));
	m_io_RcpName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpName"));
	
	m_io_IntegTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.IntegTime"));
	m_io_StepNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StepNo"));
	m_io_DeadTimeVew.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.DeadTime"));
	m_io_StartLevelSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StartLevelSW"));
	m_io_StartLevelView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.StartLevel"));

	m_io_JustLevelSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLevelSW"));
	m_io_JustLevelView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLevel"));
	m_io_JustDiffSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustDiffSW"));
	m_io_JustDiffView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustDiff"));
	m_io_JustLimitSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLimitSW"));
	m_io_JustLimitView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.JustLimitTm"));

	m_io_EndLevelSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLevelSW"));
	m_io_EndLevelView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLevel"));
	m_io_EndDiff1SW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffJustSW"));
	m_io_EndDiff1View.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffJust"));
	m_io_EndDiff2SW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffPeakSW"));
	m_io_EndDiff2View.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndDiffPeak"));
	m_io_EndFlatSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatSW"));
	m_io_EndFlatLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatLevel"));
	m_io_EndFlatTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndFlatTime"));
	m_io_EndLimitSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLimitSW"));
	m_io_EndLimitView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.EndLimitTm"));

	m_io_OverEtchTimeSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchTMSW"));
	m_io_OverEtchTimeView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchTime"));
	m_io_OverEtchRateSW.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchRTSW"));
	m_io_OverEtchRateView.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.OverEtchRate"));

	m_io_WaveCfgNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgWaveNo"));
	m_io_WaveCfgName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgWaveName"));

	m_io_WaveA.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgBaseWave1"));
	m_io_WaveB1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgBaseWave2"));
	m_io_WaveB1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.CfgBaseWave3"));

	// Step1
	m_io_Wave_A_Cnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.WaveCnt"));
	m_io_A_OffGainType1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.OffGainType1"));
	m_io_A_OffGainType2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.OffGainType2"));
	m_io_A_OffGainVal1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.OffGainVal1"));
	m_io_A_OffGainVal2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.OffGainVal2"));
	m_io_A_Reciprocal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_A.Reciprocal"));
	m_io_Wave_B1_Cnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.WaveCnt"));
	m_io_B1_OffGainType1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.OffGainType1"));
	m_io_B1_OffGainType2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.OffGainType2"));
	m_io_B1_OffGainVal1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.OffGainVal1"));
	m_io_B1_OffGainVal2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.OffGainVal2"));
	m_io_B1_Reciprocal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B1.Reciprocal"));
	m_io_Wave_B2_Cnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.WaveCnt"));
	m_io_B2_OffGainType1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.OffGainType1"));
	m_io_B2_OffGainType2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.OffGainType2"));
	m_io_B2_OffGainVal1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.OffGainVal1"));
	m_io_B2_OffGainVal2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.OffGainVal2"));
	m_io_B2_Reciprocal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Wave_B2.Reciprocal"));

	// Step2
	m_io_WaveSel1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.Wave1"));
	m_io_WaveSel2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.Wave2"));
	m_io_WaveSel3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.Wave3"));
	m_io_WaveOperand1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.Operand1"));
	m_io_WaveOperand2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.Operand2"));
	m_io_Step2_AveCnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step2.AveCnt"));

	// Step3
	m_io_Step3_Reciprocal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.Reciprocal"));
	m_io_Step3_OffsetType.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.OffsetType"));
	m_io_Step3_OffsetVal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.ManualOffset"));
	m_io_Step3_MaxLvlType.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.MaxLvlType"));
	m_io_Step3_MaxLevel.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.ManualMax"));
	m_io_Step3_AveCnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step3.AveCnt"));

	// Step4
	m_io_Step4_DiffEnable1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.DiffEnable1"));
	m_io_Step4_RepeatAveCnt1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.RepeatAveDiff1"));
	m_io_Step4_DiffEnable2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.DiffEnable2"));
	m_io_Step4_RepeatAveCnt2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.RepeatAveDiff2"));
	m_io_Step4_AveCnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.AveCnt"));
	m_io_Step4_CorrType.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.CorrectType"));
	m_io_Step4_CorrVal.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Step4.CorrectVal"));

	m_io_PC.SetSpecialColor(1);
	m_io_RcpNo.SetSpecialColor(1);
	m_io_RcpName.SetSpecialColor(1);
	m_io_IntegTime.SetSpecialColor(1);
	m_io_StepNo.SetSpecialColor(1);
	m_io_DeadTimeVew.SetSpecialColor(1);
	m_io_StartLevelSW.SetSpecialColor(1);
	m_io_StartLevelView.SetSpecialColor(1);

	m_io_JustLevelSW.SetSpecialColor(1);
	m_io_JustLevelView.SetSpecialColor(1);
	m_io_JustDiffSW.SetSpecialColor(1);
	m_io_JustDiffView.SetSpecialColor(1);
	m_io_JustLimitSW.SetSpecialColor(1);
	m_io_JustLimitView.SetSpecialColor(1);

	m_io_EndLevelSW.SetSpecialColor(1);
	m_io_EndLevelView.SetSpecialColor(1);
	m_io_EndDiff1SW.SetSpecialColor(1);
	m_io_EndDiff1View.SetSpecialColor(1);
	m_io_EndDiff2SW.SetSpecialColor(1);
	m_io_EndDiff2View.SetSpecialColor(1);
	m_io_EndFlatSW.SetSpecialColor(1);
	m_io_EndFlatLevel.SetSpecialColor(1);
	m_io_EndFlatTime.SetSpecialColor(1);
	m_io_EndLimitSW.SetSpecialColor(1);
	m_io_EndLimitView.SetSpecialColor(1);

	m_io_OverEtchTimeSW.SetSpecialColor(1);
	m_io_OverEtchTimeView.SetSpecialColor(1);
	m_io_OverEtchRateSW.SetSpecialColor(1);
	m_io_OverEtchRateView.SetSpecialColor(1);

	m_io_WaveCfgNo.SetSpecialColor(1);
	m_io_WaveCfgName.SetSpecialColor(1);
	m_io_WaveA.SetSpecialColor(1);
	m_io_WaveB1.SetSpecialColor(1);
	m_io_WaveB2.SetSpecialColor(1);

	m_io_Wave_A_Cnt.SetSpecialColor(1);
	m_io_A_OffGainType1.SetSpecialColor(1);
	m_io_A_OffGainType2.SetSpecialColor(1);
	m_io_A_OffGainVal1.SetSpecialColor(1);
	m_io_A_OffGainVal2.SetSpecialColor(1);
	m_io_A_Reciprocal.SetSpecialColor(1);
	m_io_Wave_B1_Cnt.SetSpecialColor(1);
	m_io_B1_OffGainType1.SetSpecialColor(1);
	m_io_B1_OffGainType2.SetSpecialColor(1);
	m_io_B1_OffGainVal1.SetSpecialColor(1);
	m_io_B1_OffGainVal2.SetSpecialColor(1);
	m_io_B1_Reciprocal.SetSpecialColor(1);
	m_io_Wave_B2_Cnt.SetSpecialColor(1);
	m_io_B2_OffGainType1.SetSpecialColor(1);
	m_io_B2_OffGainType2.SetSpecialColor(1);
	m_io_B2_OffGainVal1.SetSpecialColor(1);
	m_io_B2_OffGainVal2.SetSpecialColor(1);
	m_io_B2_Reciprocal.SetSpecialColor(1);

	// Step2
	m_io_WaveSel1.SetSpecialColor(1);
	m_io_WaveSel2.SetSpecialColor(1);
	m_io_WaveSel3.SetSpecialColor(1);
	m_io_WaveOperand1.SetSpecialColor(1);
	m_io_WaveOperand2.SetSpecialColor(1);
	m_io_Step2_AveCnt.SetSpecialColor(1);

	// Step3
	m_io_Step3_Reciprocal.SetSpecialColor(1);
	m_io_Step3_OffsetType.SetSpecialColor(1);
	m_io_Step3_OffsetVal.SetSpecialColor(1);
	m_io_Step3_MaxLvlType.SetSpecialColor(1);
	m_io_Step3_MaxLevel.SetSpecialColor(1);
	m_io_Step3_AveCnt.SetSpecialColor(1);

	// Step4
	m_io_Step4_DiffEnable1.SetSpecialColor(1);
	m_io_Step4_RepeatAveCnt1.SetSpecialColor(1);
	m_io_Step4_DiffEnable2.SetSpecialColor(1);
	m_io_Step4_RepeatAveCnt2.SetSpecialColor(1);
	m_io_Step4_AveCnt.SetSpecialColor(1);
	m_io_Step4_CorrType.SetSpecialColor(1);
	m_io_Step4_CorrVal.SetSpecialColor(1);


	strcpy(m_szRecipeName, m_pRecipe->szRecipeName);
	m_iRecipeNo = m_pRcpCtrl->FindRcpbyName(m_iPMNo, m_szRecipeName);

	m_pIODB->IOPointWrite(m_io_PC.GetIOHdl(), &m_iPMNo);
	m_pIODB->IOPointWrite(m_io_RcpNo.GetIOHdl(), &m_iRecipeNo);
	m_pIODB->IOPointWrite(m_io_RcpName.GetIOHdl(), m_szRecipeName);
	m_pIODB->IOPointWrite(m_io_IntegTime.GetIOHdl(), &m_pRecipe->iIntegrationTime);
	iVal = m_iStepNo + 1;
	m_pIODB->IOPointWrite(m_io_StepNo.GetIOHdl(), &iVal);

	/* Dead Time*/
	m_pIODB->IOPointWrite(m_io_DeadTimeVew.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tStart.fDeadTime);
	iVal = m_pRecipe->Step[m_iStepNo].tStart.bStartLevelEnable;
	iVal += m_pRecipe->Step[m_iStepNo].tStart.eLevelType;
	m_pIODB->IOPointWrite(m_io_StartLevelSW.GetIOHdl(), &iVal);
	m_pIODB->IOPointWrite(m_io_StartLevelView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tStart.dStartLevel);
	if (iVal == 0)
		GetDlgItem(IDC_ED_STARTLEVEL)->EnableWindow(FALSE);

	/* Just */
	iVal = m_pRecipe->Step[m_iStepNo].tJust.bJustLevelEnable;
	iVal += m_pRecipe->Step[m_iStepNo].tJust.eLevelType;
	m_pIODB->IOPointWrite(m_io_JustLevelSW.GetIOHdl(), &iVal);

	m_pIODB->IOPointWrite(m_io_JustLevelView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tJust.dJustLevel);
	if (iVal == 0)
		GetDlgItem(IDC_ED_JUSTLEVEL)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_JustDiffSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tJust.bJustDiffEnable);
	m_pIODB->IOPointWrite(m_io_JustDiffView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tJust.dJustDiffFromStart);
	if (m_pRecipe->Step[m_iStepNo].tJust.bJustDiffEnable == 0)
		GetDlgItem(IDC_ED_JUSTDIFF)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_JustLimitSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tJust.bJustLimitEnable);
	m_pIODB->IOPointWrite(m_io_JustLimitView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tJust.fJustLimitTime);
	if (m_pRecipe->Step[m_iStepNo].tJust.bJustLimitEnable == 0)
		GetDlgItem(IDC_ED_JUSTLIMIT)->EnableWindow(FALSE);

	/* End */
	iVal = m_pRecipe->Step[m_iStepNo].tEnd.bEndLevelEnable;
	iVal += m_pRecipe->Step[m_iStepNo].tEnd.eLevelType;
	m_pIODB->IOPointWrite(m_io_EndLevelSW.GetIOHdl(), &iVal);
	m_pIODB->IOPointWrite(m_io_EndLevelView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.dEndLevel);
	if (iVal == 0)
		GetDlgItem(IDC_ED_ENDLEVEL)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_EndDiff1SW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.bEndDiff1Enable);
	m_pIODB->IOPointWrite(m_io_EndLevelView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.dEndDiffFromJust);
	if (m_pRecipe->Step[m_iStepNo].tEnd.bEndDiff1Enable == 0)
		GetDlgItem(IDC_ED_ENDDIFF1)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_EndDiff2SW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.bEndDiff2Enable);
	m_pIODB->IOPointWrite(m_io_EndDiff2View.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.dEndDiffFromPeak);
	if (m_pRecipe->Step[m_iStepNo].tEnd.bEndDiff2Enable == 0)
		GetDlgItem(IDC_ED_ENDDIFF2)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_EndFlatSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.bEndFlatEnable);
	m_pIODB->IOPointWrite(m_io_EndFlatLevel.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.dEndFlatWave);
	m_pIODB->IOPointWrite(m_io_EndFlatTime.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.fEndFlatTime);
	if (m_pRecipe->Step[m_iStepNo].tEnd.bEndFlatEnable == 0) {
		GetDlgItem(IDC_ED_FLATLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_FLATTIME)->EnableWindow(FALSE);
	}
	m_pIODB->IOPointWrite(m_io_EndLimitSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.bEndLimitEnable);
	m_pIODB->IOPointWrite(m_io_EndLimitView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tEnd.fEndLimitTime);
	if (m_pRecipe->Step[m_iStepNo].tEnd.bEndLimitEnable == 0)
		GetDlgItem(IDC_ED_ENDLIMIT)->EnableWindow(FALSE);

	/* Over Etch */
	m_pIODB->IOPointWrite(m_io_OverEtchTimeSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tOverEtch.bOverEtchTimeEnable);
	m_pIODB->IOPointWrite(m_io_OverEtchTimeView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tOverEtch.fOverEtchTime);
	if (m_pRecipe->Step[m_iStepNo].tOverEtch.bOverEtchTimeEnable == 0)
		GetDlgItem(IDC_ED_OVERETCHTIME)->EnableWindow(FALSE);

	m_pIODB->IOPointWrite(m_io_OverEtchRateSW.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tOverEtch.bOverEtchRateEnable);
	m_pIODB->IOPointWrite(m_io_OverEtchRateView.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].tOverEtch.fOverEtchRate);
	if (m_pRecipe->Step[m_iStepNo].tOverEtch.bOverEtchRateEnable == 0)
		GetDlgItem(IDC_ED_OVERETCHRATE)->EnableWindow(FALSE);

	/* Wave CFG */
	m_pIODB->IOPointWrite(m_io_WaveCfgNo.GetIOHdl(), &m_pRecipe->Step[m_iStepNo].iUseWave);
	m_pIODB->IOPointWrite(m_io_WaveCfgName.GetIOHdl(), &m_pWaveCfg->szWaveCfgName);

	if (m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo)
		sprintf(szStr, "%d:%6.2fum", m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo, m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo -1]);
	else
		strcpy(szStr, "Not Use");
	m_pIODB->IOPointWrite(m_io_WaveA.GetIOHdl(), szStr);

	if (m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo)
		sprintf(szStr, "%d:%6.2fum", m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo, m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo -1]);
	else
		strcpy(szStr, "Not Use");
	m_pIODB->IOPointWrite(m_io_WaveB1.GetIOHdl(), szStr);

	if (m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo)
		sprintf(szStr, "%d:%6.2fum", m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo, m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo - 1]);
	else
		strcpy(szStr, "Not Use");
	m_pIODB->IOPointWrite(m_io_WaveB2.GetIOHdl(), szStr);

	m_pIODB->IOPointWrite(m_io_Wave_A_Cnt.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_A_OffGainType1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_A_OffGainType2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_A_OffGainVal1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1);;
	m_pIODB->IOPointWrite(m_io_A_OffGainVal2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_A_Reciprocal.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal);
	m_pIODB->IOPointWrite(m_io_Wave_B1_Cnt.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_B1_OffGainType1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_B1_OffGainType2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_B1_OffGainVal1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1);
	m_pIODB->IOPointWrite(m_io_B1_OffGainVal2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_B1_Reciprocal.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal);
	m_pIODB->IOPointWrite(m_io_Wave_B2_Cnt.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_B2_OffGainType1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_B2_OffGainType2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_B2_OffGainVal1.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1);
	m_pIODB->IOPointWrite(m_io_B2_OffGainVal2.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_B2_Reciprocal.GetIOHdl(), &m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal);

	// Step2
	m_pIODB->IOPointWrite(m_io_WaveSel1.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.eWave1);
	m_pIODB->IOPointWrite(m_io_WaveSel2.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.eWave2);
	m_pIODB->IOPointWrite(m_io_WaveSel3.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.eWave3);
	m_pIODB->IOPointWrite(m_io_WaveOperand1.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.eOperandType1);
	m_pIODB->IOPointWrite(m_io_WaveOperand2.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.eOperandType2);
	m_pIODB->IOPointWrite(m_io_Step2_AveCnt.GetIOHdl(), &m_pWaveCfg->tStep2_WaveJoin.iAveCnt);

	// Step3
	m_pIODB->IOPointWrite(m_io_Step3_Reciprocal.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.iReciprocal);
	m_pIODB->IOPointWrite(m_io_Step3_OffsetType.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.eOffsetType);
	m_pIODB->IOPointWrite(m_io_Step3_OffsetVal.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.dUserOffset);
	m_pIODB->IOPointWrite(m_io_Step3_MaxLvlType.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType);
	m_pIODB->IOPointWrite(m_io_Step3_MaxLevel.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel);
	m_pIODB->IOPointWrite(m_io_Step3_AveCnt.GetIOHdl(), &m_pWaveCfg->tStep3_WaveNorm.iAveCnt);

	// Step4
	m_pIODB->IOPointWrite(m_io_Step4_DiffEnable1.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable);
	m_pIODB->IOPointWrite(m_io_Step4_RepeatAveCnt1.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_DiffEnable2.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable);
	m_pIODB->IOPointWrite(m_io_Step4_RepeatAveCnt2.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_AveCnt.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_CorrType.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect);
	m_pIODB->IOPointWrite(m_io_Step4_CorrVal.GetIOHdl(), &m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal);



	m_io_PC.UpdateDisplay();
	m_io_RcpNo.UpdateDisplay();
	m_io_RcpName.UpdateDisplay();
	m_io_IntegTime.UpdateDisplay();
	m_io_StepNo.UpdateDisplay();
	m_io_DeadTimeVew.UpdateDisplay();
	m_io_StartLevelSW.UpdateDisplay();
	m_io_StartLevelView.UpdateDisplay();
	m_io_JustLevelSW.UpdateDisplay();
	m_io_JustLevelView.UpdateDisplay();
	m_io_JustDiffSW.UpdateDisplay();
	m_io_JustDiffView.UpdateDisplay();
	m_io_JustLimitSW.UpdateDisplay();
	m_io_JustLimitView.UpdateDisplay();
	m_io_EndLevelSW.UpdateDisplay();
	m_io_EndLevelView.UpdateDisplay();
	m_io_EndDiff1SW.UpdateDisplay();
	m_io_EndDiff1View.UpdateDisplay();
	m_io_EndDiff2SW.UpdateDisplay();
	m_io_EndDiff2View.UpdateDisplay();
	m_io_EndFlatSW.UpdateDisplay();
	m_io_EndFlatLevel.UpdateDisplay();
	m_io_EndFlatTime.UpdateDisplay();
	m_io_EndLimitSW.UpdateDisplay();
	m_io_EndLimitView.UpdateDisplay();
	m_io_OverEtchTimeSW.UpdateDisplay();
	m_io_OverEtchTimeView.UpdateDisplay();
	m_io_OverEtchRateSW.UpdateDisplay();
	m_io_OverEtchRateView.UpdateDisplay();

	m_io_WaveCfgNo.UpdateDisplay();
	m_io_WaveCfgName.UpdateDisplay();

	m_io_WaveA.UpdateDisplay();
	m_io_WaveB1.UpdateDisplay();
	m_io_WaveB2.UpdateDisplay();

	m_io_Wave_A_Cnt.UpdateDisplay();
	m_io_A_OffGainType1.UpdateDisplay();
	m_io_A_OffGainType2.UpdateDisplay();
	m_io_A_OffGainVal1.UpdateDisplay();
	m_io_A_OffGainVal2.UpdateDisplay();
	m_io_A_Reciprocal.UpdateDisplay();
	m_io_Wave_B1_Cnt.UpdateDisplay();
	m_io_B1_OffGainType1.UpdateDisplay();
	m_io_B1_OffGainType2.UpdateDisplay();
	m_io_B1_OffGainVal1.UpdateDisplay();
	m_io_B1_OffGainVal2.UpdateDisplay();
	m_io_B1_Reciprocal.UpdateDisplay();
	m_io_Wave_B2_Cnt.UpdateDisplay();
	m_io_B2_OffGainType1.UpdateDisplay();
	m_io_B2_OffGainType2.UpdateDisplay();
	m_io_B2_OffGainVal1.UpdateDisplay();
	m_io_B2_OffGainVal2.UpdateDisplay();
	m_io_B2_Reciprocal.UpdateDisplay();

	// Step2
	m_io_WaveSel1.UpdateDisplay();
	m_io_WaveSel2.UpdateDisplay();
	m_io_WaveSel3.UpdateDisplay();
	m_io_WaveOperand1.UpdateDisplay();
	m_io_WaveOperand2.UpdateDisplay();
	m_io_Step2_AveCnt.UpdateDisplay();

	// Step3
	m_io_Step3_Reciprocal.UpdateDisplay();
	m_io_Step3_OffsetType.UpdateDisplay();
	m_io_Step3_OffsetVal.UpdateDisplay();
	m_io_Step3_MaxLvlType.UpdateDisplay();
	m_io_Step3_MaxLevel.UpdateDisplay();
	m_io_Step3_AveCnt.UpdateDisplay();

	// Step4
	m_io_Step4_DiffEnable1.UpdateDisplay();
	m_io_Step4_RepeatAveCnt1.UpdateDisplay();
	m_io_Step4_DiffEnable2.UpdateDisplay();
	m_io_Step4_RepeatAveCnt2.UpdateDisplay();
	m_io_Step4_AveCnt.UpdateDisplay();
	m_io_Step4_CorrType.UpdateDisplay();
	m_io_Step4_CorrVal.UpdateDisplay();




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
