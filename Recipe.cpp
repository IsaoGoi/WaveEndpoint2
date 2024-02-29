#include "pch.h"
#include "WaveEndPoint.h"
#include "Recipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

_TCHAR gpszRecipePath[16] = "RECIPE";


// コンストラクション
CRecipe::CRecipe()
{
	// ロードレシピNo.
	m_nLoadRecipeNo = 0;

#if 0
	// メモリの確保
	h_Recipe= GlobalAlloc(GPTR,sizeof(RECIPE));
	m_tRecipe = (RECIPE *)GlobalLock(h_Recipe);

	h_OldRecipe= GlobalAlloc(GPTR,sizeof(RECIPE));
	m_tOldRecipe = (RECIPE *)GlobalLock(h_OldRecipe);
#endif
}
CRecipe::~CRecipe()
{
#if 0
	GlobalUnlock(h_Recipe);
	GlobalFree(h_Recipe);

	GlobalUnlock(h_OldRecipe);
	GlobalFree(h_OldRecipe);
#endif
}

/*
** レシピファイルロード
*/
int CRecipe::RecipeFileLoad(int nPMNo,int iRcpNo)
{
	int	rv = RCP_FILE_SUCCESS;
	CString	szBuf('\0',512);
	CString	szFileName('\0',64);
	char	chBuf[512+1];
	char	*token;
	double	dVal;	
	int		iVal;	
	int		i;
	char	*pContext;

	EnterCriticalSection(&((CWaveEndPointApp *)AfxGetApp())->m_Recipe_Interlock);

	// ロードレシピNo.クリア
	m_nLoadRecipeNo = 0;
	m_szLoadRecipeName = _T("");
	m_szLoadRecipeTitle = _T("");

	// 初期値
	m_tRecipe.iIntegrationTime = 100000;

	m_iNew = TRUE;

	// パラメーターチェック
	if(nPMNo < PM1 || nPMNo > PM5){
		LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
		return(RCP_FILE_INV_PARAM);
	}
		
	memset((char *)&m_tRecipe,'\0',sizeof(RECIPE));

	m_tRecipe.iIntegrationTime = 100000;

	CStdioFile fp_Recipe;
	
	szFileName.Format("%s\\PM%d\\RCP%03.03d.csv",gpszRecipePath,(nPMNo+1),iRcpNo);
//	if( !fp_Recipe.Open(szFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone)){
	if (!fp_Recipe.Open(szFileName, CFile::modeRead | CFile::typeText)) {
			rv = RCP_FILE_OPENERR;
		m_tOldRecipe = m_tRecipe;
		LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
		return(rv);
	}

	while(1){
		if(fp_Recipe.ReadString(szBuf) == FALSE)
			break;
		strcpy(chBuf,(LPCSTR)szBuf);
		token = strtok_s(chBuf,",",&pContext);
		if(token==NULL)
			continue;

		if(strcmp(token,LABEL_RCPNAME)==0){
			token = strtok_s(NULL,",",&pContext);
			if(token!=NULL)
				strncpy(m_tRecipe.szRecipeName,token,RCP_LEN_NAME);
		}
		else if(strcmp(token,LABEL_COMMENT)==0){
			token = strtok_s(NULL,",",&pContext);
			if(token!=NULL)
				strncpy(m_tRecipe.szComment,token,RCP_LEN_COMMENT);
		}
		else if (strcmp(token, LABEL_VERSION) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token != NULL)
				m_tRecipe.iVersion = atoi(token);
		}
		
		else if (strcmp(token, LABEL_INTEGTIME) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token != NULL)
				m_tRecipe.iIntegrationTime = atoi(token);
		}
		else if (strcmp(token, LABEL_DIFFDISPMAX) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token != NULL)
				m_tRecipe.dDiffDisplayMax = atof(token);
		}
		else if (strcmp(token, LABEL_SAMPLINGTIME) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token != NULL)
				m_tRecipe.iSamplingTime = atoi(token);
		}

		// Step Start
		else if(strcmp(token, LABEL_STEP_START)==0)
			continue;
		// Step End
		else if (strcmp(token, LABEL_STEP_END) == 0)
			++m_tRecipe.iStepCnt;

		else if (strcmp(token, LABEL_USE_WAVE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].iUseWave = atoi(token);
		}
		else if(strcmp(token, LABEL_STEP_COMMENT)==0){
			token = strtok_s(NULL,",",&pContext);
			if(token==NULL)
				continue;
			strncpy(m_tRecipe.Step[m_tRecipe.iStepCnt].szComment, token, RCP_LEN_COMMENT);
		}

		else if(strcmp(token, LABEL_DEAD_TIME)==0){
			token = strtok_s(NULL,",",&pContext);
			if(token==NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.fDeadTime = atof(token);
		}
		else if (strcmp(token, LABEL_START_LVL_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.bStartLevelEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_START_LVL_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.eLevelType = (LEVEL_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_START_LEVEL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.dStartLevel = atof(token);
		}
		else if (strcmp(token, LABEL_START_LIMIT_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.bStartLimitEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_START_LIMIT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.fStartLimitTime = atof(token);
		}
		else if (strcmp(token, LABEL_SEL_AUTO_GAIN) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.bAutoGain = atoi(token);
		}
		else if (strcmp(token, LABEL_AUTO_GAIN_LVL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.dAutoGainLevel = atof(token);
		}
		else if (strcmp(token, LABEL_AUTO_GAIN_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.eAutoGainType = (AUTOGAIN_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_AUTO_GAIN_PEAK_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.eAutoGainPeakDetectType = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_AUTO_GAIN_PEAK_LEVEL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.dStartAutoGainPeakChgValue = atof(token);
		}
		else if (strcmp(token, LABEL_AUTO_GAIN_PEAK_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tStart.dStartAutoGainPeakChgRate = atof(token);
		}
		// Just
		else if (strcmp(token, LABEL_JUST_LEVEL_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bJustLevelEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_LEVEL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dJustLevel= atof(token);
		}
		else if (strcmp(token, LABEL_JUST_LVL_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.eLevelType = (LEVEL_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_DIFF_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bJustDiffEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_DIFF) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dJustDiffFromStart = atof(token);
		}
		else if (strcmp(token, LABEL_JUST_LIMIT_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bJustLimitEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_LIMIT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.fJustLimitTime = atof(token);
		}
		else if (strcmp(token, LABEL_JUST_PEAK) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bJustPeakDetect= atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_PEAK_HL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bJustPeakHigh = atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_PEAK_VAL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dJustPeakChgValue = atof(token);
		}
		else if (strcmp(token, LABEL_JUST_PEAK_SELECT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.ePeakDetectType = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_PEAK_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dJustPeakChgRate = atof(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.bBoxDetect = atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_SELECT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.eBoxDetectType = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_RATE) == 0) {
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dBoxRate = atof(token);
		}

		else if (strcmp(token, LABEL_JUST_BOX_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.eBoxType = (BOX_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_LEVEL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.dBoxLevel = atof(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_TIME) == 0) {
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.fBoxTime = (float)atof(token);
		}
		else if (strcmp(token, LABEL_JUST_BOX_WAVE) == 0) {
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tRecipe.Step[m_tRecipe.iStepCnt].tJust.eWaveType = (WAVE_TYPE)atoi(token);
		}

		// End
		else if (strcmp(token, LABEL_END_LEVEL_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.bEndLevelEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_END_LEVEL) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndLevel = atof(token);
		}
		else if (strcmp(token, LABEL_END_LVL_TYPE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.eLevelType = (LEVEL_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_END_DIFF1_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.bEndDiff1Enable = atoi(token);
		}
		else if (strcmp(token, LABEL_END_DIFF1_SELECT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.eEndDiffJustSelect = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_END_DIFF1) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndDiffFromJust = atof(token);
		}
		else if (strcmp(token, LABEL_END_DIFF1_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndDiffFromJustRate = atof(token);
		}
		else if (strcmp(token, LABEL_END_DIFF2_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.bEndDiff2Enable = atoi(token);
		}
		else if (strcmp(token, LABEL_END_DIFF2_SELECT) == 0) {
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.eEndDiffPeakSelect = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_END_DIFF2) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndDiffFromPeak = atof(token);
		}
		else if (strcmp(token, LABEL_END_DIFF2_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndDiffFromPeakRate = atof(token);
		}
		else if (strcmp(token, LABEL_END_FLAT_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.bEndFlatEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_END_FLAT_SELECT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.eEndDiffFlatSelect = (DETECT_TYPE)atoi(token);
		}
		else if (strcmp(token, LABEL_END_FLAT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndFlatWave = atof(token);
		}
		else if (strcmp(token, LABEL_END_FLAT_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.dEndFlatRate = atof(token);
		}
		else if (strcmp(token, LABEL_END_FLAT_TIME) == 0) {
		token = strtok_s(NULL, ",", &pContext);
		if (token == NULL)
			continue;
		m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.fEndFlatTime= atof(token);
		}
		else if (strcmp(token, LABEL_END_LIMIT_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.bEndLimitEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_END_LIMIT) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tEnd.fEndLimitTime = atof(token);
		}

		// Over Etch
		else if (strcmp(token, LABEL_OVET_TIME_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tOverEtch.bOverEtchTimeEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_OVET_TIME) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tOverEtch.fOverEtchTime = atof(token);
		}
		else if (strcmp(token, LABEL_OVET_RATE_SW) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tOverEtch.bOverEtchRateEnable = atoi(token);
		}
		else if (strcmp(token, LABEL_OVET_RATE) == 0) {
			token = strtok_s(NULL, ",", &pContext);
			if (token == NULL)
				continue;
			m_tRecipe.Step[m_tRecipe.iStepCnt].tOverEtch.fOverEtchRate = atof(token);
		}
	}/* while */

	// 微分表示スケール初期化
	if (m_tRecipe.dDiffDisplayMax < 0.000001f)
		m_tRecipe.dDiffDisplayMax = 0.001f;

	if(rv == RCP_FILE_SUCCESS){
		m_nLoadRecipeNo = iRcpNo;
		m_szLoadRecipeName = szFileName;
		m_szLoadRecipeTitle = m_tRecipe.szRecipeName;
		fp_Recipe.GetStatus(m_LoadFileStatus);
	}
	fp_Recipe.Close();
	m_iNew = FALSE;

	m_tOldRecipe = m_tRecipe;

	LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
	return(rv);
}
/*
** レシピファイルセーブ
*/
int CRecipe::RecipeFileSave(int nPMNo,int iRcpNo)
{
	int		i;
	int	rv = RCP_FILE_SUCCESS;
	CString		szBuf('\0',128);
	CString	szFileName('\0',64);
	char	szPPID[40+1];
	CFileException	e;

	EnterCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);

	// ロードレシピNo.クリア
	m_nLoadRecipeNo = 0;
	m_szLoadRecipeName = _T("");
	m_szLoadRecipeTitle = _T("");

	// パラメーターチェック
	if(nPMNo < PM1 || nPMNo > PM5){
		LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
		return(RCP_FILE_INV_PARAM);
	}
	//レシピファイル保存パスチェック
	CFileFind	filefind;
	szFileName.Format("%s",gpszRecipePath);
	if(filefind.FindFile((LPCSTR)szFileName)== FALSE){
		if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE){
			LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
			return(RCP_FILE_OPENERR);
		}
	}
	else{
		filefind.FindNextFile();
		if(filefind.IsDirectory()==FALSE){
			if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE){
				filefind.Close();
				LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
				return(RCP_FILE_OPENERR);
			}
		}
		filefind.Close();
	}
	szFileName.Format("%s\\PM%d",gpszRecipePath,(nPMNo+1));


	if(filefind.FindFile((LPCSTR)szFileName)== FALSE){
		if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE){
			LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
			return(RCP_FILE_OPENERR);
		}
	}
	else{
		filefind.FindNextFile();
		if(filefind.IsDirectory()==FALSE){
			if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE){
				filefind.Close();
				LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
				return(RCP_FILE_OPENERR);
			}
		}
		filefind.Close();
	}
	
	//レシピファイルのオープン
	szFileName.Format("%s\\PM%d\\RCP%03.03d.csv",gpszRecipePath,(nPMNo+1),(iRcpNo));

	CStdioFile fp_Recipe;

	if (!fp_Recipe.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyNone, &e)) {
		int iError = GetLastError();
		rv = RCP_FILE_OPENERR;
		LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
		return(rv);
	}

	szBuf.Format("%s,%s\n",LABEL_RCPNAME,m_tRecipe.szRecipeName);
	fp_Recipe.WriteString((LPCSTR)szBuf);

	szBuf.Format("%s,%s\n",LABEL_COMMENT,m_tRecipe.szComment);
	fp_Recipe.WriteString((LPCSTR)szBuf);

	szBuf.Format("%s,%03.03d\n", LABEL_VERSION, m_tRecipe.iVersion);
	fp_Recipe.WriteString((LPCSTR)szBuf);

	szBuf.Format("%s,%d\n", LABEL_INTEGTIME, m_tRecipe.iIntegrationTime);
	fp_Recipe.WriteString((LPCSTR)szBuf);

	if (m_tRecipe.dDiffDisplayMax < 0.000001f)
		m_tRecipe.dDiffDisplayMax = 0.001f;
	szBuf.Format("%s,%5.3f\n", LABEL_DIFFDISPMAX, m_tRecipe.dDiffDisplayMax);
	fp_Recipe.WriteString((LPCSTR)szBuf);

	szBuf.Format("%s,%d\n", LABEL_SAMPLINGTIME, m_tRecipe.iSamplingTime);
	fp_Recipe.WriteString((LPCSTR)szBuf);


	for (i = 0; i < m_tRecipe.iStepCnt; i++) {
		szBuf.Format("%s,\n", LABEL_STEP_START);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_USE_WAVE, m_tRecipe.Step[i].iUseWave);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%s\n", LABEL_STEP_COMMENT, m_tRecipe.Step[i].szComment);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		// Start
		szBuf.Format("%s,%6.2f,Sec\n", LABEL_DEAD_TIME, m_tRecipe.Step[i].tStart.fDeadTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_START_LVL_SW, m_tRecipe.Step[i].tStart.bStartLevelEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_START_LEVEL, m_tRecipe.Step[i].tStart.dStartLevel);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_START_LVL_TYPE, (int)m_tRecipe.Step[i].tStart.eLevelType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_START_LVL_WAVE, (int)m_tRecipe.Step[i].tStart.eWaveType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_START_LIMIT_SW, m_tRecipe.Step[i].tStart.bStartLimitEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_START_LIMIT, m_tRecipe.Step[i].tStart.fStartLimitTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_SEL_AUTO_GAIN, m_tRecipe.Step[i].tStart.bAutoGain);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_AUTO_GAIN_LVL, m_tRecipe.Step[i].tStart.dAutoGainLevel);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_AUTO_GAIN_TYPE, m_tRecipe.Step[i].tStart.eAutoGainType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_AUTO_GAIN_PEAK_TYPE, m_tRecipe.Step[i].tStart.eAutoGainPeakDetectType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_AUTO_GAIN_PEAK_LEVEL, m_tRecipe.Step[i].tStart.dStartAutoGainPeakChgValue);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_AUTO_GAIN_PEAK_RATE, m_tRecipe.Step[i].tStart.dStartAutoGainPeakChgRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		// Just
		szBuf.Format("%s,%d\n", LABEL_JUST_LEVEL_SW, m_tRecipe.Step[i].tJust.bJustLevelEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_JUST_LEVEL, m_tRecipe.Step[i].tJust.dJustLevel);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_LVL_TYPE, (int)m_tRecipe.Step[i].tJust.eLevelType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_DIFF_SW, m_tRecipe.Step[i].tJust.bJustDiffEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_JUST_DIFF, m_tRecipe.Step[i].tJust.dJustDiffFromStart);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_LIMIT_SW, m_tRecipe.Step[i].tJust.bJustLimitEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_JUST_LIMIT, m_tRecipe.Step[i].tJust.fJustLimitTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_PEAK, m_tRecipe.Step[i].tJust.bJustPeakDetect);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_PEAK_SELECT, m_tRecipe.Step[i].tJust.ePeakDetectType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_PEAK_HL, m_tRecipe.Step[i].tJust.bJustPeakHigh);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f\n", LABEL_JUST_PEAK_VAL, m_tRecipe.Step[i].tJust.dJustPeakChgValue);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f\n", LABEL_JUST_PEAK_RATE, m_tRecipe.Step[i].tJust.dJustPeakChgRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		// Just Box
		szBuf.Format("%s,%d\n", LABEL_JUST_BOX_SW, m_tRecipe.Step[i].tJust.bBoxDetect);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_BOX_SELECT, m_tRecipe.Step[i].tJust.eBoxDetectType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_BOX_TYPE, m_tRecipe.Step[i].tJust.eBoxType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f\n", LABEL_JUST_BOX_LEVEL, m_tRecipe.Step[i].tJust.dBoxLevel);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f\n", LABEL_JUST_BOX_RATE, m_tRecipe.Step[i].tJust.dBoxRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);
		
		szBuf.Format("%s,%6.2f,Sec\n", LABEL_JUST_BOX_TIME, m_tRecipe.Step[i].tJust.fBoxTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_JUST_BOX_WAVE, m_tRecipe.Step[i].tJust.eWaveType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		// End
		szBuf.Format("%s,%d\n", LABEL_END_LEVEL_SW, m_tRecipe.Step[i].tEnd.bEndLevelEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_END_LEVEL, m_tRecipe.Step[i].tEnd.dEndLevel);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_LVL_TYPE, (int)m_tRecipe.Step[i].tEnd.eLevelType);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_DIFF1_SW, m_tRecipe.Step[i].tEnd.bEndDiff1Enable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_DIFF1_SELECT, m_tRecipe.Step[i].tEnd.eEndDiffJustSelect);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_END_DIFF1, m_tRecipe.Step[i].tEnd.dEndDiffFromJust);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_END_DIFF1_RATE, m_tRecipe.Step[i].tEnd.dEndDiffFromJustRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_DIFF2_SW, m_tRecipe.Step[i].tEnd.bEndDiff2Enable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_DIFF2_SELECT, m_tRecipe.Step[i].tEnd.eEndDiffPeakSelect);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_END_DIFF2, m_tRecipe.Step[i].tEnd.dEndDiffFromPeak);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_END_DIFF2_RATE, m_tRecipe.Step[i].tEnd.dEndDiffFromPeakRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_FLAT_SW, m_tRecipe.Step[i].tEnd.bEndFlatEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_FLAT_SELECT, m_tRecipe.Step[i].tEnd.eEndDiffFlatSelect);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%f,dg\n", LABEL_END_FLAT, m_tRecipe.Step[i].tEnd.dEndFlatWave);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,dg\n", LABEL_END_FLAT_RATE, m_tRecipe.Step[i].tEnd.dEndFlatRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_END_FLAT_TIME, m_tRecipe.Step[i].tEnd.fEndFlatTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_END_LIMIT_SW, m_tRecipe.Step[i].tEnd.bEndLimitEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_END_LIMIT, m_tRecipe.Step[i].tEnd.fEndLimitTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		// End
		szBuf.Format("%s,%d\n", LABEL_OVET_TIME_SW, m_tRecipe.Step[i].tOverEtch.bOverEtchTimeEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_OVET_TIME, m_tRecipe.Step[i].tOverEtch.fOverEtchTime);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%d\n", LABEL_OVET_RATE_SW, m_tRecipe.Step[i].tOverEtch.bOverEtchRateEnable);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,%6.2f,Sec\n", LABEL_OVET_RATE, m_tRecipe.Step[i].tOverEtch.fOverEtchRate);
		fp_Recipe.WriteString((LPCSTR)szBuf);

		szBuf.Format("%s,\n", LABEL_STEP_END);
		fp_Recipe.WriteString((LPCSTR)szBuf);
	}/* for */

	if(rv == RCP_FILE_SUCCESS){
		m_nLoadRecipeNo = iRcpNo;
		m_szLoadRecipeName = szFileName;
		m_szLoadRecipeTitle = m_tRecipe.szRecipeName;
		
		fp_Recipe.GetStatus(m_LoadFileStatus);

		CPCRcpCtrl* pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();
		pRcpCtrl->UpdatePCRecipe(nPMNo, iRcpNo, &m_tRecipe, m_LoadFileStatus.m_mtime);
		m_tOldRecipe = m_tRecipe;
	}
	fp_Recipe.Close();
	LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);

	return(rv);
}
int CRecipe::RecipeGetDate(CTime &filetime)
{
	filetime = m_LoadFileStatus.m_mtime;
	return(RCP_FILE_SUCCESS);
}


