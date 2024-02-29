#pragma once


// CDlgPassword ダイアログ

class CDlgPassword : public CDialogX
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPassword();

// ダイアログ データ
	enum { IDD = IDD_PASSWORD };
	CButton	m_BtnClear;
	CString	m_strPassword;
	CString	m_strUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	//{{AFX_MSG(CDlgPassword)
	afx_msg void OnBtnclear();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
};
