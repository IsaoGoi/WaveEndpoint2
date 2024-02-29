#pragma once
#include "DialogX.h"


// CDldRowWaveMon ダイアログ

class CDldRowWaveMon : public CDialogX
{
	DECLARE_DYNAMIC(CDldRowWaveMon)

public:
	CDldRowWaveMon(CWnd* pParent = nullptr,int iPM = PM1);   // 標準コンストラクター
	virtual ~CDldRowWaveMon();

	int			m_iPMNo;
	CWaveData_Eth*	m_pWaveData_Eth;
	CWaveData_USB* m_pWaveData_USB;
	CEPDConfig* m_pConfig;
	CWaveSamplingSave* m_pWaveSampling;
	CRect		m_Rect_Wnd;
	CRect		m_Rect_Graph;
	int			m_iHeight;
	int			m_iWidth;

	int			m_bSamplingStart;

	POINT		m_pGraph[EPD_WAVE_TOTAL + 3];
	CBitmap		m_pBmpPC;

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROWWAVEMON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK()		{ return; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	CStatic m_bmpPCName;


	afx_msg void OnBnClickedBtnSave();
};
