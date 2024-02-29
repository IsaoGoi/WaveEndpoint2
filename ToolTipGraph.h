#pragma once


class CToolTipGraph : public CWnd
{
// コンストラクション
public:
	CToolTipGraph(CWnd *pParent);

// アトリビュート
public:
	char	szTime[32 +1];
	char	szData[32 +1];
	char	szRow[32 + 1];
	char	szDiff[32 + 1];


// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CToolTipCTCGlass)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CToolTipGraph();
	void	Active(CPoint point);
	void	Deactive();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CToolTipGraph)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CRect	m_WinRect;
	CFont	m_Font;
	LOGFONT	m_logfont;
};

