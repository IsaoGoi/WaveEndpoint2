#pragma once


// CDlgAutoRcpChk ダイアログ
#define	CHK_TBL_MAX		1000
#define	CHK_ITEM_MAX	8
typedef struct {
	char	szFileName[64 + 1];
	int		bCheck;
	int		iResult;
	double	dTotalTime;
	int		iItemCount;
	char	szItemName[CHK_ITEM_MAX][16 + 1];
	double	dItemTime[CHK_ITEM_MAX];
	double	dItemLevel[CHK_ITEM_MAX];
}RCP_CHK_DATA;



class CDlgAutoRcpChk : public CDialogX
{
	DECLARE_DYNAMIC(CDlgAutoRcpChk)

public:
	CDlgAutoRcpChk(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgAutoRcpChk();
	void	OnBnClickedBtnRcp(int iPM);
	void	SetFileList();
	void	ChangePC(int iPM);
	void	ChkListUpdate(int bCreate);
// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTO_RCPCHECK };
#endif

	CIODB			*m_pIODB;
	CEPDSimulation* m_pEPDSimulation;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnRcp1() { OnBnClickedBtnRcp(PM1); }
	afx_msg void OnBnClickedBtnRcp2() { OnBnClickedBtnRcp(PM2); }
	afx_msg void OnBnClickedBtnRcp3() { OnBnClickedBtnRcp(PM3); }
	afx_msg void OnBnClickedBtnRcp4() { OnBnClickedBtnRcp(PM4); }
	afx_msg void OnBnClickedBtnRcp5() { OnBnClickedBtnRcp(PM5); }

	CIOEdit m_io_RcpName;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CVListCtrl m_list_RawData;
	afx_msg void OnBnClickedRdPc1() { ChangePC(PM1); }
	afx_msg void OnBnClickedRdPc2() { ChangePC(PM2); }
	afx_msg void OnBnClickedRdPc3() { ChangePC(PM3); }
	afx_msg void OnBnClickedRdPc4() { ChangePC(PM4); }
	afx_msg void OnBnClickedRdPc5() { ChangePC(PM5); }
	afx_msg void OnBnClickedBtnAllsel();
	afx_msg void OnBnClickedBtnAllunsel();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnUnselect();
};
