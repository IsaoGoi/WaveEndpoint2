#pragma once
// AlarmPopup.h : ヘッダー ファイル
//
/////////////////////////////////////////////////////////////////////////////
// CAlarmPopup ウィンドウ

class CAlarmPopup : public CBitmapButton
{
// コンストラクション
public:
	CAlarmPopup();

	CDlgAlarm	*m_pDlgAlarm;
// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAlarmPopup)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CAlarmPopup();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CAlarmPopup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int	m_bReverse;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

