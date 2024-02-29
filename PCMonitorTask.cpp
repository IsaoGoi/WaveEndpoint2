#include "pch.h"
#include "mmsystem.h"
#include "clockread_ms.h"
#include "WaveEndPoint.h"
#include "PCMonitorTask.h"


PC_MONITOR	m_tPC_Monitor[PM_MAX] = { {0,0},{0,0},{0,0},{0,0},{0,0} };
static	unsigned			m_uiEvtID;


void CALLBACK PCMonitor(UINT uEntID, UINT uMsgID, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	int			iPM;
	int			i,j,k,n;
	int			iStep;
	CWaveData_Eth* pWaveData_Eth = NULL;
	CWaveData_USB* pWaveData_USB = NULL;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
		pWaveData_Eth = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_Eth();
	else
		pWaveData_USB = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_USB();

	for (iPM = 0; iPM < PM_MAX; iPM++) {
		if (++m_tPC_Monitor[iPM].iCurrCycle == m_tPC_Monitor[iPM].iSamplingCycle) {
			m_tPC_Monitor[iPM].iCurrCycle = 0;

			if (pConfig->m_tEPDConfig.iPCEnable[iPM] == FALSE)
				continue;

			if (m_tPC_Monitor[iPM].iStart == FALSE)
				continue;

			if (m_tPC_Monitor[iPM].index >= EPD_SAMPLING_POINT)
				continue;

			if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV && pWaveData_Eth) {
				for (i = 0; i < EPD_WAVE_TOTAL; i++)
					m_tPC_Monitor[iPM].usMonitorData[m_tPC_Monitor[iPM].index][i] = pWaveData_Eth->m_pWaveData[iPM]->usWaveData[0][i];
			}
			else if (pConfig->m_tEPDConfig.Driver == USB_DRV && pWaveData_USB) {
				for (i = 0; i < EPD_WAVE_TOTAL; i++)
					m_tPC_Monitor[iPM].usMonitorData[m_tPC_Monitor[iPM].index][i] = pWaveData_USB->m_pWaveData[iPM]->usWaveData[0][i];
			}
			else {
				for (i = 0; i < EPD_WAVE_TOTAL; i++)
					m_tPC_Monitor[iPM].usMonitorData[m_tPC_Monitor[iPM].index][i] = 0;
			}

			m_tPC_Monitor[iPM].index++;
			if (m_tPC_Monitor[iPM].index == EPD_SAMPLING_POINT) {
				if(m_tPC_Monitor[iPM].iSamplingCycle==6){
					StopPCMonitor(iPM);
				}
				else {
					m_tPC_Monitor[iPM].iSamplingCycle++;
					for (i = 0, j = 1; i < EPD_SAMPLING_POINT / m_tPC_Monitor[iPM].iSamplingCycle * (m_tPC_Monitor[iPM].iSamplingCycle - 1);) {
						for (k = 0; k < EPD_WAVE_TOTAL; k++) {
							for (n = 0; n < (m_tPC_Monitor[iPM].iSamplingCycle - 1); n++)
								m_tPC_Monitor[iPM].usMonitorData[i + n][k] = m_tPC_Monitor[iPM].usMonitorData[j + n][k];
						}
						j += m_tPC_Monitor[iPM].iSamplingCycle;
						i += (m_tPC_Monitor[iPM].iSamplingCycle - 1);
					}
					m_tPC_Monitor[iPM].index = i;
				}
			}
		}
	}/* for */
}

void StartPCMonitorTask() 
{
	m_uiEvtID = ::timeSetEvent(100, 1, PCMonitor, 0, TIME_PERIODIC);
}
void StopPCMonitorTask() 
{
	timeKillEvent(m_uiEvtID);
}


void StartPCMonitor(int iPM)
{
	m_tPC_Monitor[iPM].iStart = TRUE;
	m_tPC_Monitor[iPM].index = 0;
	m_tPC_Monitor[iPM].iSamplingCycle = 1;
	m_tPC_Monitor[iPM].iCurrCycle = 0;
}
void StopPCMonitor(int iPM)
{
	m_tPC_Monitor[iPM].iStart = FALSE;
}
