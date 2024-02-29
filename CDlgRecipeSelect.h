#pragma once
#include "VListCt.h"


// CDlgRecipeSelect ダイアログ

class CDlgRecipeSelect : public CDialogX
{
	DECLARE_DYNAMIC(CDlgRecipeSelect)

public:
	CDlgRecipeSelect(CWnd* pParent = nullptr, int iPM = PM1);   // 標準コンストラクター
	virtual ~CDlgRecipeSelect();

	int			m_iPMNo;
	CBitmap		m_bmpPC;
	CPCRcpCtrl* m_pRcpCtrl;

	int			m_iSelectRcpNo;

	
	int			GetSelectRcpNo() { return(m_iSelectRcpNo); }

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RCPSELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CVListCtrl m_list_Recipe;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
