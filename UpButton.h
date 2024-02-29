#pragma once
// UpButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUpButton ウィンドウ

class CUpButton : public CBitmapButton
{
// コンストラクション
public:
	CUpButton();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUpButton)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CUpButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CUpButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
class CDownButton : public CBitmapButton
{
// コンストラクション
public:
	CDownButton();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDownButton)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDownButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDownButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

