#include "pch.h"
#include "mmsystem.h"
#include "WaveEndPoint.h"
#include "WaveDriver.h"
#include "clockread_ms.h"


static	unsigned			m_uiEvtID[PM_MAX] = {0xFFFFFFFF,0xFFFFFFFF ,0xFFFFFFFF ,0xFFFFFFFF ,0xFFFFFFFF };
plf::nanotimer				m_Sampling_timer;
static	int					m_iSamplingEnable[PM_MAX] = {FALSE,FALSE, FALSE, FALSE, FALSE};
static	CIODB*		m_pIODB;
static	int         m_iohdlSensorStatus[PM_MAX];
static	int         m_iWaitRecover[PM_MAX] = {0,0,0,0,0};
static	int         m_iReStartReq[PM_MAX] = { 0,0,0,0,0 };

static	PUNIT_PARAMETER	m_tParam[PM_MAX];
static	char		m_szUnitID[PM_MAX][32+1];

static unsigned		m_unsSensorStatus[PM_MAX + 1] = { 1,1,1,1,1 };



#if __DEBUG_TEST__
static	unsigned short	usSimurationWave[PM_MAX] = { 0,1024*5,1024 * 10,1024 * 20,1024 * 30 };
static	int				m_iDirettionUp[PM_MAX] = {TRUE,FALSE ,TRUE ,TRUE ,FALSE};

static	int		m_iRandomPt = 0;
static	int		m_iRandomCalc[PM_MAX][16] = {
	{ 16,255,8,270,66,1040,90,128,4020,300,3,512,10,9000,99,100 },
	{ 1,1,1,1,1,1,1,2,2,3,1,2,1,9,2,2 },
	{ 2,255,8,270,866,1040,90,128,44,300,3,512,30,90,199,400 },
	{ 904,255,8,270,66,1040,90,128,1120,300,3,512,3310,1900,939,800 },
	{ 10,25,8,27,66,14,9,18,42,3,3,5,2,10,9,1 },
};
#endif

static unsigned short	m_usTestData = 0;

