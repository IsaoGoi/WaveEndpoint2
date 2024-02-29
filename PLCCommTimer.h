#pragma once
#include <sys/timeb.h>
#include "ExtThread.h"

#define		PLC_TIMER_MAX		16

#define		PLC_TI_TIMEOUT		3

#define		PLC_T1_RESET		0
#define		PLC_T1_INFO			1
#define		PLC_T1_RECIPE		2
#define		PLC_T1_START		3
#define		PLC_T1_STOP			4
#define		PLC_T1_SKIP			5

#define		PLC_T4_END_DETECT	10
#define		PLC_T4_STEP_END		11
#define		PLC_T4_LOG_EVT		12

typedef struct {
	int		iTimerStart;
	int		iTimer_ms;
	struct	_timeb _Start_tm;
	struct	_timeb _End_tm;
}PLC_TIMER_TBL;


class CPLCCommTimer : public CExtThread
{
public:
	CPLCCommTimer();
	virtual			~CPLCCommTimer() { ; }
	virtual	void	StartWork();						/* Start Up(Over ride)*/
	virtual	void	DoWork();							/* Ω⁄Øƒﬁèàóù(Over ride)*/
	virtual	void	EndWork();							/* å„èàóù  (Over ride)*/

	void	StartPLCTimer(int iPC, int iTimerNo, int iTimeOut_Sec);
	void	StopPLCTimer(int iPC, int iTimerNo);

	PLC_TIMER_TBL	m_tPLCTimer[PM_MAX][PLC_TIMER_MAX];

private:
	BOOL		m_bInitialized;

};

