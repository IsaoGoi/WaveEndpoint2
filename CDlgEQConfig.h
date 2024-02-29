#pragma once


// CDlgEQConfig ダイアログ

class CDlgEQConfig : public CDialogX
{
	DECLARE_DYNAMIC(CDlgEQConfig)

public:
	CDlgEQConfig(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgEQConfig();

	void ModifyCheck();   // 標準コンストラクター

	CEPDConfig* m_pEPDConfig;
	CIODB*		m_pIODB;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQCONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel() { return; }
	CIOEdit m_io_PC1;
	CIOEdit m_io_PC2;
	CIOEdit m_io_PC3;
	CIOEdit m_io_PC4;
	CIOEdit m_io_PC5;
	CIOEdit m_io_LogSaveCycle;
	CIOEdit m_io_WaveCnt;
	CIOEdit m_io_Driver;
};
