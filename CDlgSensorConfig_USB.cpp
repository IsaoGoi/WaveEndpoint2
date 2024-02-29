// CDlgSensorConfig_USB.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgSensorConfig_USB.h"
#include "WaveDriver.h"


static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };

static UINT UID_StatusFLD[5] = {
IDC_ED_STAT_BIT0,
IDC_ED_STAT_BIT3,
IDC_ED_STAT_BIT4,
IDC_ED_STAT_BIT5,
IDC_ED_STAT_BIT7
};

static UINT UID_SensorStatus[5] = {
	/* 0 */	IDC_BMP_BIT0,
	/* 1 */	IDC_BMP_BIT3,
	/* 2 */	IDC_BMP_BIT4,
	/* 3 */	IDC_BMP_BIT5,
	/* 4 */	IDC_BMP_BIT7,
};
static UINT m_bmpOffError[4] =
{
	/* 0 */IDB_RED_CIRCLE,		//Error(Invalid)
	/* 1 */IDB_GRAY_CIRCLE,		//Disable
};
static UINT m_bmpOnError[4] =
{
	/* 0 */IDB_GRAY_CIRCLE,		//Disable
	/* 1 */IDB_RED_CIRCLE,		//Error(Invalid)
};



// CDlgSensorConfig_USB ダイアログ

IMPLEMENT_DYNAMIC(CDlgSensorConfig_USB, CDialogX)

CDlgSensorConfig_USB::CDlgSensorConfig_USB(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_SENSORCFG, pParent)
	, m_dCalib_A0(0)
	, m_dCalib_B1(0)
	, m_dCalib_B2(0)
	, m_dCalib_B3(0)
	, m_dCalib_B4(0)
	, m_dCalib_B5(0)
{
	m_iPMNo = iPM;

	SetBackColor(180, 255, 180);
	CDialogX::Create(IDD_SENSORCFG_USB, pParent);
}

CDlgSensorConfig_USB::~CDlgSensorConfig_USB()
{
}

void CDlgSensorConfig_USB::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_INTEGRATIONTIME, m_io_IntegrationTime);
	DDX_Control(pDX, IDC_ED_GAIN2, m_io_Gain);
	DDX_Control(pDX, IDC_ED_TRIGGEREDGE, m_io_TriggerEdge);
	DDX_Control(pDX, IDC_ED_TRIGGERMODE, m_io_TriggerMode);
	DDX_Control(pDX, IDC_ED_EEPROMLOADDEF, m_io_EEPRomLoadDef);
	DDX_Control(pDX, IDC_ED_UNITID, m_io_UnitID);
	DDX_Control(pDX, IDC_ED_SENSORNAME, m_io_SensorName);
	DDX_Control(pDX, IDC_ED_SERIALNO, m_io_SerialNo);
	DDX_Control(pDX, IDC_ED_UPPER_WAVE, m_io_UpperWaveLen);
	DDX_Control(pDX, IDC_ED_LOWER_WAVE, m_io_LowerWaveLen);
#if 0
	DDX_Control(pDX, IDC_ED_CALIBRATE_A, m_io_Calibration_A);
	DDX_Control(pDX, IDC_ED_CALIBRATE_B1, m_io_Calibration_B1);
	DDX_Control(pDX, IDC_ED_CALIBRATE_B2, m_io_Calibration_B2);
	DDX_Control(pDX, IDC_ED_CALIBRATE_B3, m_io_Calibration_B3);
	DDX_Control(pDX, IDC_ED_CALIBRATE_B4, m_io_Calibration_B4);
	DDX_Control(pDX, IDC_ED_CALIBRATE_B5, m_io_Calibration_B5);
#endif
	DDX_Control(pDX, IDC_ED_COOLCOUNT, m_io_CoolingCnt);
	DDX_Control(pDX, IDC_ED_STAT_BIT0, m_io_Status_Bit0);
	DDX_Control(pDX, IDC_ED_STAT_BIT3, m_io_Status_Bit3);
	DDX_Control(pDX, IDC_ED_STAT_BIT4, m_io_Status_Bit4);
	DDX_Control(pDX, IDC_ED_STAT_BIT5, m_io_Status_Bit5);
	DDX_Control(pDX, IDC_ED_STAT_BIT7, m_io_Status_Bit7);
	DDX_Control(pDX, IDC_ED_DLL_VERSION, m_io_DLLVersion);
	DDX_Text(pDX, IDC_ED_CALIBRATE_A, m_dCalib_A0);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B1, m_dCalib_B1);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B2, m_dCalib_B2);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B3, m_dCalib_B3);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B4, m_dCalib_B4);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B5, m_dCalib_B5);
}


