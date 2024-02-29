// CDlgHeadPage.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgHeadPage.h"
#include "CDlgChangeEPDMode.h"


// CDlgHeadPage ダイアログ
static UINT UID_SensorStatus[PM_MAX] = {
	/* 0 */	IDC_BMP_SENSOR1,
	/* 1 */	IDC_BMP_SENSOR2,
	/* 2 */	IDC_BMP_SENSOR3,
	/* 3 */	IDC_BMP_SENSOR4,
	/* 4 */	IDC_BMP_SENSOR5,
};
static UINT m_bmpSensorStatus[6] =
{
	/* 0 */IDB_GRAY_CIRCLE,		//Disable
	/* 1 */IDB_GREEN_CIRCLE,	//Normal
	/* 2 */IDB_YELLOW_CIRCLE,	//USB Disconnect
	/* 3 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 4 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 5 */IDB_RED_CIRCLE,		//Error(Invalid)
};

static UINT UID_EPDStatus[PM_MAX] = {
	/* 0 */	IDC_BMP_STATUS1,
	/* 1 */	IDC_BMP_STATUS2,
	/* 2 */	IDC_BMP_STATUS3,
	/* 3 */	IDC_BMP_STATUS4,
	/* 4 */	IDC_BMP_STATUS5,
};
static UINT m_bmpEPDStatus[9*1] =
{
	/* 0 */IDB_YELLOW_CIRCLE,	//0:Unknown
	/* 1 */IDB_GREEN_CIRCLE,	//1:IDLE
	/* 2 */IDB_PINK_CIRCLE,		//2:Dead Time
	/* 3 */IDB_PINK_CIRCLE,		//3:Just
	/* 4 */IDB_PINK_CIRCLE,		//4:End
	/* 5 */IDB_PINK_CIRCLE,		//5:Over Etch
	/* 6 */IDB_GREEN_CIRCLE,		//6:Complete
	/* 7 */IDB_RED_CIRCLE,		//Eddor End
	/* 8 */IDB_RED_CIRCLE,	//H/W Error
};

static UINT m_bmpPLCStatus[6] =
{
	/* 0 */IDB_GRAY_CIRCLE,		//Disable
	/* 1 */IDB_GREEN_CIRCLE,	//Normal
	/* 2 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 3 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 4 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 5 */IDB_RED_CIRCLE,		//Error(Invalid)
};

static UINT UID_SensorID[PM_MAX] = {
	/* 0 */	IDC_ED_SENSOR1,
	/* 1 */	IDC_ED_SENSOR2,
	/* 2 */	IDC_ED_SENSOR3,
	/* 3 */	IDC_ED_SENSOR4,
	/* 4 */	IDC_ED_SENSOR5,
};
static UINT UID_StatusID[PM_MAX] = {
	/* 0 */	IDC_ED_STATUS1,
	/* 1 */	IDC_ED_STATUS2,
	/* 2 */	IDC_ED_STATUS3,
	/* 3 */	IDC_ED_STATUS4,
	/* 4 */	IDC_ED_STATUS5,
};
static UINT UID_ModeID[PM_MAX] = {
	/* 0 */	IDC_ED_MODE1,
	/* 1 */	IDC_ED_MODE2,
	/* 2 */	IDC_ED_MODE3,
	/* 3 */	IDC_ED_MODE4,
	/* 4 */	IDC_ED_MODE5,
};



IMPLEMENT_DYNAMIC(CDlgHeadPage, CDialogX)

CDlgHeadPage::CDlgHeadPage(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_HEAD, pParent)
{
	m_BtnAlarmPopup = NULL;
	m_BtnPassword = NULL;

	SetBackColor(192, 192, 255);
	CDialogX::Create(IDD_HEAD, pParent);
}

CDlgHeadPage::~CDlgHeadPage()
{
	if (m_BtnAlarmPopup) {
		delete m_BtnAlarmPopup;
	}
	if(m_BtnPassword)
		delete	m_BtnPassword;

}

void CDlgHeadPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_SENSOR1, m_io_Sensor1);
	DDX_Control(pDX, IDC_ED_SENSOR2, m_io_Sensor2);
	DDX_Control(pDX, IDC_ED_SENSOR3, m_io_Sensor3);
	DDX_Control(pDX, IDC_ED_SENSOR4, m_io_Sensor4);
	DDX_Control(pDX, IDC_ED_SENSOR5, m_io_Sensor5);
	DDX_Control(pDX, IDC_ED_MODE1, m_io_EPDMode1);
	DDX_Control(pDX, IDC_ED_MODE2, m_io_EPDMode2);
	DDX_Control(pDX, IDC_ED_MODE3, m_io_EPDMode3);
	DDX_Control(pDX, IDC_ED_MODE4, m_io_EPDMode4);
	DDX_Control(pDX, IDC_ED_MODE5, m_io_EPDMode5);
	DDX_Control(pDX, IDC_ED_STATUS1, m_io_EPDStatus1);
	DDX_Control(pDX, IDC_ED_STATUS2, m_io_EPDStatus2);
	DDX_Control(pDX, IDC_ED_STATUS3, m_io_EPDStatus3);
	DDX_Control(pDX, IDC_ED_STATUS4, m_io_EPDStatus4);
	DDX_Control(pDX, IDC_ED_STATUS5, m_io_EPDStatus5);
	DDX_Control(pDX, IDC_ED_PLCCOMM, m_io_PLCStatus);
}


BEGIN_MESSAGE_MAP(CDlgHeadPage, CDialogX)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_MODE, &CDlgHeadPage::OnBnClickedBtnMode)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgHeadPage メッセージ ハンドラー


