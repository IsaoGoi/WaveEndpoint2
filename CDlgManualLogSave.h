#pragma once


// CDlgManualLogSave ダイアログ

class CDlgManualLogSave : public CDialogX
{
	DECLARE_DYNAMIC(CDlgManualLogSave)

public:
	CDlgManualLogSave(CWnd* pParent = nullptr, int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgManualLogSave();


	int			m_iPMNo;
	CBitmap		m_bmpPC;
	CIODB		*m_pIODB;
	CWaveSamplingSave	*m_pWaveSampling;
//	CWaveData_Eth		*m_pWaveData;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUAL_LOGSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CIOEdit m_io_SamplingTime;
	CIOEdit m_io_SaveMaxTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStop();
};
