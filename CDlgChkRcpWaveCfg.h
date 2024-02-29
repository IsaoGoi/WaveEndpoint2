#pragma once


// CDlgChkRcpWaveCfg ダイアログ

class CDlgChkRcpWaveCfg : public CDialogX
{
	DECLARE_DYNAMIC(CDlgChkRcpWaveCfg)

public:
	CDlgChkRcpWaveCfg(CWnd* pParent = nullptr,int iPM=PM1, int iWaveCfgNo=1);   // 標準コンストラクター
	virtual ~CDlgChkRcpWaveCfg();

	CEPDConfig*		m_pConfig;
	CIODB*			m_pIODB;
	CWaveCfgCtrl*	m_pWaveCfgCtrl;
	CCalcWaveCfg*	m_pWaveCfg;
	int				m_iWaveCfgNo;
	int				m_iPMNo;
	
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHKRCPWAVECFG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	CIOEdit m_io_WaveCfgName;
	CIOEdit m_io_WaveCfgComment;

	CComboBox m_cmb_WaveA;
	CComboBox m_cmb_WaveB1;
	CComboBox m_cmb_WaveB2;

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

	afx_msg void OnBnClickedBtnSave1();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
