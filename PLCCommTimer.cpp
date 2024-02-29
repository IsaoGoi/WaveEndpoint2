#include "pch.h"
#include "WaveEndPoint.h"
#include "PLCCommTimer.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPLCCommTimer::CPLCCommTimer()
{
}
/*
**=======================**
**      初期処理
**=======================**
*/
void CPLCCommTimer::StartWork()
{
	int		i,j;

	for (i = 0; i < PM_MAX; i++)
		for (j = 0; j < PLC_TIMER_MAX; j++)
			memset(&m_tPLCTimer[i][j], '\0', sizeof(PLC_TIMER_TBL));
}
/*
**=======================**
**  スレッド終了処理
**=======================**
*/
void CPLCCommTimer::EndWork()
{
}
/*
**=======================**
**  スレッドメイン処理
**=======================**
*/
void CPLCCommTimer::DoWork()
{
	int		i, j;
	__time64_t		tTimeCount;
	CAlarm* pAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetAlarmClass();


	for (i = 0; i < PM_MAX; i++) {
		for (j = 0; j < PLC_TIMER_MAX; j++) {
			if (m_tPLCTimer[i][j].iTimerStart) {
				_ftime64_s(&m_tPLCTimer[i][j]._End_tm);
				tTimeCount = (m_tPLCTimer[i][j]._End_tm.time * 1000) + m_tPLCTimer[i][j]._End_tm.millitm;
				tTimeCount -= ((m_tPLCTimer[i][j]._Start_tm.time * 1000) + m_tPLCTimer[i][j]._Start_tm.millitm);
				if (m_tPLCTimer[i][j].iTimer_ms <= tTimeCount) {
					
					pAlarm->AlarmON((100 * (i + 1)) + j);
					m_tPLCTimer[i][j].iTimerStart = FALSE;
				}
			}
		}/* for */
	}/* for */
}

void	CPLCCommTimer::StartPLCTimer(int iPC, int iTimerNo,int iTimeOut_Sec)
{
	if (m_tPLCTimer[iPC][iTimerNo].iTimerStart)
		return;
	if(iTimeOut_Sec == 0)
		return;


	m_tPLCTimer[iPC][iTimerNo].iTimer_ms = iTimeOut_Sec * 1000;
	_ftime64_s(&m_tPLCTimer[iPC][iTimerNo]._Start_tm);
	m_tPLCTimer[iPC][iTimerNo].iTimerStart = TRUE;
}
void	CPLCCommTimer::StopPLCTimer(int iPC, int iTimerNo)
{
	m_tPLCTimer[iPC][iTimerNo].iTimerStart = FALSE;
}
