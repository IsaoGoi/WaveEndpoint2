#pragma once

typedef struct {
	int		iStart;
	int		index;
	int		iSamplingCycle;
	int		iCurrCycle;
	unsigned short	usMonitorData[EPD_SAMPLING_POINT + 1][EPD_WAVE_TOTAL + 1];
}PC_MONITOR;

extern PC_MONITOR	m_tPC_Monitor[PM_MAX];

extern void StartPCMonitor(int iPM);
extern void StopPCMonitor(int iPM);
extern void StartPCMonitorTask();
extern void StopPCMonitorTask();
