// CDlgSensorConfig.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgSensorConfig.h"


static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };

// CDlgSensorConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgSensorConfig, CDialogX)

CDlgSensorConfig::CDlgSensorConfig(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_SENSORCFG, pParent)
	, m_dCalib_A0(0)
{
	m_iPMNo = iPM;

	SetBackColor(180, 255, 180);
	CDialogX::Create(IDD_SENSORCFG, pParent);
}

CDlgSensorConfig::~CDlgSensorConfig()
{
	if (m_pEPDConfig) {
		m_pEPDConfig->WaveCfgFileSave();
	}

}

void CDlgSensorConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_INTEGRATIONTIME, m_io_IntegrationTime);
	DDX_Control(pDX, IDC_ED_INIT_UNITID, m_io_UnitID);
	DDX_Control(pDX, IDC_ED_INIT_SENSORID, m_io_SensorName);
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
	DDX_Control(pDX, IDC_ED_MAC_ADDRESS, m_io_MACAddress);
	DDX_Control(pDX, IDC_ED_INIT_IPADDR, m_io_Init_IPAddr);
	DDX_Control(pDX, IDC_ED_INIT_SUBNET, m_io_Init_SubNet);
	DDX_Control(pDX, IDC_ED_INIT_GATEWAY, m_io_Init_DefGateWay);
	DDX_Control(pDX, IDC_ED_INIT_PORTNO, m_io_PortNo);
	DDX_Control(pDX, IDC_ED_INIT_VENDER, m_io_VenderName);
	DDX_Control(pDX, IDC_ED_INIT_PRODUCT, m_io_ProductName);
	DDX_Control(pDX, IDC_ED_INIT_SERIAL, m_io_SerialNo);
	DDX_Control(pDX, IDC_ED_INIT_FIRMWARE, m_io_FirmWare);
	DDX_Control(pDX, IDC_ED_EPD_IP1, m_io_Edit_IP1);
	DDX_Control(pDX, IDC_ED_EPD_IP2, m_io_Edit_IP2);
	DDX_Control(pDX, IDC_ED_EPD_IP3, m_io_Edit_IP3);
	DDX_Control(pDX, IDC_ED_EPD_IP4, m_io_Edit_IP4);
	DDX_Control(pDX, IDC_ED_PC_IP1, m_io_PC_IP1);
	DDX_Control(pDX, IDC_ED_PC_IP2, m_io_PC_IP2);
	DDX_Control(pDX, IDC_ED_PC_IP3, m_io_PC_IP3);
	DDX_Control(pDX, IDC_ED_PC_IP4, m_io_PC_IP4);
	DDX_Text(pDX, IDC_ED_CALIBRATE_A, m_dCalib_A0);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B1, m_dCalib_B1);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B2, m_dCalib_B2);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B3, m_dCalib_B3);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B4, m_dCalib_B4);
	DDX_Text(pDX, IDC_ED_CALIBRATE_B5, m_dCalib_B5);
}


BEGIN_MESSAGE_MAP(CDlgSensorConfig, CDialogX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GETMACADDR, &CDlgSensorConfig::OnBnClickedBtnGetmacaddr)
	ON_BN_CLICKED(IDC_BTN_GETNETINFO, &CDlgSensorConfig::OnBnClickedBtnGetnetinfo)
	ON_BN_CLICKED(IDC_BTN_GETPORTNO, &CDlgSensorConfig::OnBnClickedBtnGetportno)
	ON_BN_CLICKED(IDC_BTN_GETPRODUCT, &CDlgSensorConfig::OnBnClickedBtnGetproduct)
	ON_BN_CLICKED(IDC_BTN_GETUNITINFO, &CDlgSensorConfig::OnBnClickedBtnGetunitinfo)
	ON_BN_CLICKED(IDC_BTN_READCALIB, &CDlgSensorConfig::OnBnClickedBtnReadcalib)
	ON_BN_CLICKED(IDC_BTN_READWAVELEN, &CDlgSensorConfig::OnBnClickedBtnReadwavelen)
	ON_BN_CLICKED(IDC_BTN_SETIPADDR, &CDlgSensorConfig::OnBnClickedBtnSetipaddr)
	ON_BN_CLICKED(IDC_BTN_GETWAVECNT2, &CDlgSensorConfig::OnBnClickedBtnIPAddress)
	ON_BN_CLICKED(IDC_BTN_SETINTEGRATION, &CDlgSensorConfig::OnBnClickedBtnSetintegration)
	ON_BN_CLICKED(IDC_BTN_GETINTEGRATE, &CDlgSensorConfig::OnBnClickedBtnGetintegrate)
