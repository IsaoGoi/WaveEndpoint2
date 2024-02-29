#pragma once
// AccessButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAccessButton ウィンドウ

class CAccessButton : public CBitmapButton
{
// コンストラクション
public:
	CAccessButton();

// アトリビュート
public:

// オペレーション
public:
		void PasswordDraw();
		void InputLogOn();

private:
	CStatic	*m_strPassword;								// ﾊﾟｽﾜｰﾄﾞﾀｲﾄﾙ
	CBitmap	*m_passBitmap;								
	CStatic	*m_passString;								
	CString strUserID;
	CString strUserFullName;
	int		iUserLevel;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAccessButton)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CAccessButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CAccessButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

