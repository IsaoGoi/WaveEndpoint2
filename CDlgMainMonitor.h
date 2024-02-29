#pragma once


// CDlgMainMonitor ダイアログ

class CDlgMainMonitor : public CDialogX
{
	DECLARE_DYNAMIC(CDlgMainMonitor)

public:
	CDlgMainMonitor(CWnd* pParent = nullptr,int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgMainMonitor();


	int				m_iPMNo;
	CIODB*			m_pIODB;
	CWaveCfgCtrl*	m_pWaveCfg;
	CEPDThread*		m_pEPDThread;
	CRect			m_pGraphRect;
	int				m_io_EPDStatus;
	unsigned		m_unsEPDStatus;


	int				m_iDrawCount;
	POINT			m_RawGraphPoint[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	POINT			m_CalcGraphPoint[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	POINT			m_2ndDiffGraphPoint[EPD_WAVE_DATA_MAX + 1];

	int				m_io_EPDMode;

//	CRect			m_OriginalRect;
	CRect			m_CurrRect;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_PC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel() { return; }
	virtual void OnOK()		{ return; }
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	CIOEdit m_io_RcpName;
	CIOEdit m_io_WaveCfg;
	CIOEdit m_io_RawData;
	CIOEdit m_io_CalcData;
	CIOEdit m_io_Set_DeadTime;
	CIOEdit m_io_DeadTime;
	CIOEdit m_io_Set_StartLevel;
	CIOEdit m_io_StartLevel;
	CIOEdit m_io_Set_JustLevel;
	CIOEdit m_io_JustLevel;
#if 0
	CIOEdit m_io_Set_JustDiff;
	CIOEdit m_io_JustDiff;
#endif
	CIOEdit m_io_PeakData;
	CIOEdit m_io_Set_EndLevel;
	CIOEdit m_io_EndLevel;
#if 0
	CIOEdit m_io_Set_EndDiffJust;
	CIOEdit m_io_EndDiffJust;
	CIOEdit m_io_Set_EndDiffPeak;
	CIOEdit m_io_EndDiffPeak;
#endif
	CIOEdit m_io_Set_OverEtchTime;
	CIOEdit m_io_OverEtchTime;
	CIOEdit m_io_TotalTime;
	CIOEdit m_io_Step;
	CIOEdit m_io_JustLimitTime;
	CIOEdit m_io_JustTime;
	CIOEdit m_io_EndLimitTime;
	CIOEdit m_io_EndTime;
	afx_msg void OnBnClickedBtnRcp();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedChkRawdata();
	CIOEdit m_io_InfoLotID;
	CIOEdit m_io_InfoCstID;
	CIOEdit m_io_InfoGlsID;
	CIOEdit m_io_InfoSlotNo;
	afx_msg void OnBnClickedChkRawdataSave();
	CButton m_Chk_Detail;
	afx_msg void OnBnClickedChkDetail();
	CIOEdit m_io_DiffData;
};
