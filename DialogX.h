#pragma once


// CDialogX ダイアログ

class CDialogX : public CDialog
{
	DECLARE_DYNAMIC(CDialogX)

public:
	CDialogX(UINT id,CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDialogX();
	void SetBackColor(int r,int g,int b);

	int		m_iBack_R;
	int		m_iBack_G;
	int		m_iBack_B;
	int		m_bInitial;

	CBrush	m_brBack;
	
// ダイアログ データ
//	enum { IDD = IDD_DIALOGX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
