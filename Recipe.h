
#pragma once

/////////////////////////////////////////////////////////////////////////////
// レシピデータ管理  #define 定義
/////////////////////////////////////////////////////////////////////////////

/*
** 戻り値
*/
#define	RCP_FILE_SUCCESS	 0				//  0:正常終了
#define	RCP_FILE_INV_PARAM	-1				// -1:PM No./Recipe No.指定誤り
#define	RCP_FILE_NOT_FOUND	-2				// -2:ファイル無し
#define	RCP_FILE_OPENERR	-3				// -3:オープンエラー
#define	RCP_FILE_READERR	-4				// -4:読み込みエラー
#define	RCP_FILE_WRITEERR	-5				// -5:書き込みエラー
#define	RCP_FILE_CREATREERR	-6				// -6:クリエイトエラー
#define	RCP_FILE_RCPNOERR	-7				// -7:レシピ番号ｴﾗｰ
#define	RCP_FILE_RCPFULL	-8				// -8:レシピFull
#define	RCP_FILE_NOTINIT	-9				// -9:初期ロードしていない
#define	RCP_FILE_DELERR		-10				// -10:削除エラー


/*
** レシピ
*/
#define	RCP_FILE_MAX		100
#define	RCP_STEP_MAX		5				// ステップ最大数

#define	RCP_LEN_NAME		32				// レシピ名
#define	RCP_LEN_COMMENT		32				// コメント


/*
** レシピファイルラベル
*/
#define	LABEL_RCPNAME		"Recipe Name"
#define	LABEL_COMMENT		"Recipe Comment"
#define	LABEL_VERSION		"Recipe Version"
#define	LABEL_INTEGTIME		"Integration Time"
#define	LABEL_DIFFDISPMAX	"Diffrential Display Max"
#define	LABEL_SAMPLINGTIME	"Sampling Time"

#define	LABEL_STEP_START	"Step Start"
#define	LABEL_STEP_END		"Step End"
#define	LABEL_STEP_COMMENT	"Step Comment"
#define	LABEL_USE_WAVE		"Use Wave No."
#define	LABEL_DEAD_TIME		"Dead Time"
#define	LABEL_START_LVL_SW	"Start Lvl Enable"
#define	LABEL_START_LEVEL	"Start Level"
#define	LABEL_START_LVL_TYPE "Start Lvl Type"
#define	LABEL_START_LVL_WAVE "Start Lvl Wave"
#define	LABEL_START_LIMIT	"Start Limit Time"
#define	LABEL_START_LIMIT_SW "Start Limit Enable"
#define	LABEL_SEL_AUTO_GAIN	"Select Auto Gain"
#define	LABEL_AUTO_GAIN_LVL	"Auto Gain Level"
#define	LABEL_AUTO_GAIN_TYPE "Select Auto Gain Type"
#define	LABEL_AUTO_GAIN_PEAK_TYPE "Peak Type for Auto Gain"
#define	LABEL_AUTO_GAIN_PEAK_LEVEL "Peak Level for Auto Gain"
#define	LABEL_AUTO_GAIN_PEAK_RATE "Peak Rate for Auto Gain"

#define	LABEL_JUST_LEVEL_SW	"Just Enable"
#define	LABEL_JUST_LEVEL	"Just Level"
#define	LABEL_JUST_LVL_TYPE "Just Lvl Type"
#define	LABEL_JUST_DIFF_SW	"Just Diff. Enable"
#define	LABEL_JUST_DIFF		"Just from Start Diff."
#define	LABEL_JUST_LIMIT_SW	"Just Limit Enable"
#define	LABEL_JUST_LIMIT	"Just Limit Time"
#define	LABEL_JUST_PEAK		"Just Peak Detect"
#define	LABEL_JUST_PEAK_SELECT	"Just Peak Type Select"
#define	LABEL_JUST_PEAK_HL	"Just Peak HighLow"
#define	LABEL_JUST_PEAK_VAL	"Just Peak Change Value"
#define	LABEL_JUST_PEAK_RATE "Just Peak Change Rate"
#define	LABEL_JUST_BOX_SW	"Just Box Enable"
#define	LABEL_JUST_BOX_SELECT "Just Box Type Select"
#define	LABEL_JUST_BOX_TYPE	"Just Box Type"
#define	LABEL_JUST_BOX_LEVEL "Just Box Level"
#define	LABEL_JUST_BOX_RATE "Just Box Rate"
#define	LABEL_JUST_BOX_TIME "Just Box Time"
#define	LABEL_JUST_BOX_WAVE "Just Box WaveType"

