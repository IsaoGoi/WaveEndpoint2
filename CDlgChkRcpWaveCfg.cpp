// CDlgChkRcpWaveCfg.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgChkRcpWaveCfg.h"


// CDlgChkRcpWaveCfg ダイアログ

IMPLEMENT_DYNAMIC(CDlgChkRcpWaveCfg, CDialogX)

CDlgChkRcpWaveCfg::CDlgChkRcpWaveCfg(CWnd* pParent /*=nullptr*/,int iPM,int iWaveCfgNo)
	: CDialogX(IDD_CHKRCPWAVECFG, pParent)
{
	m_pWaveCfg = NULL;

	m_iWaveCfgNo = iWaveCfgNo;
	m_iPMNo = iPM;
	SetBackColor(255, 192, 255);
}

CDlgChkRcpWaveCfg::~CDlgChkRcpWaveCfg()
{
	if (m_pWaveCfg)
		delete m_pWaveCfg;
}

void CDlgChkRcpWaveCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_FILE1, m_io_WaveCfgName);
	DDX_Control(pDX, IDC_ED_COMMENT, m_io_WaveCfgComment);
	DDX_Control(pDX, IDC_CMB_WAVE_A, m_cmb_WaveA);
	DDX_Control(pDX, IDC_CMB_WAVE_B1,m_cmb_WaveB1);
	DDX_Control(pDX, IDC_CMB_WAVE_B2,m_cmb_WaveB2);
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
	DDX_Control(pDX, IDC_ED_DISP_DIFF_SCALE, m_io_DISP_DiffScale);
	DDX_Control(pDX, IDC_ED_DISP_DIFF_MAX, m_io_DISP_DiffMax_Val);
	DDX_Control(pDX, IDC_ED_DISP_DIFF_EXP, m_io_DISP_DiffMax_Exp);

}


BEGIN_MESSAGE_MAP(CDlgChkRcpWaveCfg, CDialogX)
	ON_BN_CLICKED(IDC_BTN_SAVE1, &CDlgChkRcpWaveCfg::OnBnClickedBtnSave1)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgChkRcpWaveCfg メッセージ ハンドラー
