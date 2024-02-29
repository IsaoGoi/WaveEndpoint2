#pragma once
#include <afxwin.h>
#include "DldRowWaveMon.h"



class CWaveWnd :
    public CWnd
{
public:

    CWaveWnd(CWnd* pParent, CRect& rc, DWORD dwStyle);
    virtual ~CWaveWnd();

    CDldRowWaveMon* m_pWaveMon[PM_MAX];


    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnClose();
};

