#pragma once

#include "VListct.h"


// CDlgChkRcpEdit ダイアログ

class CDlgChkRcpEdit : public CDialogX
{
	DECLARE_DYNAMIC(CDlgChkRcpEdit)

	CEPDConfig* m_pConfig;
	CIODB* m_pIODB;
	CRecipe* m_pRecipe;
	CPCRcpCtrl* m_pPCRcpCtrl;
	CWaveCfgCtrl* m_pWaveCfgCtrl;
	HTREEITEM		m_hCurrSelItem;
	int			m_iWaveCfgCnt;
	int			m_iCfgTable[WAVECFG_FILE_MAX + 1];
	int			m_iCurrStepNo;

	int			m_iPMNo;
	int			m_iRecipeNo;


public:
	CDlgChkRcpEdit(CWnd* pParent = nullptr,int iPM = PM1,int iRcpNo = 1);   // 標準コンストラクター
	virtual ~CDlgChkRcpEdit();

	void	SetEditFields();
	void	ShowStepList();
	void	ShowEditStep(int iStep);
	void	ModifyCheck();
// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHKRCPEDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	CVListCtrl m_List_Step;
	CIOEdit m_io_RcpName;
	CIOEdit m_io_RcpComment;
	CIOEdit m_io_StepComment;
	CIOEdit m_io_DeadTime;
	CIOEdit m_io_StartLevel;
	CIOEdit m_io_StartLimitTime;
	CIOEdit m_io_AutoGain;
	CComboBox m_cmb_WaveCfg;
	BOOL m_bStartLevel;
	BOOL m_bAutoGain;
	CButton m_btn_Start_Over;
	CButton m_btn_Start_Under;
	BOOL m_bStartLimit;
	BOOL m_bEndLevel;
	BOOL m_bEndDiffJust;
	BOOL m_bEndDiffPeak;
	BOOL m_bEndFlat;
	BOOL m_bEndLimit;
	CButton m_btn_EneLevelOver;
	CButton m_btn_EneLevelUnder;
	BOOL m_bJustLevel;
	CButton m_btn_Just_Over;
	CButton m_btn_Just_Under;
	BOOL m_bJustDiff;
	BOOL m_bJustLimit;
	BOOL m_bOverEtchTime;
	BOOL m_bOverEtchRate;
	CIOEdit m_io_JustLevel;
	CIOEdit m_io_JustDiff;
	CIOEdit m_io_JustLimitTime;
	CIOEdit m_io_JustPeakLevel;

	CIOEdit m_io_EndLevel;
	CIOEdit m_io_EndDiffJust;
	CIOEdit m_io_EndDiffPeak;
	CIOEdit m_io_EndFlatLevel;
	CIOEdit m_io_EndFlatTime;
	CIOEdit m_io_EndLimitTime;
	CIOEdit m_io_OverEtchTime;
	CIOEdit m_io_OverEtchRate;
	int m_iStepNo;
	afx_msg void OnLvnItemchangedListStep(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnAddStep();
	afx_msg void OnBnClickedBtnDelStep();
	afx_msg void OnBnClickedBtnInsertStep();
	afx_msg void OnBnClickedChkStartLevel();
	afx_msg void OnBnClickedChkStartLimit();
	afx_msg void OnBnClickedChkAutoGain();
	afx_msg void OnBnClickedChkJustLevel();
	afx_msg void OnBnClickedChkJustDiff();
	afx_msg void OnBnClickedChkJustLimit();
	afx_msg void OnBnClickedChkEndLevel();
	afx_msg void OnBnClickedChkEndDiff();
	afx_msg void OnBnClickedChkEndDiff2();
	afx_msg void OnBnClickedChkEndFlat();
	afx_msg void OnBnClickedChkEndLimit();
	afx_msg void OnBnClickedChkOvTime();
	afx_msg void OnBnClickedChkOvRate();
	afx_msg void OnBnClickedBtnSave();
	BOOL m_bJustPeak;
	CButton m_btn_Just_Peak_H;
	CButton m_btn_Just_Peak_L;
	CIOEdit m_io_IntegrationTime;
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedBtnWavecfgedt();
	afx_msg void OnBnClickedChkJustPeak();

	BOOL m_bJustBoxEnable;
	CIOEdit m_io_JustBoxLevel;
	CIOEdit m_io_JustBoxTime;
	CButton m_btn_Just_Box_Ascend;
	CButton m_btn_Just_Box_Descend;
	CButton m_btn_Just_Box_Flat;

	afx_msg void OnBnClickedChkJustBox();

	CButton m_btn_StartGainTime;
	CButton m_btn_StartGainPeak;
	CIOEdit m_io_JustPeakSelect;
	CIOEdit m_io_JustPeakRate;
	CIOEdit m_io_JustBoxSelect;
	CIOEdit m_io_JustBoxRate;
	CIOEdit m_io_EndDIffJustSelect;
	CIOEdit m_io_EndDIffJustRate;
	CIOEdit m_io_EndDIffPeakSelect;
	CIOEdit m_io_EndDIffPeakRate;
	CIOEdit m_io_EndFlatSelect;
	CIOEdit m_io_EndFlatRate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CIOEdit m_io_DiffMax;

	CIOEdit m_io_Just_UseWave;
	CIOEdit m_io_StartLevel_Wave;


};
