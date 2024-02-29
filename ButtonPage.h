#pragma once
#include <afxwin.h>
#include "DlgCommadPage.h"
#include "CmdButton.h"

#define		BUTTON_MAX	7
#define		BITMAP_MAX	4

class CChildView;
class CMainWnd;
class CWaveWnd;
class CDlgConfig;
class CDlgPCMonitor;
class CDlgAnalysis;
class CDlgSimulation;
class CDlgRecipeChk;
class CDlgRcpEdit;
class CDlgEPDLog;
class CDlgAlarmLog;

class CButtonPage :
    public CWnd
{
public:
	CButtonPage(CWnd* pParent, CRect& rc, DWORD dwStyle);
	virtual ~CButtonPage();

	void ChkMouseMove(CPoint point);
	void ChangeFunction(int FuncNo, int iSubNo);
	void ChangeMainScreen();
	void DisplayUpdate();


	CChildView*		m_pView;
	CCmdButton* m_buttons[BUTTON_MAX];				// ÎÞÀÝÃ°ÌÞÙ
	CBitmap*		m_bitmap[BUTTON_MAX][BITMAP_MAX];	// ËÞ¯ÄÏ¯ÌßÃ°ÌÞÙ

	int				m_iSelScreenNo;
	int				m_iSelectNo[BUTTON_MAX + 1];
	int				m_nSelectedItem;

	CWnd*			m_pCurrFuncWnd;
	CMainWnd*		m_pMainWnd;
	CWaveWnd*		m_pWaveWnd;
	CDlgConfig*		m_pConfigWnd;
	CDlgPCMonitor*	m_pPCMonitor;
	CDlgAnalysis*	m_pAnalysis;
	CDlgSimulation* m_pSimulation;
	CDlgRecipeChk*  m_pRecipeChk;
	CDlgRcpEdit*	m_pRcpEdit;
	CDlgEPDLog*		m_pEPDLog;
	CDlgAlarmLog* m_pAlarmLog;

	DECLARE_MESSAGE_MAP()

	CDlgCommadPage* m_pCommandPage;


	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

