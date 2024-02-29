#pragma once
// EditDouble.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// Dummy ウィンドウ

class CEditDouble : public CEdit
{
// コンストラクション
public:
	CEditDouble();
	virtual void SetScale(double dMin,double dMax);

// アトリビュート
public:
	int		m_bSelect;
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	int iDecPt;
	int iMinusFlg;
	int iMinusChk;
	int iPaintFlg;
	CString stDt;
	double	m_dMin;
	double	m_dMax;


	LOGFONT m_logfont;
	CFont * m_pFont;


// インプリメンテーション
public:
	virtual ~CEditDouble();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
};

class CEditInteger : public CEditDouble
{
// コンストラクション
public:
	CEditInteger();
	virtual void SetScale(int iMin,int iMax);

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	int iDecPt;
	int iMinusFlg;
	int iMinusChk;
	int iPaintFlg;
	CString stDt;
	int	m_iMin;
	int	m_iMax;


// インプリメンテーション
public:
	virtual ~CEditInteger();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};
class CEditEnum : public CEditDouble
{
// コンストラクション
public:
	CEditEnum();
	virtual void SetScale(int iMin,int iMax,char *pEnum[]);

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	CString stDt;
	int	m_iMin;
	int	m_iMax;
	char **m_pszEnumStr;


// インプリメンテーション
public:
	virtual ~CEditEnum();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};
class CEditString : public CEditDouble
{
// コンストラクション
public:
	CEditString();
	virtual void SetScale(int iMaxLen);

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	CString stDt;
	int	m_iMaxLen;


// インプリメンテーション
public:
	virtual ~CEditString();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditString)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

