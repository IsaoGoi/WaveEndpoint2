#pragma once

#include "DialogX.h"
#include "AccessButton.h"

// CDlgHeadPage ダイアログ


class CDlgHeadPage : public CDialogX
{
	DECLARE_DYNAMIC(CDlgHeadPage)

public:
	CDlgHeadPage(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgHeadPage();

	CRect	m_Rect_Time;
	CIODB	*m_pIODB;
	CEPDConfig* m_pEPDConfig;
	CBitmap m_bmp_SensorStatus[5];
	CAlarmPopup	*m_BtnAlarmPopup;
	CAccessButton* m_BtnPassword;							// ﾊﾟｽﾜｰﾄﾞ入力ﾎﾞﾀﾝ
	void LogIn() { m_BtnPassword->InputLogOn(); }

	int			m_ioHdlSensor[PM_MAX + 1];
	unsigned	m_unsSensorStatus[PM_MAX + 1];
	int			m_ioHdlStatus[PM_MAX + 1];
	unsigned	m_unsEPDStatus[PM_MAX + 1];
	unsigned	m_unsPLCStatus;

	CRect		m_rc_DrawBmp;
	int			m_bBmpUpdate;
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	CIOEdit m_io_Sensor1;
	CIOEdit m_io_Sensor2;
	CIOEdit m_io_Sensor3;
	CIOEdit m_io_Sensor4;
	CIOEdit m_io_Sensor5;
	CIOEdit m_io_EPDMode1;
	CIOEdit m_io_EPDMode2;
	CIOEdit m_io_EPDMode3;
	CIOEdit m_io_EPDMode4;
	CIOEdit m_io_EPDMode5;
	afx_msg void OnBnClickedBtnMode();
	CIOEdit m_io_EPDStatus1;
	CIOEdit m_io_EPDStatus2;
	CIOEdit m_io_EPDStatus3;
	CIOEdit m_io_EPDStatus4;
	CIOEdit m_io_EPDStatus5;
	CIOEdit m_io_PLCStatus;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
