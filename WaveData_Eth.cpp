#include "pch.h"
#include "mmsystem.h"
#include "WaveEndPoint.h"
#include "clockread_ms.h"


static	unsigned			m_uiEvtID[PM_MAX] = {0xFFFFFFFF,0xFFFFFFFF ,0xFFFFFFFF ,0xFFFFFFFF ,0xFFFFFFFF };
plf::nanotimer				m_Sampling_timer[PM_MAX];
static	int					m_iSamplingEnable[PM_MAX] = {FALSE,FALSE, FALSE, FALSE, FALSE};
static	int					m_iEthernetInit[PM_MAX] = { FALSE,FALSE, FALSE, FALSE, FALSE };
static	int					m_iInitParamNo[PM_MAX] = { 0,0, 0, 0, 0};

static	CIODB*		m_pIODB;
static	int         m_iohdlSensorStatus[PM_MAX];
static	int         m_iWaitRecover[PM_MAX] = {0,0,0,0,0};
static	char		m_szUnitID[PM_MAX][32+1];
static	int			m_i_ExitInterlock;




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

void CALLBACK EPDSampling_Eth(UINT uEntID, UINT uMsgID, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	int			iPM;
	int			rv;
	int			i,j,k;
	int			iSensorStatus;
	unsigned	unsVal;
	unsigned	unsOldVal;
	long		hDevHandle;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	CWaveData_Eth* pWaveData = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_Eth();
	CWaveSamplingSave	*m_pWaveSampling = ((CWaveEndPointApp*)AfxGetApp())->GetWaveSamplingSave();

	iPM = dwUser;


// #ifdef _CYCLETIME_CHECK_
	double			dmsec;
	char			szDebug[32 + 1];

	sprintf(szDebug, "Sampling PM%d\n", (iPM + 1));
	TRACE(szDebug);

	dmsec = m_Sampling_timer[iPM].get_elapsed_ms();
	if (iPM == PM1) {
		sprintf(szDebug, "WaveSampling=%fmsec\n", dmsec);
		TRACE(szDebug);
	}
	if (dmsec < 80)
		return;


	m_Sampling_timer[iPM].start();
	//#endif

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

	m_pIODB->IOPointRead(m_iohdlSensorStatus[iPM], &unsVal);
	if (unsVal == 0)
		return;
	else
		m_iSamplingEnable[iPM] = TRUE;

	if(m_iSamplingEnable[iPM]==FALSE)
		return;


	if (m_iEthernetInit[iPM] == FALSE) {
		rv = pWaveData->GetParameter(iPM, m_iInitParamNo[iPM]);
		if (rv) {
			if (++m_iInitParamNo[iPM] == 9)
				m_iEthernetInit[iPM] = TRUE;
		}
		return;
	}

	for (i = EPD_SAMPLING_POINT - 1; i > 0; i--) {
		memcpy((char *)pWaveData->m_pWaveData[iPM]->usWaveData[i], (char*)pWaveData->m_pWaveData[iPM]->usWaveData[i - 1], EPD_WAVE_TOTAL*2);
	}

	if (m_i_ExitInterlock == TRUE)
		return;

	rv = pWaveData->WaveDataRead(iPM, pWaveData->m_pWaveData[iPM]->usWaveData[0]);

	if (rv) {
		if (m_pWaveSampling->m_iPCSamplingStatus[iPM] == SAMPLING_BUSY) {
			m_pWaveSampling->WaveSamplingReq(iPM, pWaveData->m_pWaveData[iPM]->usWaveData[0]);
		}
		// Statsu = Normal
		unsVal = 1;
		m_pIODB->IOPointRead(m_iohdlSensorStatus[iPM], &unsOldVal);
		if (unsVal != unsOldVal) {
			m_pIODB->IOPointWrite(m_iohdlSensorStatus[iPM], &unsVal);
		}
	}
	else{
		if (iPM == PM1) {
			strcpy(szDebug, "Data Read Error\n");
			TRACE(szDebug);
		}
		return;
	}

	if (pWaveData->m_iTotal[iPM] < EPD_SAMPLING_POINT)
		pWaveData->m_iTotal[iPM]++;

}


