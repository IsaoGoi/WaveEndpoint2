#pragma once


// CDlgPCSelect ダイアログ

class CDlgPCSelect : public CDialogX
{
	DECLARE_DYNAMIC(CDlgPCSelect)

public:
	CDlgPCSelect(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgPCSelect();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PC_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }

	void OnBnClickedBtnPC(int iPC);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnPc1() { OnBnClickedBtnPC(PM1); }
	afx_msg void OnBnClickedBtnPc2() { OnBnClickedBtnPC(PM2); }
	afx_msg void OnBnClickedBtnPc3() { OnBnClickedBtnPC(PM3); }
	afx_msg void OnBnClickedBtnPc4() { OnBnClickedBtnPC(PM4); }
	afx_msg void OnBnClickedBtnPc5() { OnBnClickedBtnPC(PM5); }
	afx_msg void OnBnClickedBtnViewsort();
};