END_MESSAGE_MAP()


// CDlgSensorConfig メッセージ ハンドラー


BOOL CDlgSensorConfig::OnInitDialog()
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

	if(m_pEPDConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
		m_pWaveData_Eth = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_Eth();
	else
		m_pWaveData_Eth = NULL;

	szIOName.Format("PM%d.EPD.MACAddr", (m_iPMNo + 1));
	m_io_MACAddress.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
	
	szIOName.Format("PM%d.EPD.FixIPAddr", (m_iPMNo + 1));
	m_io_Init_IPAddr.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixSubNetAddr", (m_iPMNo + 1));
	m_io_Init_SubNet.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixGateWayAddr", (m_iPMNo + 1));
	m_io_Init_DefGateWay.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixPortNo", (m_iPMNo + 1));
	m_io_PortNo.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixPortNo", (m_iPMNo + 1));
	m_io_PortNo.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixVender", (m_iPMNo + 1));
	m_io_VenderName.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixProduct", (m_iPMNo + 1));
	m_io_ProductName.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixSerial", (m_iPMNo + 1));
	m_io_SerialNo.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.EPD.FixFirmware", (m_iPMNo + 1));
	m_io_FirmWare.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.UnitID", (m_iPMNo + 1));
	m_io_UnitID.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.SensorName", (m_iPMNo + 1));
	m_io_SensorName.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

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

	szIOName.Format("PM%d.WaveLengthUpper", (m_iPMNo + 1));
	m_io_UpperWaveLen.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.WaveLengthLower", (m_iPMNo + 1));
	m_io_LowerWaveLen.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	szIOName.Format("PM%d.IntegrationTime", (m_iPMNo + 1));
	m_io_IntegrationTime.SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));

	m_io_Edit_IP1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD_EDIT_IP1"));
	m_io_Edit_IP2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD_EDIT_IP2"));
	m_io_Edit_IP3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD_EDIT_IP3"));
	m_io_Edit_IP4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.EPD_EDIT_IP4"));

	m_io_PC_IP1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC_EDIT_IP1"));
	m_io_PC_IP2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC_EDIT_IP2"));
	m_io_PC_IP3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC_EDIT_IP3"));
	m_io_PC_IP4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.PC_EDIT_IP4"));

	m_io_MACAddress.SetSpecialColor(1);
	m_io_Init_IPAddr.SetSpecialColor(1);
	m_io_Init_SubNet.SetSpecialColor(1);
	m_io_Init_DefGateWay.SetSpecialColor(1);
	m_io_PortNo.SetSpecialColor(1);
	m_io_PortNo.SetSpecialColor(1);
	m_io_VenderName.SetSpecialColor(1);
	m_io_ProductName.SetSpecialColor(1);
	m_io_SerialNo.SetSpecialColor(1);
	m_io_FirmWare.SetSpecialColor(1);
	m_io_UnitID.SetSpecialColor(1);
	m_io_SensorName.SetSpecialColor(1);
#if 0
	m_io_Calibration_A.SetSpecialColor(1);
	m_io_Calibration_B1.SetSpecialColor(1);
	m_io_Calibration_B2.SetSpecialColor(1);
	m_io_Calibration_B3.SetSpecialColor(1);
	m_io_Calibration_B4.SetSpecialColor(1);
	m_io_Calibration_B5.SetSpecialColor(1);
