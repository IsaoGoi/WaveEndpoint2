// CDlgSimulation.cpp : 実装ファイル
//

#include "pch.h"
#include "vector"
#include "string"
#include "WaveEndPoint.h"
#include "CDlgSimulation.h"
#include "CDlgAnalysis.h"
#include "CDlgWaveCfgList.h"
#include "Calculate.h"
#include <gdiplus.h>
#include <gdipluspixelformats.h>



std::vector<std::string>	g_tPC1_List2(1000);
std::vector<std::string>	g_tPC2_List2(1000);
std::vector<std::string>	g_tPC3_List2(1000);
std::vector<std::string>	g_tPC4_List2(1000);
std::vector<std::string>	g_tPC5_List2(1000);

std::vector<SAMPLE_WAVE>	g_tDisplayData2(6000);

extern void get_samplinglist(int iPM, std::vector<std::string>& pList);

static UINT				m_uidY_Display[5] = { IDC_ED_Y1_1,IDC_ED_Y1_2,IDC_ED_Y1_3,IDC_ED_Y1_4,IDC_ED_Y1_5 };
static UINT				m_uiRadioID[PM_MAX] = { IDC_RD_PC1 ,IDC_RD_PC2 ,IDC_RD_PC3 ,IDC_RD_PC4, IDC_RD_PC5 };


// CDlgSimulation ダイアログ
IMPLEMENT_DYNAMIC(CDlgSimulation, CDialogX)

CDlgSimulation::CDlgSimulation(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_SIMULATION, pParent)
	, m_szPCNo(_T(""))
{
	m_pImage = NULL;
	m_iSamplingTime = 0;
	m_bSelectFile = FALSE;
	m_pWaveCfg = NULL;
	m_iWaveCfgNo = 0;

	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_SIMULATION, pParent);

}

CDlgSimulation::~CDlgSimulation()
{
	if (m_pImage)
		delete m_pImage;
	if (m_pWaveCfg)
		delete m_pWaveCfg;


}

void CDlgSimulation::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_Tree_Sampling);
	DDX_Control(pDX, IDC_CMB_WAVE_A, m_cmb_WaveA);
	DDX_Control(pDX, IDC_CMB_WAVE_B1, m_cmb_WaveB1);
	DDX_Control(pDX, IDC_CMB_WAVE_B2, m_cmb_WaveB2);
	DDX_Control(pDX, IDC_ED_FILE1, m_io_WaveCfgName);
	DDX_Control(pDX, IDC_ED_COMMENT, m_io_WaveCfgComment);

	DDX_Text(pDX, IDC_ED_PCNO, m_szPCNo);
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


BEGIN_MESSAGE_MAP(CDlgSimulation, CDialogX)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CDlgSimulation::OnTvnSelchangedTreeFile)
	ON_BN_CLICKED(IDC_BTN_GRAPH1, &CDlgSimulation::OnBnClickedBtnGraph1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_LOAD1, &CDlgSimulation::OnBnClickedBtnLoad1)
	ON_BN_CLICKED(IDC_BTN_SAVE1, &CDlgSimulation::OnBnClickedBtnSave1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RD_PC1, &CDlgSimulation::OnBnClickedRdPc1)
	ON_BN_CLICKED(IDC_RD_PC2, &CDlgSimulation::OnBnClickedRdPc2)
	ON_BN_CLICKED(IDC_RD_PC3, &CDlgSimulation::OnBnClickedRdPc3)
	ON_BN_CLICKED(IDC_RD_PC4, &CDlgSimulation::OnBnClickedRdPc4)
	ON_BN_CLICKED(IDC_RD_PC5, &CDlgSimulation::OnBnClickedRdPc5)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSimulation メッセージ ハンドラー