BOOL CDlgHeadPage::OnInitDialog()
{
	int			i;
	CRect		rc;
	CString		szIOName('\0', 32);

	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.right = rc.left + HEAD_WINDOW_WIDTH;
	MoveWindow(rc);

	GetClientRect(m_Rect_Time);
	m_Rect_Time.DeflateRect(5, 5);
	m_Rect_Time.top = rc.bottom-100;
	GetDlgItem(IDC_RECT_TIME)->MoveWindow(m_Rect_Time);


	rc.SetRect(45, 920, 145,1020);
	m_BtnAlarmPopup = new CAlarmPopup();
	m_BtnAlarmPopup->Create(NULL, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_OWNERDRAW, rc, this, 0);
	m_BtnAlarmPopup->ShowWindow(SW_HIDE);


	/*
	** ﾊﾟｽﾜｰﾄﾞ入力ボタン
	*/
	rc.SetRect(15, 135, 175, 200);
	m_BtnPassword = new CAccessButton();
	m_BtnPassword->Create(NULL, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_OWNERDRAW, rc, this, 0);


	for (i = 0; i < PM_MAX; i++) {
		szIOName.Format("CTC.PC%d.EPD.Sensor", (i + 1));
		m_ioHdlSensor[i] = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointRead(m_ioHdlSensor[i], &m_unsSensorStatus[i]);

		szIOName.Format("PM%d.EPD.Status", (i + 1));
		m_ioHdlStatus[i] = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointRead(m_ioHdlStatus[i], &m_unsEPDStatus[i]);

		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i] == 0) {
			GetDlgItem(UID_SensorID[i])->EnableWindow(FALSE);
			GetDlgItem(UID_StatusID[i])->EnableWindow(FALSE);
			GetDlgItem(UID_ModeID[i])->EnableWindow(FALSE);
			
			GetDlgItem(UID_SensorStatus[i])->ShowWindow(SW_HIDE);
			GetDlgItem(UID_EPDStatus[i])->ShowWindow(SW_HIDE);
			GetDlgItem(UID_ModeID[i])->ShowWindow(SW_HIDE);
		}

	}

	m_io_EPDMode1.SetIOHdl(m_pIODB->IOPointGetHandle("PM1.EPD.Mode"));
	m_io_EPDMode2.SetIOHdl(m_pIODB->IOPointGetHandle("PM2.EPD.Mode"));
	m_io_EPDMode3.SetIOHdl(m_pIODB->IOPointGetHandle("PM3.EPD.Mode"));
	m_io_EPDMode4.SetIOHdl(m_pIODB->IOPointGetHandle("PM4.EPD.Mode"));
	m_io_EPDMode5.SetIOHdl(m_pIODB->IOPointGetHandle("PM5.EPD.Mode"));

	m_io_Sensor1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC1.EPD.Sensor"));
	m_io_Sensor2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC2.EPD.Sensor"));
	m_io_Sensor3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC3.EPD.Sensor"));
	m_io_Sensor4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC4.EPD.Sensor"));
	m_io_Sensor5.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC5.EPD.Sensor"));

	m_io_EPDStatus1.SetIOHdl(m_pIODB->IOPointGetHandle("PM1.EPD.Status"));
	m_io_EPDStatus2.SetIOHdl(m_pIODB->IOPointGetHandle("PM2.EPD.Status"));
	m_io_EPDStatus3.SetIOHdl(m_pIODB->IOPointGetHandle("PM3.EPD.Status"));
	m_io_EPDStatus4.SetIOHdl(m_pIODB->IOPointGetHandle("PM4.EPD.Status"));
	m_io_EPDStatus5.SetIOHdl(m_pIODB->IOPointGetHandle("PM5.EPD.Status"));
	if(m_pEPDConfig->m_tPLCConfig.iEnable == TRUE)
		m_io_PLCStatus.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PLCStatus"));
	else
		m_io_PLCStatus.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KeyenceStatus"));

	m_pIODB->IOPointRead(m_io_PLCStatus.GetIOHdl(), &m_unsPLCStatus);

	m_io_EPDMode1.SetSpecialColor(1);
	m_io_EPDMode2.SetSpecialColor(1);
	m_io_EPDMode3.SetSpecialColor(1);
	m_io_EPDMode4.SetSpecialColor(1);
	m_io_EPDMode5.SetSpecialColor(1);
	m_io_Sensor1.SetSpecialColor(1);
	m_io_Sensor2.SetSpecialColor(1);
	m_io_Sensor3.SetSpecialColor(1);
	m_io_Sensor4.SetSpecialColor(1);
	m_io_Sensor5.SetSpecialColor(1);
	m_io_EPDStatus1.SetSpecialColor(1);
	m_io_EPDStatus2.SetSpecialColor(1);
	m_io_EPDStatus3.SetSpecialColor(1);
	m_io_EPDStatus4.SetSpecialColor(1);
	m_io_EPDStatus5.SetSpecialColor(1);
	m_io_PLCStatus.SetSpecialColor(1);

	GetClientRect(&m_rc_DrawBmp);
	m_rc_DrawBmp.right = m_rc_DrawBmp.left + 50;
	m_rc_DrawBmp.top += 50;
	m_rc_DrawBmp.bottom -= 40;
	m_bBmpUpdate = TRUE;
	InvalidateRect(&m_rc_DrawBmp);

	SetTimer(1, 900, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}




