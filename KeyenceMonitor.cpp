
#include "pch.h"
#include "WaveEndPoint.h"
#include "KeyenceMonitor.h"
#include <sys/timeb.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if 0
typedef struct{
	int	iDevice;
	int	iStartAddr;
	int	iReadBlock;
	int	iPriority;
}PLC_READADR_INFO;

#define		LW_ADDR_OFFSET	0x5000

static PLC_READADR_INFO m_iReadAddrInfo[32]={
	/* 0 */{MC_W_DEVCODE,	0x5000,  0x300,		0},
	/* 1 */{MC_W_DEVCODE,	0x5300,  0x300,		0},
	/* 2 */{MC_W_DEVCODE,	0x5600,  0x300,		0},
	/* 3 */{MC_W_DEVCODE,	0x5900,  0x300,		0},
	/* 4 */{MC_W_DEVCODE,	0x5C00,  0x300,		0},
	/* 5 */{MC_W_DEVCODE,	0x5F00,  0x300,		0},
	/* 6 */{MC_W_DEVCODE,	0x6200,  0x300,		0},
	/* 7 */{MC_W_DEVCODE,	0x6500,  0x300,		0},		// FDC
	/* 8 */{MC_W_DEVCODE,	0x6800,  0x300,		0},
	/* 9 */{MC_W_DEVCODE,	0x6B00,  0x300,		0},
	/*10 */{MC_W_DEVCODE,	0x6E00,  0x300,		0},
	/*11 */{MC_W_DEVCODE,	0x7100,  0x300,		0},
	/*12 */{MC_W_DEVCODE,	0x7400,  0x300,		0},
	/*13 */{MC_W_DEVCODE,	0x7700,  0x300,		0},
	/*14 */{MC_W_DEVCODE,	0x7A00,  0x300,		0},
	/*15 */{MC_W_DEVCODE,	0x7D00,  0x300,		0},
	/*16 */{MC_W_DEVCODE,	0x8000,  0x300,		0},
	/*17 */{MC_W_DEVCODE,	0x8300,  0x300,		0},
	/*18 */{MC_W_DEVCODE,	0x8600,  0x300,		0},
	/*19 */{MC_W_DEVCODE,	0x8900,  0x300,		0},
	/*20 */{MC_W_DEVCODE,	0x8C00,  0x300,		0},
	/*21 */{MC_W_DEVCODE,	0x8F00,  0x300,		0},
	/*22 */{MC_W_DEVCODE,	0x9200,  0x300,		0},
	/*23 */{MC_W_DEVCODE,	0x9500,  0x300,		0},
	/*24 */{MC_W_DEVCODE,	0x9800,  0x300,		0},
	/*25 */{MC_W_DEVCODE,	0x9B00,  0x300,		0},
	/*26 */{MC_W_DEVCODE,	0x9E00,  0x300,		0},
	/*27 */{MC_W_DEVCODE,	0xA100,  0x300,		0},
	/*28 */{-1,				-1,		-1,			-1}
};

static int	m_iCurrBlock=0;
static int	m_iGroup=1;
static int	m_bTimeCalc = TRUE;
#endif




//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////

CKeyenceMonitor::CKeyenceMonitor()
{
}

CKeyenceMonitor::~CKeyenceMonitor()
{

}
void	CKeyenceMonitor::StartWork()
{
	int		i;
	char	szIOName[32];
	struct	_timeb timebuffer;

	CWaveEndPointApp *pApp = (CWaveEndPointApp*)AfxGetApp();
	
	m_pReadBuffTop = pApp->GetKeyenceMonitorDataAddr();
	m_pReadBuff = m_pReadBuffTop;

	m_pKeyenceSock = pApp->GetKeyenceSock();

	m_pIODB = pApp->GetIODB();

	m_PLCStatIOHdl = m_pIODB->IOPointGetHandle("CTC.KeyenceStatus");

	MELSEC_STATUS	iStatus;
	m_pKeyenceSock->PLC_GetUDPStatus(&iStatus);

	m_ioCycleTime = m_pIODB->IOPointGetHandle("CTC.KeyMonCycle");


	m_iReadPoint = 0;

	_ftime64_s(&timebuffer);
	m_dStartTime = (double)timebuffer.millitm;
	m_dStartTime /= 1000.0f;
	m_dStartTime += (double)timebuffer.time;

	//	m_pIODB->IOPointWrite(m_PLCStatIOHdl,&iStatus);
}

void	CKeyenceMonitor::DoWork()
{
	int		RcvLen;
	BOOL	rv;
	int		i;
	unsigned char	RecvBuff[2048];
	struct	_timeb timebuffer;
	char	timeline[32];
	double	dCycleTime;
	int		iLoop;
	errno_t err;

	MELSEC_STATUS	iStatus;

	m_pKeyenceSock->PLC_GetUDPStatus(&iStatus);
	m_pIODB->IOPointWrite(m_PLCStatIOHdl, &iStatus);

	if (iStatus <= MC_NOT_CONECT)
		return;

	m_iAddress = 0x1400;
	m_iReadSize = 0x300;

	rv = m_pKeyenceSock->PLC_ReadCommand(MC_W_DEVCODE, m_iAddress, m_iReadSize, (char*)RecvBuff, &RcvLen);
	if (rv == TRUE) {
		for (i = 0; i < m_iReadSize; i++) {
			*(m_pReadBuff + m_iAddress + i) = *(unsigned short*)(RecvBuff + (i * 2));
		}/* for */
	}
#if 0
	if (m_tKeyRegInfo[m_iReadPoint].ReadPT + 0x300 >= m_tKeyRegInfo[m_iReadPoint].EndAddr) {
		m_tKeyRegInfo[m_iReadPoint].ReadPT = 0;
#endif
		_ftime64_s(&timebuffer);
		m_dEndTime = (double)timebuffer.millitm;
		m_dEndTime /= 1000.0f;
		m_dEndTime += (double)timebuffer.time;

		dCycleTime = m_dEndTime - m_dStartTime;
		m_pIODB->IOPointWrite(m_ioCycleTime, &dCycleTime);

		m_dStartTime = m_dEndTime;
#if 0
		++m_iReadPoint;
	}
#endif
}
void	CKeyenceMonitor::EndWork()
{
}

