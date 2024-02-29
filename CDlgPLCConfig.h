#pragma once


// CDlgPLCConfig ダイアログ

class CDlgPLCConfig : public CDialogX
{
	DECLARE_DYNAMIC(CDlgPLCConfig)

public:
	CDlgPLCConfig(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgPLCConfig();
	void	ModifyCheck();

	CEPDConfig* m_pEPDConfig;
	CIODB* m_pIODB;


	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLCCONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK();
	CIOEdit m_io_PCNo;
	CIOEdit m_io_PCNetNo;
	CIOEdit m_io_PCNode;
	CIOEdit m_io_PLCNo;
	CIOEdit m_io_PLCNetNo;
	CIOEdit m_io_PLCNode;
	CIOEdit m_io_PLC_PortNo;
	CIOEdit m_io_PLC_IP1;
	CIOEdit m_io_PLC_IP2;
	CIOEdit m_io_PLC_IP3;
	CIOEdit m_io_PLC_IP4;
	CIOEdit m_io_Omron_Enable;
	CIOEdit m_io_KeyenceEnable;
	CIOEdit m_io_Keyence_IP1;
	CIOEdit m_io_Keyence_IP2;
	CIOEdit m_io_Keyence_IP3;
	CIOEdit m_io_Keyence_IP4;
	CIOEdit m_io_PC_IP1;
	CIOEdit m_io_PC_IP2;
	CIOEdit m_io_PC_IP3;
	CIOEdit m_io_PC_IP4;
	CIOEdit m_io_KeyencePort;
};