void CALLBACK EPDSampling_USB(UINT uEntID, UINT uMsgID, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	int			iPM;
	int			rv;
	int			i,j,k;
	int			iSensorStatus;
	unsigned	unsVal;
	unsigned	unsOldVal;
	long		hDevHandle;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	CWaveData_USB* pWaveData = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_USB();
	CWaveSamplingSave	*m_pWaveSampling = ((CWaveEndPointApp*)AfxGetApp())->GetWaveSamplingSave();

	iPM = dwUser;


#ifdef _CYCLETIME_CHECK_
	double			dmsec;
	char			szDebug[32 + 1];

	dmsec = m_Sampling_timer.get_elapsed_ms();
	sprintf(szDebug, "WaveSampling=%fmsec\n", dmsec);
	TRACE(szDebug);
	m_Sampling_timer.start();
#endif

#ifdef __SENSOR__
	for (i = 0; i < 1024; i++)
		pWaveData->m_pWaveData[PM1]->usWaveData[0][i] = m_usTestData;

	m_usTestData += 200;

	if (m_pWaveSampling->m_iPCSamplingStatus[PM1] == SAMPLING_BUSY) {
		m_pWaveSampling->WaveSamplingReq(PM1, pWaveData->m_pWaveData[PM1]->usWaveData[0]);
	}
#endif



//	for (iPM = 0; iPM < PM_MAX; iPM++) {
	if (pConfig->m_tEPDConfig.iPCEnable[iPM] == FALSE)
		return;
	if(m_iSamplingEnable[iPM]==FALSE)
		return;

	if (m_iWaitRecover[iPM]) {
		if (m_iWaitRecover[iPM] == 1) {

			if (((CWaveEndPointApp*)AfxGetApp())->m_szUnitID[iPM][0] != '\0') {
				hDevHandle = USB_OpenTargetDevice(iPM, ((CWaveEndPointApp*)AfxGetApp())->m_szUnitID[iPM]);
				if (hDevHandle == INVALID_HANDLE_VALUE) {
					hDevHandle = USB_OpenDevice(iPM, 0x2909);
				}
			}
			else
				hDevHandle = USB_OpenDevice(iPM, 0x2909);

			if (hDevHandle != INVALID_HANDLE_VALUE) {
				USB_OpenPipe(iPM);

				//				unsVal = 1;		// Statsu = Normal
				//				m_pIODB->IOPointWrite(m_iohdlSensorStatus[iPM], &unsVal);
				m_iWaitRecover[iPM] = 0;
			}
			else {

				m_pIODB->IOPointRead(m_iohdlSensorStatus[iPM], &unsOldVal);
				if (m_unsSensorStatus[iPM] != unsOldVal) {
					m_pIODB->IOPointWrite(m_iohdlSensorStatus[iPM], &m_unsSensorStatus[iPM]);
				}
				m_iWaitRecover[iPM] = 100;
			}
		}
		else {
			--m_iWaitRecover[iPM];
		}
		return;
	}

	for (i = EPD_SAMPLING_POINT - 1; i > 0; i--) {
		memcpy((char *)pWaveData->m_pWaveData[iPM]->usWaveData[i], (char*)pWaveData->m_pWaveData[iPM]->usWaveData[i - 1], EPD_WAVE_TOTAL*2);
	}
	rv = USB_GetSensorData(iPM, (unsigned short)pConfig->m_tEPDConfig.iWaveMax, pWaveData->m_pWaveData[iPM]->usWaveData[0]);

	if (rv == USBDEV_SUCCESS) {
		if (m_pWaveSampling->m_iPCSamplingStatus[iPM] == SAMPLING_BUSY) {
			m_pWaveSampling->WaveSamplingReq(iPM, pWaveData->m_pWaveData[iPM]->usWaveData[0]);
		}
		// Statsu = Normal
		m_unsSensorStatus[iPM] = 1;
		m_pIODB->IOPointRead(m_iohdlSensorStatus[iPM], &unsOldVal);
		if (m_unsSensorStatus[iPM] != unsOldVal) {
			m_pIODB->IOPointWrite(m_iohdlSensorStatus[iPM], &m_unsSensorStatus[iPM]);
		}
	}
	else if (rv == USBDEV_BULK_NOT_UPDATED) {
		return;
	}
	else {
		// Read ErrorŽž‚Ì‚ÝCheck Device
		m_unsSensorStatus[iPM] = 1;
		iSensorStatus = USB_CheckDevice(iPM);

		if (iSensorStatus == USBDEV_CHECK_NORMAL) {
			rv = USB_GetSensorData(iPM, (unsigned short)pConfig->m_tEPDConfig.iWaveMax, pWaveData->m_pWaveData[iPM]->usWaveData[0]);

			if (rv == USBDEV_SUCCESS) {
				if (m_pWaveSampling->m_iPCSamplingStatus[iPM] == SAMPLING_BUSY) {
					m_pWaveSampling->WaveSamplingReq(iPM, pWaveData->m_pWaveData[iPM]->usWaveData[0]);
				}
			}
		}
		else if (iSensorStatus == USBDEV_CHECK_REMOVE) {
			m_unsSensorStatus[iPM] = 2;
		}
		else {
			m_unsSensorStatus[iPM] = 3;
		}
		m_pIODB->IOPointRead(m_iohdlSensorStatus[iPM], &unsOldVal);
		if (m_unsSensorStatus[iPM] != unsOldVal) {
			m_pIODB->IOPointWrite(m_iohdlSensorStatus[iPM], &m_unsSensorStatus[iPM]);
		}
		if (m_unsSensorStatus[iPM] != 1) {
			USB_ClosePipe(iPM);
			USB_CloseDevice(iPM);
			m_iWaitRecover[iPM] = 100;
		}
	}

#if 0
		for (i = 0; i < EPD_WAVE_TOTAL; i++) {
			pWaveData->m_pWaveData[iPM]->usWaveData[0][i] = usSimurationWave[iPM];
			if (m_iDirettionUp[iPM]) {
				if (65500 < (usSimurationWave[iPM] + m_iRandomCalc[iPM][m_iRandomPt])) {
					usSimurationWave[iPM] = 65535;
					m_iDirettionUp[iPM] = FALSE;
				}
				else
					usSimurationWave[iPM] += m_iRandomCalc[iPM][m_iRandomPt];
			}
			else {
				if (usSimurationWave[iPM] <= m_iRandomCalc[iPM][m_iRandomPt]) {
					usSimurationWave[iPM] = 0;
					m_iDirettionUp[iPM] = TRUE;
				}
				else
					usSimurationWave[iPM] -= m_iRandomCalc[iPM][m_iRandomPt];
			}
			if (++m_iRandomPt >= 16)
				m_iRandomPt = 0;
		}
		if (m_pWaveSampling->m_iPCSamplingStatus[iPM] == SAMPLING_BUSY) {
			m_pWaveSampling->WaveSamplingReq(iPM, pWaveData->m_pWaveData[iPM]->usWaveData[0]);
		}
#endif
//	}
	if (pWaveData->m_iTotal[iPM] < EPD_SAMPLING_POINT)
		pWaveData->m_iTotal[iPM]++;

	if (m_iReStartReq[iPM]) {
		if (m_unsSensorStatus[iPM] == 1) {
			USB_ClosePipe(iPM);
			USB_CloseDevice(iPM);
			m_iWaitRecover[iPM] = 10;
		}
		m_iReStartReq[iPM] = FALSE;
	}
}


