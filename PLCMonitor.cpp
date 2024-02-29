// PMLogger.cpp: PLCMonitor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mmsystem.h"
#include "WaveEndPoint.h"
#include "PLCMonitor.h"
#include "PLCCalc.h"
// #include "clockread_ms.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//unsigned short m_usProcStart[PM_MAX+1];
//plf::nanotimer m_PLC_timer;
//static	unsigned			m_uiEvtID;

#if 0
void CALLBACK PCSampling(UINT uEntID, UINT uMsgID, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	double			dmsec;
	char			szDebug[32+1];
	int				iohdl;
	CIODB* pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();


	dmsec = m_PLC_timer.get_elapsed_ms();

	sprintf(szDebug, "%5.1fmsec", dmsec);
	iohdl = pIODB->IOPointGetHandle("CTC.PLCSamplingCyc");
	pIODB->IOPointWrite(iohdl, szDebug);

	m_PLC_timer.start();
}
#endif
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPLCMonitor::CPLCMonitor()
{
}

CPLCMonitor::~CPLCMonitor()
{

}
void	CPLCMonitor::StartWork()
{
	int	i;

	CWaveEndPointApp*pApp = (CWaveEndPointApp *)AfxGetApp();
	m_pFinsSock = pApp->GetFinsSock();
	m_pIODB = pApp->GetIODB();


	m_pReadBuffTop = pApp->GetPLCMonitorDataAddr();
	m_pReadBuff    = m_pReadBuffTop;

	m_iAddress     = PLC_READ_START_ADDR;
	m_iReadSize    = PLC_READ_BLOCK_MAX;

//	_ftime64_s( &_PCCycle_Start);

	m_io_KeepAlive = m_pIODB->IOPointGetHandle("CTC.KeepAlive");
	m_iKeepAliveSendEnable=FALSE;

//	m_PLC_timer.start();
#if 0
	m_uiEvtID = ::timeSetEvent(500, 20, PCSampling, 0, TIME_PERIODIC);
#endif
}

void	CPLCMonitor::DoWork()
{
	int		RcvLen;
	BOOL	rv;
	int		i;
	unsigned uData;
	unsigned char	RecvBuff[2048+16];
	unsigned int	uiKeepAlive;


	// Keep Alive
	if(m_iKeepAliveSendEnable){
		m_pIODB->IOPointRead(m_io_KeepAlive,&uiKeepAlive);
		if(uiKeepAlive==0){
			uiKeepAlive = 1;
			m_pIODB->IOPointWrite(m_io_KeepAlive,&uiKeepAlive);
		}
		m_iKeepAliveSendEnable = FALSE;
	}
	else
		m_iKeepAliveSendEnable = TRUE;

	/*----------------------------*/
	/*---	EM 0- 5000 受信	   ---*/
	/*----------------------------*/
	rv = m_pFinsSock->PLC_ReadCommand(m_iAddress,m_iReadSize,(char *)RecvBuff,&RcvLen);
	if(rv==TRUE){
		for(i=0;i<m_iReadSize;i++){
			uData = *(unsigned char*)(RecvBuff + (i * 2));
			uData <<= 8;
			uData |= *(unsigned char*)(RecvBuff + (i * 2 + 1));
			m_pReadBuff[i] = (unsigned short)uData;
		}/* for */
	}
}
void	CPLCMonitor::EndWork()
{
//	timeKillEvent(m_uiEvtID);
}