BOOL CDlgSimulation::OnInitDialog()
{
	CRect		rc;
	int			i;
	CString	szStr('\0', 32);
	double		dVal;

	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;
	MoveWindow(rc);

	// Wave Graph Rect
	rc.DeflateRect(10, 10);
	rc.OffsetRect(160,340);
	GetDlgItem(IDC_RECT_GRAPH)->GetClientRect(m_Rect_Graph);
	m_Rect_Graph.OffsetRect(rc.left, rc.top);
	GetDlgItem(IDC_RECT_GRAPH)->MoveWindow(m_Rect_Graph);
	m_Rect_Graph.DeflateRect(2, 2);

	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	m_pWaveCfg = new CCalcWaveCfg();

	m_iPMNo = -1;
	for (i = 0; i < PM_MAX; i++) {
		if (m_pConfig->m_tEPDConfig.iPCEnable[i]) {
			GetDlgItem(m_uiRadioID[i])->EnableWindow(TRUE);
			if (m_iPMNo == -1) {
				m_iPMNo = i;
			}
		}
		else
			GetDlgItem(m_uiRadioID[i])->EnableWindow(FALSE);
	}/* for */
	if (m_iPMNo != -1) {
		((CButton *)GetDlgItem(m_uiRadioID[m_iPMNo]))->SetCheck(TRUE);
		m_szPCNo.Format("PC%d",(m_iPMNo+1));
	}


	m_io_WaveCfgName.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpName"));
	m_io_WaveCfgComment.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.ED.RcpComment"));

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

	SetPCWaveCfg(m_iPMNo);


	DisplaySamplingList();

	UpdateData(FALSE);
	SetTimer(1, 1000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgSimulation::SetPCWaveCfg(int iPM)
{
	CString	szStr('\0', 32);
	int		i;
	char	szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char	szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];
	int		iWaveBaseNo[4];
	char	szWaveCfgDate[32 + 1];

	m_cmb_WaveA.ResetContent();
	m_cmb_WaveB1.ResetContent();
	m_cmb_WaveB2.ResetContent();

	szStr = _T("0:No Select");
	m_cmb_WaveA.InsertString(0, szStr);
	m_cmb_WaveB1.InsertString(0, szStr);
	m_cmb_WaveB2.InsertString(0, szStr);


	for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
		szStr.Format("%d:%6.2fum", (i + 1), m_pConfig->m_tWaveConfig2[iPM].fWaveLength[i]);
		m_cmb_WaveA.InsertString((i + 1), szStr);
		m_cmb_WaveB1.InsertString((i + 1), szStr);
		m_cmb_WaveB2.InsertString((i + 1), szStr);
	}/* for */

	if (m_pWaveCfgCtrl->GetInfoFirst(m_iPMNo, &m_iWaveCfgNo, szWaveCfgName, szWaveCfgComment, &iWaveBaseNo[0], &iWaveBaseNo[1], &iWaveBaseNo[2], &iWaveBaseNo[3], szWaveCfgDate) == FALSE)
		m_iWaveCfgNo = 1;

	m_pWaveCfg->LoadWaveCfg(m_iPMNo, m_iWaveCfgNo);

	szStr.Format("%03.03d", m_iWaveCfgNo);
	GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

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


	m_io_WaveCfgName.UpdateDisplay();
	m_io_WaveCfgComment.UpdateDisplay();

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

}



