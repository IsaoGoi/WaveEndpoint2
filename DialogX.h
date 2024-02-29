#pragma once


// CDialogX �_�C�A���O

class CDialogX : public CDialog
{
	DECLARE_DYNAMIC(CDialogX)

public:
	CDialogX(UINT id,CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDialogX();
	void SetBackColor(int r,int g,int b);

	int		m_iBack_R;
	int		m_iBack_G;
	int		m_iBack_B;
	int		m_bInitial;

	CBrush	m_brBack;
	
// �_�C�A���O �f�[�^
//	enum { IDD = IDD_DIALOGX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
