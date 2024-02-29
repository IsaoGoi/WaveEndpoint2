#pragma once


// CCmdButton

class CCmdButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CCmdButton)

public:
	CCmdButton();
	virtual ~CCmdButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

private:
	CWnd* m_pParentWnd;
};


