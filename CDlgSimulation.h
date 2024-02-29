#pragma once



// CDlgSimulation ダイアログ

typedef struct {
	double	dMin_X_Val;
	double	dMax_X_Val;
	double	d_X_Scale;

	double	dMin_Y_Val;
	double	dMax_Y_Val;
	double	d_Y_Scale;
	double	d_Y_Minimum;
}GRAPH_MAX;


class CDlgSimulation : public CDialogX
{
	DECLARE_DYNAMIC(CDlgSimulation)

public:
	CDlgSimulation(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgSimulation();

	void	DisplaySamplingList();
	void   CalcuWaveData();

	CImageList* m_pImage;
	CEPDConfig* m_pConfig;
	CIODB*		m_pIODB;
	CWaveCfgCtrl* m_pWaveCfgCtrl;
	CCalcWaveCfg* m_pWaveCfg;
	int			m_iWaveCfgNo;

	CRect		m_Rect_Graph;
	POINT		m_pGraph[18000 + 10];
	POINT		m_pGraphDiff[18000 + 10];
	int			m_iSamplingTime;

	int			m_bSelectFile;
	double		m_dData_A[18000];
	double		m_dCalcStep1[3][18000];
	double		m_dCalcStep2[18000];

	double		m_dAveStep2_Total;
	double		m_dAveStep2Data[18000];

	double		m_dStep3Min;
	double		m_dStep3Max;
	double		m_dStep3Scale_Min;
	double		m_dStep3Scale_Max;
	double		m_dCalcStep3[18000];

	double		m_dAveStep3_Total;
	double		m_dAveStep3Data[18000];

	double		m_dCalcStep4[18000];
	double		m_dCalcStep4_2[18000];
	double		m_dCalcStep4_Result[18000];

	double		m_dAveStep4_Total[6];
	double		m_dAveStep4Data[6][18000];

	double		m_dAveStep4_Total_2[6];
	double		m_dAveStep4Data_2[6][18000];



	WRITE_WAVEDATA	m_tWaveHead;

	GRAPH_MAX	m_tGraph_Scale[2];

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }
	CTreeCtrl m_Tree_Sampling;
	CComboBox m_cmb_WaveA;
	CComboBox m_cmb_WaveB1;
	CComboBox m_cmb_WaveB2;
	afx_msg void OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnGraph1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnLoad1();
	afx_msg void OnBnClickedBtnSave1();
	CIOEdit m_io_WaveCfgName;
	CIOEdit m_io_WaveCfgComment;
	afx_msg void OnClose();

	CString m_szPCNo;
	afx_msg void OnBnClickedRdPc1() { OnBnClickedRdPc(PM1); }
	afx_msg void OnBnClickedRdPc2() { OnBnClickedRdPc(PM2); }
	afx_msg void OnBnClickedRdPc3() { OnBnClickedRdPc(PM3); }
	afx_msg void OnBnClickedRdPc4() { OnBnClickedRdPc(PM4); }
	afx_msg void OnBnClickedRdPc5() { OnBnClickedRdPc(PM5); }
	void OnBnClickedRdPc(int iPM);

	int		m_iPMNo;
	void SetPCWaveCfg(int iPM);
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
	CIOEdit m_io_DISP_DiffScale;
	CIOEdit m_io_DISP_DiffMax_Val;
	CIOEdit m_io_DISP_DiffMax_Exp;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
