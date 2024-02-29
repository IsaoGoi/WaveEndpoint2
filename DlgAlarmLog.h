#pragma once


// CDlgAlarmLog ダイアログ

class CDlgAlarmLog : public CDialogX
{
	DECLARE_DYNAMIC(CDlgAlarmLog)

public:
	CDlgAlarmLog(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgAlarmLog();

	CAlarm*			m_pAlarm;
	CImageList		m_img_List;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALARMLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CVListCtrl m_list_alarm;
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }
};
