#include "pch.h"
#include "WaveEndPoint.h"

#define		RUN_EPD_LOGPATH		"Run_EPD_Log"
#define		RUN_RCP_LOGPATH		"Run_RCP_Log"
#define		RUN_CFG_LOGPATH		"Run_CFG_Log"


CEPDLog::CEPDLog()
{
}
void CEPDLog::InitialLogTable()
{
	HANDLE				hFind;
	WIN32_FIND_DATA		win32fd;
	int					iCnt;
	CString				szSerchPath('\0', 32);
	char				szStr[128];
	char* token;
	char* pContext;
	int					i, j;
	int					rv;
	EPD_LOGPATH			tLogPath;

	memset(&m_tEPDLogTable,'\0',sizeof(EPD_LOG_TBL));

	iCnt = 0;

	szSerchPath.Format("%s\\*.Log", RUN_EPD_LOGPATH);
	hFind = FindFirstFile(szSerchPath, &win32fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		if ((win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			if (strncmp(win32fd.cFileName, "PC1", 3)==0)
				m_tEPDLogTable.tEPDLogPath[iCnt].iPCNo = PM1;
			else if (strncmp(win32fd.cFileName, "PC2", 3) == 0)
				m_tEPDLogTable.tEPDLogPath[iCnt].iPCNo = PM2;
			else if (strncmp(win32fd.cFileName, "PC3", 3) == 0)
				m_tEPDLogTable.tEPDLogPath[iCnt].iPCNo = PM3;
			else if (strncmp(win32fd.cFileName, "PC4", 3) == 0)
				m_tEPDLogTable.tEPDLogPath[iCnt].iPCNo = PM4;
			else if (strncmp(win32fd.cFileName, "PC5", 3) == 0)
				m_tEPDLogTable.tEPDLogPath[iCnt].iPCNo = PM5;

			sprintf(m_tEPDLogTable.tEPDLogPath[iCnt].szEPDLogPath,"%s\\%s", RUN_EPD_LOGPATH ,win32fd.cFileName);
			sprintf(m_tEPDLogTable.tEPDLogPath[iCnt].szRcpLogPath,"%s\\%s", RUN_RCP_LOGPATH, win32fd.cFileName);
			sprintf(m_tEPDLogTable.tEPDLogPath[iCnt].szCfgLogPath,"%s\\%s", RUN_CFG_LOGPATH, win32fd.cFileName);

			strcpy(szStr, &win32fd.cFileName[4]);
			token = strtok_s(szStr, "_", &pContext);
			if (token) {
				strcpy(m_tEPDLogTable.tEPDLogPath[iCnt].szGlassID, token);
				token = strtok_s(NULL, "_", &pContext);
				if (token) {
					strcpy(m_tEPDLogTable.tEPDLogPath[iCnt].szYYYYMMDD, token);

					token = strtok_s(NULL, "_.", &pContext);
					if (token) {
						strcpy(m_tEPDLogTable.tEPDLogPath[iCnt].szhhmmss, token);
					}
				}
			}

//			m_tEPDLogTable.tEPDLogPath[iCnt].szDateTime[strlen(m_tEPDLogTable.tEPDLogPath[iCnt].szDateTime)-4] = '\0';
			iCnt++;
		}
	} while (FindNextFile(hFind, &win32fd));

	m_tEPDLogTable.iLogCount = iCnt;

	FindClose(hFind);

	if (m_tEPDLogTable.iLogCount) {
		for (i = 0; i < m_tEPDLogTable.iLogCount - 1; i++) {
			for (j = i + 1; j < m_tEPDLogTable.iLogCount; j++) {
				rv = strncmp(m_tEPDLogTable.tEPDLogPath[i].szYYYYMMDD, m_tEPDLogTable.tEPDLogPath[j].szYYYYMMDD, 8);
				if (rv > 0) {
					tLogPath = m_tEPDLogTable.tEPDLogPath[i];
					m_tEPDLogTable.tEPDLogPath[i] = m_tEPDLogTable.tEPDLogPath[j];
					m_tEPDLogTable.tEPDLogPath[j] = tLogPath;
				}
				else if (rv == 0) {
					if (strncmp(m_tEPDLogTable.tEPDLogPath[i].szhhmmss, m_tEPDLogTable.tEPDLogPath[j].szhhmmss, 6) > 0) {
						tLogPath = m_tEPDLogTable.tEPDLogPath[i];
						m_tEPDLogTable.tEPDLogPath[i] = m_tEPDLogTable.tEPDLogPath[j];
						m_tEPDLogTable.tEPDLogPath[j] = tLogPath;
					}
				}
			}
		}
	}
}
int CEPDLog::AddEPDLog(int iPC,EPD_WAVEDATA2* pEPDData, RECIPE* pRecipe, SET_WAVECFG* pWaveCfg)
{
	CString		szSavePath('\0', MAX_PATH);
	CString		szLogFileName('\0', MAX_PATH);
	CString		szStr('\0', MAX_PATH);
	char		szLogRegName[MAX_PATH+1];
	CTime		t = CTime::GetCurrentTime();
	int			iSet;
	CFile		fp;
	int			i;
	int			ioHdl;
	unsigned	unsMode;
	CIODB* pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	CFileFind	filefind;
	char* token;
	char* pContext;


	szStr.Format("PM%d.EPD.Mode",(iPC+1));
	ioHdl = pIODB->IOPointGetHandle(szStr);
	pIODB->IOPointRead(ioHdl, &unsMode);
	// Remote
	if (unsMode == 2) {
		if (pEPDData->szGlassName[0] > ' ') {
			szStr = pEPDData->szGlassName;
			szStr.Trim();
			szLogFileName.Format("PC%d_%s_", (iPC + 1), szStr);

			szLogFileName.Format("PC%d_%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.Log", 
				(iPC + 1), 
				szStr,
				t.GetYear(),
				t.GetMonth(),
				t.GetDay(),
				t.GetHour(),
				t.GetMinute(),
				t.GetSecond());
		}
		else {
			szLogFileName.Format("PC%d_NoName_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.Log",
				(iPC + 1),
				t.GetYear(),
				t.GetMonth(),
				t.GetDay(),
				t.GetHour(),
				t.GetMinute(),
				t.GetSecond());
		}
	}
	// Local
	else {
		szLogFileName.Format("PC%d_Local_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.Log",
			(iPC + 1),
			t.GetYear(),
			t.GetMonth(),
			t.GetDay(),
			t.GetHour(),
			t.GetMinute(),
			t.GetSecond());

	}



	if (m_tEPDLogTable.iLogCount == EPD_LOG_MAX) {
		RemoveEPDLog();
	}
	iSet = m_tEPDLogTable.iLogCount;
	if (iSet == EPD_LOG_MAX)
		iSet--;

	/*
	** FolderçÏê¨
	*/
	if (filefind.FindFile(RUN_EPD_LOGPATH) == FALSE) {
		if (CreateDirectory(RUN_EPD_LOGPATH, NULL) == FALSE) {
			return(FALSE);
		}
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory(RUN_EPD_LOGPATH, NULL) == FALSE) {
				return(FALSE);
			}
		}
		filefind.Close();
	}
	if (filefind.FindFile(RUN_RCP_LOGPATH) == FALSE) {
		if (CreateDirectory(RUN_RCP_LOGPATH, NULL) == FALSE) {
			return(FALSE);
		}
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory(RUN_RCP_LOGPATH, NULL) == FALSE) {
				return(FALSE);
			}
		}
		filefind.Close();
	}
	if (filefind.FindFile(RUN_CFG_LOGPATH) == FALSE) {
		if (CreateDirectory(RUN_CFG_LOGPATH, NULL) == FALSE) {
			return(FALSE);
		}
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory(RUN_CFG_LOGPATH, NULL) == FALSE) {
				return(FALSE);
			}
		}
		filefind.Close();
	}



	m_tEPDLogTable.tEPDLogPath[iSet].iPCNo = iPC;
	strcpy(szLogRegName, szLogFileName);
	szLogRegName[strlen(szLogRegName) - 4] = '\0';
	token = strtok_s(&szLogRegName[4], "_", &pContext);
	if (token) {
		strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szGlassID, token);
		token = strtok_s(NULL, "_", &pContext);
		if (token) {
			strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szYYYYMMDD, token);
			token = strtok_s(NULL, "_.", &pContext);
			if (token) {
				strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szhhmmss, token);
			}
		}
	}

