#pragma once

/*
** Wave Config
*/
#define	WAVECFG_FILE_MAX		100

#define	WAVECFG_LEN_NAME		32				// ���V�s��
#define	WAVECFG_LEN_COMMENT		32				// �R�����g

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
// �g�`-A
#define		CALC_CFG_STEP1_BASEWAVE_A		"Base_Step1_Wave_A"
#define		CALC_CFG_STEP1_A_COUNT			"Base_Step1_A_Count"
#define		CALC_CFG_STEP1_A_CALC1_TYPE		"Base_Step1_A_Calc1_Type"
#define		CALC_CFG_STEP1_A_CALC1_VAL		"Base_Step1_A_Calc1_Value"
#define		CALC_CFG_STEP1_A_CALC2_TYPE		"Base_Step1_A_Calc2_Type"
#define		CALC_CFG_STEP1_A_CALC2_VAL		"Base_Step1_A_Calc2_Value"
#define		CALC_CFG_STEP1_A_RECIPROCAL		"Base_Step1_A__Reciprocal"

// �g�`-B1
#define		CALC_CFG_STEP1_BASEWAVE_B1		"Base_Step1_Wave_B1"
#define		CALC_CFG_STEP1_B1_COUNT			"Base_Step1_B1_Count"
#define		CALC_CFG_STEP1_B1_CALC1_TYPE	"Base_Step1_B1_Calc1_Type"
#define		CALC_CFG_STEP1_B1_CALC1_VAL		"Base_Step1_B1_Calc1_Value"
#define		CALC_CFG_STEP1_B1_CALC2_TYPE	"Base_Step1_B1_Calc2_Type"
#define		CALC_CFG_STEP1_B1_CALC2_VAL		"Base_Step1_B1_Calc2_Value"
#define		CALC_CFG_STEP1_B1_RECIPROCAL	"Base_Step1_B1_Reciprocal"

// �g�`-B2
#define		CALC_CFG_STEP1_BASEWAVE_B2		"Base_Step1_Wave_B2"
#define		CALC_CFG_STEP1_B2_COUNT			"Base_Step1_B2_Count"
#define		CALC_CFG_STEP1_B2_CALC1_TYPE	"Base_Step1_B2_Calc1_Type"
#define		CALC_CFG_STEP1_B2_CALC1_VAL		"Base_Step1_B2_Calc1_Value"
#define		CALC_CFG_STEP1_B2_CALC2_TYPE	"Base_Step1_B2_Calc2_Type"
#define		CALC_CFG_STEP1_B2_CALC2_VAL		"Base_Step1_B2_Calc2_Value"
#define		CALC_CFG_STEP1_B2_RECIPROCAL	"Base_Step1_B2_Reciprocal"

/*
** Step2 : A/B1/B2���Z
*/
#define		CALC_CFG_STEP2_WAVE_SELECT1	"Base_Step2_WaveSelect1"
#define		CALC_CFG_STEP2_CALC1	    "Base_Step2_CalcType1"
#define		CALC_CFG_STEP2_WAVE_SELECT2	"Base_Step2_WaveSelect2"
#define		CALC_CFG_STEP2_CALC2	    "Base_Step2_CalcType2"
#define		CALC_CFG_STEP2_WAVE_SELECT3	"Base_Step2_WaveSelect3"
#define		CALC_CFG_STEP2_AVECNT	    "Base_Step2_AveCnt"

/*
** Step3 : �t���w��
*/
#define		CALC_CFG_STEP3_RECIPROCAL	"Base_Step3_Reciprocal"
#define		CALC_CFG_STEP3_OFFSET_TYPE	"Base_Step3_OffsetType"
#define		CALC_CFG_STEP3_OFFSET_VAL   "Base_Step3_OffsetVal"
#define		CALC_CFG_STEP3_MAX_TYPE	    "Base_Step3_MaxType"
#define		CALC_CFG_STEP3_MAX_VAL	    "Base_Step3_MaxVal"
#define		CALC_CFG_STEP3_AVECNT	    "Base_Step3_AveCnt"

