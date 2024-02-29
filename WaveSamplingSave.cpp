// PMLogger.cpp: PLCMonitor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "WaveEndPoint.h"
#include "WaveSamplingSave.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int	m_iDbgLogCnt = 0;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CWaveSamplingSave::CWaveSamplingSave()
{
}

CWaveSamplingSave::~CWaveSamplingSave()
{

}
void	CWaveSamplingSave::StartWork()
{
	int	i;


	CWaveEndPointApp*pApp = (CWaveEndPointApp *)AfxGetApp();
//	m_pWaveData = pApp->GetWaveData_Eth();
	m_pIODB = pApp->GetIODB();

	for(i=0;i<PM_MAX;i++){
		m_iPCSamplingStatus[i] = SAMPLING_IDLE;
		m_pWriteWaveData[i]=NULL;
	
		m_tWriteWaveInfo[i].iSampling100msec=1;
		m_tWriteWaveInfo[i].iSamplingMax = (m_tWriteWaveInfo[i].iSampling100msec * 10) * 60;
		m_tWriteWaveInfo[i].iSamplingCount = 0;
		m_tWriteWaveInfo[i].iSamplingCycle = 0;
		memset(m_tWriteWaveInfo[i].szLogFileName,'\0',sizeof(m_tWriteWaveInfo[i].szLogFileName));
	}/* for */
}

