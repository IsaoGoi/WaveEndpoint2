#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEditDLimit ウィンドウ

class CEditDLimit : public CEdit
{
// コンストラクション
public:
	CEditDLimit();

// アトリビュート
public:
	void SetSpin(CSpinButtonCtrl* pCtrl){m_pCtrl = pCtrl;};

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditLimit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetRange(double dMin,double dMax,int iPointColumn);
	void GetRange(double *p_dMin,double *p_dMax) {*p_dMin=m_dMin;*p_dMax=m_dMax;return;}
	virtual ~CEditDLimit();

private:
	int m_iPointColumn;
	double m_dMin;
	double m_dMax;
	CSpinButtonCtrl*	m_pCtrl;

	
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditDLimit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CEditLimit : public CEdit
{
// コンストラクション
public:
	CEditLimit();

// アトリビュート
public:
	void SetSpin(CSpinButtonCtrl* pCtrl){m_pCtrl = pCtrl;};

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditLimit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetRange(int iMin,int iMax);
	virtual ~CEditLimit();

private:
	int m_iMin;
	int m_iMax;
		CSpinButtonCtrl*	m_pCtrl;

	
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditLimit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/*
** I/O フィールド設定用
*/
class CEdit_IO : public CEdit
{
// コンストラクション
public:
	CEdit_IO();

// アトリビュート
public:
	void SetIOType(IO_TYPE iotype) {m_iotype = iotype;}
	void SetIOScale(IO_SCALE *pioscale) {m_ioscale = *pioscale;}

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdit_IO)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CEdit_IO();

private:
	IO_TYPE		m_iotype;
	IO_SCALE	m_ioscale;
	
	
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEdit_IO)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