#endif
	m_io_UpperWaveLen.SetSpecialColor(1);
	m_io_LowerWaveLen.SetSpecialColor(1);
	m_io_IntegrationTime.SetSpecialColor(1);
	m_io_Edit_IP1.SetSpecialColor(1);
	m_io_Edit_IP2.SetSpecialColor(1);
	m_io_Edit_IP3.SetSpecialColor(1);
	m_io_Edit_IP4.SetSpecialColor(1);
	m_io_PC_IP1.SetSpecialColor(1);
	m_io_PC_IP2.SetSpecialColor(1);
	m_io_PC_IP3.SetSpecialColor(1);
	m_io_PC_IP4.SetSpecialColor(1);

	m_io_MACAddress.UpdateDisplay();
	m_io_Init_IPAddr.UpdateDisplay();
	m_io_Init_SubNet.UpdateDisplay();
	m_io_Init_DefGateWay.UpdateDisplay();
	m_io_PortNo.UpdateDisplay();
	m_io_PortNo.UpdateDisplay();
	m_io_VenderName.UpdateDisplay();
	m_io_ProductName.UpdateDisplay();
	m_io_SerialNo.UpdateDisplay();
	m_io_FirmWare.UpdateDisplay();
	m_io_UnitID.UpdateDisplay();
	m_io_SensorName.UpdateDisplay();
#if 0
	m_io_Calibration_A.UpdateDisplay();
	m_io_Calibration_B1.UpdateDisplay();
	m_io_Calibration_B2.UpdateDisplay();
	m_io_Calibration_B3.UpdateDisplay();
	m_io_Calibration_B4.UpdateDisplay();
	m_io_Calibration_B5.UpdateDisplay();
#endif
	m_io_UpperWaveLen.UpdateDisplay();
	m_io_LowerWaveLen.UpdateDisplay();
	m_io_IntegrationTime.UpdateDisplay();
	m_io_Edit_IP1.UpdateDisplay();
	m_io_Edit_IP2.UpdateDisplay();
	m_io_Edit_IP3.UpdateDisplay();
	m_io_Edit_IP4.UpdateDisplay();
	m_io_PC_IP1.UpdateDisplay();
	m_io_PC_IP2.UpdateDisplay();
	m_io_PC_IP3.UpdateDisplay();
	m_io_PC_IP4.UpdateDisplay();
	m_pIODB->IOPointWrite(m_io_Edit_IP1.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[0]);
	m_pIODB->IOPointWrite(m_io_Edit_IP2.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[1]);
	m_pIODB->IOPointWrite(m_io_Edit_IP3.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[2]);
	m_pIODB->IOPointWrite(m_io_Edit_IP4.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[3]);

	m_pIODB->IOPointWrite(m_io_PC_IP1.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[0]);
	m_pIODB->IOPointWrite(m_io_PC_IP2.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[1]);
	m_pIODB->IOPointWrite(m_io_PC_IP3.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[2]);
	m_pIODB->IOPointWrite(m_io_PC_IP4.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[3]);


	m_dCalib_A0 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dA0;
	m_dCalib_B1 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB1;
	m_dCalib_B2 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB2;
	m_dCalib_B3 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB3;
	m_dCalib_B4 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB4;
	m_dCalib_B5 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB5;


	UpdateData(FALSE);

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgSensorConfig::OnTimer(UINT_PTR nIDEvent)
{
	int		iUpdate = FALSE;

	m_io_MACAddress.ChkValueChange();
	m_io_Init_IPAddr.ChkValueChange();
	m_io_Init_SubNet.ChkValueChange();
	m_io_Init_DefGateWay.ChkValueChange();
	m_io_PortNo.ChkValueChange();
	m_io_PortNo.ChkValueChange();
	m_io_VenderName.ChkValueChange();
	m_io_ProductName.ChkValueChange();
	m_io_SerialNo.ChkValueChange();
	m_io_FirmWare.ChkValueChange();
	m_io_UnitID.ChkValueChange();
	m_io_SensorName.ChkValueChange();
#if 0
	m_io_Calibration_A.ChkValueChange();
	m_io_Calibration_B1.ChkValueChange();
	m_io_Calibration_B2.ChkValueChange();
	m_io_Calibration_B3.ChkValueChange();
	m_io_Calibration_B4.ChkValueChange();
	m_io_Calibration_B5.ChkValueChange();
#endif
	m_io_UpperWaveLen.ChkValueChange();
	m_io_LowerWaveLen.ChkValueChange();
	m_io_IntegrationTime.ChkValueChange();
	m_io_Edit_IP1.ChkValueChange();
	m_io_Edit_IP2.ChkValueChange();
	m_io_Edit_IP3.ChkValueChange();
	m_io_Edit_IP4.ChkValueChange();
	m_io_PC_IP1.ChkValueChange();
	m_io_PC_IP2.ChkValueChange();
	m_io_PC_IP3.ChkValueChange();
	m_io_PC_IP4.ChkValueChange();

	if (m_io_Edit_IP1.m_bUpdate) {
		m_io_Edit_IP1.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_Edit_IP1.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[0]);
		iUpdate = TRUE;
	}
	if (m_io_Edit_IP2.m_bUpdate) {
		m_io_Edit_IP2.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_Edit_IP2.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[1]);
		iUpdate = TRUE;
	}
	if (m_io_Edit_IP3.m_bUpdate) {
		m_io_Edit_IP3.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_Edit_IP3.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[2]);
		iUpdate = TRUE;
	}
	if (m_io_Edit_IP4.m_bUpdate) {
		m_io_Edit_IP4.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_Edit_IP4.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[3]);
		iUpdate = TRUE;
	}

	if (m_io_PC_IP1.m_bUpdate) {
		m_io_PC_IP1.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_PC_IP1.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[0]);
		iUpdate = TRUE;
	}
	if (m_io_PC_IP2.m_bUpdate) {
		m_io_PC_IP2.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_PC_IP2.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[1]);
		iUpdate = TRUE;
	}
	if (m_io_PC_IP3.m_bUpdate) {
		m_io_PC_IP3.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_PC_IP3.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[2]);
		iUpdate = TRUE;
	}
	if (m_io_PC_IP4.m_bUpdate) {
		m_io_PC_IP4.m_bUpdate = FALSE;
		m_pIODB->IOPointRead(m_io_PC_IP4.GetIOHdl(), &m_pEPDConfig->m_iMyIPAddress[3]);
		iUpdate = TRUE;
	}
	if (iUpdate)
		m_pEPDConfig->WaveCfgFileSave();

	CDialogX::OnTimer(nIDEvent);
}


