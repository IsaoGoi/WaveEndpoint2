#pragma once
#include "ToolTipGraph.h"


// CDlgEPDLog ダイアログ

class CDlgEPDLog : public CDialogX
{
	DECLARE_DYNAMIC(CDlgEPDLog)

public:
	CDlgEPDLog(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgEPDLog();

	void DisplayRecipe(int iStep);

	CImageList* m_pImage;
	CEPDConfig* m_pConfig;
	CEPDLog*	m_pEPDLog;
	HTREEITEM	m_hCurrSelItem;

	EPD_WAVEDATA2	m_tEPDData;
	RECIPE			m_tRecipe;
	SET_WAVECFG		m_tStepWaveCfg[RCP_STEP_MAX];
	int				m_iPMNo;
	CToolTipGraph* m_pToolTip;
	int				m_iToolTipDataNo;

	CRect			m_RectGraph;
	CRect			m_RectUpdate;

	POINT			m_RawGraphPoint[EPD_WAVE_DATA_MAX + 1];
	POINT			m_CalcGraphPoint[EPD_WAVE_DATA_MAX + 1];

	double			m_dTimeScale;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EPDLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }
	CTreeCtrl m_Tree_Log;
	afx_msg void OnTvnSelchangedTreeLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedChkRowdata();
	afx_msg void OnBnClickedBtnRcp1() { DisplayRecipe(0); }
	afx_msg void OnBnClickedBtnRcp2() { DisplayRecipe(1); }
	afx_msg void OnBnClickedBtnRcp3() { DisplayRecipe(2); }
	afx_msg void OnBnClickedBtnRcp4() { DisplayRecipe(3); }
	afx_msg void OnBnClickedBtnRcp5() { DisplayRecipe(4); }

	CVListCtrl m_list_log;
	afx_msg void OnBnClickedRadioPc1() { SelectPC(PM1); }
	afx_msg void OnBnClickedRadioPc2() { SelectPC(PM2); }
	afx_msg void OnBnClickedRadioPc3() { SelectPC(PM3); }
	afx_msg void OnBnClickedRadioPc4() { SelectPC(PM4); }
	afx_msg void OnBnClickedRadioPc5() { SelectPC(PM5); }
	void SelectPC(int iPCNo);


	afx_msg void OnLvnItemchangedListLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
