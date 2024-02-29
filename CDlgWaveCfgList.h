#pragma once
#include "VListct.h"

// CDlgWaveCfgList ダイアログ

class CDlgWaveCfgList : public CDialogX
{
	DECLARE_DYNAMIC(CDlgWaveCfgList)

public:
	CDlgWaveCfgList(CWnd* pParent = nullptr,int iPM=PM1,int bSave=FALSE,int iWaveCfgNo=1);   // 標準コンストラクター
	virtual ~CDlgWaveCfgList();
	void SetWaveCfgList();

	int		m_iPMNo;
	int		m_iSave;
	int		m_iWaveCfgNo;
	char	m_szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char	m_szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];

	CIODB*	m_pIODB;
	CEPDConfig* m_pConfig;
	CWaveCfgCtrl* m_pWaveCfgCtrl;
	CImageList* m_pImage;
	CBitmap	m_bmpPC;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVECFG_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CVListCtrl m_list_WaveCfg;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CIOEdit m_io_CfgName;
	CIOEdit m_io_CfgComment;
	afx_msg void OnLvnItemchangedListWavecfg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnRemove();
};
