#pragma once


// CDlgSensorConfig ダイアログ

class CDlgSensorConfig : public CDialogX
{
	DECLARE_DYNAMIC(CDlgSensorConfig)

public:
	CDlgSensorConfig(CWnd* pParent = nullptr, int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgSensorConfig();

	int			m_iPMNo;
	CIODB		*m_pIODB;
	CWaveData_Eth*	m_pWaveData_Eth;
	CWaveData_USB*	m_pWaveData_USB;

	CEPDConfig* m_pEPDConfig;

	CBitmap		m_bmpPC;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORCFG };
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
	CIOEdit m_io_UnitID;
	CIOEdit m_io_SensorName;
	CIOEdit m_io_SerialNo;
	CIOEdit m_io_UpperWaveLen;
	CIOEdit m_io_LowerWaveLen;
	CIOEdit m_io_Calibration_A;
	CIOEdit m_io_Calibration_B1;
	CIOEdit m_io_Calibration_B2;
	CIOEdit m_io_Calibration_B3;
	CIOEdit m_io_Calibration_B4;
	CIOEdit m_io_Calibration_B5;
	CIOEdit m_io_MACAddress;
	CIOEdit m_io_Init_IPAddr;
	CIOEdit m_io_Init_SubNet;
	CIOEdit m_io_Init_DefGateWay;
	CIOEdit m_io_PortNo;
	CIOEdit m_io_VenderName;
	CIOEdit m_io_ProductName;
	CIOEdit m_io_FirmWare;
	CIOEdit m_io_Edit_IP1;
	CIOEdit m_io_Edit_IP2;
	CIOEdit m_io_Edit_IP3;
	CIOEdit m_io_Edit_IP4;
	CIOEdit m_io_PC_IP1;
	CIOEdit m_io_PC_IP2;
	CIOEdit m_io_PC_IP3;
	CIOEdit m_io_PC_IP4;
	afx_msg void OnBnClickedBtnGetmacaddr();
	afx_msg void OnBnClickedBtnGetnetinfo();
	afx_msg void OnBnClickedBtnGetportno();
	afx_msg void OnBnClickedBtnGetproduct();
	afx_msg void OnBnClickedBtnGetunitinfo();
	afx_msg void OnBnClickedBtnReadcalib();
	afx_msg void OnBnClickedBtnReadwavelen();
	afx_msg void OnBnClickedBtnSetipaddr();
	afx_msg void OnBnClickedBtnIPAddress();
	afx_msg void OnBnClickedBtnSetintegration();
	afx_msg void OnBnClickedBtnGetintegrate();
	double m_dCalib_A0;
	double m_dCalib_B1;
	double m_dCalib_B2;
	double m_dCalib_B3;
	double m_dCalib_B4;
	double m_dCalib_B5;
};