BOOL CDlgChkRcpWaveCfg::OnInitDialog()
{
	CRect		rc;
	int			i;
	CString	szStr('\0', 32);

	CDialogX::OnInitDialog();

	// Wave Graph Rect

	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pWaveCfg = new CCalcWaveCfg();

	szStr = _T("0:No Select");
	m_cmb_WaveA.InsertString(0, szStr);
	m_cmb_WaveB1.InsertString(0, szStr);
	m_cmb_WaveB2.InsertString(0, szStr);


	m_io_WaveCfgName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.WaveCfgName"));
	m_io_WaveCfgComment.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.WaveCfgComment"));

	for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
		szStr.Format("%d:%6.2fum", (i + 1), m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i]);
		m_cmb_WaveA.InsertString((i + 1), szStr);
		m_cmb_WaveB1.InsertString((i + 1), szStr);
		m_cmb_WaveB2.InsertString((i + 1), szStr);
	}

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

	// 微分表示Scale
	m_io_DISP_DiffScale.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Disp.DiffScaleEnable"));
	m_io_DISP_DiffMax_Val.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Disp.DiffScaleVal"));
	m_io_DISP_DiffMax_Exp.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Disp.DiffScaleExp"));


	m_io_WaveCfgName.SetSpecialColor(2);
	m_io_WaveCfgComment.SetSpecialColor(2);

	m_io_Wave_A_Cnt.SetSpecialColor(2);
	m_io_A_OffGainType1.SetSpecialColor(2);
	m_io_A_OffGainType2.SetSpecialColor(2);
	m_io_A_OffGainVal1.SetSpecialColor(2);
	m_io_A_OffGainVal2.SetSpecialColor(2);
	m_io_A_Reciprocal.SetSpecialColor(2);
	m_io_Wave_B1_Cnt.SetSpecialColor(2);
	m_io_B1_OffGainType1.SetSpecialColor(2);
	m_io_B1_OffGainType2.SetSpecialColor(2);
	m_io_B1_OffGainVal1.SetSpecialColor(2);
	m_io_B1_OffGainVal2.SetSpecialColor(2);
	m_io_B1_Reciprocal.SetSpecialColor(2);
	m_io_Wave_B2_Cnt.SetSpecialColor(2);
	m_io_B2_OffGainType1.SetSpecialColor(2);
	m_io_B2_OffGainType2.SetSpecialColor(2);
	m_io_B2_OffGainVal1.SetSpecialColor(2);
	m_io_B2_OffGainVal2.SetSpecialColor(2);
	m_io_B2_Reciprocal.SetSpecialColor(2);

	// Step2
	m_io_WaveSel1.SetSpecialColor(2);
	m_io_WaveSel2.SetSpecialColor(2);
	m_io_WaveSel3.SetSpecialColor(2);
	m_io_WaveOperand1.SetSpecialColor(2);
	m_io_WaveOperand2.SetSpecialColor(2);
	m_io_Step2_AveCnt.SetSpecialColor(2);

	// Step3
	m_io_Step3_Reciprocal.SetSpecialColor(2);
	m_io_Step3_OffsetType.SetSpecialColor(2);
	m_io_Step3_OffsetVal.SetSpecialColor(2);
	m_io_Step3_MaxLvlType.SetSpecialColor(2);
	m_io_Step3_MaxLevel.SetSpecialColor(2);
	m_io_Step3_AveCnt.SetSpecialColor(2);

	// Step4
	m_io_Step4_DiffEnable1.SetSpecialColor(2);
	m_io_Step4_RepeatAveCnt1.SetSpecialColor(2);
	m_io_Step4_DiffEnable2.SetSpecialColor(2);
	m_io_Step4_RepeatAveCnt2.SetSpecialColor(2);
	m_io_Step4_AveCnt.SetSpecialColor(2);
	m_io_Step4_CorrType.SetSpecialColor(2);
	m_io_Step4_CorrVal.SetSpecialColor(2);

	m_io_DISP_DiffScale.SetSpecialColor(2);
	m_io_DISP_DiffMax_Val.SetSpecialColor(2);
	m_io_DISP_DiffMax_Exp.SetSpecialColor(2);


	m_pWaveCfg->LoadWaveCfg(m_iPMNo,m_iWaveCfgNo);

	szStr.Format("%03.03d", m_iWaveCfgNo);
	GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

	m_pIODB->IOPointWrite(m_io_WaveCfgName.GetIOHdl(), m_pWaveCfg->m_pWaveCfg->szWaveCfgName);
	m_pIODB->IOPointWrite(m_io_WaveCfgComment.GetIOHdl(), m_pWaveCfg->m_pWaveCfg->szWaveCfgComment);

	m_cmb_WaveA.SetCurSel(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo);
	m_cmb_WaveB1.SetCurSel(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo);
	m_cmb_WaveB2.SetCurSel(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo);


	m_pIODB->IOPointWrite(m_io_Wave_A_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_A_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_A_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_A_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1);;
	m_pIODB->IOPointWrite(m_io_A_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_A_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal);
	m_pIODB->IOPointWrite(m_io_Wave_B1_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_B1_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_B1_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_B1_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1);
	m_pIODB->IOPointWrite(m_io_B1_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_B1_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal);
	m_pIODB->IOPointWrite(m_io_Wave_B2_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt);
	m_pIODB->IOPointWrite(m_io_B2_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1);
	m_pIODB->IOPointWrite(m_io_B2_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2);
	m_pIODB->IOPointWrite(m_io_B2_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1);
	m_pIODB->IOPointWrite(m_io_B2_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2);
	m_pIODB->IOPointWrite(m_io_B2_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal);

	// Step2
	m_pIODB->IOPointWrite(m_io_WaveSel1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave1);
	m_pIODB->IOPointWrite(m_io_WaveSel2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2);
	m_pIODB->IOPointWrite(m_io_WaveSel3.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3);
	m_pIODB->IOPointWrite(m_io_WaveOperand1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType1);
	m_pIODB->IOPointWrite(m_io_WaveOperand2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType2);
	m_pIODB->IOPointWrite(m_io_Step2_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt);

	// Step3
	m_pIODB->IOPointWrite(m_io_Step3_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iReciprocal);
	m_pIODB->IOPointWrite(m_io_Step3_OffsetType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eOffsetType);
	m_pIODB->IOPointWrite(m_io_Step3_OffsetVal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserOffset);
	m_pIODB->IOPointWrite(m_io_Step3_MaxLvlType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType);
	m_pIODB->IOPointWrite(m_io_Step3_MaxLevel.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel);
	m_pIODB->IOPointWrite(m_io_Step3_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt);

	// Step4
	m_pIODB->IOPointWrite(m_io_Step4_DiffEnable1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable);
	m_pIODB->IOPointWrite(m_io_Step4_RepeatAveCnt1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_DiffEnable2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable);
	m_pIODB->IOPointWrite(m_io_Step4_RepeatAveCnt2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
	m_pIODB->IOPointWrite(m_io_Step4_CorrType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect);
	m_pIODB->IOPointWrite(m_io_Step4_CorrVal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal);

	m_pIODB->IOPointWrite(m_io_DISP_DiffScale.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable);
	m_pIODB->IOPointWrite(m_io_DISP_DiffMax_Val.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val);
	m_pIODB->IOPointWrite(m_io_DISP_DiffMax_Exp.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp);

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

	m_io_DISP_DiffScale.UpdateDisplay();
	m_io_DISP_DiffMax_Val.UpdateDisplay();
	m_io_DISP_DiffMax_Exp.UpdateDisplay();

	SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgChkRcpWaveCfg::OnBnClickedBtnSave1()
{
	int  iBaseWave[4];
	char szWaveCfgName[64];
	CString	szStr('\0', 64);

	iBaseWave[0] = m_cmb_WaveA.GetCurSel();
	iBaseWave[1] = m_cmb_WaveB1.GetCurSel();
	iBaseWave[2] = m_cmb_WaveB2.GetCurSel();

	m_pIODB->IOPointRead(m_io_WaveCfgName.GetIOHdl(), szWaveCfgName);

	if (MessageBox(_T("Do you want Save Wave Config?"), _T("Wave Config File"), MB_YESNO) == IDNO)
		return;

	if (strchr(szWaveCfgName, ' '))
		*(strchr(szWaveCfgName, ' ')) = '\0';
	if (strlen(szWaveCfgName) == 0) {
		MessageBox(_T("Invalid Wave Config Name"), _T("Wave Config File"), MB_ICONERROR);
		return;
	}
	if (iBaseWave[0] == 0 && iBaseWave[1] == 0 && iBaseWave[2] == 0) {
		MessageBox(_T("Empty Base Wave Data No."), _T("Wave Config File"), MB_ICONERROR);
		return;
	}

	m_pIODB->IOPointRead(m_io_WaveCfgName.GetIOHdl(), m_pWaveCfg->m_pWaveCfg->szWaveCfgName);
	m_pIODB->IOPointRead(m_io_WaveCfgComment.GetIOHdl(), m_pWaveCfg->m_pWaveCfg->szWaveCfgComment);

	m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo = m_cmb_WaveA.GetCurSel();
	m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo = m_cmb_WaveB1.GetCurSel();
	m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo = m_cmb_WaveB2.GetCurSel();

	m_pIODB->IOPointRead(m_io_Wave_A_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt);
	m_pIODB->IOPointRead(m_io_A_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1);
	m_pIODB->IOPointRead(m_io_A_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2);
	m_pIODB->IOPointRead(m_io_A_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1);;
	m_pIODB->IOPointRead(m_io_A_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2);
	m_pIODB->IOPointRead(m_io_A_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal);
	m_pIODB->IOPointRead(m_io_Wave_B1_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt);
	m_pIODB->IOPointRead(m_io_B1_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1);
	m_pIODB->IOPointRead(m_io_B1_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2);
	m_pIODB->IOPointRead(m_io_B1_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1);
	m_pIODB->IOPointRead(m_io_B1_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2);
	m_pIODB->IOPointRead(m_io_B1_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal);
	m_pIODB->IOPointRead(m_io_Wave_B2_Cnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt);
	m_pIODB->IOPointRead(m_io_B2_OffGainType1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1);
	m_pIODB->IOPointRead(m_io_B2_OffGainType2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2);
	m_pIODB->IOPointRead(m_io_B2_OffGainVal1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1);
	m_pIODB->IOPointRead(m_io_B2_OffGainVal2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2);
	m_pIODB->IOPointRead(m_io_B2_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal);

	// Step2
	m_pIODB->IOPointRead(m_io_WaveSel1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave1);
	m_pIODB->IOPointRead(m_io_WaveSel2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2);
	m_pIODB->IOPointRead(m_io_WaveSel3.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3);
	m_pIODB->IOPointRead(m_io_WaveOperand1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType1);
	m_pIODB->IOPointRead(m_io_WaveOperand2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType2);
	m_pIODB->IOPointRead(m_io_Step2_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt);

	// Step3
	m_pIODB->IOPointRead(m_io_Step3_Reciprocal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iReciprocal);
	m_pIODB->IOPointRead(m_io_Step3_OffsetType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eOffsetType);
	m_pIODB->IOPointRead(m_io_Step3_OffsetVal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserOffset);
	m_pIODB->IOPointRead(m_io_Step3_MaxLvlType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType);
	m_pIODB->IOPointRead(m_io_Step3_MaxLevel.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel);
	m_pIODB->IOPointRead(m_io_Step3_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt);

	// Step4
	m_pIODB->IOPointRead(m_io_Step4_DiffEnable1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable);
	m_pIODB->IOPointRead(m_io_Step4_RepeatAveCnt1.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt);
	m_pIODB->IOPointRead(m_io_Step4_DiffEnable2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable);
	m_pIODB->IOPointRead(m_io_Step4_RepeatAveCnt2.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt);
	m_pIODB->IOPointRead(m_io_Step4_AveCnt.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
	m_pIODB->IOPointRead(m_io_Step4_CorrType.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect);
	m_pIODB->IOPointRead(m_io_Step4_CorrVal.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal);

	m_pIODB->IOPointRead(m_io_DISP_DiffScale.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable);
	m_pIODB->IOPointRead(m_io_DISP_DiffMax_Val.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val);
	m_pIODB->IOPointRead(m_io_DISP_DiffMax_Exp.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp);

	m_pWaveCfg->SaveWaveCfg(m_iPMNo,m_iWaveCfgNo);
}


void CDlgChkRcpWaveCfg::OnClose()
{
	CDialogX::OnClose();
}
void CDlgChkRcpWaveCfg::OnCancel()
{
	char szWaveCfgName[64];
	char szComment[64];
	int		iVal;
	double	dVal;


	int		bChange = FALSE;

	m_pIODB->IOPointRead(m_io_WaveCfgName.GetIOHdl(), szWaveCfgName);
	m_pIODB->IOPointRead(m_io_WaveCfgComment.GetIOHdl(), szComment);
	if (strncmp(szWaveCfgName, m_pWaveCfg->m_pWaveCfg->szWaveCfgName, 32))
		bChange = TRUE;
	else if (strncmp(szComment, m_pWaveCfg->m_pWaveCfg->szWaveCfgComment, 32))
		bChange = TRUE;


	if(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo != m_cmb_WaveA.GetCurSel())
		bChange = TRUE;
	else if(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo != m_cmb_WaveB1.GetCurSel())
		bChange = TRUE;
	else if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo = m_cmb_WaveB2.GetCurSel())
		bChange = TRUE;

	if (bChange == FALSE) {
		m_pIODB->IOPointRead(m_io_Wave_A_Cnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_A_OffGainType1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_A_OffGainType2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_A_OffGainVal1.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_A_OffGainVal2.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_A_Reciprocal.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Wave_B1_Cnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B1_OffGainType1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B1_OffGainType2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B1_OffGainVal1.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B1_OffGainVal2.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B1_Reciprocal.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Wave_B2_Cnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B2_OffGainType1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B2_OffGainType2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B2_OffGainVal1.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B2_OffGainVal2.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_B2_Reciprocal.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal)
			bChange = TRUE;

		// Step2
		m_pIODB->IOPointRead(m_io_WaveSel1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_WaveSel2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_WaveSel3.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_WaveOperand1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType1)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_WaveOperand2.GetIOHdl(), &iVal);
		if (iVal = m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType2)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step2_AveCnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt)
			bChange = TRUE;

		// Step3
		m_pIODB->IOPointRead(m_io_Step3_Reciprocal.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iReciprocal)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step3_OffsetType.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eOffsetType)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step3_OffsetVal.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserOffset)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step3_MaxLvlType.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step3_MaxLevel.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step3_AveCnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt)
			bChange = TRUE;

		// Step4
		m_pIODB->IOPointRead(m_io_Step4_DiffEnable1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step4_RepeatAveCnt1.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step4_DiffEnable2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable)
			bChange = TRUE;

		m_pIODB->IOPointRead(m_io_Step4_RepeatAveCnt2.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step4_AveCnt.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step4_CorrType.GetIOHdl(), &iVal);
		if (iVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_Step4_CorrVal.GetIOHdl(), &dVal);
		if (dVal != m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal)
			bChange = TRUE;

		m_pIODB->IOPointRead(m_io_DISP_DiffScale.GetIOHdl(), &iVal);
		if(iVal != m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_DISP_DiffMax_Val.GetIOHdl(), &dVal);
		if(dVal != m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val)
			bChange = TRUE;
		m_pIODB->IOPointRead(m_io_DISP_DiffMax_Exp.GetIOHdl(), &iVal);
		if(iVal != m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp)
			bChange = TRUE;
	}
	if (bChange) {
		if (MessageBox(_T("Some Parameter are changed.\n Do youwant to Save ?"), _T("Wave Config Edit"), MB_YESNO) == IDNO)
			CDialogX::OnCancel();
		OnBnClickedBtnSave1();
		return;
	}
	CDialogX::OnCancel();
}