void CDlgSensorConfig::OnBnClickedBtnGetmacaddr()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixMACAddr() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnGetnetinfo()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixNetwork() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnGetportno()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixPortNo() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnGetproduct()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixProductInfo() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnGetunitinfo()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixUnitInfo() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnReadcalib()
{
	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixWaveParam() == TRUE) {

			m_dCalib_A0 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dA0;
			m_dCalib_B1 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB1;
			m_dCalib_B2 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB2;
			m_dCalib_B3 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB3;
			m_dCalib_B4 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB4;
			m_dCalib_B5 = m_pEPDConfig->m_tWaveCaribrate[m_iPMNo].dB5;
			UpdateData(FALSE);
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		}
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnReadwavelen()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_FixWaveScale() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnSetipaddr()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_NetworkParamWrite() == TRUE)
		{
			if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ARP_Add() == TRUE)
				MessageBox("Parameter Set Success!", "Parameter Read", MB_OK);
			else
				MessageBox("Parameter Set Error!", "Parameter Read", MB_ICONERROR);
		}
		else
			MessageBox("Parameter Set Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnIPAddress()
{
	//	if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_NetworkParamRead() == TRUE)


	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_Network() == TRUE) {
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);

			m_pIODB->IOPointWrite(m_io_Edit_IP1.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[0]);
			m_pIODB->IOPointWrite(m_io_Edit_IP2.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[1]);
			m_pIODB->IOPointWrite(m_io_Edit_IP3.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[2]);
			m_pIODB->IOPointWrite(m_io_Edit_IP4.GetIOHdl(), &m_pEPDConfig->m_tEPDEtherConfig[m_iPMNo].iIP_Addr[3]);
		}
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnSetintegration()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamWrite_IntegrationTime() == TRUE)
			MessageBox("Parameter Set Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Set Error!", "Parameter Read", MB_ICONERROR);
	}
}


void CDlgSensorConfig::OnBnClickedBtnGetintegrate()
{
	if (m_pWaveData_Eth) {
		if (m_pWaveData_Eth->m_pEthDriver[m_iPMNo]->UDP_ParamRead_IntegrationTime() == TRUE)
			MessageBox("Parameter Read Success!", "Parameter Read", MB_OK);
		else
			MessageBox("Parameter Read Error!", "Parameter Read", MB_ICONERROR);
	}
}
