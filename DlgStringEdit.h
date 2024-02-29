#pragma once
// DlgStringEdit.h : ヘッダー ファイル
//
#include "DialogX.h"
#include "IOEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgStringEdit ダイアログ

class CDlgStringEdit : public CDialogX
{
// コンストラクション
public:
	CDlgStringEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ
	BOOL DlgCreate(CWnd *pParent = NULL);
	void SetValue(LPCSTR pszVal,int iMaxSize,CIOEdit *pEdit);
	void GetValue(LPSTR pszVal);
	CIOEdit *GetEditFld() {return(m_pIOEdit);}
	void SetPasswordFld(); 

// ダイアログ データ
	//{{AFX_DATA(CDlgStringEdit)
	enum { IDD = IDD_STRINGEDIT };
	CEdit	m_edFld;
	CString	m_szFld;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgStringEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgStringEdit)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtn0();
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	afx_msg void OnBtn3();
	afx_msg void OnBtn4();
	afx_msg void OnBtn5();
	afx_msg void OnBtn6();
	afx_msg void OnBtn7();
	afx_msg void OnBtn8();
	afx_msg void OnBtn9();
	afx_msg void OnBtnA();
	afx_msg void OnBtnB();
	afx_msg void OnBtnC();
	afx_msg void OnBtnD();
	afx_msg void OnBtnE();
	afx_msg void OnBtnF();
	afx_msg void OnBtnG();
	afx_msg void OnBtnH();
	afx_msg void OnBtnI();
	afx_msg void OnBtnJ();
	afx_msg void OnBtnK();
	afx_msg void OnBtnL();
	afx_msg void OnBtnM();
	afx_msg void OnBtnN();
	afx_msg void OnBtnO();
	afx_msg void OnBtnP();
	afx_msg void OnBtnQ();
	afx_msg void OnBtnR();
	afx_msg void OnBtnS();
	afx_msg void OnBtnT();
	afx_msg void OnBtnU();
	afx_msg void OnBtnV();
	afx_msg void OnBtnW();
	afx_msg void OnBtnX();
	afx_msg void OnBtnY();
	afx_msg void OnBtnZ();
	afx_msg void OnBtnAtmark();
	afx_msg void OnBtnBksp();
	afx_msg void OnBtnCaps();
	afx_msg void OnBtnDel2();
	afx_msg void OnBtnMin();
	afx_msg void OnBtnPlus();
	afx_msg void OnBtnSpace();
	afx_msg void OnBtnCanma();
	afx_msg void OnBtnColon();
	afx_msg void OnBtnDot();
	afx_msg void OnBtnQuestion();
	afx_msg void OnBtnSemicolon();
	afx_msg void OnChangeEdFld();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CIOEdit		*m_pIOEdit;
	CString		m_szEdit;
	int			m_iMaxSize;
	BOOL		m_bCapsLock;
	int			m_bPassword;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
