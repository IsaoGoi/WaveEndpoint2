#include "pch.h"
#include "WaveEndPoint.h"
#include "CCalcWaveCfg.h"

static	char* gpszWaveCfgPath = "WaveCfg";

CCalcWaveCfg::CCalcWaveCfg()
{
	// ロードレシピNo.
	m_nLoadWaveCfgNo = 0;

	//ファイル検索
	m_pFileFind = new CFileFind();


	// メモリの確保
	h_WaveCfg = GlobalAlloc(GPTR, sizeof(SET_WAVECFG));
	m_pWaveCfg = (SET_WAVECFG*)GlobalLock(h_WaveCfg);

	h_OldWaveCfg = GlobalAlloc(GPTR, sizeof(SET_WAVECFG));
	m_pOldWaveCfg = (SET_WAVECFG*)GlobalLock(h_OldWaveCfg);
}
CCalcWaveCfg::~CCalcWaveCfg()
{
	GlobalUnlock(h_WaveCfg);
	GlobalFree(h_WaveCfg);

	GlobalUnlock(h_OldWaveCfg);
	GlobalFree(h_OldWaveCfg);

	delete m_pFileFind;
}

int	CCalcWaveCfg::LoadWaveCfg(int iPM, int  iWaveCfgNo)
{
	int	rv = FALSE;
	CString	szStr('\0', 128);
	char	szBuff[128];
	CString	szFileName('\0', 64);
	char*	p;

	// ロードレシピNo.クリア
	m_nLoadWaveCfgNo = 0;
	m_szLoadWaveCfgName = _T("");
	m_szLoadWaveCfgTitle = _T("");

	m_iNew = TRUE;

	memset((char*)m_pWaveCfg, '\0', sizeof(SET_WAVECFG));

	CStdioFile fp_WaveCfg;

	szFileName.Format("%s\\PC%d\\Wave%03.03d.csv", gpszWaveCfgPath, (iPM+1),iWaveCfgNo);
	if (!fp_WaveCfg.Open(szFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone)) {
		return(FALSE);
	}

	while (fp_WaveCfg.ReadString(szStr)) {
		strcpy(szBuff, (LPCSTR)szStr);
		p = strtok(szBuff, ",");
		if (p) {
			if (strncmp(p, CALC_CFG_NAME, strlen(CALC_CFG_NAME)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					strncpy(m_pWaveCfg->szWaveCfgName,p, WAVECFG_LEN_NAME);
			}
			else if (strncmp(p, CALC_CFG_COMMENT, strlen(CALC_CFG_COMMENT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					strncpy(m_pWaveCfg->szWaveCfgComment, p, WAVECFG_LEN_COMMENT);
			}
			/*
			** Wave-A 設定
			*/
			else if (strncmp(p, CALC_CFG_STEP1_BASEWAVE_A, strlen(CALC_CFG_STEP1_BASEWAVE_A)) == 0) {
				p = strtok(NULL, ",");
				if (p) {
					m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo = atoi(p);
					rv = TRUE;
				}
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_COUNT, strlen(CALC_CFG_STEP1_A_COUNT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_CALC1_TYPE, strlen(CALC_CFG_STEP1_A_CALC1_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_CALC1_VAL, strlen(CALC_CFG_STEP1_A_CALC1_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_CALC2_TYPE, strlen(CALC_CFG_STEP1_A_CALC2_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_CALC2_VAL, strlen(CALC_CFG_STEP1_A_CALC2_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_A_RECIPROCAL, strlen(CALC_CFG_STEP1_A_RECIPROCAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal = atoi(p);
			}

			/*
			** Wave-B1 設定
			*/
			else if (strncmp(p, CALC_CFG_STEP1_BASEWAVE_B1, strlen(CALC_CFG_STEP1_BASEWAVE_B1)) == 0) {
				p = strtok(NULL, ",");
				if (p) {
					m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo = atoi(p);
					rv = TRUE;
				}
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_COUNT, strlen(CALC_CFG_STEP1_B1_COUNT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_CALC1_TYPE, strlen(CALC_CFG_STEP1_B1_CALC1_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_CALC1_VAL, strlen(CALC_CFG_STEP1_B1_CALC1_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_CALC2_TYPE, strlen(CALC_CFG_STEP1_B1_CALC2_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_CALC2_VAL, strlen(CALC_CFG_STEP1_B1_CALC2_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B1_RECIPROCAL, strlen(CALC_CFG_STEP1_B1_RECIPROCAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal = atoi(p);
			}

			/*
			** Wave-B2 設定
			*/
			else if (strncmp(p, CALC_CFG_STEP1_BASEWAVE_B2, strlen(CALC_CFG_STEP1_BASEWAVE_B2)) == 0) {
				p = strtok(NULL, ",");
				if (p) {
					m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo = atoi(p);
					rv = TRUE;
				}
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_COUNT, strlen(CALC_CFG_STEP1_B2_COUNT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_CALC1_TYPE, strlen(CALC_CFG_STEP1_B2_CALC1_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_CALC1_VAL, strlen(CALC_CFG_STEP1_B2_CALC1_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_CALC2_TYPE, strlen(CALC_CFG_STEP1_B2_CALC2_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2 = (CALC_OFFSETGAIN)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_CALC2_VAL, strlen(CALC_CFG_STEP1_B2_CALC2_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2 = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP1_B2_RECIPROCAL, strlen(CALC_CFG_STEP1_B2_RECIPROCAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal = atoi(p);
			}

			/*
			** Step2 : A/B1/B2演算
			*/
			else if (strncmp(p, CALC_CFG_STEP2_WAVE_SELECT1, strlen(CALC_CFG_STEP2_WAVE_SELECT1)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep2_WaveJoin.eWave1 = (CALC_WAVETYPE)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP2_CALC1, strlen(CALC_CFG_STEP2_CALC1)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep2_WaveJoin.eOperandType1 = (CALC_OPERAND)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP2_WAVE_SELECT2, strlen(CALC_CFG_STEP2_WAVE_SELECT2)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep2_WaveJoin.eWave2 = (CALC_WAVETYPE2)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP2_CALC2, strlen(CALC_CFG_STEP2_CALC2)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep2_WaveJoin.eOperandType2 = (CALC_OPERAND)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP2_WAVE_SELECT3, strlen(CALC_CFG_STEP2_WAVE_SELECT3)) == 0) {
				p = strtok(NULL, ",");
					if (p)
						m_pWaveCfg->tStep2_WaveJoin.eWave3 = (CALC_WAVETYPE2)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP2_AVECNT, strlen(CALC_CFG_STEP2_AVECNT)) == 0) {
				p = strtok(NULL, ",");
					if (p)
						m_pWaveCfg->tStep2_WaveJoin.iAveCnt = atoi(p);
			}

			/*
			** Step3 : 逆数指定
			*/
			else if (strncmp(p, CALC_CFG_STEP3_RECIPROCAL, strlen(CALC_CFG_STEP3_RECIPROCAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.iReciprocal = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP3_OFFSET_TYPE, strlen(CALC_CFG_STEP3_OFFSET_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.eOffsetType = (CALC_USER_SET)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP3_OFFSET_VAL, strlen(CALC_CFG_STEP3_OFFSET_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.dUserOffset = atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP3_MAX_TYPE, strlen(CALC_CFG_STEP3_MAX_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType = (CALC_USER_SET)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP3_MAX_VAL, strlen(CALC_CFG_STEP3_MAX_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel= atof(p);
			}
			else if (strncmp(p, CALC_CFG_STEP3_AVECNT, strlen(CALC_CFG_STEP3_AVECNT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep3_WaveNorm.iAveCnt = atoi(p);
			}

			/*
			** Step4 : 微分演算
			*/
			else if (strncmp(p, CALC_CFG_STEP4_DIFF1, strlen(CALC_CFG_STEP4_DIFF1)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_REPEAT_CNT1, strlen(CALC_CFG_STEP4_REPEAT_CNT1)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_DIFF2, strlen(CALC_CFG_STEP4_DIFF2)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_REPEAT_CNT2, strlen(CALC_CFG_STEP4_REPEAT_CNT2)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_AVECNT, strlen(CALC_CFG_STEP4_AVECNT)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_CORRECT_TYPE, strlen(CALC_CFG_STEP4_CORRECT_TYPE)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect = (CALC_OPERAND)atoi(p);
			}
			else if (strncmp(p, CALC_CFG_STEP4_CORRECT_VAL, strlen(CALC_CFG_STEP4_CORRECT_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal = atof(p);
			}
			/*
			** 微分表示Scale設定
			*/
			else if (strncmp(p, CALC_CFG_DIFF_DISPLAY, strlen(CALC_CFG_DIFF_DISPLAY)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable = atoi(p);
			}
			else if (strncmp(p, CALC_CFG_DIFF_VAL, strlen(CALC_CFG_DIFF_VAL)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val = atof(p);
			}
			else if (strncmp(p, CALC_CFG_DIFF_EXP, strlen(CALC_CFG_DIFF_EXP)) == 0) {
				p = strtok(NULL, ",");
				if (p)
					m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp = atoi(p);
			}
		}
	}/* while*/
	// Data無効
	if(rv == FALSE)
		memset((char*)m_pWaveCfg, '\0', sizeof(SET_WAVECFG));
	else{
		m_nLoadWaveCfgNo = iWaveCfgNo;
		m_szLoadWaveCfgName = szFileName;
		m_szLoadWaveCfgTitle = m_pWaveCfg->szWaveCfgName;
		fp_WaveCfg.GetStatus(m_LoadFileStatus);
		m_iNew = FALSE;
		m_pOldWaveCfg = m_pWaveCfg;
	}
	fp_WaveCfg.Close();
	return(rv);
}
int	CCalcWaveCfg::SaveWaveCfg(int iPM, int iWaveCfgNo)
{
	int			rv = FALSE;
	CStdioFile	fp;
	char		szBuff[128];
	CString	szFileName('\0', 64);

	//レシピファイル保存パスチェック
	CFileFind	filefind;
	szFileName.Format("%s", gpszWaveCfgPath);
	if (filefind.FindFile((LPCSTR)szFileName) == FALSE) {
		if (CreateDirectory((LPCSTR)szFileName, NULL) == FALSE) {
			return(FALSE);
		}
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory((LPCSTR)szFileName, NULL) == FALSE) {
				return(FALSE);
			}
		}
		filefind.Close();
	}

	szFileName.Format("%s\\PC%d", gpszWaveCfgPath,(iPM+1));
	if (filefind.FindFile((LPCSTR)szFileName) == FALSE) {
		if (CreateDirectory((LPCSTR)szFileName, NULL) == FALSE) {
			return(FALSE);
		}
	}
	else {
		filefind.FindNextFile();
		if (filefind.IsDirectory() == FALSE) {
			if (CreateDirectory((LPCSTR)szFileName, NULL) == FALSE) {
				return(FALSE);
			}
		}
		filefind.Close();
	}


	//レシピファイルのオープン
	szFileName.Format("%s\\PC%d\\Wave%03.3d.csv", gpszWaveCfgPath,(iPM+1),iWaveCfgNo);

	if (!fp.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
		int iError = GetLastError();
		return(FALSE);
	}
	sprintf(szBuff, "%s,%s\n", CALC_CFG_NAME, m_pWaveCfg->szWaveCfgName);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%s\n", CALC_CFG_COMMENT, m_pWaveCfg->szWaveCfgComment);
	fp.WriteString(szBuff);


	/* Step1 Wave
	** Wave-A 設定
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_BASEWAVE_A, m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_A_COUNT, m_pWaveCfg->tStep1_Wave_AB[0].iWaveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_A_CALC1_TYPE, m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_A_CALC1_VAL, m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_A_CALC2_TYPE, m_pWaveCfg->tStep1_Wave_AB[0].eGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_A_CALC2_VAL, m_pWaveCfg->tStep1_Wave_AB[0].dGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_A_RECIPROCAL, m_pWaveCfg->tStep1_Wave_AB[0].iReciprocal);
	fp.WriteString(szBuff);

	/* 
	** Wave-B1 設定
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_BASEWAVE_B1, m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B1_COUNT, m_pWaveCfg->tStep1_Wave_AB[1].iWaveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B1_CALC1_TYPE, m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_B1_CALC1_VAL, m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B1_CALC2_TYPE, m_pWaveCfg->tStep1_Wave_AB[1].eGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_B1_CALC2_VAL, m_pWaveCfg->tStep1_Wave_AB[1].dGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B1_RECIPROCAL, m_pWaveCfg->tStep1_Wave_AB[1].iReciprocal);
	fp.WriteString(szBuff);

	/*
	** Wave-B2 設定
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_BASEWAVE_B2, m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B2_COUNT, m_pWaveCfg->tStep1_Wave_AB[2].iWaveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B2_CALC1_TYPE, m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_B2_CALC1_VAL, m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B2_CALC2_TYPE, m_pWaveCfg->tStep1_Wave_AB[2].eGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP1_B2_CALC2_VAL, m_pWaveCfg->tStep1_Wave_AB[2].dGainOffset2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP1_B2_RECIPROCAL, m_pWaveCfg->tStep1_Wave_AB[2].iReciprocal);
	fp.WriteString(szBuff);

	/*
	** Step2 : A/B1/B2演算
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_WAVE_SELECT1, m_pWaveCfg->tStep2_WaveJoin.eWave1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_CALC1, m_pWaveCfg->tStep2_WaveJoin.eOperandType1);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_WAVE_SELECT2, m_pWaveCfg->tStep2_WaveJoin.eWave2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_CALC2, m_pWaveCfg->tStep2_WaveJoin.eOperandType2);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_WAVE_SELECT3, m_pWaveCfg->tStep2_WaveJoin.eWave3);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP2_AVECNT, m_pWaveCfg->tStep2_WaveJoin.iAveCnt);
	fp.WriteString(szBuff);

	/*
	** Step3 : 逆数指定
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP3_RECIPROCAL, m_pWaveCfg->tStep3_WaveNorm.iReciprocal);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP3_OFFSET_TYPE, m_pWaveCfg->tStep3_WaveNorm.eOffsetType);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP3_OFFSET_VAL, m_pWaveCfg->tStep3_WaveNorm.dUserOffset);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP3_MAX_TYPE, m_pWaveCfg->tStep3_WaveNorm.eMaxLevelType);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP3_MAX_VAL, m_pWaveCfg->tStep3_WaveNorm.dUserMaxLevel);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP3_AVECNT, m_pWaveCfg->tStep3_WaveNorm.iAveCnt);
	fp.WriteString(szBuff);

	/*
	** Step4 : 微分演算
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_DIFF1, m_pWaveCfg->tStep4_WaveDiff.iDiff1Enable);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_REPEAT_CNT1, m_pWaveCfg->tStep4_WaveDiff.iDiff1RptAveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_DIFF2, m_pWaveCfg->tStep4_WaveDiff.iDiff2Enable);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_REPEAT_CNT2, m_pWaveCfg->tStep4_WaveDiff.iDiff2RptAveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_AVECNT, m_pWaveCfg->tStep4_WaveDiff.iDiffAveCnt);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_STEP4_CORRECT_TYPE, m_pWaveCfg->tStep4_WaveDiff.eDiffCorrect);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_STEP4_CORRECT_VAL, m_pWaveCfg->tStep4_WaveDiff.dDiffCorrectVal);
	fp.WriteString(szBuff);

	/*
	** 微分表示Scale設定
	*/
	sprintf(szBuff, "%s,%d\n", CALC_CFG_DIFF_DISPLAY, m_pWaveCfg->tDiffDisplayScale.iDisplayScaleEnable);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%f\n", CALC_CFG_DIFF_VAL, m_pWaveCfg->tDiffDisplayScale.dDisplayVal_val);
	fp.WriteString(szBuff);

	sprintf(szBuff, "%s,%d\n", CALC_CFG_DIFF_EXP, m_pWaveCfg->tDiffDisplayScale.iDisplayVal_exp);
	fp.WriteString(szBuff);

	m_nLoadWaveCfgNo = iWaveCfgNo;
	m_szLoadWaveCfgName = szFileName;
	m_szLoadWaveCfgTitle = m_pWaveCfg->szWaveCfgName;
	fp.GetStatus(m_LoadFileStatus);
	m_iNew = FALSE;
	m_pOldWaveCfg = m_pWaveCfg;


	CWaveCfgCtrl* pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
	pWaveCfgCtrl->UpdateWaveCfg(iPM,iWaveCfgNo, m_pWaveCfg, m_LoadFileStatus.m_mtime);

	fp.Close();
	rv = TRUE;

	return(rv);
}
int	CCalcWaveCfg::DeleteWaveCfg(int iPM,int iWaveCfgNo)
{
	int	rv = TRUE;
	CString	szFileName('\0', 64);

	szFileName.Format("%s\\PC%d\\Wave%03.3d.csv", gpszWaveCfgPath, (iPM+1),iWaveCfgNo);

	TRY
	{
		CFile::Remove(szFileName);
	}
	CATCH(CFileException, e)
	{
		DWORD dw = GetLastError();
		rv = FALSE;
	}
	END_CATCH

	// ロードレシピNo.クリア
	if (rv == TRUE) {
		m_nLoadWaveCfgNo = 0;
		m_szLoadWaveCfgName = _T("");
		m_szLoadWaveCfgTitle = _T("");

		CWaveCfgCtrl* pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();
		pWaveCfgCtrl->DeleteWaveCfg(iPM,iWaveCfgNo);
	}
	return(rv);
}

int	CCalcWaveCfg::WaveCfgFindbyName(int iPM,char* pszWaveCfgName)
{
	int		iWaveCfgNo;
	CWaveCfgCtrl* pWaveCfgCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetWaveCfgCtrl();

	iWaveCfgNo = pWaveCfgCtrl->FindWaveCfgbyName(iPM,pszWaveCfgName);

	if (iWaveCfgNo) {
		if (LoadWaveCfg(iPM,iWaveCfgNo) == TRUE) {
			return(iWaveCfgNo);
		}
	}
	return(0);

}
int	CCalcWaveCfg::WaveCfgGetDate(CTime& filetime)
{
	if (m_nLoadWaveCfgNo) {
		filetime = m_LoadFileStatus.m_mtime;
		return(TRUE);
	}
	return(FALSE);
}


CWaveCfgCtrl::CWaveCfgCtrl()
{
	int	iPM;

	for(iPM=0;iPM<PM_MAX;iPM++)
		memset(m_TblWaveCfg[iPM], '\0', sizeof(WAVECFGCTRL) * WAVECFG_FILE_MAX);
}
CWaveCfgCtrl::~CWaveCfgCtrl()
{

}

int CWaveCfgCtrl::MakeTable()
{
	int				i;
	int				iPM;
	CEPDConfig*		pConfig = ((CWaveEndPointApp *)AfxGetApp())->GetEPDConfig();
	CCalcWaveCfg*	pWaveCfg;


	pWaveCfg = new CCalcWaveCfg();


	/*
	** Wave Config
	*/
	for(iPM=0;iPM<PM_MAX;iPM++){
		if (pConfig->m_tEPDConfig.iPCEnable[iPM] == 0) {
			for (i = 0; i < WAVECFG_FILE_MAX; i++) {
				m_TblWaveCfg[iPM][i].iEnable = FALSE;
			}
			continue;
		}

		for (i = 0; i < WAVECFG_FILE_MAX; i++) {
			if (pWaveCfg->LoadWaveCfg(iPM, (i + 1)) != TRUE) {
				m_TblWaveCfg[iPM][i].iEnable = FALSE;
			}
			else {
				m_TblWaveCfg[iPM][i].iEnable = TRUE;
				strncpy(m_TblWaveCfg[iPM][i].szName, pWaveCfg->m_pWaveCfg->szWaveCfgName, WAVECFG_LEN_NAME);
				m_TblWaveCfg[iPM][i].szName[WAVECFG_LEN_NAME] = '\0';
				strncpy(m_TblWaveCfg[iPM][i].szComment, pWaveCfg->m_pWaveCfg->szWaveCfgComment, WAVECFG_LEN_COMMENT);
				m_TblWaveCfg[iPM][i].szComment[WAVECFG_LEN_COMMENT] = '\0';
				m_TblWaveCfg[iPM][i].iBaseWaveNo[0] = pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[0].iWaveNo;
				m_TblWaveCfg[iPM][i].iBaseWaveNo[1] = pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[1].iWaveNo;
				m_TblWaveCfg[iPM][i].iBaseWaveNo[2] = pWaveCfg->m_pWaveCfg->tStep1_Wave_AB[2].iWaveNo;

				m_TblWaveCfg[iPM][i].iUpdateYear = pWaveCfg->m_LoadFileStatus.m_mtime.GetYear();
				m_TblWaveCfg[iPM][i].iUpdateMonth = pWaveCfg->m_LoadFileStatus.m_mtime.GetMonth();
				m_TblWaveCfg[iPM][i].iUpdateDay = pWaveCfg->m_LoadFileStatus.m_mtime.GetDay();
				m_TblWaveCfg[iPM][i].iUpdateHour = pWaveCfg->m_LoadFileStatus.m_mtime.GetHour();
				m_TblWaveCfg[iPM][i].iUpdateMinute = pWaveCfg->m_LoadFileStatus.m_mtime.GetMinute();
				m_TblWaveCfg[iPM][i].iUpdateSecond = pWaveCfg->m_LoadFileStatus.m_mtime.GetSecond();
			}
		}/* for */
	}/* for */
	delete pWaveCfg;
	return(TRUE);
}
int CWaveCfgCtrl::UpdateWaveCfg(int iPM,int iWaveCfgNo, SET_WAVECFG* pWaveCfg, CTime& tTime)
{
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iEnable = TRUE;
	strncpy(m_TblWaveCfg[iPM][iWaveCfgNo - 1].szName, pWaveCfg->szWaveCfgName, WAVECFG_LEN_NAME);
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].szName[WAVECFG_LEN_NAME] = '\0';
	strncpy(m_TblWaveCfg[iPM][iWaveCfgNo - 1].szComment, pWaveCfg->szWaveCfgComment, WAVECFG_LEN_COMMENT);
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].szComment[WAVECFG_LEN_COMMENT] = '\0';
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[0] = pWaveCfg->tStep1_Wave_AB[0].iWaveNo;
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[1] = pWaveCfg->tStep1_Wave_AB[1].iWaveNo;
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[2] = pWaveCfg->tStep1_Wave_AB[2].iWaveNo;

	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateYear = tTime.GetYear();
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateMonth = tTime.GetMonth();
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateDay = tTime.GetDay();
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateHour = tTime.GetHour();
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateMinute = tTime.GetMinute();
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateSecond = tTime.GetSecond();

	return(TRUE);

}
int CWaveCfgCtrl::DeleteWaveCfg(int iPM,int iWaveCfgNo)
{
	m_TblWaveCfg[iPM][iWaveCfgNo - 1].iEnable = FALSE;
	memset(&m_TblWaveCfg[iPM][iWaveCfgNo - 1], '\0', sizeof(WAVECFGCTRL));
	return(TRUE);
}
int CWaveCfgCtrl::GetWaveCfgInfo(int iPM,int iWaveCfgNo, char* pszWaveCfgName, char* pszComment, int* piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime)
{
	if (m_TblWaveCfg[iPM][iWaveCfgNo - 1].iEnable == FALSE)
		return(FALSE);

	strncpy(pszWaveCfgName, m_TblWaveCfg[iPM][iWaveCfgNo - 1].szName, WAVECFG_LEN_NAME);
	strncpy(pszComment, m_TblWaveCfg[iPM][iWaveCfgNo - 1].szComment, WAVECFG_LEN_COMMENT);
	*piBaseWave1 = m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[0];
	*piBaseWave2 = m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[1];
	*piBaseWave3 = m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[2];
	*piBaseWave4 = m_TblWaveCfg[iPM][iWaveCfgNo - 1].iBaseWaveNo[3];

	sprintf(pszDateTime, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateYear,
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateMonth,
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateDay,
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateHour,
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateMinute,
		m_TblWaveCfg[iPM][iWaveCfgNo - 1].iUpdateSecond);
	return(TRUE);

}
int CWaveCfgCtrl::CheckWaveCfgNameExists(int iPM,int iWaveCfgNo, char* pszWaveCfgName)
{
	int			i;
	CString		str1('\0', 32);
	CString		str2('\0', 32);

	str1 = pszWaveCfgName;
	str1 += _T("                                ");
	str1 = str1.Left(WAVECFG_LEN_NAME);
	str1.MakeUpper();

	for (i = 0; i < WAVECFG_FILE_MAX; i++) {
		if (m_TblWaveCfg[iPM][i].iEnable == FALSE)
			continue;

		if (iWaveCfgNo == (i + 1))
			continue;

		str2 = m_TblWaveCfg[iPM][i].szName;
		str2 += _T("                                ");
		str2 = str2.Left(WAVECFG_LEN_NAME);
		str2.MakeUpper();

		if (str1 == str2)
			return(FALSE);

	}/* for */
	return(TRUE);

}
int CWaveCfgCtrl::FindWaveCfgbyName(int iPM,char* pszWaveCfgName)
{
	int			i;
	CString		str1('\0', 32);
	CString		str2('\0', 32);

	str1 = pszWaveCfgName;
	str1 += _T("                                ");
	str1 = str1.Left(WAVECFG_LEN_NAME);
	str1.MakeUpper();

	for (i = 0; i < WAVECFG_FILE_MAX; i++) {
		if (m_TblWaveCfg[iPM][i].iEnable == FALSE)
			continue;

		str2 = m_TblWaveCfg[iPM][i].szName;
		str2 += _T("                                ");
		str2 = str2.Left(WAVECFG_LEN_NAME);
		str2.MakeUpper();

		if (str1 == str2)
			return((i + 1));

	}/* for */
	return(0);

}
int CWaveCfgCtrl::GetInfoFirst(int iPM,int* piWaveCfgNo, char* pszWaveCfgName, char* pszComment, int* piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime)
{

	for (m_iFindPoint = 0; m_iFindPoint < WAVECFG_FILE_MAX; m_iFindPoint++) {
		if (m_TblWaveCfg[iPM][m_iFindPoint].iEnable == FALSE)
			continue;

		strncpy(pszWaveCfgName, m_TblWaveCfg[iPM][m_iFindPoint].szName, WAVECFG_LEN_NAME);
		strncpy(pszComment, m_TblWaveCfg[iPM][m_iFindPoint].szComment, WAVECFG_LEN_COMMENT);
		*piBaseWave1 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[0];
		*piBaseWave2 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[1];
		*piBaseWave3 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[2];
		*piBaseWave4 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[3];

		sprintf(pszDateTime, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateYear,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateMonth,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateDay,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateHour,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateMinute,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateSecond);
		*piWaveCfgNo = m_iFindPoint + 1;
		return(TRUE);
	}
	return(FALSE);

}
int CWaveCfgCtrl::GetInfoNext(int iPM,int* piWaveCfgNo, char* pszWaveCfgName, char* pszComment, int* piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime)
{
	m_iFindPoint++;
	if (m_iFindPoint >= WAVECFG_FILE_MAX)
		return(FALSE);

	for (; m_iFindPoint < WAVECFG_FILE_MAX; m_iFindPoint++) {
		if (m_TblWaveCfg[iPM][m_iFindPoint].iEnable == FALSE)
			continue;

		strncpy(pszWaveCfgName, m_TblWaveCfg[iPM][m_iFindPoint].szName, WAVECFG_LEN_NAME);
		strncpy(pszComment, m_TblWaveCfg[iPM][m_iFindPoint].szComment, WAVECFG_LEN_COMMENT);
		*piBaseWave1 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[0];
		*piBaseWave2 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[1];
		*piBaseWave3 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[2];
		*piBaseWave4 = m_TblWaveCfg[iPM][m_iFindPoint].iBaseWaveNo[3];

		sprintf(pszDateTime, "%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateYear,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateMonth,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateDay,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateHour,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateMinute,
			m_TblWaveCfg[iPM][m_iFindPoint].iUpdateSecond);
		*piWaveCfgNo = m_iFindPoint + 1;

		return(TRUE);
	}
	return(FALSE);
}