void CDlgHeadPage::OnTimer(UINT_PTR nIDEvent)
{
	int			i;
	unsigned	unsStatus;
	CAlarm*		pAlarm;


	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	InvalidateRect(m_Rect_Time);

	m_io_EPDMode1.ChkValueChange();
	m_io_EPDMode2.ChkValueChange();
	m_io_EPDMode3.ChkValueChange();
	m_io_EPDMode4.ChkValueChange();
	m_io_EPDMode5.ChkValueChange();
	m_io_Sensor1.ChkValueChange();
	m_io_Sensor2.ChkValueChange();
	m_io_Sensor3.ChkValueChange();
	m_io_Sensor4.ChkValueChange();
	m_io_Sensor5.ChkValueChange();
	m_io_EPDStatus1.ChkValueChange();
	m_io_EPDStatus2.ChkValueChange();
	m_io_EPDStatus3.ChkValueChange();
	m_io_EPDStatus4.ChkValueChange();
	m_io_EPDStatus5.ChkValueChange();
	m_io_PLCStatus.ChkValueChange();

	for (i = 0; i < PM_MAX; i++) {
		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			m_pIODB->IOPointRead(m_ioHdlSensor[i], &unsStatus);
			if (unsStatus != m_unsSensorStatus[i]) {
				m_bBmpUpdate = TRUE;
			}
			m_pIODB->IOPointRead(m_ioHdlStatus[i], &unsStatus);
			if (unsStatus != m_unsEPDStatus[i]) {
				m_bBmpUpdate = TRUE;
			}
			if (GetDlgItem(UID_SensorID[i])->IsWindowEnabled()==FALSE)
				GetDlgItem(UID_SensorID[i])->EnableWindow(TRUE);
			if (GetDlgItem(UID_StatusID[i])->IsWindowEnabled() == FALSE)
				GetDlgItem(UID_StatusID[i])->EnableWindow(TRUE);
			if (GetDlgItem(UID_SensorStatus[i])->IsWindowVisible() == FALSE)
				GetDlgItem(UID_SensorStatus[i])->ShowWindow(SW_SHOW);
			if (GetDlgItem(UID_EPDStatus[i])->IsWindowVisible() == FALSE)
				GetDlgItem(UID_EPDStatus[i])->ShowWindow(SW_SHOW);
		}
		else {
			if (GetDlgItem(UID_SensorID[i])->IsWindowEnabled() == TRUE)
				GetDlgItem(UID_SensorID[i])->EnableWindow(FALSE);
			if (GetDlgItem(UID_StatusID[i])->IsWindowEnabled() == TRUE)
				GetDlgItem(UID_StatusID[i])->EnableWindow(FALSE);
			if (GetDlgItem(UID_SensorStatus[i])->IsWindowVisible() == TRUE)
				GetDlgItem(UID_SensorStatus[i])->EnableWindow(SW_HIDE);
			if (GetDlgItem(UID_EPDStatus[i])->IsWindowVisible() == TRUE)
				GetDlgItem(UID_EPDStatus[i])->EnableWindow(SW_HIDE);
		}
	}
	m_pIODB->IOPointRead(m_io_PLCStatus.GetIOHdl(), &unsStatus);
	if (m_unsPLCStatus != unsStatus) {
		m_bBmpUpdate = TRUE;
		m_unsPLCStatus = unsStatus;
	}
	if(m_bBmpUpdate)
		InvalidateRect(&m_rc_DrawBmp);

	pAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetAlarmClass();
	if (pAlarm->m_iCurrAlmTotal) {
		if (m_BtnAlarmPopup->IsWindowVisible() == FALSE)
			m_BtnAlarmPopup->ShowWindow(SW_SHOW);
	}
	else {
		if (m_BtnAlarmPopup->IsWindowVisible() == TRUE)
			m_BtnAlarmPopup->ShowWindow(SW_HIDE);
	}

	CDialogX::OnTimer(nIDEvent);
}