#define	LABEL_END_LEVEL_SW	"End Enable"
#define	LABEL_END_LEVEL		"End Level"
#define	LABEL_END_LVL_TYPE	"End Lvl Type"
#define	LABEL_END_DIFF1_SW	"End Diff1 Enable"
#define	LABEL_END_DIFF1_SELECT	"End Diff1 Type Select"
#define	LABEL_END_DIFF1		"End from Just Diff."
#define	LABEL_END_DIFF1_RATE "End from Just Rate."
#define	LABEL_END_DIFF2_SW	"End Diff2 Enable"
#define	LABEL_END_DIFF2_SELECT	"End Peak Type Select"
#define	LABEL_END_DIFF2		"End from Peak Diff."
#define	LABEL_END_DIFF2_RATE "End from Peak RATE."
#define	LABEL_END_FLAT_SW	"End Flat Enable"
#define	LABEL_END_FLAT_SELECT "End Flat Type Select"
#define	LABEL_END_FLAT		"End Flat Wave"
#define	LABEL_END_FLAT_RATE	"End Flat Rate"
#define	LABEL_END_FLAT_TIME	"End Flat Time"
#define	LABEL_END_LIMIT_SW	"End Limit Enable"
#define	LABEL_END_LIMIT		"End Limit Time"

#define	LABEL_OVET_TIME_SW	"OE Time Enable"
#define	LABEL_OVET_TIME		"Over Etch Time"
#define	LABEL_OVET_RATE_SW	"OE Rate Enable"
#define	LABEL_OVET_RATE		"Over Etch Rate"



/*
** 単位
*/
#define	UNIT_SEC			"Sec"
#define	UNIT_PERCENT		"%"
#define	UNIT_DG				"dg"


typedef enum {
	OVER_LEVEL=0,
	UNDER_LEVEL
}LEVEL_TYPE;

typedef enum {
	AUTOGAIN_TIME = 0,
	AUTOGAIN_PEAK
}AUTOGAIN_TYPE;

typedef enum {
	BOX_ASCEND = 0,
	BOX_DESCEND,
	BOX_FLAT,
}BOX_TYPE;

typedef enum {
	DETECT_LEVEL = 0,
	DETECT_RATE,
	DETECT_LEVEL_AND_RATE,
	DETECT_LEVEL_OR_RATE,
}DETECT_TYPE;

typedef enum {
	NORMAL_WAVE = 0,
	DIFF_WAVE,
}WAVE_TYPE;

/////////////////////////////////////////////////////////////////////////////
// 処理レシピ定義
/////////////////////////////////////////////////////////////////////////////
typedef struct {
	float	fDeadTime;
	int		bStartLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dStartLevel;
	WAVE_TYPE eWaveType;
	int		bStartLimitEnable;
	float	fStartLimitTime;

	int		bAutoGain;
	double	dAutoGainLevel;
}OLD_START_PHASE;
typedef struct {
	float	fDeadTime;
	int		bStartLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dStartLevel;
	WAVE_TYPE eWaveType;
	int		bStartLimitEnable;
	float	fStartLimitTime;

	int		bAutoGain;
	AUTOGAIN_TYPE eAutoGainType;
	double	dAutoGainLevel;
	DETECT_TYPE	eAutoGainPeakDetectType;
	double	dStartAutoGainPeakChgValue;
	double	dStartAutoGainPeakChgRate;

}START_PHASE;

typedef struct {
	int		bJustLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dJustLevel;
	int		bJustDiffEnable;
	double	dJustDiffFromStart;
	int		bJustLimitEnable;
	float	fJustLimitTime;
	int		bJustPeakDetect;
	int		bJustPeakHigh;
	double	dJustPeakChgValue;
}OLD_JUST_PHASE;

typedef struct {
	// Just Level
	int		bJustLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dJustLevel;
	// Diffrent from Start
	int		bJustDiffEnable;
	double	dJustDiffFromStart;
	// Limit Time
	int		bJustLimitEnable;
	float	fJustLimitTime;
	// Peak Detect
	int		bJustPeakDetect;
	DETECT_TYPE	ePeakDetectType;
	int		bJustPeakHigh;
	double	dJustPeakChgValue;
	double	dJustPeakChgRate;
	// Box Detect
	int		bBoxDetect;
	DETECT_TYPE	eBoxDetectType;
	BOX_TYPE	eBoxType;
	double	dBoxLevel;
	double	dBoxRate;
	float	fBoxTime;
	WAVE_TYPE eWaveType;
}JUST_PHASE;

typedef struct {
	int		bEndLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dEndLevel;
	int		bEndDiff1Enable;
	double	dEndDiffFromJust;
	int		bEndDiff2Enable;
	double	dEndDiffFromPeak;
	int		bEndFlatEnable;
	double	dEndFlatWave;
	float	fEndFlatTime;
	int		bEndLimitEnable;
	float	fEndLimitTime;
}OLD_END_PHASE;