void CDlgSimulation::DisplaySamplingList()
{
	int				iPM, i, iCnt;
	CString			szWork('\0',512);
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
			case PM1:	tList = g_tPC1_List2; break;
			case PM2:	tList = g_tPC2_List2; break;
			case PM3:	tList = g_tPC3_List2; break;
			case PM4:	tList = g_tPC4_List2; break;
			case PM5:	tList = g_tPC5_List2; break;
			}/* switch */

			tList.clear();
			get_samplinglist(iPM, tList);
			iCnt = (int)tList.size();

			switch (iPM) {
			case PM1:	g_tPC1_List2 = tList; break;
			case PM2:	g_tPC2_List2 = tList; break;
			case PM3:	g_tPC3_List2 = tList; break;
			case PM4:	g_tPC4_List2 = tList; break;
			case PM5:	g_tPC5_List2 = tList; break;
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


void CDlgSimulation::OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
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
	SAMPLE_WAVE		m_tWave;
	int				i;

	hTreeHdl = m_Tree_Sampling.GetSelectedItem();
	if (hTreeHdl) {
		index = m_Tree_Sampling.GetItemData(hTreeHdl);

		if (index < 10000)
			return;

		iPCNo = (int)(index / 10000);
		--iPCNo;

		iSelectNo = index % 10000;

		switch (iPCNo) {
		case PM1:	szFileName.Format("RowWaveLog\\%s", g_tPC1_List2[iSelectNo].c_str()); break;
		case PM2:	szFileName.Format("RowWaveLog\\%s", g_tPC2_List2[iSelectNo].c_str()); break;
		case PM3:	szFileName.Format("RowWaveLog\\%s", g_tPC3_List2[iSelectNo].c_str()); break;
		case PM4:	szFileName.Format("RowWaveLog\\%s", g_tPC4_List2[iSelectNo].c_str()); break;
		case PM5:	szFileName.Format("RowWaveLog\\%s", g_tPC5_List2[iSelectNo].c_str()); break;
		}

		g_tDisplayData2.clear();
		if (fp.Open(szFileName, CFile::modeRead)) {
			fp.Read(&m_tWaveHead, sizeof(WRITE_WAVEDATA));

			while (1) {
				if (fp.Read(&m_tWave, sizeof(SAMPLE_WAVE)) == 0)
					break;
				g_tDisplayData2.push_back(m_tWave);
			}/* while */
			fp.Close();
			m_bSelectFile = TRUE;
		}
	}
	m_iSamplingTime = m_tWaveHead.iSampling100msec;


	for (i = 0; i < 2; i++) {
		m_tGraph_Scale[i].dMin_X_Val = 0.0f;
		m_tGraph_Scale[i].dMax_X_Val = (double)g_tDisplayData2.size();
		m_tGraph_Scale[i].d_X_Scale = m_tGraph_Scale[i].dMax_X_Val - m_tGraph_Scale[i].dMin_X_Val;

		m_tGraph_Scale[i].dMin_Y_Val = 0;
		m_tGraph_Scale[i].dMax_Y_Val = 1000.00f;
		m_tGraph_Scale[i].d_Y_Scale = 65535.00f;
	}/* for */
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


	if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo == 0){
		return;
	}
	if (m_bSelectFile == FALSE) {
		return;
	}
	CalcuWaveData();
	*pResult = 0;
}


