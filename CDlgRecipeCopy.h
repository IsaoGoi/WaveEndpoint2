#pragma once

#include "VListct.h"
// CDlgRecipeCopy ダイアログ

class CDlgRecipeCopy : public CDialogX
{
	DECLARE_DYNAMIC(CDlgRecipeCopy)

public:
	CDlgRecipeCopy(CWnd* pParent = nullptr,int iPCNo=PM1,int iRecipeNo=0);   // 標準コンストラクター
	virtual ~CDlgRecipeCopy();

	void	SetFromRecipeList(int iPC);
	void	SetToRecipeList(int iPC);

	int		m_iFromPCNo;
	int		m_iFromRcpNo;
	int		m_iToPCNo;
	int		m_iToRcpNo;

	CImageList* m_pImage;
	CPCRcpCtrl	*m_pRcpCtrl;
	CIODB		*m_pIODB;
	CEPDConfig* m_pConfig;



// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RCPCOPY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_szFromNo;
	CString m_szToNo;
	CString m_szFromName;
	CIOEdit m_io_ToName;
	CComboBox m_cmb_from_pc;
	CComboBox m_cmb_to_pc;
	CVListCtrl m_list_from;
	CVListCtrl m_list_to;
	afx_msg void OnCbnSelchangeCmbFrom();
	afx_msg void OnCbnSelchangeCmbTo();
	afx_msg void OnLvnItemchangedListFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
};