typedef struct {
	int		bEndLevelEnable;
	LEVEL_TYPE	eLevelType;
	double	dEndLevel;
	int		bEndDiff1Enable;
	DETECT_TYPE	eEndDiffJustSelect;
	double	dEndDiffFromJust;
	double	dEndDiffFromJustRate;

	int		bEndDiff2Enable;
	DETECT_TYPE	eEndDiffPeakSelect;
	double	dEndDiffFromPeak;
	double	dEndDiffFromPeakRate;
	int		bEndFlatEnable;
	DETECT_TYPE	eEndDiffFlatSelect;
	double	dEndFlatWave;
	double	dEndFlatRate;
	float	fEndFlatTime;
	int		bEndLimitEnable;
	float	fEndLimitTime;
}END_PHASE;

typedef struct {
	int		bOverEtchTimeEnable;
	float	fOverEtchTime;
	int		bOverEtchRateEnable;
	float	fOverEtchRate;
}OVETCH_PHASE;


/*
** ステップデータ
*/
typedef struct {
	int					iUseWave;
	char				szComment[RCP_LEN_COMMENT + 1];
	START_PHASE			tStart;
	JUST_PHASE			tJust;
	END_PHASE			tEnd;
	OVETCH_PHASE		tOverEtch;
}RCP_STEP;

typedef struct {
	int					iUseWave;
	char				szComment[RCP_LEN_COMMENT + 1];
	OLD_START_PHASE		tStart;
	OLD_JUST_PHASE		tJust;
	OLD_END_PHASE		tEnd;
	OVETCH_PHASE		tOverEtch;
}OLD_RCP_STEP;

/*
** 処理レシピ
*/
typedef struct {
	char	szRecipeName[RCP_LEN_NAME+1];
	char	szComment[RCP_LEN_COMMENT+1];	// レシピ名称
	int		iStepCnt;
	int		iVersion;						// レシピバージョン
	int		iIntegrationTime;
	double	dDiffDisplayMax;
	int		iSamplingTime;
	int		iReserve[40];
	RCP_STEP Step[RCP_STEP_MAX];			// ステップデータ
}RECIPE;
typedef struct {
	char	szRecipeName[RCP_LEN_NAME + 1];
	char	szComment[RCP_LEN_COMMENT + 1];	// レシピ名称
	int		iStepCnt;
	int		iVersion;						// レシピバージョン
	int		iIntegrationTime;
	OLD_RCP_STEP Step[RCP_STEP_MAX];			// ステップデータ
}OLD_RECIPE;


/////////////////////////////////////////////////////////////////////////////
// CRecipeダイアログ

class CRecipe
{
// コンストラクション
public:
	CRecipe();
	~CRecipe();

	// File Load
	virtual int RecipeFileLoad(int nPMNo,int iRcpNo);
	virtual int RecipeFileSave(int nPMNo,int iRcpNo);
	virtual int RecipeFileDelete(int nPMNo,int iRcpNo);
	virtual int RecipeFindbyName(int iPMNo,char *pszRcpName);
	virtual int RecipeGetDate(CTime &filetime);

	// GlobalAlloc ハンドル
	HGLOBAL		h_Recipe;
	HGLOBAL		h_OldRecipe;

	// レシピデータ
	RECIPE		m_tRecipe;
	RECIPE		m_tOldRecipe;

	// ロードレシピNo.
	int		m_nLoadRecipeNo;

	int			m_iNew;
	CString		m_szLoadRecipeName;
	CString		m_szLoadRecipeTitle;

	CFileStatus  m_LoadFileStatus;

protected:


};

typedef struct {
	int		iEnable;
	char	szRecipeName[RCP_LEN_NAME + 1];	// レシピ名称
	char	szComment[RCP_LEN_COMMENT + 1];	// レシピ名称
	int		iUpdateYear;
	int		iUpdateMonth;
	int		iUpdateDay;
	int		iUpdateHour;
	int		iUpdateMinute;
	int		iUpdateSecond;
}PCRECIPECTRL;


class CPCRcpCtrl : public CObject
{
	// コンストラクション
public:
	CPCRcpCtrl();
	~CPCRcpCtrl();

	int MakeTable();
	int UpdatePCRecipe(int iPM, int iRcpNo, RECIPE* pRecipe, CTime& tTime);
	int DeletePCRecipe(int iPM, int iRcpNo);
	int GetRecipeInfo(int iPM, int iRcpNo, char* pszRecipeName, char* pszComment, char* pszDateTime);
	int CheckRcpNameExists(int iPM, int iRcpNo, char* pszRecipeName);
	int FindRcpbyName(int iPM, char* pszRecipeName);


	int	m_iFindPoint;
	int GetInfoFirst(int iPM, int *piRcpNo, char* pszRecipeName, char* pszComment, char* pszDateTime);
	int GetInfoNext(int iPM, int *piRcpNo, char* pszRecipeName, char* pszComment, char* pszDateTime);

	PCRECIPECTRL	m_TblPCRecipe[PM_MAX][RCP_FILE_MAX + 1];
};