CWaveData_USB::CWaveData_USB()
{
	int			i;
	long		m_hDevHandle;
	CString		szIOName('\0', 32);
	unsigned	unsVal;


	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	for (i = 0; i < PM_MAX; i++) {
		if (((CWaveEndPointApp*)AfxGetApp())->m_szUnitID[i][0] > ' ') {
			szIOName.Format("CTC.PC%d.EPD.Sensor", (i + 1));
			m_iohdlSensorStatus[i] = m_pIODB->IOPointGetHandle(szIOName);


			if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
				h_WaveData[i] = GlobalAlloc(GPTR, sizeof(WAVE_DATA));
				m_pWaveData[i] = (WAVE_DATA*)GlobalLock(h_WaveData[i]);
				WaveDriverInit(i);

				m_hDevHandle = USB_OpenTargetDevice(i, ((CWaveEndPointApp*)AfxGetApp())->m_szUnitID[i]);

				if (m_hDevHandle != INVALID_HANDLE_VALUE) {
					USB_OpenPipe(i);
					m_iSamplingEnable[i] = TRUE;

					unsVal = 1;		// Statsu = Normal
					m_pIODB->IOPointWrite(m_iohdlSensorStatus[i], &unsVal);

					GetParameter(i);
					if (m_tParam[i].iIntegrationTime < 5000) {
						m_tParam[i].iIntegrationTime = 5000;
						USB_SetParameter(i, &m_tParam[i]);
						GetParameter(i);
					}
					ReadUnitInformation(i);
					ReadCalibrationValue(i);
					GetStatusRequest(i);
					GetDllVersion(i);

				}
			}
			else {
				h_WaveData[i] = 0;
			}
		}
	}/* for */

	for (i = 0; i < PM_MAX; i++) {
		if (((CWaveEndPointApp*)AfxGetApp())->m_szUnitID[i][0] <= ' ') {
			szIOName.Format("CTC.PC%d.EPD.Sensor", (i + 1));
			m_iohdlSensorStatus[i] = m_pIODB->IOPointGetHandle(szIOName);


			if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
				h_WaveData[i] = GlobalAlloc(GPTR, sizeof(WAVE_DATA));
				m_pWaveData[i] = (WAVE_DATA*)GlobalLock(h_WaveData[i]);
				WaveDriverInit(i);

				m_hDevHandle = USB_OpenDevice(i, 0x2909);

				if (m_hDevHandle != INVALID_HANDLE_VALUE) {
					USB_OpenPipe(i);
					m_iSamplingEnable[i] = TRUE;

					unsVal = 1;		// Statsu = Normal
					m_pIODB->IOPointWrite(m_iohdlSensorStatus[i], &unsVal);

					GetParameter(i);
					if (m_tParam[i].iIntegrationTime < 5000) {
						m_tParam[i].iIntegrationTime = 5000;
						USB_SetParameter(i, &m_tParam[i]);
						GetParameter(i);
					}
					ReadUnitInformation(i);
					ReadCalibrationValue(i);
					GetStatusRequest(i);
					GetDllVersion(i);

				}
			}	
			else {
				h_WaveData[i] = 0;
			}
		}
	}/* for */



#ifdef _CYCLETIME_CHECK_
	m_Sampling_timer.start();
#endif

}


CWaveData_USB::~CWaveData_USB()
{
	int	i;

	for (i = 0; i < PM_MAX; i++) {
		if (m_uiEvtID[i] != 0xFFFFFFFF)
			timeKillEvent(m_uiEvtID[i]);
	}/* for */

	Sleep(1000);

	for (i = 0; i < PM_MAX; i++) {
		if (h_WaveData) {
			GlobalUnlock(h_WaveData[i]);
			GlobalFree(h_WaveData[i]);
			USB_ClosePipe(i);
			USB_CloseDevice(i);
			WaveDriverEnd(i);
		}
	}/* for */
}