int CRecipe::RecipeFileDelete(int nPMNo, int iRcpNo)
{
	int	rv = RCP_FILE_SUCCESS;
	CString	szFileName('\0',64);

	EnterCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);

	// パラメーターチェック
	if(nPMNo < PM1 || nPMNo > PM5){
		LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
		return(RCP_FILE_INV_PARAM);
	}

	szFileName.Format("%s\\PM%d\\RCP%03.03d.csv",gpszRecipePath,(nPMNo+1),(iRcpNo));

	TRY
	{    
		CFile::Remove(szFileName);
	}
	CATCH( CFileException, e )
	{
		DWORD dw = GetLastError();

		rv  = RCP_FILE_DELERR;
	}
	END_CATCH
	// ロードレシピNo.クリア
	if(rv == RCP_FILE_SUCCESS){
		m_nLoadRecipeNo = 0;
		m_szLoadRecipeName = _T("");
		m_szLoadRecipeTitle = _T("");

		CPCRcpCtrl* pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();
		pRcpCtrl->DeletePCRecipe(nPMNo, iRcpNo);

	}
	LeaveCriticalSection(&((CWaveEndPointApp*)AfxGetApp())->m_Recipe_Interlock);
	return(rv);
}

int CRecipe::RecipeFindbyName(int iPMNo, char *pszRcpName)
{
	int		iRcpNo;

	CPCRcpCtrl	*pPCRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();

	iRcpNo = pPCRcpCtrl->FindRcpbyName(iPMNo,pszRcpName);

	if(iRcpNo){
		if(RecipeFileLoad(iPMNo,iRcpNo) == RCP_FILE_SUCCESS){
			return(iRcpNo);
		}
	}
	return(0);
}
/*
**============================================**
** PC Recipe Control for Update
** 
*/

