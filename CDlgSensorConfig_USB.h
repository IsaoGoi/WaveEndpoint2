#pragma once


// CDlgSensorConfig_USB ダイアログ

class CDlgSensorConfig_USB : public CDialogX
{
	DECLARE_DYNAMIC(CDlgSensorConfig_USB)

public:
	CDlgSensorConfig_USB(CWnd* pParent = nullptr, int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgSensorConfig_USB();

	int			m_iPMNo;
	CIODB		*m_pIODB;
	CWaveData_USB*	m_pWaveData_USB;
	CEPDConfig* m_pEPDConfig;

	CBitmap		m_bmpPC;
	unsigned	m_unsBitStatus[5 + 1];

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORCFG_USB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK()		{ return; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CIOEdit m_io_IntegrationTime;
	CIOEdit m_io_Gain;
	CIOEdit m_io_TriggerEdge;
	CIOEdit m_io_TriggerMode;
	afx_msg void OnBnClickedBtnGetprm();
	afx_msg void OnBnClickedBtnSetprm();
	CIOEdit m_io_EEPRomLoadDef;
	afx_msg void OnBnClickedBtnSetloaddef();
	CIOEdit m_io_UnitID;
	CIOEdit m_io_SensorName;
	CIOEdit m_io_SerialNo;
	CIOEdit m_io_UpperWaveLen;
	CIOEdit m_io_LowerWaveLen;
	afx_msg void OnBnClickedBtnReadinfo();
	afx_msg void OnBnClickedBtnWriteinfo();
	CIOEdit m_io_Calibration_A;
	CIOEdit m_io_Calibration_B1;
	CIOEdit m_io_Calibration_B2;
	CIOEdit m_io_Calibration_B3;
	CIOEdit m_io_Calibration_B4;
	CIOEdit m_io_Calibration_B5;
	afx_msg void OnBnClickedBtnReadcalib();
	afx_msg void OnBnClickedBtnWritecalib();
	CIOEdit m_io_CoolingCnt;
	CIOEdit m_io_Status_Bit0;
	CIOEdit m_io_Status_Bit3;
	CIOEdit m_io_Status_Bit4;
	CIOEdit m_io_Status_Bit5;
	CIOEdit m_io_Status_Bit7;
	afx_msg void OnPaint();
	CIOEdit m_io_DLLVersion;
	double m_dCalib_A0;
	double m_dCalib_B1;
	double m_dCalib_B2;
	double m_dCalib_B3;
	double m_dCalib_B4;
	double m_dCalib_B5;
};
