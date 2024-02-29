#pragma once

/*
** Wave Config
*/
#define	WAVECFG_FILE_MAX		100

#define	WAVECFG_LEN_NAME		32				// レシピ名
#define	WAVECFG_LEN_COMMENT		32				// コメント

#define	WAVECFG_REPEAT_AVE_MAX	5

typedef enum {
	CALC_OPE_NONE =0,
	CALC_OPE_PULUS,					// +
	CALC_OPE_MINUS,					// -
	CALC_OPE_MULTIPULY,				// x
	CALC_OPE_DIVISION,				// %
}CALC_OPERAND;

typedef enum {
	CALC_NONE = 0,
	CALC_GAIN,
	CALC_OFFSET,
}CALC_OFFSETGAIN;

typedef enum {
	BASE_WAVE1_A,
	BASE_WAVE1_B1,
	BASE_WAVE1_B2,
}CALC_WAVETYPE;

typedef enum {
	BASE_WAVE2_NONE = 0,
	BASE_WAVE2_A,
	BASE_WAVE2_B1,
	BASE_WAVE2_B2,
}CALC_WAVETYPE2;

typedef enum {
	BASE_AUTO_SET = 0,
	BASE_USER_SET,
}CALC_USER_SET;


#define		CALC_CFG_NAME			"Base_Wave_Name"
#define		CALC_CFG_COMMENT		"Base_Wave_Comment"

/*
** Step1
*/
// 波形-A
#define		CALC_CFG_STEP1_BASEWAVE_A		"Base_Step1_Wave_A"
#define		CALC_CFG_STEP1_A_COUNT			"Base_Step1_A_Count"
#define		CALC_CFG_STEP1_A_CALC1_TYPE		"Base_Step1_A_Calc1_Type"
#define		CALC_CFG_STEP1_A_CALC1_VAL		"Base_Step1_A_Calc1_Value"
#define		CALC_CFG_STEP1_A_CALC2_TYPE		"Base_Step1_A_Calc2_Type"
#define		CALC_CFG_STEP1_A_CALC2_VAL		"Base_Step1_A_Calc2_Value"
#define		CALC_CFG_STEP1_A_RECIPROCAL		"Base_Step1_A__Reciprocal"

// 波形-B1
#define		CALC_CFG_STEP1_BASEWAVE_B1		"Base_Step1_Wave_B1"
#define		CALC_CFG_STEP1_B1_COUNT			"Base_Step1_B1_Count"
#define		CALC_CFG_STEP1_B1_CALC1_TYPE	"Base_Step1_B1_Calc1_Type"
#define		CALC_CFG_STEP1_B1_CALC1_VAL		"Base_Step1_B1_Calc1_Value"
#define		CALC_CFG_STEP1_B1_CALC2_TYPE	"Base_Step1_B1_Calc2_Type"
#define		CALC_CFG_STEP1_B1_CALC2_VAL		"Base_Step1_B1_Calc2_Value"
#define		CALC_CFG_STEP1_B1_RECIPROCAL	"Base_Step1_B1_Reciprocal"

// 波形-B2
#define		CALC_CFG_STEP1_BASEWAVE_B2		"Base_Step1_Wave_B2"
#define		CALC_CFG_STEP1_B2_COUNT			"Base_Step1_B2_Count"
#define		CALC_CFG_STEP1_B2_CALC1_TYPE	"Base_Step1_B2_Calc1_Type"
#define		CALC_CFG_STEP1_B2_CALC1_VAL		"Base_Step1_B2_Calc1_Value"
#define		CALC_CFG_STEP1_B2_CALC2_TYPE	"Base_Step1_B2_Calc2_Type"
#define		CALC_CFG_STEP1_B2_CALC2_VAL		"Base_Step1_B2_Calc2_Value"
#define		CALC_CFG_STEP1_B2_RECIPROCAL	"Base_Step1_B2_Reciprocal"

