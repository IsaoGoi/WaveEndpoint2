#pragma once
//
#include "DialogX.h"
#include "IOEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit ダイアログ

class CDlgEnumEdit : public CDialogX
{
// コンストラクション
public:
	BOOL DlgCreate(CWnd* pParent = NULL);
	CDlgEnumEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void SetValue(unsigned uint_val,LPSTR pEnumString[],CIOEdit *pEdit);
	void GetValue(unsigned *uint_val);
	CIOEdit *GetEditFld() {return(m_pIOEdit);}

	// ダイアログ データ
	//{{AFX_DATA(CDlgEnumEdit)
	enum { IDD = IDD_ENUMEDIT };
	CListBox	m_list_EnumStr;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgEnumEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgEnumEdit)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CIOEdit		*m_pIOEdit;
	char m_szEnumString[32][64];
	unsigned m_uint_Val;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

