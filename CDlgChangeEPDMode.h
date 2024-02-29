#pragma once


// CDlgChangeEPDMode ダイアログ

class CDlgChangeEPDMode : public CDialogX
{
	DECLARE_DYNAMIC(CDlgChangeEPDMode)

public:
	CDlgChangeEPDMode(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgChangeEPDMode();
	void	BtnRemote(int iPC);
	void	BtnLocal(int iPC);

	CIODB* m_pIODB;
	
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHGEPDMODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() { return; }
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CIOEdit m_io_EPDMode1;
	CIOEdit m_io_EPDMode2;
	CIOEdit m_io_EPDMode3;
	CIOEdit m_io_EPDMode4;
	CIOEdit m_io_EPDMode5;
	afx_msg void OnBnClickedBtnRemote1() { BtnRemote(PM1); }
	afx_msg void OnBnClickedBtnRemote2() { BtnRemote(PM2); }
	afx_msg void OnBnClickedBtnRemote3() { BtnRemote(PM3); }
	afx_msg void OnBnClickedBtnRemote4() { BtnRemote(PM4); }
	afx_msg void OnBnClickedBtnRemote5() { BtnRemote(PM5); }
	afx_msg void OnBnClickedBtnLocal1() { BtnLocal(PM1); }
	afx_msg void OnBnClickedBtnLocal2() { BtnLocal(PM2); }
	afx_msg void OnBnClickedBtnLocal3() { BtnLocal(PM3); }
	afx_msg void OnBnClickedBtnLocal4() { BtnLocal(PM4); }
	afx_msg void OnBnClickedBtnLocal5() { BtnLocal(PM5); }
};
