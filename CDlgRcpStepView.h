#pragma once


// CDlgRcpStepView ダイアログ

class CDlgRcpStepView : public CDialogX
{
	DECLARE_DYNAMIC(CDlgRcpStepView)

public:
	CDlgRcpStepView(CWnd* pParent = nullptr, int iPM=PM1, RECIPE *pRecipe=NULL, SET_WAVECFG * pWaveCfg = NULL,int iStep=0);   // 標準コンストラクター
	virtual ~CDlgRcpStepView();

	int		m_iPMNo;
	char	m_szRecipeName[MAX_PATH+1];
	int		m_iRecipeNo;
	int		m_iStepNo;
	CIODB* m_pIODB;
	CPCRcpCtrl* m_pRcpCtrl;
	CEPDConfig* m_pConfig;
	RECIPE*			m_pRecipe;
	SET_WAVECFG*	m_pWaveCfg;



// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RCP_STEPVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() { return; }
public:
	virtual BOOL OnInitDialog();
	CIOEdit m_io_PC;
	CIOEdit m_io_RcpNo;
	CIOEdit m_io_RcpName;
	CIOEdit m_io_StepNo;
	CIOEdit m_io_DeadTimeVew;
	CIOEdit m_io_StartLevelSW;
	CIOEdit m_io_StartLevelView;
	CIOEdit m_io_JustLevelSW;
	CIOEdit m_io_JustLevelView;
	CIOEdit m_io_JustDiffSW;
	CIOEdit m_io_JustDiffView;
	CIOEdit m_io_JustLimitSW;
	CIOEdit m_io_JustLimitView;
	CIOEdit m_io_EndLevelSW;
	CIOEdit m_io_EndLevelView;
	CIOEdit m_io_EndDiff1SW;
	CIOEdit m_io_EndDiff1View;
	CIOEdit m_io_EndDiff2SW;
	CIOEdit m_io_EndDiff2View;
	CIOEdit m_io_EndFlatSW;
	CIOEdit m_io_EndFlatLevel;
	CIOEdit m_io_EndFlatTime;
	CIOEdit m_io_EndLimitSW;
	CIOEdit m_io_EndLimitView;
	CIOEdit m_io_OverEtchTimeSW;
	CIOEdit m_io_OverEtchTimeView;
	CIOEdit m_io_OverEtchRateSW;
	CIOEdit m_io_OverEtchRateView;

	CIOEdit m_io_WaveCfgNo;
	CIOEdit m_io_WaveCfgName;
	CIOEdit m_io_WaveA;
	CIOEdit m_io_WaveB1;
	CIOEdit m_io_WaveB2;

	CIOEdit m_io_Wave_A_Cnt;
	CIOEdit m_io_A_OffGainType1;
	CIOEdit m_io_A_OffGainType2;
	CIOEdit m_io_A_OffGainVal1;
	CIOEdit m_io_A_OffGainVal2;
	CIOEdit m_io_A_Reciprocal;
	CIOEdit m_io_Wave_B1_Cnt;
	CIOEdit m_io_B1_OffGainType1;
	CIOEdit m_io_B1_OffGainType2;
	CIOEdit m_io_B1_OffGainVal1;
	CIOEdit m_io_B1_OffGainVal2;
	CIOEdit m_io_B1_Reciprocal;
	CIOEdit m_io_Wave_B2_Cnt;
	CIOEdit m_io_B2_OffGainType1;
	CIOEdit m_io_B2_OffGainType2;
	CIOEdit m_io_B2_OffGainVal1;
	CIOEdit m_io_B2_OffGainVal2;
	CIOEdit m_io_B2_Reciprocal;

	CIOEdit m_io_WaveSel1;
	CIOEdit m_io_WaveSel2;
	CIOEdit m_io_WaveSel3;
	CIOEdit m_io_WaveOperand1;
	CIOEdit m_io_WaveOperand2;
	CIOEdit m_io_Step2_AveCnt;
	CIOEdit m_io_Step3_Reciprocal;
	CIOEdit m_io_Step3_OffsetType;
	CIOEdit m_io_Step3_OffsetVal;
	CIOEdit m_io_Step3_MaxLvlType;
	CIOEdit m_io_Step3_MaxLevel;
	CIOEdit m_io_Step3_AveCnt;
	CIOEdit m_io_Step4_DiffEnable1;
	CIOEdit m_io_Step4_RepeatAveCnt1;
	CIOEdit m_io_Step4_DiffEnable2;
	CIOEdit m_io_Step4_RepeatAveCnt2;
	CIOEdit m_io_Step4_AveCnt;
	CIOEdit m_io_Step4_CorrType;
	CIOEdit m_io_Step4_CorrVal;



	CIOEdit m_io_IntegTime;
};
