#pragma once
#include "VListct.h"


// CDlgAlarm ダイアログ

class CDlgAlarm : public CDialogX
{
	DECLARE_DYNAMIC(CDlgAlarm)

public:
	CDlgAlarm(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgAlarm();

	CImageList   m_img_List;
	
	CAlarm		*m_pAlarm;
	void ShowAlarm();
	void UpdateAlarm();
	void HideAlarm();



	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALARM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CVListCtrl m_list_alarm;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAllClear();
	virtual void OnCancel();
};