//	strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szDateTime, &szLogRegName[4]);
#if 0	
	sprintf(m_tEPDLogTable.tEPDLogPath[iSet].szDateTime,"%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		t.GetYear(),
		t.GetMonth(),
		t.GetDay(),
		t.GetHour(),
		t.GetMinute(),
		t.GetSecond());
#endif
	/*
	** EPD Run Data Save
	*/
	if (pEPDData) {
		szSavePath.Format("%s\\%s",RUN_EPD_LOGPATH, szLogFileName);

		strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szEPDLogPath, szSavePath);
		if (!fp.Open(szSavePath, CFile::modeCreate | CFile::modeWrite)) {
				int iError = GetLastError();
				return(FALSE);
		}
		fp.Write(pEPDData, sizeof(EPD_WAVEDATA2));
		fp.Close();
	}
	else {
		memset(m_tEPDLogTable.tEPDLogPath[iSet].szEPDLogPath,'\0',MAX_PATH);
	}
	/*
	** EPD Recipe Save
	*/
	if (pRecipe) {
		szSavePath.Format("%s\\%s", RUN_RCP_LOGPATH, szLogFileName);

		strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szRcpLogPath, szSavePath);
		if (!fp.Open(szSavePath, CFile::modeCreate | CFile::modeWrite)) {
			int iError = GetLastError();
			return(FALSE);
		}
		fp.Write(pRecipe, sizeof(RECIPE));
		fp.Close();
	}
	else {
		memset(m_tEPDLogTable.tEPDLogPath[iSet].szRcpLogPath, '\0', MAX_PATH);
	}

	/*
	** EPD WaveCfg Save
	*/
	if (pWaveCfg) {
		szSavePath.Format("%s\\%s", RUN_CFG_LOGPATH, szLogFileName);

		strcpy(m_tEPDLogTable.tEPDLogPath[iSet].szCfgLogPath, szSavePath);
		if (!fp.Open(szSavePath, CFile::modeCreate | CFile::modeWrite)) {
			int iError = GetLastError();
			return(FALSE);
		}
		for(i=0;i< RCP_STEP_MAX;i++)
			fp.Write((pWaveCfg+i), sizeof(SET_WAVECFG));

		fp.Close();
	}
	else {
		memset(m_tEPDLogTable.tEPDLogPath[iSet].szCfgLogPath, '\0', MAX_PATH);
	}
	if (m_tEPDLogTable.iLogCount < EPD_LOG_MAX)
		m_tEPDLogTable.iLogCount++;

	return(TRUE);
}
void CEPDLog::LoadEPDLogList()
{
	m_tDisplayLogTable = m_tEPDLogTable;
}
int CEPDLog::ReadEPDLog(int	iPos, EPD_WAVEDATA2 * pEPDData, RECIPE *pRecipe, SET_WAVECFG *pWaveCfg)
{
	CFile		fp;
	int			iError;
	int			i;
	int			iReadSize;
	OLD_RECIPE	tOldRecipe;
	/*
	** EPD Run Data Load
	*/
	if (!fp.Open(m_tEPDLogTable.tEPDLogPath[iPos].szEPDLogPath, CFile::modeRead)) {
		int iError = GetLastError();
		return(FALSE);
	}
	fp.Read(pEPDData, sizeof(EPD_WAVEDATA2));
	fp.Close();

	/*
	** EPD Recipe Data Load
	*/
	if (!fp.Open(m_tEPDLogTable.tEPDLogPath[iPos].szRcpLogPath, CFile::modeRead)) {
		int iError = GetLastError();
		return(FALSE);
	}
	iReadSize = fp.Read(pRecipe, sizeof(RECIPE));
	fp.Close();

	if (iReadSize == sizeof(OLD_RECIPE)) {
		if (!fp.Open(m_tEPDLogTable.tEPDLogPath[iPos].szRcpLogPath, CFile::modeRead)) {
			int iError = GetLastError();
			return(FALSE);
		}
		iReadSize = fp.Read(&tOldRecipe, sizeof(OLD_RECIPE));
		fp.Close();

		ZeroMemory(pRecipe, sizeof(RECIPE));
		pRecipe->iIntegrationTime = tOldRecipe.iIntegrationTime;
		pRecipe->iStepCnt = tOldRecipe.iStepCnt;
		pRecipe->iVersion = tOldRecipe.iVersion;
		strncpy(pRecipe->szComment,tOldRecipe.szComment,sizeof(tOldRecipe.szComment));
		pRecipe->szComment[sizeof(tOldRecipe.szComment)-1]='\0';
		strncpy(pRecipe->szRecipeName, tOldRecipe.szRecipeName, sizeof(tOldRecipe.szRecipeName));
		pRecipe->szRecipeName[sizeof(tOldRecipe.szRecipeName) - 1] = '\0';
		for (i = 0; i < RCP_STEP_MAX; i++) {
			pRecipe->Step[i].iUseWave = tOldRecipe.Step[i].iUseWave;
			strncpy(pRecipe->Step[i].szComment, tOldRecipe.Step[i].szComment, sizeof(tOldRecipe.Step[i].szComment));
			pRecipe->Step[i].szComment[sizeof(tOldRecipe.Step[i].szComment) - 1] = '\0';

			pRecipe->Step[i].tOverEtch = tOldRecipe.Step[i].tOverEtch;

			pRecipe->Step[i].tStart.bAutoGain = tOldRecipe.Step[i].tStart.bAutoGain;
			pRecipe->Step[i].tStart.bStartLevelEnable = tOldRecipe.Step[i].tStart.bStartLevelEnable;
			pRecipe->Step[i].tStart.bStartLimitEnable = tOldRecipe.Step[i].tStart.bStartLimitEnable;
			pRecipe->Step[i].tStart.dAutoGainLevel = tOldRecipe.Step[i].tStart.dAutoGainLevel;
			pRecipe->Step[i].tStart.dStartLevel = tOldRecipe.Step[i].tStart.dStartLevel;
			pRecipe->Step[i].tStart.eLevelType = tOldRecipe.Step[i].tStart.eLevelType;
			pRecipe->Step[i].tStart.fDeadTime = tOldRecipe.Step[i].tStart.fDeadTime;
			pRecipe->Step[i].tStart.fStartLimitTime = tOldRecipe.Step[i].tStart.fStartLimitTime;

			pRecipe->Step[i].tEnd.bEndDiff1Enable = tOldRecipe.Step[i].tEnd.bEndDiff1Enable;
			pRecipe->Step[i].tEnd.bEndDiff2Enable = tOldRecipe.Step[i].tEnd.bEndDiff2Enable;
			pRecipe->Step[i].tEnd.bEndFlatEnable = tOldRecipe.Step[i].tEnd.bEndFlatEnable;
			pRecipe->Step[i].tEnd.bEndLevelEnable = tOldRecipe.Step[i].tEnd.bEndLevelEnable;
			pRecipe->Step[i].tEnd.bEndLimitEnable = tOldRecipe.Step[i].tEnd.bEndLimitEnable;
			pRecipe->Step[i].tEnd.dEndDiffFromJust = tOldRecipe.Step[i].tEnd.dEndDiffFromJust;
			pRecipe->Step[i].tEnd.dEndDiffFromPeak = tOldRecipe.Step[i].tEnd.dEndDiffFromPeak;
			pRecipe->Step[i].tEnd.dEndFlatWave = tOldRecipe.Step[i].tEnd.dEndFlatWave;
			pRecipe->Step[i].tEnd.dEndLevel = tOldRecipe.Step[i].tEnd.dEndLevel;
			pRecipe->Step[i].tEnd.eLevelType = tOldRecipe.Step[i].tEnd.eLevelType;
			pRecipe->Step[i].tEnd.fEndFlatTime = tOldRecipe.Step[i].tEnd.fEndFlatTime;
			pRecipe->Step[i].tEnd.fEndLimitTime = tOldRecipe.Step[i].tEnd.fEndLimitTime;

			pRecipe->Step[i].tJust.bBoxDetect = FALSE;
			pRecipe->Step[i].tJust.bJustDiffEnable = tOldRecipe.Step[i].tJust.bJustDiffEnable;
			pRecipe->Step[i].tJust.bJustLevelEnable = tOldRecipe.Step[i].tJust.bJustLevelEnable;
			pRecipe->Step[i].tJust.bJustLimitEnable = tOldRecipe.Step[i].tJust.bJustLimitEnable;
			pRecipe->Step[i].tJust.bJustPeakDetect = tOldRecipe.Step[i].tJust.bJustPeakDetect;
			pRecipe->Step[i].tJust.bJustPeakHigh = tOldRecipe.Step[i].tJust.bJustPeakHigh;
			pRecipe->Step[i].tJust.dBoxLevel = 0.0f;
			pRecipe->Step[i].tJust.dJustDiffFromStart = tOldRecipe.Step[i].tJust.dJustDiffFromStart;
			pRecipe->Step[i].tJust.dJustLevel = tOldRecipe.Step[i].tJust.dJustLevel;
			pRecipe->Step[i].tJust.dJustPeakChgValue = tOldRecipe.Step[i].tJust.dJustPeakChgValue;
			pRecipe->Step[i].tJust.eBoxType = BOX_ASCEND;
			pRecipe->Step[i].tJust.eLevelType = tOldRecipe.Step[i].tJust.eLevelType;
			pRecipe->Step[i].tJust.fBoxTime = 0.0f;
			pRecipe->Step[i].tJust.fJustLimitTime = tOldRecipe.Step[i].tJust.fJustLimitTime;
		}
	}


	/*
	** Wave Config Load
	*/
	if (!fp.Open(m_tEPDLogTable.tEPDLogPath[iPos].szCfgLogPath, CFile::modeRead)) {
		int iError = GetLastError();
		return(FALSE);
	}
	for(i=0;i<RCP_STEP_MAX;i++)
		fp.Read((pWaveCfg+i), sizeof(SET_WAVECFG));

	fp.Close();
	return(TRUE);

}

