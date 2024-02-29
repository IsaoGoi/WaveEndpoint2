
// ChildView.h : CChildView クラスのインターフェイス
//
#pragma once
#include "ButtonPage.h"
#include "CDlgHeadPage.h"

#define	HEAD_WINDOW_WIDTH	200


// CChildView ウィンドウ

class CChildView : public CWnd
{
// コンストラクション
public:
	CChildView();
	void CreateScreen();


	CButtonPage*	m_pButtonPage;
	CDlgHeadPage*	m_pDlgHeadPage;
	
	// 属性
public:

// 操作
public:

// オーバーライド
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildView();

	// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

