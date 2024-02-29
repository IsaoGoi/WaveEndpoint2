#pragma once

#include <sys/timeb.h>
#include "ExtThread.h"

#define		PLC_READ_START_ADDR		29000
#define		PLC_READ_BLOCK_MAX		999
#define		PLC_READ_ADDR_MAX		29999

typedef struct{
	UINT	StartAddr;
	UINT	nBlock;
}
DATAFORMAT;

class CIODB;

class CPLCMonitor  : public CExtThread 
{
public:
	CPLCMonitor();
	virtual ~CPLCMonitor();
	virtual	void	StartWork();		/* Start Up(Over ride)*/
	virtual	void	DoWork();			/* Ω⁄Øƒﬁèàóù(Over ride)*/
	virtual	void	EndWork();			/* å„èàóù  (Over ride)*/

private:
	CFinsSock *m_pFinsSock;
	CIODB	  *m_pIODB;

	int					m_iAddress;
	int					m_iReadSize;
	unsigned short		*m_pReadBuffTop;
	unsigned short		*m_pReadBuff;
	unsigned short		*m_pReadCIOTop;

	char szStart[128];
	char szEnd[64];

	struct	_timeb _PCCycle_Start;
	struct	_timeb _PCCycle_End;

	double		m_dStartTime;
	double		m_dEndTime;
	int			m_ioCycleTime;
	int			m_ioSampleCycTime;

	int			m_io_KeepAlive;
	int			m_iKeepAliveSendEnable;
};
