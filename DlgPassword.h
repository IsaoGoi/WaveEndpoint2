#pragma once


// CDlgPassword �_�C�A���O

class CDlgPassword : public CDialogX
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgPassword();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PASSWORD };
	CButton	m_BtnClear;
	CString	m_strPassword;
	CString	m_strUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	//{{AFX_MSG(CDlgPassword)
	afx_msg void OnBtnclear();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
};