void CDlgSimulation::OnBnClickedBtnGraph1()
{
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

	if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo == 0 ){
		MessageBox("Not Select Base Wave Length!", "Simulation", MB_ICONERROR);
		return;
	}
	if (m_bSelectFile == FALSE) {
		MessageBox("Not Select Wave File!", "Simulation", MB_ICONERROR);
		return;
	}
	CalcuWaveData();
}
void CDlgSimulation::CalcuWaveData()
{
	int		i,j;
	int		iWaveCnt;
	double	dVal;
	CString	szStr;

	//逆数計算用 Min/Max
	m_dStep3Min = 0.0f;		// 初期値
	m_dStep3Max = 100.0f;		// 初期値

	for (i = 0; i < 6; i++) {
		m_dAveStep4_Total[i] = 0.0f;
		m_dAveStep4_Total_2[i] = 0.0f;
	}/* for */

#if 0
	for (i = 0; i < 2; i++) {
		m_tGraph_Scale[i].dMin_Y_Val = 0;
		m_tGraph_Scale[i].dMax_Y_Val = 1000.00f;
		if(i==0)
			m_tGraph_Scale[i].d_Y_Scale = 65535.00f;
		else
			m_tGraph_Scale[i].d_Y_Scale = 1.00f;
		m_tGraph_Scale[i].d_Y_Minimum = 0.0f;
	}/* for */
#endif
	dVal = m_tGraph_Scale[0].dMax_X_Val / 10.0f * (double)m_tWaveHead.iSampling100msec;
	szStr.Format("%5.1f", dVal);
	GetDlgItem(IDC_ED_TOTAL_TM)->SetWindowText(szStr);

	dVal /= 2.0f;
	szStr.Format("%5.1f", dVal);
	GetDlgItem(IDC_ED_HALF_TM)->SetWindowText(szStr);



	for (i = 0; i < (int)m_tGraph_Scale[0].dMax_X_Val; i++) {
		for (iWaveCnt = 0; iWaveCnt < 3; iWaveCnt++) {
			dVal = 0;

			/*
			** ============================
			** Step1 A/B1/B2 波形取り出し
			** ============================
			*/
			if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iWaveNo) {
				dVal = (double)g_tDisplayData2[i].usData[m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iWaveNo - 1];
				for (j = 0; j < m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iWaveCnt; j++) {
					dVal += (double)g_tDisplayData2[i].usData[m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iWaveNo + j];
				}/* for */
				dVal /= (double)(m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iWaveCnt + 1);

				// Gain / Offset
				if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].eGainOffset1 == CALC_GAIN)
					dVal *= m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].dGainOffset1;
				else if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].eGainOffset1 == CALC_OFFSET)
					dVal += m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].dGainOffset1;

				if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].eGainOffset2 == CALC_GAIN)
					dVal *= m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].dGainOffset2;
				else if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].eGainOffset2 == CALC_OFFSET)
					dVal += m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].dGainOffset2;

				// 逆数指定
				if (m_pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[iWaveCnt].iReciprocal)
					dVal = (double)1.000f / dVal;
			}

			if (iWaveCnt == 0)
				m_dData_A[i] = dVal;

			m_dCalcStep1[iWaveCnt][i] = dVal;
		}/* for */
		/*
		** 逆数計算用 Min/Max更新
		*/
		if (m_dStep3Min > m_dData_A[i])
			m_dStep3Min = m_dData_A[i];
		if (m_dStep3Max < m_dData_A[i])
			m_dStep3Max = m_dData_A[i];

		/*
	   ** ============================
	   ** Step2 A/B1/B2演算
	   ** ============================
	   */
		dVal = m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave1][i];

		if (m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType1 && m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2) {
			switch (m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType1) {
			case CALC_OPE_PULUS:	dVal += m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2 - 1][i]; break;
			case CALC_OPE_MINUS:	dVal -= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2 - 1][i]; break;
			case CALC_OPE_MULTIPULY:dVal *= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2 - 1][i]; break;
			case CALC_OPE_DIVISION: dVal /= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave2 - 1][i]; break;
			}/* switch */
		}
		if (m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType2 && m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3) {
			switch (m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eOperandType2) {
			case CALC_OPE_PULUS:	dVal += m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3 - 1][i]; break;
			case CALC_OPE_MINUS:	dVal -= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3 - 1][i]; break;
			case CALC_OPE_MULTIPULY:dVal *= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3 - 1][i]; break;
			case CALC_OPE_DIVISION: dVal /= m_dCalcStep1[m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.eWave3 - 1][i]; break;
			}/* switch */
		}
		m_dCalcStep2[i] = dVal;

		// 移動平均
		if (m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt) {
			if (i < m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt) {
				if (i == 0)
					m_dAveStep2_Total = m_dCalcStep2[i];
				else
					m_dAveStep2_Total += m_dCalcStep2[i];
				m_dAveStep2Data[i] = m_dAveStep2_Total / (double)(i + 1);
			}
			else {
				m_dAveStep2_Total -= m_dCalcStep2[i - m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt];
				m_dAveStep2_Total += m_dCalcStep2[i];
				m_dAveStep2Data[i] = m_dAveStep2_Total / (double)(m_pWaveCfg->m_pWaveCfg->tStep2_WaveJoin.iAveCnt);
			}
		}
		else {
			m_dAveStep2Data[i] = m_dCalcStep2[i];
		}
		if (i == 0) {
			m_tGraph_Scale[0].d_Y_Scale = m_dAveStep2Data[i] + 100;
			m_tGraph_Scale[0].d_Y_Minimum = 0.0f;
			m_tGraph_Scale[0].dMin_Y_Val = 0.0f;
			m_tGraph_Scale[0].dMax_Y_Val = m_dAveStep2Data[i];

		}
		else {
			if (m_tGraph_Scale[0].d_Y_Scale <= m_dAveStep2Data[i])
				m_tGraph_Scale[0].d_Y_Scale = m_dAveStep2Data[i] + 100;

			if (m_tGraph_Scale[0].d_Y_Minimum > m_dAveStep2Data[i])
				m_tGraph_Scale[0].d_Y_Minimum = m_dAveStep2Data[i];

			if (m_tGraph_Scale[0].dMin_Y_Val > m_dAveStep2Data[i])
				m_tGraph_Scale[0].dMin_Y_Val = m_dAveStep2Data[i];
			if(m_tGraph_Scale[0].dMax_Y_Val <= m_dAveStep2Data[i])
				m_tGraph_Scale[0].dMax_Y_Val = m_dAveStep2Data[i];
		}


		/*
		** ============================
		** Step3 逆数設定
		** ============================
		*/
		if (m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iReciprocal) {
			if (m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eOffsetType == BASE_AUTO_SET)
				m_dStep3Scale_Min = m_dStep3Min;
			else
				m_dStep3Scale_Min = m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserOffset;

			if (m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType == BASE_AUTO_SET)
				m_dStep3Scale_Max = m_dStep3Max;
			else
				m_dStep3Scale_Max = m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel;

			m_dCalcStep3[i] = 1.000f / (m_dAveStep2Data[i] - m_dStep3Scale_Min) / (m_dStep3Scale_Max - m_dStep3Scale_Min);

			// 移動平均
			if (m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt) {
				if (i < m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt) {
					if (i == 0)
						m_dAveStep3_Total = m_dCalcStep3[i];
					else
						m_dAveStep3_Total += m_dCalcStep3[i];
					m_dAveStep3Data[i] = m_dAveStep3_Total / (double)(i + 1);
				}
				else {
					m_dAveStep3_Total -= m_dCalcStep3[i - m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt];
					m_dAveStep3_Total += m_dCalcStep3[i];
					m_dAveStep3Data[i] = m_dAveStep3_Total / (double)(m_pWaveCfg->m_pWaveCfg->tStep3_WaveNorm.iAveCnt);
				}
			}
			else {
				m_dAveStep3Data[i] = m_dCalcStep3[i];
			}
		}
		else {
			m_dAveStep3Data[i] = m_dAveStep2Data[i];
		}
		/*
		** ============================
		** Step4 微分演算
		** ============================
		*/
		// 一階微分
		m_dCalcStep4_Result[i] = m_dAveStep3Data[i];
		m_dCalcStep4[i] = m_dAveStep3Data[i];
		m_dCalcStep4_2[i] = m_dAveStep3Data[i];

		if (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable) {
			// 最小2乗法
			if (i < m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt * 2)
				m_dCalcStep4[i] = 0.0f;
			else
				m_dCalcStep4[i] = calcLeastSquaresMethod(i, m_dAveStep3Data);

			m_dCalcStep4_Result[i] = m_dCalcStep4[i];
			/*
			** 移動平均1
			*/
			if (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt) {
				if (i < m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt) {
					m_dAveStep4_Total[0] += m_dCalcStep4[i];
					m_dAveStep4Data[0][i] = m_dAveStep4_Total[0] / (double)(i + 1);

					for (j = 1; j <= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt; j++) {
						m_dAveStep4_Total[j] += m_dAveStep4Data[j - 1][i];
						m_dAveStep4Data[j][i] = m_dAveStep4_Total[j] / (double)(i + 1);
					}/* for */
				}
				else {
					m_dAveStep4_Total[0] -= m_dCalcStep4[i - m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt];
					m_dAveStep4_Total[0] += m_dCalcStep4[i];
					m_dAveStep4Data[0][i] = m_dAveStep4_Total[0] / (double)(m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);

					// Average Repeat
					for (j = 1; j <= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt; j++) {
						m_dAveStep4_Total[j] -= m_dAveStep4Data[j - 1][i - m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt];
						m_dAveStep4_Total[j] += m_dAveStep4Data[j - 1][i];
						m_dAveStep4Data[j][i] = m_dAveStep4_Total[j] / (double)(m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
					}/* for */
				}
				m_dCalcStep4_2[i] = m_dAveStep4Data[m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt][i];
				m_dCalcStep4_Result[i] = m_dCalcStep4_2[i];
			}
		}

		// 二階微分
		if (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable) {
			// 最小2乗法
			if (i < m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt * 2)
				m_dCalcStep4_2[i] = 0.0f;
			else
				m_dCalcStep4_2[i] = calcLeastSquaresMethod(i, m_dCalcStep4);

			m_dCalcStep4_Result[i] = m_dCalcStep4_2[i];

			/*
			** 移動平均1
			*/
			if (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt) {
				if (i < m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt) {
					m_dAveStep4_Total_2[0] += m_dCalcStep4_2[i];
					m_dAveStep4Data_2[0][i] = m_dAveStep4_Total_2[0] / (double)(i + 1);

					for (j = 1; j <= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt; j++) {
						m_dAveStep4_Total_2[j] += m_dAveStep4Data_2[j - 1][i];
						m_dAveStep4Data_2[j][i] = m_dAveStep4_Total_2[j] / (double)(i + 1);
					}/* for */
				}
				else {
					m_dAveStep4_Total_2[0] -= m_dCalcStep4_2[i - m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt];
					m_dAveStep4_Total_2[0] += m_dCalcStep4_2[i];
					m_dAveStep4Data_2[0][i] = m_dAveStep4_Total_2[0] / (double)(m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);

					// Average Repeat
					for (j = 1; j <= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt; j++) {
						m_dAveStep4_Total_2[j] -= m_dAveStep4Data_2[j - 1][i - m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt];
						m_dAveStep4_Total_2[j] += m_dAveStep4Data_2[j - 1][i];
						m_dAveStep4Data_2[j][i] = m_dAveStep4_Total_2[j] / (double)(m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
					}/* for */
				}
				m_dCalcStep4_Result[i] = m_dAveStep4Data_2[m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt][i];
			}
		}
		if (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect != CALC_OPE_NONE) {
			switch (m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect) {
			case CALC_OPE_PULUS:		m_dCalcStep4_Result[i] += m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal; break;
			case CALC_OPE_MINUS:		m_dCalcStep4_Result[i] -= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal; break;
			case CALC_OPE_MULTIPULY:	m_dCalcStep4_Result[i] *= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal; break;
			case CALC_OPE_DIVISION:		m_dCalcStep4_Result[i] /= m_pWaveCfg->m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal; break;
			}/* switch */
		}
		if (i == 0) {
			m_tGraph_Scale[1].d_Y_Scale = m_dCalcStep4_Result[i];
			m_tGraph_Scale[1].d_Y_Minimum = m_dCalcStep4_Result[i];
		}
		else{
			if (m_tGraph_Scale[1].d_Y_Scale <= m_dCalcStep4_Result[i])
				m_tGraph_Scale[1].d_Y_Scale = m_dCalcStep4_Result[i];

			if (m_tGraph_Scale[1].d_Y_Minimum > m_dCalcStep4_Result[i])
				m_tGraph_Scale[1].d_Y_Minimum = m_dCalcStep4_Result[i];
		}

	}


	for (i = 0; i < 5; i++) {
		dVal = m_tGraph_Scale[0].dMin_Y_Val + ((m_tGraph_Scale[0].dMax_Y_Val - m_tGraph_Scale[0].dMin_Y_Val) / 4 * i);
		szStr.Format("%7.2f", dVal);
		GetDlgItem(m_uidY_Display[i])->SetWindowText(szStr);
	}

	/*
	** 微分表示Scaleセット
	*/
	if (m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable) {
		m_tGraph_Scale[1].d_Y_Scale = m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val * pow(10.0f, (double)m_pWaveCfg->m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp);
		m_tGraph_Scale[1].d_Y_Minimum = m_tGraph_Scale[1].d_Y_Scale * -1.0f;
	}
	else {
		if (fabs(m_tGraph_Scale[1].d_Y_Scale) < fabs(m_tGraph_Scale[1].d_Y_Minimum)) {
			m_tGraph_Scale[1].d_Y_Scale = fabs(m_tGraph_Scale[1].d_Y_Minimum);
		}
		else {
			m_tGraph_Scale[1].d_Y_Minimum = m_tGraph_Scale[1].d_Y_Scale * -1.0f;
		}
	}
	szStr.Format("%e", m_tGraph_Scale[1].d_Y_Scale);
	GetDlgItem(IDC_ED_DIFF_MAX2)->SetWindowTextA(szStr);
	szStr.Format("%e", m_tGraph_Scale[1].d_Y_Minimum);
	GetDlgItem(IDC_ED_DIFF_MIN)->SetWindowTextA(szStr);
	szStr = _T("0.000");
	GetDlgItem(IDC_ED_DIFF_0)->SetWindowTextA(szStr);

	InvalidateRect(m_Rect_Graph);
}


