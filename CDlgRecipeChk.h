#pragma once

#include "CDlgWaveRef.h"
#include "ToolTipGraph.h"


// CDlgRecipeChk ダイアログ

class CDlgRecipeChk : public CDialogX
{
	DECLARE_DYNAMIC(CDlgRecipeChk)

public:
	CDlgRecipeChk(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgRecipeChk();

	void	DisplaySamplingList();
	void	OnBnClickedBtnRcp(int iPM);


	// ダイアログ データ
	CImageList* m_pImage;
	CEPDConfig* m_pConfig;
	CIODB* m_pIODB;
	CWaveCfgCtrl* m_pWaveCfg;
	CEPDSimulation* m_pEPDSimulation;
	CDlgWaveRef*	m_pDlgWaveRef;
	CToolTipGraph* m_pToolTip;

	CRect			m_pGraphRect;
	int				m_io_EPDStatus;
	unsigned		m_unsEPDStatus;
	int				m_iSamplingTime;

	int				m_iDrawCount;
	POINT			m_RawGraphPoint[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	POINT			m_CalcGraphPoint[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	int				m_GraphPoint_All_x[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	int				m_GraphPoint_All_y[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	POINT			m_2ndDiffGraphPoint[EPD_WAVE_DATA_MAX + 1];

	int				m_iSelectPM;
	int				m_iSelectRcpNo;
	int				m_iSelectGraph;
	int				m_bRectSelect;
	CRect			m_SelRect;
	CPoint			m_SelStartPoint;

	int				m_bGetDataStart;
	double			m_dSelStartLevel;
	double			m_dSelStartTime;
	double			m_dSelEndLevel;
	double			m_dSelEndTime;
	double			m_dSelPeakLevel;
	double			m_dSelPeakTime;
	double			m_dSelMinLevel;
	double			m_dSelMinTime;
	double			m_dSelDiffLevel;

	int				m_io_RefStartTime;
	int				m_io_RefStartLevel;
	int				m_io_RefEndTime;
	int				m_io_RefEndLevel;
	int				m_io_RefPeakTime;
	int				m_io_RefPeakLevel;
	int				m_io_RefMinTime;
	int				m_io_RefMinLevel;
	int				m_io_RefDiffLevel;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECIPE_CHK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree_Sampling;
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
	CIOEdit m_io_PeakData;
	CIOEdit m_io_Set_EndLevel;
	CIOEdit m_io_EndLevel;
	CIOEdit m_io_Set_OverEtchTime;
	CIOEdit m_io_OverEtchTime;
	CIOEdit m_io_TotalTime;
	CIOEdit m_io_Step;
	CIOEdit m_io_JustLimitTime;
	CIOEdit m_io_JustTime;
	CIOEdit m_io_EndLimitTime;
	CIOEdit m_io_EndTime;


	virtual BOOL OnInitDialog();
	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }
	afx_msg void OnBnClickedBtnRcp1() { OnBnClickedBtnRcp(PM1); }
	afx_msg void OnBnClickedBtnRcp2() { OnBnClickedBtnRcp(PM2); }
	afx_msg void OnBnClickedBtnRcp3() { OnBnClickedBtnRcp(PM3); }
	afx_msg void OnBnClickedBtnRcp4() { OnBnClickedBtnRcp(PM4); }
	afx_msg void OnBnClickedBtnRcp5() { OnBnClickedBtnRcp(PM5); }
	afx_msg void OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedChkRawdata();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnrcpedt();
	afx_msg void OnBnClickedBtnDrawwave();
	afx_msg void OnBnClickedBtnGraphsel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	afx_msg void OnBnClickedBtnAutorcpchk();
	afx_msg void OnBnClickedBtnAutorcpstop();

	CIOEdit m_io_Differential;

	int		m_iStep;
};