void CDlgHeadPage::OnPaint()
{
	CBitmap		curr_bmp;
	BITMAP		bmp;
	CBitmap*	pOldBitmap = NULL;
	CDC*		pdc;
	CDC			cDC;
	CString		szStr('\0', 40);
	int			i;

	int			x, y;

	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
					   // 描画メッセージで CDialogX::OnPaint() を呼び出さないでください。
	CFont font;
	VERIFY(font.CreateFont(
		30,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Times New Roman"));       // lpszFacename

	CFont* def_font = dc.SelectObject(&font);

	CTime	tm = CTime::GetCurrentTime();
	szStr.Format("%04.04d/%d/%d", tm.GetYear(), tm.GetMonth(), tm.GetDay());

	x = m_Rect_Time.left + 35;
	y = m_Rect_Time.top + 10;
	dc.SetTextColor(RGB(0, 0, 0));
	dc.TextOutA(x, y, szStr);

	szStr.Format("%02.02d:%02.02d:%02.02d", tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	x = m_Rect_Time.left + 35;
	y = m_Rect_Time.top + 40;
	dc.TextOutA(x, y, szStr);

	dc.SelectObject(def_font);
	font.DeleteObject();

	// Bitmap
	if (m_bBmpUpdate){
		for (i = 0; i < PM_MAX; i++) {
			if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
				m_pIODB->IOPointRead(m_ioHdlSensor[i], &m_unsSensorStatus[i]);
				m_pIODB->IOPointRead(m_ioHdlStatus[i], &m_unsEPDStatus[i]);
			}
			else {
				m_unsSensorStatus[i] = 0;
				m_unsEPDStatus[i] = 0;
			}
			// Sensor Status
			pdc = GetDlgItem(UID_SensorStatus[i])->GetWindowDC();		// 描画用のデバイス コンテキスト
			if (pdc) {
				cDC.CreateCompatibleDC(pdc);

				curr_bmp.LoadBitmap(m_bmpSensorStatus[m_unsSensorStatus[i]]);
				curr_bmp.GetBitmap(&bmp);

				if (pOldBitmap == NULL)
					pOldBitmap = cDC.SelectObject(&curr_bmp);
				else
					cDC.SelectObject(&curr_bmp);

				pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

				GetDlgItem(UID_SensorStatus[i])->ReleaseDC(pdc);
				if (pOldBitmap) {
					pOldBitmap->DeleteObject();
					pOldBitmap = NULL;
				}
				curr_bmp.DeleteObject();
				cDC.DeleteDC();
			}

			// EPD Status
			pdc = GetDlgItem(UID_EPDStatus[i])->GetWindowDC();		// 描画用のデバイス コンテキスト
			if (pdc) {
				cDC.CreateCompatibleDC(pdc);

				curr_bmp.LoadBitmap(m_bmpEPDStatus[m_unsEPDStatus[i]]);
				curr_bmp.GetBitmap(&bmp);

				if (pOldBitmap == NULL)
					pOldBitmap = cDC.SelectObject(&curr_bmp);
				else
					cDC.SelectObject(&curr_bmp);

				pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

				GetDlgItem(UID_EPDStatus[i])->ReleaseDC(pdc);
				if (pOldBitmap) {
					pOldBitmap->DeleteObject();
					pOldBitmap = NULL;
				}
				curr_bmp.DeleteObject();
				cDC.DeleteDC();
			}

		}/* for */
		// PLC Status 
		pdc = GetDlgItem(IDC_BMP_PLC)->GetWindowDC();		// 描画用のデバイス コンテキスト
		if (pdc) {
			cDC.CreateCompatibleDC(pdc);

			curr_bmp.LoadBitmap(m_bmpPLCStatus[m_unsPLCStatus]);
			curr_bmp.GetBitmap(&bmp);

			if (pOldBitmap == NULL)
				pOldBitmap = cDC.SelectObject(&curr_bmp);
			else
				cDC.SelectObject(&curr_bmp);

			pdc->TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &cDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(212, 208, 200));

			GetDlgItem(IDC_BMP_PLC)->ReleaseDC(pdc);
			if (pOldBitmap) {
				pOldBitmap->DeleteObject();
				pOldBitmap = NULL;
			}
			curr_bmp.DeleteObject();
			cDC.DeleteDC();
		}
		m_bBmpUpdate = FALSE;
	}
}


void CDlgHeadPage::OnBnClickedBtnMode()
{
	CDlgChangeEPDMode* pDlg;

	pDlg = new CDlgChangeEPDMode(this);
	pDlg->DoModal();

	delete pDlg;

}


BOOL CDlgHeadPage::OnEraseBkgnd(CDC* pDC)
{
	CDialogX::OnEraseBkgnd(pDC);

	m_BtnPassword->PasswordDraw();
	return TRUE;

}
