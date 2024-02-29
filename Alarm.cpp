#include "pch.h"
#include "WaveEndPoint.h"
#include "Alarm.h"

CAlarm::CAlarm()
{

#if 0
	h_AlarmTbl = GlobalAlloc(GPTR, sizeof(ALARM_TBL) * ALARM_MAX);
	m_tAlarmTbl = (ALARM_TBL*)GlobalLock(h_AlarmTbl);

	h_CurrAlarm = GlobalAlloc(GPTR, sizeof(ALARM_TBL) * ALARM_MAX);
	m_tCurrAlarm = (ALARM_TBL*)GlobalLock(h_CurrAlarm);

	h_AlarmLogTbl = GlobalAlloc(GPTR, sizeof(ALARM_LOG) * ALARM_LOG_MAX);
	m_tAlarmLogTbl = (ALARM_LOG*)GlobalLock(h_AlarmLogTbl);
#endif
	m_iCurrAlmTotal = 0;

	InitializeCriticalSection(&m_UpdateAlarm);

}
CAlarm::~CAlarm()
{
#if 0
	if (h_AlarmTbl) {
		GlobalUnlock(h_AlarmTbl);
		GlobalFree(h_AlarmTbl);
	}
	if (h_CurrAlarm) {
		GlobalUnlock(h_CurrAlarm);
		GlobalFree(h_CurrAlarm);
	}
	if (h_AlarmLogTbl) {
		GlobalUnlock(h_AlarmLogTbl);
		GlobalFree(h_AlarmLogTbl);
	}
#endif
	DeleteCriticalSection(&m_UpdateAlarm);

}