void CDlgChkRcpWaveCfg::OnTimer(UINT_PTR nIDEvent)
{
	unsigned		unsVal;

	/*
	** Step2 A/B1/B2演算
	*/
	m_pIODB->IOPointRead(m_io_WaveOperand1.GetIOHdl(), &unsVal);
	if (unsVal == CALC_OPE_NONE) {
		if (GetDlgItem(IDC_ED_WAVE_SEL2)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_SEL2)->EnableWindow(FALSE);
	}
	else {
		if (GetDlgItem(IDC_ED_WAVE_SEL2)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_SEL2)->EnableWindow(TRUE);
	}

	m_pIODB->IOPointRead(m_io_WaveOperand2.GetIOHdl(), &unsVal);
	if (unsVal == CALC_OPE_NONE) {
		if (GetDlgItem(IDC_ED_WAVE_SEL3)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_SEL3)->EnableWindow(FALSE);
	}
	else {
		if (GetDlgItem(IDC_ED_WAVE_SEL3)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_SEL3)->EnableWindow(TRUE);
	}

	/*
	** Step3 逆数設定
	*/
	m_pIODB->IOPointRead(m_io_Step3_Reciprocal.GetIOHdl(), &unsVal);
	if (unsVal) {
		if (GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_TYPE)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_TYPE)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_TYPE)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_TYPE)->EnableWindow(TRUE);

		if (GetDlgItem(IDC_ED_WAVE_STEP3_AVECNT)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_STEP3_AVECNT)->EnableWindow(TRUE);

		m_pIODB->IOPointRead(m_io_Step3_OffsetType.GetIOHdl(), &unsVal);
		if (unsVal == BASE_AUTO_SET) {
			if (GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->EnableWindow(FALSE);
		}
		else {
			if (GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->EnableWindow(TRUE);
		}

		m_pIODB->IOPointRead(m_io_Step3_MaxLvlType.GetIOHdl(), &unsVal);
		if (unsVal == BASE_AUTO_SET) {
			if (GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->IsWindowEnabled() == TRUE)
				GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->EnableWindow(FALSE);
		}
		else {
			if (GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->IsWindowEnabled() == FALSE)
				GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->EnableWindow(TRUE);
		}
	}
	else {
		if (GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_TYPE)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_TYPE)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_TYPE)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_TYPE)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP3_OFFSET_VAL)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP3_MAXLVL_VAL)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_ED_WAVE_STEP3_AVECNT)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP3_AVECNT)->EnableWindow(FALSE);

	}
	/*
	** Step4 微分設定
	*/
	m_pIODB->IOPointRead(m_io_Step4_DiffEnable1.GetIOHdl(), &unsVal);
	if (unsVal) {
		if (GetDlgItem(IDC_ED_STEP4_DIFF1_RPTAVE)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_STEP4_DIFF1_RPTAVE)->EnableWindow(TRUE);
	}
	else {
		if (GetDlgItem(IDC_ED_STEP4_DIFF1_RPTAVE)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_STEP4_DIFF1_RPTAVE)->EnableWindow(FALSE);
	}
	m_pIODB->IOPointRead(m_io_Step4_DiffEnable2.GetIOHdl(), &unsVal);
	if (unsVal) {
		if (GetDlgItem(IDC_ED_STEP4_DIFF2_RPTAVE)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_STEP4_DIFF2_RPTAVE)->EnableWindow(TRUE);
	}
	else {
		if (GetDlgItem(IDC_ED_STEP4_DIFF2_RPTAVE)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_STEP4_DIFF2_RPTAVE)->EnableWindow(FALSE);
	}
	m_pIODB->IOPointRead(m_io_Step4_CorrType.GetIOHdl(), &unsVal);
	if (unsVal == CALC_OPE_NONE) {
		if (GetDlgItem(IDC_ED_WAVE_STEP4_CORR_VAL)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_WAVE_STEP4_CORR_VAL)->EnableWindow(FALSE);
	}
	else {
		if (GetDlgItem(IDC_ED_WAVE_STEP4_CORR_VAL)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_WAVE_STEP4_CORR_VAL)->EnableWindow(TRUE);

	}
	m_pIODB->IOPointRead(m_io_DISP_DiffScale.GetIOHdl(), &unsVal);
	if (unsVal) {
		if (GetDlgItem(IDC_ED_DISP_DIFF_MAX)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_DISP_DIFF_MAX)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_ED_DISP_DIFF_EXP)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_ED_DISP_DIFF_EXP)->EnableWindow(TRUE);
	}
	else {
		if (GetDlgItem(IDC_ED_DISP_DIFF_MAX)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_DISP_DIFF_MAX)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_ED_DISP_DIFF_EXP)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_ED_DISP_DIFF_EXP)->EnableWindow(FALSE);
	}

	CDialogX::OnTimer(nIDEvent);
}