/*
** Step4 : �������Z
*/
#define		CALC_CFG_STEP4_DIFF1		"Base_Step4_Diff1"
#define		CALC_CFG_STEP4_REPEAT_CNT1	"Base_Step4_RepeatCnt1"
#define		CALC_CFG_STEP4_DIFF2		"Base_Step4_Diff2"
#define		CALC_CFG_STEP4_REPEAT_CNT2	"Base_Step4_RepeatCnt2"
#define		CALC_CFG_STEP4_AVECNT	    "Base_Step4_AveCnt"
#define		CALC_CFG_STEP4_CORRECT_TYPE	"Base_Step4_Correct_Type"
#define		CALC_CFG_STEP4_CORRECT_VAL	"Base_Step4_Correct_Val"

/*
** �����\��Scale�ݒ�
*/
#define		CALC_CFG_DIFF_DISPLAY		"Display_Scale_Enable"
#define		CALC_CFG_DIFF_VAL		    "Display_Value"
#define		CALC_CFG_DIFF_EXP		    "Display_Exp"


typedef struct {
	int				iWaveNo;					// �g�`�ԍ�(0:���w��)
	int				iWaveCnt;					// ���ω�����g�`�_��
	CALC_OFFSETGAIN	eGainOffset1;				// Gain/Offset�w��
	double			dGainOffset1;				// Gain/Offset�l
	CALC_OFFSETGAIN	eGainOffset2;				// Gain/Offset�w��
	double			dGainOffset2;				// Gain/Offset�l
	int				iReciprocal;				//�t���w��

}CALC_STEP1_TBL;

typedef struct {
	CALC_WAVETYPE	eWave1;						// �g�`�I��1(A/B1/B2)
	CALC_OPERAND	eOperandType1;				// ���Z�w��1
	CALC_WAVETYPE2	eWave2;						// �g�`�I��2(A/B1/B2)
	CALC_OPERAND	eOperandType2;				// ���Z�w��2
	CALC_WAVETYPE2	eWave3;						// �g�`�I��3(A/B1/B2)
	int				iAveCnt;					// �ړ�j���ϓ_��
}CALC_STEP2_TBL;

typedef struct {
	int				iReciprocal;				// �t���w��
	CALC_USER_SET	eOffsetType;				// Offset�w��(User or Auto)
	double			dUserOffset;				// User�w��Offset
	CALC_USER_SET	eMaxLevelType;				// Max���x���w��(User or Auto)
	double			dUserMaxLevel;				// User�w��Max���x��
	int				iAveCnt;					// �ړ�j���ϓ_��
}CALC_STEP3_TBL;

typedef struct {
	int				iDiff1Enable;				// ��K���� �L/����
	int				iDiff1RptAveCnt;			// �J��Ԃ��ړ�����(0=���Ȃ�/1-5=�J��Ԃ���)
	int				iDiff2Enable;				// ��K���� �L/����
	int				iDiff2RptAveCnt;			// �J��Ԃ��ړ�����(0=���Ȃ�/1-5=�J��Ԃ���)
	int				iDiffAveCnt;				// �ړ����ώ��̕��ς���_��
	CALC_OPERAND	eDiffCorrect;				// �l�����Z�w��
	double			dDiffCorrectVal;			// ���Z���l
}CALC_STEP4_TBL;

typedef struct {
	int				iDisplayScaleEnable;			// ��K���� �L/����
	double			dDisplayVal_val;			// ���Z���l
	int				iDisplayVal_exp;				// ���Z���l
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

	// GlobalAlloc �n���h��
	HGLOBAL		h_WaveCfg;
	HGLOBAL		h_OldWaveCfg;

	// ���V�s�f�[�^
	SET_WAVECFG* m_pWaveCfg;
	SET_WAVECFG* m_pOldWaveCfg;

	// ���[�h���V�sNo.
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
	char	szName[WAVECFG_LEN_NAME + 1];	// ���V�s����
	char	szComment[WAVECFG_LEN_COMMENT + 1];	// ���V�s����
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
	// �R���X�g���N�V����
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