BEGIN_MESSAGE_MAP(CDlgSensorConfig_USB, CDialogX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GETPRM, &CDlgSensorConfig_USB::OnBnClickedBtnGetprm)
	ON_BN_CLICKED(IDC_BTN_SETPRM, &CDlgSensorConfig_USB::OnBnClickedBtnSetprm)
	ON_BN_CLICKED(IDC_BTN_SETLOADDEF, &CDlgSensorConfig_USB::OnBnClickedBtnSetloaddef)
	ON_BN_CLICKED(IDC_BTN_READINFO, &CDlgSensorConfig_USB::OnBnClickedBtnReadinfo)
	ON_BN_CLICKED(IDC_BTN_WRITEINFO, &CDlgSensorConfig_USB::OnBnClickedBtnWriteinfo)
	ON_BN_CLICKED(IDC_BTN_READCALIB, &CDlgSensorConfig_USB::OnBnClickedBtnReadcalib)
	ON_BN_CLICKED(IDC_BTN_WRITECALIB, &CDlgSensorConfig_USB::OnBnClickedBtnWritecalib)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgSensorConfig_USB メッセージ ハンドラー


BOOL CDlgSensorConfig_USB::OnInitDialog()
{
	CRect		rc;
	CString	szIOName('\0', 32);
	int		i;

	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(160, 30);
	rc.OffsetRect(140, 0);
	MoveWindow(rc);


	m_bmpPC.LoadBitmapA(m_uidPCBmp[m_iPMNo]);
	((CStatic*)GetDlgItem(IDC_BMP_PC))->SetBitmap(m_bmpPC);
	

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	if (m_pEPDConfig->m_tEPDConfig.Driver == USB_DRV)
		m_pWaveData_USB = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_USB();
	else
		m_pWaveData_USB = NULL;

	szIOName.Format("PM%d.IntegrationTime", (m_iPMNo + 1));
	m_io_IntegrationTime.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Gain", (m_iPMNo + 1));
	m_io_Gain.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.TriggerEdge", (m_iPMNo + 1));
	m_io_TriggerEdge.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.TriggerMode", (m_iPMNo + 1));
	m_io_TriggerMode.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	m_io_EEPRomLoadDef.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EEPRom.LoadPrm"));
	m_pIODB->IOPointWrite(m_io_EEPRomLoadDef.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.unsEEPRomLoadParam[m_iPMNo]);


	szIOName.Format("PM%d.UnitID", (m_iPMNo + 1));
	m_io_UnitID.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.SensorName", (m_iPMNo + 1));
	m_io_SensorName.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.SerialNo", (m_iPMNo + 1));
	m_io_SerialNo.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.WaveLengthUpper", (m_iPMNo + 1));
	m_io_UpperWaveLen.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.WaveLengthLower", (m_iPMNo + 1));
	m_io_LowerWaveLen.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

#if 0
	szIOName.Format("PM%d.CalibRate_A", (m_iPMNo + 1));
	m_io_Calibration_A.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.CalibRate_B1", (m_iPMNo + 1));
	m_io_Calibration_B1.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.CalibRate_B2", (m_iPMNo + 1));
	m_io_Calibration_B2.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.CalibRate_B3", (m_iPMNo + 1));
	m_io_Calibration_B3.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.CalibRate_B4", (m_iPMNo + 1));
	m_io_Calibration_B4.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.CalibRate_B5", (m_iPMNo + 1));
	m_io_Calibration_B5.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
#endif
	szIOName.Format("PM%d.Cooling.Cnt", (m_iPMNo + 1));
	m_io_CoolingCnt.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Status.Bit0", (m_iPMNo + 1));
	m_io_Status_Bit0.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Status.Bit3", (m_iPMNo + 1));
	m_io_Status_Bit3.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Status.Bit4", (m_iPMNo + 1));
	m_io_Status_Bit4.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Status.Bit5", (m_iPMNo + 1));
	m_io_Status_Bit5.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	szIOName.Format("PM%d.Status.Bit7", (m_iPMNo + 1));
	m_io_Status_Bit7.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.DLLVersion", (m_iPMNo + 1));
	m_io_DLLVersion.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	m_io_IntegrationTime.SetSpecialColor(1);
	m_io_Gain.SetSpecialColor(1);
	m_io_TriggerEdge.SetSpecialColor(1);
	m_io_TriggerMode.SetSpecialColor(1);
	m_io_EEPRomLoadDef.SetSpecialColor(1);
	m_io_UnitID.SetSpecialColor(1);
	m_io_SensorName.SetSpecialColor(1);
	m_io_SerialNo.SetSpecialColor(1);
	m_io_UpperWaveLen.SetSpecialColor(1);
	m_io_LowerWaveLen.SetSpecialColor(1);
#if 0
	m_io_Calibration_A.SetSpecialColor(1);
	m_io_Calibration_B1.SetSpecialColor(1);
	m_io_Calibration_B2.SetSpecialColor(1);
	m_io_Calibration_B3.SetSpecialColor(1);
	m_io_Calibration_B4.SetSpecialColor(1);
	m_io_Calibration_B5.SetSpecialColor(1);
#endif
	m_io_CoolingCnt.SetSpecialColor(1);
	m_io_Status_Bit0.SetSpecialColor(1);
	m_io_Status_Bit3.SetSpecialColor(1);
	m_io_Status_Bit4.SetSpecialColor(1);
	m_io_Status_Bit5.SetSpecialColor(1);
	m_io_Status_Bit7.SetSpecialColor(1);
	m_io_DLLVersion.SetSpecialColor(1);

	m_io_IntegrationTime.UpdateDisplay();
	m_io_Gain.UpdateDisplay();
	m_io_TriggerEdge.UpdateDisplay();
	m_io_TriggerMode.UpdateDisplay();
	m_io_EEPRomLoadDef.UpdateDisplay();
	m_io_UnitID.UpdateDisplay();
	m_io_SensorName.UpdateDisplay();
	m_io_SerialNo.UpdateDisplay();
	m_io_UpperWaveLen.UpdateDisplay();
	m_io_LowerWaveLen.UpdateDisplay();
#if 0
	m_io_Calibration_A.UpdateDisplay();
	m_io_Calibration_B1.UpdateDisplay();
	m_io_Calibration_B2.UpdateDisplay();
	m_io_Calibration_B3.UpdateDisplay();
	m_io_Calibration_B4.UpdateDisplay();
	m_io_Calibration_B5.UpdateDisplay();
#endif
	m_io_CoolingCnt.UpdateDisplay();
	m_io_Status_Bit0.UpdateDisplay();
	m_io_Status_Bit3.UpdateDisplay();
	m_io_Status_Bit4.UpdateDisplay();
	m_io_Status_Bit5.UpdateDisplay();
	m_io_Status_Bit7.UpdateDisplay();
	m_io_DLLVersion.UpdateDisplay();

	for(i=0;i<5;i++)
		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(UID_StatusFLD[i]))->GetIOHdl(), &m_unsBitStatus[i]);


	m_dCalib_A0 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dA0;
	m_dCalib_B1 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB1;
	m_dCalib_B2 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB2;
	m_dCalib_B3 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB3;
	m_dCalib_B4 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB4;
	m_dCalib_B5 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB5;


	UpdateData(FALSE);
	Invalidate();

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgSensorConfig_USB::OnTimer(UINT_PTR nIDEvent)
{
	int			i;
	unsigned	unsVal;
	int			bUpdate=FALSE;

	m_io_IntegrationTime.ChkValueChange();
	m_io_Gain.ChkValueChange();
	m_io_TriggerEdge.ChkValueChange();
	m_io_TriggerMode.ChkValueChange();
	m_io_EEPRomLoadDef.ChkValueChange();
	m_io_UnitID.ChkValueChange();
	m_io_SensorName.ChkValueChange();
	m_io_SerialNo.ChkValueChange();
	m_io_UpperWaveLen.ChkValueChange();
	m_io_LowerWaveLen.ChkValueChange();
#if 0
	m_io_Calibration_A.ChkValueChange();
	m_io_Calibration_B1.ChkValueChange();
	m_io_Calibration_B2.ChkValueChange();
	m_io_Calibration_B3.ChkValueChange();
	m_io_Calibration_B4.ChkValueChange();
	m_io_Calibration_B5.ChkValueChange();
#endif
	m_io_CoolingCnt.ChkValueChange();
	m_io_Status_Bit0.ChkValueChange();
	m_io_Status_Bit3.ChkValueChange();
	m_io_Status_Bit4.ChkValueChange();
	m_io_Status_Bit5.ChkValueChange();
	m_io_Status_Bit7.ChkValueChange();
	m_io_DLLVersion.ChkValueChange();

	if(m_pWaveData_USB)
		m_pWaveData_USB->GetStatusRequest(m_iPMNo);


	for (i = 0; i < 5; i++) {
		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(UID_StatusFLD[i]))->GetIOHdl(), &unsVal);
		if (m_unsBitStatus[i] != unsVal) {
			m_unsBitStatus[i] = unsVal;
			bUpdate = TRUE;
		}
	}
	if (bUpdate)
		Invalidate(FALSE);

	CDialogX::OnTimer(nIDEvent);
}


