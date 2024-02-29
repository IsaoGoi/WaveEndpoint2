#pragma once

#include		"CDlgEQConfig.h"
#include		"CDlgWaveConfig.h"
#include		"CDlgPLCConfig.h"
#include		"CDlgSensorConfig.h"
#include		"CDlgSensorConfig_USB.h"
#include		"DlgUserList.h"

extern CRect	gDefRect;

// CDlgConfig ダイアログ

class CDlgConfig : public CDialogX
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = nullptr, CRect& rc = gDefRect);   // 標準コンストラクター
	virtual ~CDlgConfig();


	CWnd*	m_pParentWnd;
	CRect	m_rc_wnd;
	CImageList* m_pImage;
	CDialog* m_pCurrDlg;
	int		m_iCurrItemNo;
	CEPDConfig* m_pEPDConfig;

	void	ModifyCheck();

	CDlgEQConfig*		m_pDlgEQConfig;
	CDlgWaveConfig*		m_pDlgWaveConfig;
	CDlgPLCConfig*		m_pDlgPLCConfig;
	CDlgSensorConfig*	m_pDlgSensorConfig;
	CDlgSensorConfig_USB* m_pDlgSensorConfig_USB;
	CDlgUserList*		m_pDlgUserList;
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK()		{ return; }
	virtual void OnCancel()	{ return; }
	CTreeCtrl m_Tree_Config;
	afx_msg void OnTvnSelchangedTreeCfg(NMHDR* pNMHDR, LRESULT* pResult);
};
