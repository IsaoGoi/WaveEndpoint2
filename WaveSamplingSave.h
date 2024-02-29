#pragma once

#include "ExtThread.h"


#define		SAMPLING_IDLE	0
#define		SAMPLING_BUSY	1

#define		LOG_COUNT_MAX	20


typedef struct {
	unsigned short	usData[EPD_WAVE_TOTAL+1];
	void		*pNext;
}WAVEDATA;

typedef struct{
	int				iSampling100msec;
	int				iSamplingMax;
	int				iSamplingCycle;
	int				iSamplingCount;
	char			szLogFileName[128];
}WRITE_WAVEDATA;


class CWaveSamplingSave  : public CExtThread 
{
public:
	CWaveSamplingSave();
	virtual ~CWaveSamplingSave();
	virtual	void	StartWork();		/* Start Up(Over ride)*/
	virtual	void	DoWork();			/* Ω⁄Øƒﬁèàóù(Over ride)*/
	virtual	void	EndWork();			/* å„èàóù  (Over ride)*/

	void			StartWaveSampling(int iPM, int iSamplingTM);
	void			WaveSamplingReq(int iPM, unsigned short *pusData);
	void			EndWaveSampling(int iPM);


	int				m_iPCSamplingStatus[PM_MAX+1];
	WAVEDATA		*m_pWriteWaveData[PM_MAX];
	WRITE_WAVEDATA	m_tWriteWaveInfo[PM_MAX];
	CCriticalSection	m_Interlock;
	int				m_i1stTime[PM_MAX];

private:
	CIODB	  *m_pIODB;
//	CWaveData_Eth *m_pWaveData;

};
