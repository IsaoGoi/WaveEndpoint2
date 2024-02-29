#pragma once
#include <afxwin.h>
#include "CDlgMainMonitor.h"
#include "CDlgPCSelect.h"


class CMainWnd :
    public CWnd
{
public:
    CMainWnd(CWnd* pParent, CRect& rc, DWORD dwStyle);
    virtual ~CMainWnd();


    CDlgMainMonitor     *m_pDlgPCMonitor[PM_MAX];
    CDlgPCSelect        *m_pDlgPCSelect;

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnClose();
};