void	CWaveSamplingSave::DoWork()
{
	int		i,iCnt;
	CFile	fl;
	WAVEDATA* pWaveNextLog;

	for(i=0;i<PM_MAX;i++){

		if(m_iPCSamplingStatus[i]==SAMPLING_BUSY){
			if (m_tWriteWaveInfo[i].szLogFileName[0] != '\0'){
				if (fl.Open(m_tWriteWaveInfo[i].szLogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
					fl.SeekToEnd();

					while (m_pWriteWaveData[i]) {
						fl.Write(m_pWriteWaveData[i]->usData, (EPD_WAVE_TOTAL*2));

						m_Interlock.Lock();
						pWaveNextLog = (WAVEDATA *)m_pWriteWaveData[i]->pNext;
						free(m_pWriteWaveData[i]);
						if (pWaveNextLog == NULL)
							m_pWriteWaveData[i] = NULL;
						else
							m_pWriteWaveData[i] = pWaveNextLog;
						m_Interlock.Unlock();
						
						m_iDbgLogCnt++;
					}
					fl.Close();
				}
			}
#if 0
			CString	szMsg;
			szMsg.Format("Write Count =%d\n", m_iDbgLogCnt);
			TRACE(szMsg);
#endif
		}
	}/* for */
}
void	CWaveSamplingSave::EndWork()
{
	int	i;
	WAVEDATA* pWaveNextLog;

	for (i = 0; i < PM_MAX; i++) {
		m_Interlock.Lock();
		while (m_pWriteWaveData[i]) {
			pWaveNextLog = (WAVEDATA*)m_pWriteWaveData[i]->pNext;
			free(m_pWriteWaveData[i]);
			if (pWaveNextLog == NULL)
				m_pWriteWaveData[i] = NULL;
			else
				m_pWriteWaveData[i] = pWaveNextLog;
		}
		m_Interlock.Unlock();
	}
}
void CWaveSamplingSave::StartWaveSampling(int iPM, int iSamplingTM)
{
	CString		szFilePath('\0', 64);
	int			bFolder = TRUE;

	CTime	t = CTime::GetCurrentTime();

	m_pWriteWaveData[iPM] = NULL;

	m_tWriteWaveInfo[iPM].iSampling100msec = iSamplingTM;
	m_tWriteWaveInfo[iPM].iSamplingMax = 6000;
	m_tWriteWaveInfo[iPM].iSamplingCount = 0;
	m_tWriteWaveInfo[iPM].iSamplingCycle = 0;
	m_i1stTime[iPM] = TRUE;


	// Log Folder存在チェック
	CFileFind	filefind;
	szFilePath = _T("RowWaveLog");
	if (filefind.FindFile((LPCSTR)szFilePath) == FALSE) {
		if (CreateDirectory((LPCSTR)szFilePath, NULL) == FALSE)
			bFolder = FALSE;
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory((LPCSTR)szFilePath, NULL) == FALSE)
				bFolder = FALSE;
		}
		filefind.Close();
	}

	if (bFolder == FALSE) {
		AfxMessageBox("Log Folder Create Error");
		return;
	}

	sprintf(m_tWriteWaveInfo[iPM].szLogFileName,"RowWaveLog\\%04.04d%02.02d%02.02d-%02.02d%02.02d%02.02d.PM%d",
			t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),(iPM+1));

	CFile	fl;
	if (fl.Open(m_tWriteWaveInfo[iPM].szLogFileName, CFile::modeCreate | CFile::modeWrite)) {
		fl.Write(&m_tWriteWaveInfo[iPM], sizeof(WRITE_WAVEDATA));
		fl.Close();
	}

	m_iPCSamplingStatus[iPM] = SAMPLING_BUSY;
	m_iDbgLogCnt = 0;
}
void CWaveSamplingSave::WaveSamplingReq(int iPM, unsigned short* pusData)
{
	int			i;
	WAVEDATA* pWaveNextLog;
	WAVEDATA* pNext;

	if (m_iPCSamplingStatus[iPM] == SAMPLING_IDLE)
		return;

	++m_tWriteWaveInfo[iPM].iSamplingCycle;

	if (m_i1stTime[iPM]==TRUE || m_tWriteWaveInfo[iPM].iSamplingCycle >= m_tWriteWaveInfo[iPM].iSampling100msec) {
		m_tWriteWaveInfo[iPM].iSamplingCycle = 0;
		if (m_i1stTime[iPM]) {
			m_i1stTime[iPM] = FALSE;
		}
		else
			m_tWriteWaveInfo[iPM].iSamplingCycle = 0;

		if (m_tWriteWaveInfo[iPM].iSamplingCount >= m_tWriteWaveInfo[iPM].iSamplingMax){
			EndWaveSampling(iPM);
			return;
		}

		pWaveNextLog = (WAVEDATA *)malloc(sizeof(WAVEDATA));
		pWaveNextLog->pNext = NULL;
		for (i = 0; i < EPD_WAVE_TOTAL; i++)
			pWaveNextLog->usData[i] = *(pusData + i);

		m_Interlock.Lock();
		if (m_pWriteWaveData[iPM] == NULL) 
			m_pWriteWaveData[iPM] = pWaveNextLog;
		else {
			pNext = m_pWriteWaveData[iPM];
			while (pNext->pNext)
				pNext = (WAVEDATA*)pNext->pNext;
			
			pNext->pNext = pWaveNextLog;
		}
		m_Interlock.Unlock();
		++m_tWriteWaveInfo[iPM].iSamplingCount;

		CString szMsg;
		szMsg.Format("PC%d SamplingCount = %d\n", (iPM+1),m_tWriteWaveInfo[iPM].iSamplingCount);
		TRACE(szMsg);
	}
}
void CWaveSamplingSave::EndWaveSampling(int iPM)
{
	Sleep(1500);


	m_iPCSamplingStatus[iPM] = SAMPLING_IDLE;
	WAVEDATA* pWaveNextLog;

	m_Interlock.Lock();
	while (m_pWriteWaveData[iPM]) {
		pWaveNextLog = (WAVEDATA*)m_pWriteWaveData[iPM]->pNext;
		free(m_pWriteWaveData[iPM]);
		if (pWaveNextLog == NULL)
			m_pWriteWaveData[iPM] = NULL;
		else
			m_pWriteWaveData[iPM] = pWaveNextLog;
	}
	m_Interlock.Unlock();
}