void CDlgSimulation::OnPaint()
{
	int		i;
	POINT	LinePoint[2];
	int		iWidth;
	int		iHeight;
	double	dWidth;
	double	dHeight;
	double	dVal;
	/* Pen ... 点線/緑 */
	CPen	DotPen;
	CPen	GraphPen;
	CPen	GraphPen2;
	CString	szStr;

	CPaintDC dc(this); // device context for painting


	DotPen.CreatePen(PS_DOT, 0, RGB(255, 255, 255));
	GraphPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	GraphPen2.CreatePen(PS_SOLID, 2, RGB(64,255,64));

	dc.FillSolidRect(m_Rect_Graph, RGB(0, 0, 0));
	iHeight = m_Rect_Graph.Height();
	iWidth = m_Rect_Graph.Width();


	LinePoint[0].x = m_Rect_Graph.left;
	LinePoint[1].x = m_Rect_Graph.right;

	CPen* oldPen = dc.SelectObject(&DotPen);

	for (i = 1; i < 4; i++) {
		LinePoint[0].y = m_Rect_Graph.bottom - (iHeight / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);

	}

	LinePoint[0].y = m_Rect_Graph.top;
	LinePoint[1].y = m_Rect_Graph.bottom;

	for (i = 1; i < 10; i++) {
		LinePoint[0].x = m_Rect_Graph.left + (iWidth / 10 * i);
		LinePoint[1].x = LinePoint[0].x;
		dc.Polyline(&LinePoint[0], 2);

	}


	// Wave 表示
	dWidth = (double)iWidth;
	dWidth /= m_tGraph_Scale[0].d_X_Scale;

	dHeight = (double)iHeight;

	dc.SelectObject(&GraphPen);

	for (i = 0; i < (int)m_tGraph_Scale[0].d_X_Scale; i++) {
		m_pGraph[i].x = m_Rect_Graph.left + (long)((double)(i  * dWidth));
		dVal = (double)m_dAveStep2Data[i];

		dVal -= m_tGraph_Scale[0].d_Y_Minimum;
		dVal /= (m_tGraph_Scale[0].d_Y_Scale- m_tGraph_Scale[0].d_Y_Minimum);
		dVal *= dHeight;
		m_pGraph[i].y = m_Rect_Graph.bottom - (long)(dVal);


		// 微分
		m_pGraphDiff[i].x = m_Rect_Graph.left + (long)((double)(i * dWidth));
		dVal = (double)m_dCalcStep4_Result[i];
		if (dVal > m_tGraph_Scale[1].d_Y_Scale)
			dVal = m_tGraph_Scale[1].d_Y_Scale;
		else if (dVal < m_tGraph_Scale[1].d_Y_Minimum)
			dVal = m_tGraph_Scale[1].d_Y_Minimum;
			
		dVal = (dHeight/ 2.0f) * (dVal / (m_tGraph_Scale[1].d_Y_Scale))+ (dHeight / 2.0f);
		m_pGraphDiff[i].y = m_Rect_Graph.bottom - (long)(dVal);
	}/* for */

	dc.Polyline(m_pGraph, (int)m_tGraph_Scale[0].d_X_Scale);

	dc.SelectObject(&GraphPen2);
	dc.Polyline(m_pGraphDiff, (int)m_tGraph_Scale[1].d_X_Scale);

	dc.SelectObject(oldPen);

	DotPen.DeleteObject();
	GraphPen.DeleteObject();
	GraphPen2.DeleteObject();
}