/*
** Step2 : A/B1/B2演算
*/
#define		CALC_CFG_STEP2_WAVE_SELECT1	"Base_Step2_WaveSelect1"
#define		CALC_CFG_STEP2_CALC1	    "Base_Step2_CalcType1"
#define		CALC_CFG_STEP2_WAVE_SELECT2	"Base_Step2_WaveSelect2"
#define		CALC_CFG_STEP2_CALC2	    "Base_Step2_CalcType2"
#define		CALC_CFG_STEP2_WAVE_SELECT3	"Base_Step2_WaveSelect3"
#define		CALC_CFG_STEP2_AVECNT	    "Base_Step2_AveCnt"

/*
** Step3 : 逆数指定
*/
#define		CALC_CFG_STEP3_RECIPROCAL	"Base_Step3_Reciprocal"
#define		CALC_CFG_STEP3_OFFSET_TYPE	"Base_Step3_OffsetType"
#define		CALC_CFG_STEP3_OFFSET_VAL   "Base_Step3_OffsetVal"
#define		CALC_CFG_STEP3_MAX_TYPE	    "Base_Step3_MaxType"
#define		CALC_CFG_STEP3_MAX_VAL	    "Base_Step3_MaxVal"
#define		CALC_CFG_STEP3_AVECNT	    "Base_Step3_AveCnt"

/*
** Step4 : 微分演算
*/
#define		CALC_CFG_STEP4_DIFF1		"Base_Step4_Diff1"
#define		CALC_CFG_STEP4_REPEAT_CNT1	"Base_Step4_RepeatCnt1"
#define		CALC_CFG_STEP4_DIFF2		"Base_Step4_Diff2"
#define		CALC_CFG_STEP4_REPEAT_CNT2	"Base_Step4_RepeatCnt2"
#define		CALC_CFG_STEP4_AVECNT	    "Base_Step4_AveCnt"
#define		CALC_CFG_STEP4_CORRECT_TYPE	"Base_Step4_Correct_Type"
#define		CALC_CFG_STEP4_CORRECT_VAL	"Base_Step4_Correct_Val"

/*
** 微分表示Scale設定
*/
#define		CALC_CFG_DIFF_DISPLAY		"Display_Scale_Enable"
#define		CALC_CFG_DIFF_VAL		    "Display_Value"
#define		CALC_CFG_DIFF_EXP		    "Display_Exp"


typedef struct {
	int				iWaveNo;					// 波形番号(0:未指定)
	int				iWaveCnt;					// 平均化する波形点数
	CALC_OFFSETGAIN	eGainOffset1;				// Gain/Offset指定
	double			dGainOffset1;				// Gain/Offset値
	CALC_OFFSETGAIN	eGainOffset2;				// Gain/Offset指定
	double			dGainOffset2;				// Gain/Offset値
	int				iReciprocal;				//逆数指定

}CALC_STEP1_TBL;

typedef struct {
	CALC_WAVETYPE	eWave1;						// 波形選択1(A/B1/B2)
	CALC_OPERAND	eOperandType1;				// 演算指定1
	CALC_WAVETYPE2	eWave2;						// 波形選択2(A/B1/B2)
	CALC_OPERAND	eOperandType2;				// 演算指定2
	CALC_WAVETYPE2	eWave3;						// 波形選択3(A/B1/B2)
	int				iAveCnt;					// 移動j平均点数
}CALC_STEP2_TBL;

typedef struct {
	int				iReciprocal;				// 逆数指定
	CALC_USER_SET	eOffsetType;				// Offset指定(User or Auto)
	double			dUserOffset;				// User指定Offset
	CALC_USER_SET	eMaxLevelType;				// Maxレベル指定(User or Auto)
	double			dUserMaxLevel;				// User指定Maxレベル
	int				iAveCnt;					// 移動j平均点数
}CALC_STEP3_TBL;

typedef struct {
	int				iDiff1Enable;				// 一階微分 有/無効
	int				iDiff1RptAveCnt;			// 繰り返し移動平均(0=しない/1-5=繰り返し回数)
	int				iDiff2Enable;				// 二階微分 有/無効
	int				iDiff2RptAveCnt;			// 繰り返し移動平均(0=しない/1-5=繰り返し回数)
	int				iDiffAveCnt;				// 移動平均時の平均する点数
	CALC_OPERAND	eDiffCorrect;				// 四則演算指定
	double			dDiffCorrectVal;			// 演算数値
}CALC_STEP4_TBL;

