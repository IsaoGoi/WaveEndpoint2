#pragma once


#if 0
#define     EPD_SAMPLING_TIME           100         // 100msec
#define     EPD_SAMPLING_POINT          600*10      // 600•b•ª
#define     EPD_WAVE_TOTAL              2048        // 1024 Point

typedef struct {
    unsigned short      usWaveData[EPD_SAMPLING_POINT][EPD_WAVE_TOTAL+1];
}WAVE_DATA;
#endif

class CWaveData_USB
{
public:
    CWaveData_USB();
    virtual ~CWaveData_USB();

    void StartWaveSampling();

    int GetParameter(int iPMNo);
    int SetParameter(int iPMNo);
    int SetEEPRomDefaultParameter(int iPMNo,int iSet);
    int ReadUnitInformation(int iPMNo);
    int WriteUnitInformation(int iPMNo);
    int ReadCalibrationValue(int iPMNo);
    int WriteCalibrationValue(int iPMNo);
    int GetStatusRequest(int iPMNo);
    void CWaveData_USB::GetDllVersion(int iPMNo);


    int         m_iTotal[PM_MAX];
    HGLOBAL		h_WaveData[PM_MAX];
    WAVE_DATA*  m_pWaveData[PM_MAX];
    CEPDConfig* m_pEPDConfig;
};

