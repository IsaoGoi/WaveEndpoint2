#pragma once

#define		COLOR_WND_BASE		RGB(192,192,255)
#define		DISPLAY_MAIN		0
#define		DISPLAY_RECIPE		1
#define		DISPLAY_LOG			2
#define		DISPLAY_WAVE		3
#define		DISPLAY_ANALYSIS	4
#define		DISPLAY_CONFIG		5
#define		DISPLAY_EXIT		6

#define	EPD_STS_INIT		0	/* 0:Init					*/
#define	EPD_STS_IDLE		1	/* 1:IDLE 					*/
#define	EPD_STS_DEADTIME	2	/* 2:Dead Time              */
#define	EPD_STS_JUSTWAIT	3	/* 3:Just Wait     			*/
#define	EPD_STS_ENDWAIT		4	/* 4:End Wait		  	  	*/
#define	EPD_STS_OVERETCH	5	/* 5:Over Etch  	  		*/
#define	EPD_STS_COMPLETE	6	/* 6:Complete  	  			*/
#define	EPD_STS_ERROREND	7	/* 7:Error End  	  		*/
#define	EPD_STS_HW_ERROR	8	/* 8:H/W Error  	  		*/

#define	EPD_END_INIT			0	/* 0:Init					*/
#define	EPD_END_NORMAL		1	/* 1:Normal End				*/
#define	EPD_END_TIMEOVER	2	/* 2:Over Time              */
#define	EPD_END_ABORTED		3	/* 3:Abort End              */
#define	EPD_END_ERROR		4	/* 4:Error End              */

#define	EPD_WAVE_DATA_MAX	3600*10			// 30minutes


typedef struct {
	int				index;

#if 0
	int				iBaseWaveNo[RCP_STEP_MAX][4];
	int				iAddWaveNo[RCP_STEP_MAX];
	int				iSubstWaveNo[RCP_STEP_MAX];
	unsigned short	usBaseWave[RCP_STEP_MAX][4][EPD_WAVE_DATA_MAX + 1];
	unsigned short	usAddWave[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	unsigned short	usSubstWave[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
#endif
	double			dWaveData[3][RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2

	double			dWaveMin[RCP_STEP_MAX];
	double			dWaveMax[RCP_STEP_MAX];

	double			dStep2CalcData[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dAveStep2_Total[RCP_STEP_MAX];
	double			dStep2Data[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2

	double			dStep3MinData[RCP_STEP_MAX];
	double			dStep3MaxData[RCP_STEP_MAX];
	double			dStep3CalcData[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dAveStep3_Total[RCP_STEP_MAX];
	double			dStep3Data[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2


	double			dStep4DiffData_1[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dStep4DiffData_2[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dAveStep4_Total1[RCP_STEP_MAX][6];
	double			dAveStep4_Total2[RCP_STEP_MAX][6];
	double			dStep4AveData_1[RCP_STEP_MAX][6][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dStep4AveData_2[RCP_STEP_MAX][6][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2

	double			dStep4DiffResult1[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dStep4DiffResult2[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2
	double			dStep4Data[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];		// A/B1/B2


	double			dCalcData[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	double			dDiffData[RCP_STEP_MAX][EPD_WAVE_DATA_MAX + 1];
	double			dTime;

	double			dStartTime[RCP_STEP_MAX + 1];
	double			dDeadTime[RCP_STEP_MAX + 1];
	double			dJustTime[RCP_STEP_MAX + 1];
	double			dEndTime[RCP_STEP_MAX + 1];
	double			dOverEtchTime[RCP_STEP_MAX + 1];

	double			dStartLevel[RCP_STEP_MAX + 1];
	double			dJustLevel[RCP_STEP_MAX + 1];
	double			dJustDiff[RCP_STEP_MAX + 1];
	double			dPeakLevel[RCP_STEP_MAX + 1];
	double			dPeakLevelLow[RCP_STEP_MAX + 1];
	double			dEndLevel[RCP_STEP_MAX + 1];
	double			dEndDiff1[RCP_STEP_MAX + 1];
	double			dEndDiff2[RCP_STEP_MAX + 1];
	double			dEndFlatLevel[RCP_STEP_MAX + 1];

	int				iEndStatus;
	double			dTotalTime;

	char			szLotName[32 + 1];
	char			szCstName[32 + 1];
	char			szGlassName[32 + 1];
	unsigned short	usSlotNo;

}EPD_WAVEDATA2;



typedef struct {
	unsigned short usData[EPD_WAVE_TOTAL];
}SAMPLE_WAVE;