void CAlarm::LoadAlarmList()
{
	CStdioFile* fp = new CStdioFile();
	int	rv = FALSE;
	int	bOpen;
	CString	szBuf('\0', 1024);
	char	chBuf[512 + 1];
	char* token;
	char* pContext;
	int		iALID;

	memset((char*)m_tAlarmTbl, '\0', sizeof(ALARM_TBL) * ALARM_MAX);
	memset((char*)m_tCurrAlarm, '\0', sizeof(ALARM_TBL) * ALARM_MAX);


	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(_T("AlarmCfg.TXT"), CFile::modeRead | CFile::typeText);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return;
	}
	END_CATCH
	//ファイルが存在しなければ初期値を
	//セットしてファイル作成
	if (bOpen == FALSE) {
		delete fp;
		return;
	}

	while (1) {
		if (fp->ReadString(szBuf) == FALSE)
			break;

		strncpy(chBuf, (LPCSTR)szBuf, 512);
		token = strtok_s(chBuf, ",", &pContext);
		if (token == NULL)
			continue;

		else if (*token == ';')
			continue;

		/*
		** 1.ALID
		*/
		iALID = atoi(token);
		if (iALID < 1 || iALID > ALARM_MAX)
			continue;

		m_tAlarmTbl[iALID - 1].iAlarmNo = iALID;
		--iALID;

		/*
		** 2.Enable設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;

		m_tAlarmTbl[iALID].bEnable = atoi(token);


		/*
		** 4.Level設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tAlarmTbl[iALID].iAlarmLevel = atoi(token);

		/*
		** 5.Module設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tAlarmTbl[iALID].iModule = atoi(token);

		/*
		** 6.Message設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		strncpy(m_tAlarmTbl[iALID].szMeassage, token, ALM_MSG_LEN);
	}/* while */
	fp->Close();
	delete fp;
}
void CAlarm::AlarmON(int iAlarmNo)
{
	CDlgAlarm* pDlgAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetDlgAlarm();

	EnterCriticalSection(&m_UpdateAlarm);

	if (iAlarmNo < 1 || iAlarmNo >= ALARM_MAX) {
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}
	if(m_tAlarmTbl[iAlarmNo-1].bEnable==FALSE){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	if (m_tAlarmTbl[iAlarmNo - 1].bAlarmON == TRUE){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	CTime	t = CTime::GetCurrentTime();

	sprintf(m_tAlarmTbl[iAlarmNo - 1].szDate, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	m_tAlarmTbl[iAlarmNo - 1].bAlarmON = TRUE;
	m_tCurrAlarm[m_iCurrAlmTotal] = m_tAlarmTbl[iAlarmNo - 1];

	m_iCurrAlmTotal++;
	AddAlarmLog(&m_tAlarmTbl[iAlarmNo - 1]);


	pDlgAlarm->ShowAlarm();

	LeaveCriticalSection(&m_UpdateAlarm);
	return;
}

void CAlarm::AlarmOFF(int iAlarmNo)
{
	int		i, j;
	CDlgAlarm* pDlgAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetDlgAlarm();

	EnterCriticalSection(&m_UpdateAlarm);

	if (iAlarmNo < 1 || iAlarmNo >= ALARM_MAX){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	if (m_tAlarmTbl[iAlarmNo - 1].bEnable == FALSE){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	if (m_tAlarmTbl[iAlarmNo - 1].bAlarmON == FALSE){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	if(m_iCurrAlmTotal == 0){
		LeaveCriticalSection(&m_UpdateAlarm);
		return;
	}

	CTime	t = CTime::GetCurrentTime();
	sprintf(m_tAlarmTbl[iAlarmNo - 1].szDate, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
		t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	m_tAlarmTbl[iAlarmNo - 1].bAlarmON = FALSE;

	for (i = 0; i < m_iCurrAlmTotal; i++) {
		if (m_tCurrAlarm[i].iAlarmNo == iAlarmNo)
			break;
	}/* for */
	if (i < m_iCurrAlmTotal) {
		for (j = i; j < (m_iCurrAlmTotal-1); j++) 
			m_tCurrAlarm[j] = m_tCurrAlarm[j + 1];
	}
	m_iCurrAlmTotal--;
	AddAlarmLog(&m_tAlarmTbl[iAlarmNo - 1]);

	pDlgAlarm->UpdateAlarm();


	LeaveCriticalSection(&m_UpdateAlarm);
}
void CAlarm::AllReset()
{
	int		i;

	if (m_iCurrAlmTotal == 0)
		return;

	EnterCriticalSection(&m_UpdateAlarm);

	CTime	t = CTime::GetCurrentTime();


	for (i = 0; i < m_iCurrAlmTotal; i++) {
		m_tAlarmTbl[m_tCurrAlarm[i].iAlarmNo - 1].bAlarmON = FALSE;
		sprintf(m_tAlarmTbl[m_tCurrAlarm[i].iAlarmNo - 1].szDate, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		AddAlarmLog(&m_tAlarmTbl[m_tCurrAlarm[i].iAlarmNo - 1]);
	}
	m_iCurrAlmTotal = 0;
	LeaveCriticalSection(&m_UpdateAlarm);
}
void CAlarm::LoadAlarmLog()
{
	CStdioFile* fp = new CStdioFile();
	int	rv = FALSE;
	int	bOpen;
	CString	szBuf('\0', 1024);
	char	chBuf[512 + 1];
	char* token;
	char* pContext;
	int		iALID;
	int		i;
	m_iAlmLogTotal = 0;

	for(i=0;i<ALARM_LOG_MAX;i++)
		memset((char*)&m_tAlarmLogTbl[i], '\0', sizeof(ALARM_LOG));

	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(_T("AlarmLog.TXT"), CFile::modeRead | CFile::typeText);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return;
	}
	END_CATCH
		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
		if (bOpen == FALSE) {
			delete fp;
			return;
		}

	while (1) {
		if (fp->ReadString(szBuf) == FALSE)
			break;

		strncpy(chBuf, (LPCSTR)szBuf, 512);
		token = strtok_s(chBuf, ",", &pContext);
		if (token == NULL)
			continue;

		else if (*token == ';')
			continue;

		/*
		** 1.ALID
		*/
		iALID = atoi(token);
		if (iALID < 1 || iALID > ALARM_MAX)
			continue;

		m_tAlarmLogTbl[m_iAlmLogTotal].iAlarmNo = iALID;

		/*
		** 2.On/Off
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;

		m_tAlarmLogTbl[m_iAlmLogTotal].iOnOff = atoi(token);

		/*
		** 3.Module
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;

		m_tAlarmLogTbl[m_iAlmLogTotal].iModule = atoi(token);

		/*
		** 4.Level設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tAlarmLogTbl[m_iAlmLogTotal].iAlarmLevel = atoi(token);


		/*
		**5.Message設定
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		strncpy(m_tAlarmLogTbl[m_iAlmLogTotal].szMeassage, token, ALM_MSG_LEN);

		/*
		**6.Date&Time
		*/
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		strncpy(m_tAlarmLogTbl[m_iAlmLogTotal].szDate, token, ALM_MSG_LEN);

		if (++m_iAlmLogTotal >= ALARM_LOG_MAX)
			break;

	}/* while */
	fp->Close();
	delete fp;
}
void CAlarm::SaveAlarmLog()
{
	CStdioFile* fp = new CStdioFile();
	int	bOpen;
	CString	szBuf('\0', 1024);
	int		i;


	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(_T("AlarmLog.TXT"), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return;
	}
	END_CATCH
		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
	if (bOpen == FALSE) {
		delete fp;
		return;
	}

	for (i = 0; i < m_iAlmLogTotal;i++) {

		szBuf.Format("%d,%d,%d,%d,%s,%s\n",
			m_tAlarmLogTbl[i].iAlarmNo,
			m_tAlarmLogTbl[i].iOnOff,
			m_tAlarmLogTbl[i].iModule,
			m_tAlarmLogTbl[i].iAlarmLevel,
			m_tAlarmLogTbl[i].szMeassage,
			m_tAlarmLogTbl[i].szDate);

		fp->WriteString(szBuf);

	}/* for */
	fp->Close();
	delete fp;
}
void CAlarm::AddAlarmLog(ALARM_TBL* ptAlm)
{
	int		i, j;

	if (m_iAlmLogTotal >= ALARM_LOG_MAX) {
		for (i = 0, j = 1; j < ALARM_LOG_MAX; i++, j++)
			memcpy(&m_tAlarmLogTbl[i], &m_tAlarmLogTbl[j], sizeof(ALARM_LOG));

		m_tAlarmLogTbl[i].iAlarmNo = ptAlm->iAlarmNo;
		m_tAlarmLogTbl[i].iOnOff = ptAlm->bAlarmON;
		m_tAlarmLogTbl[i].iModule = ptAlm->iModule;
		m_tAlarmLogTbl[i].iAlarmLevel = ptAlm->iAlarmLevel;
		strncpy(m_tAlarmLogTbl[i].szMeassage, ptAlm->szMeassage, ALM_MSG_LEN);
		m_tAlarmLogTbl[i].szMeassage[ALM_MSG_LEN] = '\0';
		strncpy(m_tAlarmLogTbl[i].szDate, ptAlm->szDate, 30);
		m_tAlarmLogTbl[i].szDate[30] = '\0';
	}
	else {
		m_tAlarmLogTbl[m_iAlmLogTotal].iAlarmNo = ptAlm->iAlarmNo;
		m_tAlarmLogTbl[m_iAlmLogTotal].iOnOff = ptAlm->bAlarmON;
		m_tAlarmLogTbl[m_iAlmLogTotal].iModule = ptAlm->iModule;
		m_tAlarmLogTbl[m_iAlmLogTotal].iAlarmLevel = ptAlm->iAlarmLevel;
		strncpy(m_tAlarmLogTbl[m_iAlmLogTotal].szMeassage, ptAlm->szMeassage, ALM_MSG_LEN);
		m_tAlarmLogTbl[m_iAlmLogTotal].szMeassage[ALM_MSG_LEN] = '\0';
		strncpy(m_tAlarmLogTbl[m_iAlmLogTotal].szDate, ptAlm->szDate, 30);
		m_tAlarmLogTbl[m_iAlmLogTotal].szDate[30] = '\0';
		++m_iAlmLogTotal;
	}
	SaveAlarmLog();
}