void CDlgSensorConfig_USB::OnBnClickedBtnGetprm()
{
	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->GetParameter(m_iPMNo) == USBDEV_SUCCESS)
			MessageBox("Get Sensor Parameter OK!", "Get Parameter");
		else
			MessageBox("Get Sensor Parameter NG.", "Get Parameter", MB_ICONERROR);
	}
}


void CDlgSensorConfig_USB::OnBnClickedBtnSetprm()
{
	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->SetParameter(m_iPMNo) == USBDEV_SUCCESS)
			MessageBox("Set Sensor Parameter OK!", "Set Parameter");
		else
			MessageBox("Set Sensor Parameter NG.", "Set Parameter", MB_ICONERROR);
	}
}


void CDlgSensorConfig_USB::OnBnClickedBtnSetloaddef()
{
	unsigned	unsVal;

	m_pIODB->IOPointRead(m_io_EEPRomLoadDef.GetIOHdl(), &unsVal);
	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->SetEEPRomDefaultParameter(m_iPMNo, (int)unsVal) == USBDEV_SUCCESS) {
			MessageBox("EEPRom Load Default Parameter Setting OK!", "Load Default Parameter");
			m_pEPDConfig->m_tEPDConfig.unsEEPRomLoadParam[m_iPMNo] = unsVal;
			m_pEPDConfig->ConfigFileSave();
		}
		else
			MessageBox("EEPRom Load Default Parameter Setting NG.", "Load Default Parameter", MB_ICONERROR);
	}
}



