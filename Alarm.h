#pragma once

#define		ALARM_MAX			1000
#define		ALARM_LOG_MAX		1000


#define		ALM_MODULE_PC1		0
#define		ALM_MODULE_PC2		1
#define		ALM_MODULE_PC3		2
#define		ALM_MODULE_PC4		3
#define		ALM_MODULE_PC5		4
#define		ALM_MODULE_ALL		5

#define		ALM_LVL_LIGHT		0
#define		ALM_LVL_HEAVY		1

#define		ALM_MSG_LEN			80

typedef struct {
	int		bEnable;
	int		iAlarmNo;
	int		bAlarmON;
	int		iModule;
	int		iAlarmLevel;
	char	szMeassage[ALM_MSG_LEN + 1];
	char	szDate[32+ 1];
}ALARM_TBL;

typedef struct {
	int		iAlarmNo;
	int		iOnOff;
	int		iModule;
	int		iAlarmLevel;
	char	szMeassage[ALM_MSG_LEN + 1];
	char	szDate[32 + 1];
}ALARM_LOG;


class CAlarm
{
public:
	CAlarm();
	virtual ~CAlarm();

	void	LoadAlarmList();
	void	LoadAlarmLog();
	void	SaveAlarmLog();

	void AlarmON(int iAlarmNo);
	void AlarmOFF(int iAlarmNo);
	int  AlarmONCheck(int iAlarmNo) { return(m_tAlarmTbl[iAlarmNo - 1].bAlarmON); }
	void AllReset();
	void AddAlarmLog(ALARM_TBL* ptAlm);

	HGLOBAL		h_AlarmTbl;
	HGLOBAL		h_CurrAlarm;
	ALARM_TBL	m_tAlarmTbl[ALARM_MAX+1];
	ALARM_TBL	m_tCurrAlarm[ALARM_MAX + 1];
	CRITICAL_SECTION	m_UpdateAlarm;

	int			m_iCurrAlmTotal;

	/*
	** Alarm Log
	*/
	int			m_iAlmLogTotal;
	HGLOBAL		h_AlarmLogTbl;
	ALARM_LOG	m_tAlarmLogTbl[ALARM_LOG_MAX+1];


};