typedef struct {
	int				iDisplayScaleEnable;			// 一階微分 有/無効
	double			dDisplayVal_val;			// 演算数値
	int				iDisplayVal_exp;				// 演算数値
}DIFF_DISPLAY;

typedef struct {
	char	szWaveCfgName[WAVECFG_LEN_NAME + 1];
	char	szWaveCfgComment[WAVECFG_LEN_COMMENT + 1];

	CALC_STEP1_TBL	tStep1_Wave_AB[3+1];
	CALC_STEP2_TBL	tStep2_WaveJoin;
	CALC_STEP3_TBL	tStep3_WaveNorm;
	CALC_STEP4_TBL	tStep4_WaveDiff;
	DIFF_DISPLAY	tDiffDisplayScale;
}SET_WAVECFG;




class CCalcWaveCfg
{
public:
	CCalcWaveCfg();
	virtual ~CCalcWaveCfg();

	int		LoadWaveCfg(int iPM, int iWaveCfgNo);
	int		SaveWaveCfg(int iPM, int iWaveCfgNo);
	int		DeleteWaveCfg(int iPM, int iWaveCfgNo);
	int		WaveCfgFindbyName(int iPM, char* pszRcpName);
	int		WaveCfgGetDate(CTime& filetime);
#if 0
	void	CalcString_to_CalcTbl(char* p, ADDSUB_CALC_TBL *ptTbl);
	void	CalcTbl_to_CalcString(char* p, ADDSUB_CALC_TBL *ptTbl);
#endif

	// GlobalAlloc ハンドル
	HGLOBAL		h_WaveCfg;
	HGLOBAL		h_OldWaveCfg;

	// レシピデータ
	SET_WAVECFG* m_pWaveCfg;
	SET_WAVECFG* m_pOldWaveCfg;

	// ロードレシピNo.
	int		m_nLoadWaveCfgNo;

	int			m_iNew;
	CString		m_szLoadWaveCfgName;
	CString		m_szLoadWaveCfgTitle;

	CFileStatus  m_LoadFileStatus;

protected:

private:
	CFileFind* m_pFileFind;

};

typedef struct {
	int		iEnable;
	char	szName[WAVECFG_LEN_NAME + 1];	// レシピ名称
	char	szComment[WAVECFG_LEN_COMMENT + 1];	// レシピ名称
	int		iBaseWaveNo[4+1];
	int		iUpdateYear;
	int		iUpdateMonth;
	int		iUpdateDay;
	int		iUpdateHour;
	int		iUpdateMinute;
	int		iUpdateSecond;
}WAVECFGCTRL;


class CWaveCfgCtrl : public CObject
{
	// コンストラクション
public:
	CWaveCfgCtrl();
	~CWaveCfgCtrl();

	int MakeTable();
	int UpdateWaveCfg(int iPM, int iWaveCfgNo, SET_WAVECFG* pWaveCfg, CTime& tTime);
	int DeleteWaveCfg(int iPM, int iWaveCfgNo);
	int GetWaveCfgInfo(int iPM, int iWaveCfgNo, char* pszWaveCfgName, char* pszComment,int *piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime);
	int CheckWaveCfgNameExists(int iPM, int iWaveCfgNo, char* pszWaveCfgName);
	int FindWaveCfgbyName(int iPM, char* pszWaveCfgName);


	int	m_iFindPoint;
	int GetInfoFirst(int iPM, int* piWaveCfgNo, char* pszWaveCfgName, char* pszComment, int* piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime);
	int GetInfoNext(int iPM,int* piWaveCfgNo, char* pszWaveCfgName, char* pszComment, int* piBaseWave1, int* piBaseWave2, int* piBaseWave3, int* piBaseWave4, char* pszDateTime);

	int	m_iPMNo;

	WAVECFGCTRL	m_TblWaveCfg[PM_MAX][WAVECFG_FILE_MAX + 1];
};