CPCRcpCtrl::CPCRcpCtrl()
{
	int	iPM;

	for(iPM = PM1;iPM <PM_MAX;iPM++)
		memset(m_TblPCRecipe[iPM],'\0',sizeof(PCRECIPECTRL)*RCP_FILE_MAX);
}

CPCRcpCtrl::~CPCRcpCtrl()
{
}
int CPCRcpCtrl::MakeTable()
{
	int			iPM,i;
	CRecipe			*pRecipe;

	pRecipe = new CRecipe();

	for(iPM=PM1;iPM <PM_MAX;iPM++){

		/*
		** Process Recipe
		*/
		for(i=0;i<RCP_FILE_MAX;i++){
			if(pRecipe->RecipeFileLoad(iPM,(i+1)) != RCP_FILE_SUCCESS){
				m_TblPCRecipe[iPM][i].iEnable=FALSE;
			}
			else{
				m_TblPCRecipe[iPM][i].iEnable=TRUE;
				strncpy(m_TblPCRecipe[iPM][i].szRecipeName,pRecipe->m_tRecipe.szRecipeName,RCP_LEN_NAME);
				m_TblPCRecipe[iPM][i].szRecipeName[RCP_LEN_NAME] = '\0';
				strncpy(m_TblPCRecipe[iPM][i].szComment,pRecipe->m_tRecipe.szComment,RCP_LEN_COMMENT);
				m_TblPCRecipe[iPM][i].szComment[RCP_LEN_COMMENT] = '\0';
				m_TblPCRecipe[iPM][i].iUpdateYear=pRecipe->m_LoadFileStatus.m_mtime.GetYear();
				m_TblPCRecipe[iPM][i].iUpdateMonth=pRecipe->m_LoadFileStatus.m_mtime.GetMonth();
				m_TblPCRecipe[iPM][i].iUpdateDay=pRecipe->m_LoadFileStatus.m_mtime.GetDay();
				m_TblPCRecipe[iPM][i].iUpdateHour=pRecipe->m_LoadFileStatus.m_mtime.GetHour();
				m_TblPCRecipe[iPM][i].iUpdateMinute=pRecipe->m_LoadFileStatus.m_mtime.GetMinute();
				m_TblPCRecipe[iPM][i].iUpdateSecond=pRecipe->m_LoadFileStatus.m_mtime.GetSecond();
			}
		}/* for */

	}/* for */
	delete pRecipe;

	return(TRUE);
}
int CPCRcpCtrl::UpdatePCRecipe(int iPM,int iRcpNo,RECIPE *pRecipe,CTime &tTime)
{
	m_TblPCRecipe[iPM][iRcpNo-1].iEnable=TRUE;
	strncpy(m_TblPCRecipe[iPM][iRcpNo-1].szRecipeName,pRecipe->szRecipeName,RCP_LEN_NAME);
	m_TblPCRecipe[iPM][iRcpNo-1].szRecipeName[RCP_LEN_NAME] = '\0';
	strncpy(m_TblPCRecipe[iPM][iRcpNo-1].szComment,pRecipe->szComment,RCP_LEN_COMMENT);
	m_TblPCRecipe[iPM][iRcpNo-1].szComment[RCP_LEN_COMMENT] = '\0';
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateYear=tTime.GetYear();
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateMonth=tTime.GetMonth();
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateDay=tTime.GetDay();
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateHour=tTime.GetHour();
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateMinute=tTime.GetMinute();
	m_TblPCRecipe[iPM][iRcpNo-1].iUpdateSecond=tTime.GetSecond();

	return(TRUE);
}
int CPCRcpCtrl::DeletePCRecipe(int iPM,int iRcpNo)
{
	m_TblPCRecipe[iPM][iRcpNo-1].iEnable=FALSE;
	memset(&m_TblPCRecipe[iPM][iRcpNo-1],'\0',sizeof(PCRECIPECTRL));
	return(TRUE);
}
int CPCRcpCtrl::GetRecipeInfo(int iPM,int iRcpNo,char *pszRecipeName,char *pszComment,char *pszDateTime)
{
	if(m_TblPCRecipe[iPM][iRcpNo-1].iEnable==FALSE)
		return(FALSE);

	strncpy(pszRecipeName,m_TblPCRecipe[iPM][iRcpNo-1].szRecipeName,RCP_LEN_NAME);
	strncpy(pszComment,m_TblPCRecipe[iPM][iRcpNo-1].szComment,RCP_LEN_COMMENT);
	sprintf(pszDateTime,"%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateYear,
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateMonth,
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateDay,
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateHour,
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateMinute,
			m_TblPCRecipe[iPM][iRcpNo-1].iUpdateSecond);
	return(TRUE);
}
int CPCRcpCtrl::CheckRcpNameExists(int iPM,int iRcpNo,char *pszRecipeName)
{
	int			i;
	CString		str1('\0',64);
	CString		str2('\0',64);

	str1 = pszRecipeName;
	str1 +=_T("                                ");
	str1= str1.Left(RCP_LEN_NAME);
	str1.MakeUpper();

	for(i=0;i<RCP_FILE_MAX;i++){
		if(m_TblPCRecipe[iPM][i].iEnable==FALSE)
			continue;

		if(iRcpNo == (i+1))
			continue;

		str2 = m_TblPCRecipe[iPM][i].szRecipeName;
		str2 +=_T("                                ");
		str2= str2.Left(RCP_LEN_NAME);
		str2.MakeUpper();

		if(str1==str2)
			return(FALSE);

	}/* for */
	return(TRUE);


}
int CPCRcpCtrl::FindRcpbyName(int iPM,char *pszRecipeName)
{
	int			i;
	CString		str1('\0',32);
	CString		str2('\0',32);

	str1 = pszRecipeName;
	str1 +=_T("                                ");
	str1= str1.Left(RCP_LEN_NAME);
	str1.MakeUpper();

	for(i=0;i<RCP_FILE_MAX;i++){
		if(m_TblPCRecipe[iPM][i].iEnable==FALSE)
			continue;

		str2 = m_TblPCRecipe[iPM][i].szRecipeName;
		str2 +=_T("                                ");
		str2= str2.Left(RCP_LEN_NAME);
		str2.MakeUpper();

		if(str1==str2)
			return((i+1));

	}/* for */
	return(0);
}
int CPCRcpCtrl::GetInfoFirst(int iPM, int *piRcpNo, char* pszRecipeName, char* pszComment, char* pszDateTime)
{

	for (m_iFindPoint = 0; m_iFindPoint < RCP_FILE_MAX; m_iFindPoint++) {
		if (m_TblPCRecipe[iPM][m_iFindPoint].iEnable == FALSE)
			continue;

		strncpy(pszRecipeName, m_TblPCRecipe[iPM][m_iFindPoint].szRecipeName, RCP_LEN_NAME);
		strncpy(pszComment, m_TblPCRecipe[iPM][m_iFindPoint].szComment, RCP_LEN_COMMENT);
		sprintf(pszDateTime, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateYear,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateMonth,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateDay,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateHour,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateMinute,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateSecond);
		*piRcpNo = m_iFindPoint + 1;
		m_iFindPoint++;
		return(TRUE);
	}
	return(FALSE);
}
int CPCRcpCtrl::GetInfoNext(int iPM, int *piRcpNo, char* pszRecipeName, char* pszComment, char* pszDateTime)
{
	if (m_iFindPoint >= RCP_FILE_MAX)
		return(FALSE);

	for (; m_iFindPoint < RCP_FILE_MAX; m_iFindPoint++) {
		if (m_TblPCRecipe[iPM][m_iFindPoint].iEnable == FALSE)
			continue;

		strncpy(pszRecipeName, m_TblPCRecipe[iPM][m_iFindPoint].szRecipeName, RCP_LEN_NAME);
		strncpy(pszComment, m_TblPCRecipe[iPM][m_iFindPoint].szComment, RCP_LEN_COMMENT);
		sprintf(pszDateTime, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateYear,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateMonth,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateDay,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateHour,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateMinute,
			m_TblPCRecipe[iPM][m_iFindPoint].iUpdateSecond);

		*piRcpNo = m_iFindPoint + 1;
		m_iFindPoint++;
		return(TRUE);
	}
	return(FALSE);
}