void CDlgSensorConfig_USB::OnBnClickedBtnReadinfo()
{
	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->ReadUnitInformation(m_iPMNo) == USBDEV_SUCCESS)
			MessageBox("Read Unit Information OK!", "Unit Information");
		else
			MessageBox("Read Unit Information NG.", "Unit Information", MB_ICONERROR);
	}
}

void CDlgSensorConfig_USB::OnBnClickedBtnWriteinfo()
{
	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->WriteUnitInformation(m_iPMNo) == USBDEV_SUCCESS)
			MessageBox("Write Unit Information OK!", "Unit Information");
		else
			MessageBox("Write Unit Information NG.", "Unit Information", MB_ICONERROR);
	}
}

void CDlgSensorConfig_USB::OnBnClickedBtnReadcalib()
{
	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->ReadCalibrationValue(m_iPMNo) == USBDEV_SUCCESS) {

			m_dCalib_A0 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dA0;
			m_dCalib_B1 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB1;
			m_dCalib_B2 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB2;
			m_dCalib_B3 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB3;
			m_dCalib_B4 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB4;
			m_dCalib_B5 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB5;

			UpdateData(FALSE);
			MessageBox("Read Calibtarion Value OK!", "Caribration Value");
		}

		else
			MessageBox("Read Calibtarion Value NG.", "Caribration Value", MB_ICONERROR);
	}
}

void CDlgSensorConfig_USB::OnBnClickedBtnWritecalib()
{
	UpdateData(TRUE);

	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dA0 = m_dCalib_A0;
	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB1 = m_dCalib_B1;
	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB2 = m_dCalib_B2;
	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB3 = m_dCalib_B3;
	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB4 = m_dCalib_B4;
	m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB5 = m_dCalib_B5;


	if (m_pWaveData_USB) {
		if (m_pWaveData_USB->WriteCalibrationValue(m_iPMNo) == USBDEV_SUCCESS)
			MessageBox("Write Calibtarion Value OK!", "Caribration Value");
		else
			MessageBox("Write Calibtarion Value NG.", "Caribration Value", MB_ICONERROR);
	}
}

void CDlgSensorConfig_USB::OnPaint()
{
	CBitmap		curr_bmp;
	BITMAP		bmp;
	CBitmap* pOldBitmap = NULL;
	CDC* pdc;
	CDC			cDC;
	int			i;
	unsigned	unsVal;

	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。

	for (i = 0; i < 5 ; i++) {
		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(UID_StatusFLD[i]))->GetIOHdl(), &unsVal);
		pdc = GetDlgItem(UID_SensorStatus[i])->GetWindowDC();		// 描画用のデバイス コンテキスト
		if (pdc) {
			cDC.CreateCompatibleDC(pdc);
			if (i == 0)
				curr_bmp.LoadBitmap(m_bmpOffError[unsVal]);
			else 
				curr_bmp.LoadBitmap(m_bmpOnError[unsVal]);
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
	}/* for */
}
