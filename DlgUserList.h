#pragma once

// DlgUserList.h : ヘッダー ファイル
//
#include "DialogX.h"
#include "VListCt.h"
#include "ioedit.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgUserList ダイアログ

class CDlgUserList : public CDialogX
{
// コンストラクション
public:
	CDlgUserList(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDlgUserList();
	BOOL ModifyCheck();

// ダイアログ データ
	//{{AFX_DATA(CDlgUserList)
	enum { IDD = IDD_USERLIST };
	CVListCtrl	m_list_user;
	CComboBox	m_cmb_level;
	CIOEdit	m_io_ConfirmPassword;
	CIOEdit	m_io_Password;
	CIOEdit	m_io_UserName;
	CIOEdit	m_io_GlsPassword;
	CIOEdit	m_ioAutoLogOff;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgUserList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgUserList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {return;}
	virtual void OnOK();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList * m_img_Status;
	CAccessLevel *m_pAccessLevel;
	CIODB		*m_pIODB;
	int			m_iEditItem;

	void	SetUserList();
public:
//	afx_msg void OnNMClickUselist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedUselist(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