int CEPDLog::RemoveEPDLog()
{
	int			i, j;
	CString		szSavePath('\0', MAX_PATH);

//	szSavePath.Format("%s\\%s", RUN_EPD_LOGPATH, m_tEPDLogTable.tEPDLogPath[0].szEPDLogPath);
	if(strlen(m_tEPDLogTable.tEPDLogPath[0].szEPDLogPath))
		CFile::Remove(m_tEPDLogTable.tEPDLogPath[0].szEPDLogPath);

//	szSavePath.Format("%s\\%s", RUN_RCP_LOGPATH, m_tEPDLogTable.tEPDLogPath[0].szRcpLogPath);
	if(strlen(m_tEPDLogTable.tEPDLogPath[0].szRcpLogPath))
		CFile::Remove(m_tEPDLogTable.tEPDLogPath[0].szRcpLogPath);

//	szSavePath.Format("%s\\%s", RUN_CFG_LOGPATH, m_tEPDLogTable.tEPDLogPath[0].szCfgLogPath);
	if (strlen(m_tEPDLogTable.tEPDLogPath[0].szCfgLogPath))
		CFile::Remove(m_tEPDLogTable.tEPDLogPath[0].szCfgLogPath);

	for (i = 0, j = 1; i < m_tEPDLogTable.iLogCount; i++, j++)
		m_tEPDLogTable.tEPDLogPath[i] = m_tEPDLogTable.tEPDLogPath[j];

	return(TRUE);
}
