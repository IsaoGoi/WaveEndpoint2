#pragma once

#include "ExtThread.h"

typedef struct {
	int		iEnable;
	UINT	StartAddr;
	UINT	EndAddr;
	UINT	ReadPT;
	unsigned short* pusData;
}KEY_READ_REG_INFO;
#define		REG_WRITE_REQ_MAX		10
typedef struct {
	int		iRequest;
	int		iMemtype;
	int		iAddress;
	int		iLen;
	char	szWriteBuf[1024 * 2];
}KEY_WORD_WRITE_REQ;
typedef struct {
	int		iRequest;
	int		iMemtype;
	int		iAddress;
	int		ibit;
	int		iOnOff;
}KEY_BIT_WRITE_REQ;



class CKeyenceMonitor  : public CExtThread 
{
public:
	CKeyenceMonitor();
	virtual ~CKeyenceMonitor();
	virtual	void	StartWork();		/* Start Up(Over ride)*/
	virtual	void	DoWork();			/* Ω⁄Øƒﬁèàóù(Over ride)*/
	virtual	void	EndWork();			/* å„èàóù  (Over ride)*/
	virtual void	Stop()	{;}			/* Ω⁄Øƒﬁí‚é~óvãÅ	*/

public:
	CKeyenceSock *m_pKeyenceSock;
	CIODB*		m_pIODB;
	int					m_iAddress;
	int					m_iReadSize;
	
	unsigned short* m_pReadBuffTop;
	unsigned short* m_pReadBuff;

#if 0
	KEY_READ_REG_INFO	m_tKeyRegInfo[KEY_REGISTER_COUNT];
	WORD_WRITE_REQ*		m_ptWordWriteReq;
	BIT_WRITE_REQ*		m_ptBitWriteReq;
#endif

	int		m_PLCStatIOHdl;

	int		m_iMonitorSkip;

	double	m_dStartTime;
	double	m_dEndTime;
	int		m_ioCycleTime;

	int		m_iReadPoint;

};
