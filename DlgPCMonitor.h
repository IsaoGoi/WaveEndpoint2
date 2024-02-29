#pragma once

#include "VListct.h"

// CDlgPCMonitor ダイアログ

class CDlgPCMonitor : public CDialogX
{
	DECLARE_DYNAMIC(CDlgPCMonitor)

public:
	CDlgPCMonitor(CWnd* pParent = nullptr, int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgPCMonitor();


	void	SelchangeWave(int iNo);
	void	WaveClear (int iNo);

	int			m_iPMNo;
	CBitmap		m_bmpPC;
	CEPDConfig	*m_pConfig;
	CWaveData_Eth	*m_pWaveData_Eth;
	CWaveData_USB	*m_pWaveData_USB;
	CIODB		*m_pIODB;

	RECT		m_rcGraph;
	double		m_dHeight;
	double		m_dWidth;

	POINT		m_ptGraph[10][EPD_SAMPLING_POINT + 2];

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCMONITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }
	afx_msg void OnPaint();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CIOEdit m_io_Y_Max;
	CIOEdit m_io_Y_Min;
	CIOEdit m_io_X_Max;
	CVListCtrl m_list_wave1;
	CVListCtrl m_list_wave2;
	CVListCtrl m_list_wave3;
	CVListCtrl m_list_wave4;
	CVListCtrl m_list_wave5;
	afx_msg void OnLvnItemchangedListWave1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListWave2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListWave3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListWave4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListWave5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnClr1() { WaveClear(0); }
	afx_msg void OnBnClickedBtnClr2() { WaveClear(1); }
	afx_msg void OnBnClickedBtnClr3() { WaveClear(2); }
	afx_msg void OnBnClickedBtnClr4() { WaveClear(3); }
	afx_msg void OnBnClickedBtnClr5() { WaveClear(4); }
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	CIOEdit m_io_Data1;
	CIOEdit m_io_Data2;
	CIOEdit m_io_Data3;
	CIOEdit m_io_Data4;
	CIOEdit m_io_Data5;
	unsigned short		m_usLastData[10 + 1];
	int					m_iSamplingCycle;

};