static char	m_szFilter[80] = {"Wave Set File (*.set)|*.set"};

void CDlgSimulation::OnBnClickedBtnLoad1()
{
	CString	szStr('\0', 64);

	CDlgWaveCfgList* pDlg;
	
	pDlg = new CDlgWaveCfgList(this, m_iPMNo, FALSE, m_iWaveCfgNo);

	if (pDlg->DoModal() == IDOK) {
		m_iWaveCfgNo = pDlg->m_iWaveCfgNo;
		m_pWaveCfg->LoadWaveCfg(m_iPMNo,m_iWaveCfgNo);

		szStr.Format("%03.03d", m_iWaveCfgNo);
		GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

		m_pIODB->IOPointWrite(m_io_WaveCfgName.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_WaveCfgComment.GetIOHdl(), &m_pWaveCfg->m_pWaveCfg->szWaveCfgComment);


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


		m_io_WaveCfgName.UpdateDisplay();
		m_io_WaveCfgComment.UpdateDisplay();

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

	}
	delete pDlg;
}


void CDlgSimulation::OnBnClickedBtnSave1()
{
	int  iBaseWave[4];
	char szWaveCfgName[64];
	CString	szStr('\0', 64);

	CDlgWaveCfgList* pDlg;

	iBaseWave[0] = m_cmb_WaveA.GetCurSel();
	iBaseWave[1] = m_cmb_WaveB1.GetCurSel();
	iBaseWave[2] = m_cmb_WaveB2.GetCurSel();


	m_pIODB->IOPointRead(m_io_WaveCfgName.GetIOHdl(), szWaveCfgName);

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

	pDlg = new CDlgWaveCfgList(this, m_iPMNo,TRUE, m_iWaveCfgNo);
	m_pIODB->IOPointRead(m_io_WaveCfgName.GetIOHdl(), pDlg->m_szWaveCfgName);
	m_pIODB->IOPointRead(m_io_WaveCfgComment.GetIOHdl(), pDlg->m_szWaveCfgComment);

	if (pDlg->DoModal() == IDOK) {
		m_iWaveCfgNo = pDlg->m_iWaveCfgNo;
		m_pIODB->IOPointWrite(m_io_WaveCfgName.GetIOHdl(), pDlg->m_szWaveCfgName);
		m_pIODB->IOPointWrite(m_io_WaveCfgComment.GetIOHdl(), pDlg->m_szWaveCfgComment);
		m_io_WaveCfgName.UpdateDisplay();
		m_io_WaveCfgComment.UpdateDisplay();

		szStr.Format("%03.03d", m_iWaveCfgNo);
		GetDlgItem(IDC_ED_NO)->SetWindowTextA(szStr);

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
	
	delete pDlg;
}


void CDlgSimulation::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	m_Tree_Sampling.SendMessage(WM_CLOSE);
	CDialogX::OnClose();
}


void CDlgSimulation::OnBnClickedRdPc(int iPM)
{
	if (m_iPMNo == iPM)
		return;

	m_iPMNo = iPM;
	m_szPCNo.Format("PC%d", (iPM + 1));

	m_iWaveCfgNo = 1;
	SetPCWaveCfg(iPM);


	UpdateData(FALSE);
}



void CDlgSimulation::OnTimer(UINT_PTR nIDEvent)
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
		else{
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
