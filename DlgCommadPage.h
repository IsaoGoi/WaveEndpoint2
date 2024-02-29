#pragma once

class CButtonPage;


// CDlgCommadPage ダイアログ

class CDlgCommadPage : public CDialog
{
	DECLARE_DYNAMIC(CDlgCommadPage)

public:
	CDlgCommadPage(CWnd* pParent = NULL,int iDisplayID = 0,int x=0,int btm_y=0);   // 標準コンストラクタ
	virtual ~CDlgCommadPage();

// ダイアログ データ
	enum { IDD = IDD_COMMANDPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnOK()     {return;}
	virtual void OnCancel() {return;}

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	void OnBnClicked(int iSUbNo);

private:
	int		m_iDisplayID;
	int		m_iStart_x;
	int		m_iBottomt_y;
	CButtonPage	*m_pButtonPage;


public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCmdBtn1() {OnBnClicked(0);}
	afx_msg void OnBnClickedCmdBtn2() {OnBnClicked(1);}
	afx_msg void OnBnClickedCmdBtn3() {OnBnClicked(2);}
	afx_msg void OnBnClickedCmdBtn4() {OnBnClicked(3);}
	afx_msg void OnBnClickedCmdBtn5() {OnBnClicked(4);}
	afx_msg void OnBnClickedCmdBtn6() {OnBnClicked(5);}
	afx_msg void OnBnClickedCmdBtn7() {OnBnClicked(6);}
	afx_msg void OnBnClickedCmdBtn8() {OnBnClicked(7);}
	afx_msg void OnBnClickedCmdBtn9() {OnBnClicked(8);}
	afx_msg void OnBnClickedCmdBtn10() {OnBnClicked(9);}
	afx_msg void OnBnClickedCmdBtn11() {OnBnClicked(10);}
};