CWaveData_Eth::CWaveData_Eth()
{
	int			i;
	long		m_hDevHandle;
	CString		szIOName('\0', 32);
	unsigned	unsVal;


	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	for (i = 0; i < PM_MAX; i++) {
		m_pEthDriver[i] = NULL;

		szIOName.Format("CTC.PC%d.EPD.Sensor", (i + 1));
		m_iohdlSensorStatus[i] = m_pIODB->IOPointGetHandle(szIOName);


		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			h_WaveData[i] = GlobalAlloc(GPTR, sizeof(WAVE_DATA));
			m_pWaveData[i] = (WAVE_DATA*)GlobalLock(h_WaveData[i]);

			m_pEthDriver[i] = new CEthDriver(i);
		}
		else {
			h_WaveData[i] = 0;
		}
//#ifdef _CYCLETIME_CHECK_
		m_Sampling_timer[i].start();
//#endif
	}/* for */

	m_i_ExitInterlock = FALSE;

}


CWaveData_Eth::~CWaveData_Eth()
{
	int	i;

	m_i_ExitInterlock = TRUE;

	for (i = 0; i < PM_MAX; i++) {
		if (m_uiEvtID[i] != 0xFFFFFFFF)
			timeKillEvent(m_uiEvtID[i]);
	}/* for */

	Sleep(7000);

	for (i = 0; i < PM_MAX; i++) {
		if (h_WaveData) {
			GlobalUnlock(h_WaveData[i]);
			GlobalFree(h_WaveData[i]);
			delete m_pEthDriver[i];
			m_pEthDriver[i] = NULL;
		}
	}/* for */
}

void CWaveData_Eth::StartWaveSampling()
{
	int		iSamplingTime;
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		m_iTotal[i] = 0;
//		if (m_iSamplingEnable[i] == TRUE) {
//			iSamplingTime = m_tParam[i].iIntegrationTime / 1000;
			iSamplingTime = 100;
			m_uiEvtID[i] = ::timeSetEvent(iSamplingTime, 1, EPDSampling_Eth, i, TIME_PERIODIC);
//		}
	}
}
int CWaveData_Eth::GetParameter(int iPMNo, int iParamNo)
{
	int				rv;

	switch (iParamNo) {
	case 0: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixMACAddr(); break;
	case 1: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixNetwork(); break;
	case 2: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixPortNo(); break;
	case 3: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixProductInfo(); break;
	case 4: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixUnitInfo(); break;
	case 5: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixWaveParam(); break;
	case 6: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixWaveScale(); break;
	case 7: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_Network(); break;
	case 8: rv = m_pEthDriver[iPMNo]->UDP_ParamRead_IntegrationTime(); break;
	}/* switch */

	return(rv);
}
int CWaveData_Eth::SetIntegrationTime(int iPMNo)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_ParamWrite_IntegrationTime();

	return(rv);

}
int CWaveData_Eth::ReadUnitInformation(int iPMNo)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixUnitInfo();
	rv |= m_pEthDriver[iPMNo]->UDP_ParamRead_FixWaveParam();
	rv |= m_pEthDriver[iPMNo]->UDP_ParamRead_FixWaveScale();

	return(rv);
}

int CWaveData_Eth::ReadCalibrationValue(int iPMNo)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_ParamRead_FixWaveParam();

	return(rv);
}
int CWaveData_Eth::WaveDataRead(int iPMNo,unsigned short *pusWaveData)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_WaveDataRead(0, pusWaveData);

	return(rv);
}
int CWaveData_Eth::EEPROMRead(int iPMNo)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_EEPROMRead();

	return(rv);
}
int CWaveData_Eth::EEPROMWrite(int iPMNo)
{
	int				rv;

	rv = m_pEthDriver[iPMNo]->UDP_EEPROMWrite();

	return(rv);
}