void CWaveData_USB::StartWaveSampling()
{
	int		iSamplingTime;
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		m_iTotal[i] = 0;
		if (m_iSamplingEnable[i] == TRUE) {
//			iSamplingTime = m_tParam[i].iIntegrationTime / 1000;
			iSamplingTime = 100;
			m_uiEvtID[i] = ::timeSetEvent(iSamplingTime, 1, EPDSampling_USB, i, TIME_PERIODIC);
		}
	}
}
int CWaveData_USB::GetParameter(int iPMNo)
{
	int				rv;
	int				iohdl;
	unsigned		unsVal;
	CString			szIOName('\0', 32);


	rv = USB_GetParameter(iPMNo, &m_tParam[iPMNo]);

	if (rv == USBDEV_SUCCESS) {
		szIOName.Format("PM%d.IntegrationTime", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &m_tParam[iPMNo].iIntegrationTime);

		szIOName.Format("PM%d.Gain", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		if (m_tParam[iPMNo].chGain == 0xFF)
			unsVal = 2;
		else
			unsVal = (unsigned)m_tParam[iPMNo].chGain;
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		szIOName.Format("PM%d.TriggerEdge", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		if (m_tParam[iPMNo].chTriggerEdge == 0xFF)
			unsVal = 2;
		else
			unsVal = (unsigned)m_tParam[iPMNo].chTriggerEdge;
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		szIOName.Format("PM%d.TriggerMode", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		unsVal = (unsigned)m_tParam[iPMNo].chTriggerMode;
		m_pIODB->IOPointWrite(iohdl, &unsVal);
	}
	return(rv);
}
int CWaveData_USB::SetParameter(int iPMNo)
{
	int				rv;
	int				iohdl;
	unsigned		unsVal;
	CString			szIOName('\0', 32);
	int				iSamplingTime;

	szIOName.Format("PM%d.IntegrationTime", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &m_tParam[iPMNo].iIntegrationTime);

	szIOName.Format("PM%d.Gain", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &unsVal);

	if (unsVal == 2)
		m_tParam[iPMNo].chGain = 0xFF;
	else
		m_tParam[iPMNo].chGain = (char)unsVal;

	szIOName.Format("PM%d.TriggerEdge", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &unsVal);
	if (unsVal == 2)
		m_tParam[iPMNo].chTriggerEdge = 0xFF;
	else
		m_tParam[iPMNo].chTriggerEdge = (char)unsVal;

	szIOName.Format("PM%d.TriggerMode", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &unsVal);
	m_tParam[iPMNo].chTriggerMode = (char)unsVal;

	rv = USB_SetParameter(iPMNo, &m_tParam[iPMNo]);
	m_iReStartReq[iPMNo] = TRUE;

#if 0
	timeKillEvent(m_uiEvtID[iPMNo]);
	m_iTotal[iPMNo] = 0;
//	iSamplingTime = m_tParam[iPMNo].iIntegrationTime / 1000;
	iSamplingTime = 100;
	m_uiEvtID[iPMNo] = ::timeSetEvent(iSamplingTime, 1, EPDSampling, iPMNo, TIME_PERIODIC);
#endif
	return(rv);

}
int CWaveData_USB::SetEEPRomDefaultParameter(int iPMNo, int iSet)
{
	int	rv;
	unsigned char uchData;

	uchData = (unsigned char)iSet;

	rv = USB_SetEepromDefaultParameter(iPMNo, uchData);
	return(rv);
}
int CWaveData_USB::ReadUnitInformation(int iPMNo)
{
	int				rv;
	int				iohdl;
	CString			szIOName('\0', 32);

	PUNIT_INFORMATION	tInfo;

	rv = USB_ReadUnitInformation(iPMNo, &tInfo);

	if (rv == USBDEV_SUCCESS) {
		szIOName.Format("PM%d.UnitID", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, tInfo.szUnitID);

		szIOName.Format("PM%d.SensorName", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, tInfo.szSensorName);

		szIOName.Format("PM%d.SerialNo", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, tInfo.szSerialNumber);

		szIOName.Format("PM%d.WaveLengthUpper", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &tInfo.usWaveLengthUpper);

		szIOName.Format("PM%d.WaveLengthLower", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &tInfo.usWaveLengthLower);
	}
	return(rv);
}
int CWaveData_USB::WriteUnitInformation(int iPMNo)
{
	int				rv;
	int				iohdl;
	unsigned		unsVal;
	CString			szIOName('\0', 32);

	PUNIT_INFORMATION	tInfo;

	szIOName.Format("PM%d.UnitID", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, tInfo.szUnitID);

	szIOName.Format("PM%d.SensorName", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, tInfo.szSensorName);

	szIOName.Format("PM%d.SerialNo", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, tInfo.szSerialNumber);

	memset(tInfo.szReserve, '\0', 8);

	szIOName.Format("PM%d.WaveLengthUpper", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &tInfo.usWaveLengthUpper);

	szIOName.Format("PM%d.WaveLengthLower", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &tInfo.usWaveLengthLower);

	rv = USB_WriteUnitInformation(iPMNo, &tInfo);
	return(rv);
}

int CWaveData_USB::ReadCalibrationValue(int iPMNo)
{
	int				rv;

	int				iohdl;
	CString			szIOName('\0', 32);
	double			dValTbl[8];;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();


	rv = USB_ReadCalibrationValue(iPMNo, dValTbl);

	if (rv == USBDEV_SUCCESS) {
		szIOName.Format("PM%d.CalibRate_A", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[0]);
		pConfig->m_tWaveCaribrate[iPMNo].dA0 = dValTbl[0];

		szIOName.Format("PM%d.CalibRate_B1", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[1]);
		pConfig->m_tWaveCaribrate[iPMNo].dB1 = dValTbl[1];

		szIOName.Format("PM%d.CalibRate_B2", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[2]);
		pConfig->m_tWaveCaribrate[iPMNo].dB2 = dValTbl[2];

		szIOName.Format("PM%d.CalibRate_B3", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[3]);
		pConfig->m_tWaveCaribrate[iPMNo].dB3 = dValTbl[3];

		szIOName.Format("PM%d.CalibRate_B4", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[4]);
		pConfig->m_tWaveCaribrate[iPMNo].dB4 = dValTbl[4];

		szIOName.Format("PM%d.CalibRate_B5", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &dValTbl[5]);
		pConfig->m_tWaveCaribrate[iPMNo].dB5 = dValTbl[5];
	}
	return(rv);
}
int CWaveData_USB::WriteCalibrationValue(int iPMNo)
{
	int				rv;
	int				iohdl;
	double			dValTbl[8];;
	CString			szIOName('\0', 32);
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	dValTbl[0] = pConfig->m_tWaveCaribrate[iPMNo].dA0;
	dValTbl[1] = pConfig->m_tWaveCaribrate[iPMNo].dB1;
	dValTbl[2] = pConfig->m_tWaveCaribrate[iPMNo].dB2;
	dValTbl[3] = pConfig->m_tWaveCaribrate[iPMNo].dB3;
	dValTbl[4] = pConfig->m_tWaveCaribrate[iPMNo].dB4;
	dValTbl[5] = pConfig->m_tWaveCaribrate[iPMNo].dB5;


#if 0

	szIOName.Format("PM%d.CalibRate_A", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[0]);

	szIOName.Format("PM%d.CalibRate_B1", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[1]);

	szIOName.Format("PM%d.CalibRate_B2", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[2]);

	szIOName.Format("PM%d.CalibRate_B3", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[3]);

	szIOName.Format("PM%d.CalibRate_B4", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[4]);

	szIOName.Format("PM%d.CalibRate_B5", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointRead(iohdl, &dValTbl[5]);
#endif
	rv = USB_WriteCalibrationValue(iPMNo, dValTbl);
	return(rv);

}
int CWaveData_USB::GetStatusRequest(int iPMNo)
{
	int				rv;

	int				iohdl;
	CString			szIOName('\0', 32);

	unsigned char	uchFlag;
	unsigned short	usCount;
	unsigned		unsVal;

	rv = USB_GetStatusRequestA(iPMNo, &uchFlag,&usCount);

	if (rv == USBDEV_SUCCESS) {
		szIOName.Format("PM%d.Cooling.Cnt", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &usCount);

		if (uchFlag & 0x0001)	unsVal = 1;
		else					unsVal = 0;
		szIOName.Format("PM%d.Status.Bit0", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		if (uchFlag & 0x0008)	unsVal = 1;
		else					unsVal = 0;
		szIOName.Format("PM%d.Status.Bit3", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		if (uchFlag & 0x0010)	unsVal = 1;
		else					unsVal = 0;
		szIOName.Format("PM%d.Status.Bit4", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		if (uchFlag & 0x0020)	unsVal = 1;
		else					unsVal = 0;
		szIOName.Format("PM%d.Status.Bit5", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &unsVal);

		if (uchFlag & 0x0080)	unsVal = 1;
		else					unsVal = 0;
		szIOName.Format("PM%d.Status.Bit7", (iPMNo + 1));
		iohdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointWrite(iohdl, &unsVal);
	}
	return(rv);
}
void CWaveData_USB::GetDllVersion(int iPMNo)
{
	int				iohdl;
	CString			szIOName('\0', 32);
	char	szVersion[16];

	USB_GetDllVersion(iPMNo, szVersion);
	szIOName.Format("PM%d.DLLVersion", (iPMNo + 1));
	iohdl = m_pIODB->IOPointGetHandle(szIOName);
	m_pIODB->IOPointWrite(iohdl, szVersion);

}
