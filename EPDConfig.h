#pragma once

#define     PM_MAX          5
#define     PM1             0
#define     PM2             1
#define     PM3             2
#define     PM4             3
#define     PM5             4

#define     WAVE_CNT_MAX    2048


#define     EPD_SAMPLING_TIME           100         // 100msec
#define     EPD_SAMPLING_POINT          600*10      // 600�b��
#define     EPD_WAVE_TOTAL              2048        // 1024 Point

typedef struct {
    unsigned short      usWaveData[EPD_SAMPLING_POINT][EPD_WAVE_TOTAL + 1];
}WAVE_DATA;





typedef struct {
    float   fWaveLength[WAVE_CNT_MAX + 1];
    char    szWaveLenName[WAVE_CNT_MAX + 1][32+1];
}WAVE_CONFIG;

/*
** ������ �g���W��
*/
typedef struct {
    double      dA0;
    double      dB1;
    double      dB2;
    double      dB3;
    double      dB4;
    double      dB5;
}WAVE_CALIBRATE;


/* OMRON
** PLC�ʐM�p�����[�^�[
*/
typedef struct {
	int	iPC_No;								// PC No.
	int	iPC_Network_Addr;					// PC Network
	int	iPC_Node_Addr;						// PC Node
	char szPLC_IP_Addr[128];				// PLC IP Adderss
	int	iPLC_Machine_No;					// PLC Machine No
	int	iMemoryType;						// Device Type
	int	iPLC_Network_Addr;					// PLC Network Address
	int	iPLC_Node_Addr;						// PLC Node Address
	int	iResponceTimeout;					// Timeout
	int	iPortNo;							// Port No.
    int iEnable;			// Enable
}PLC_COMM_CFG;

/* MELSEC/KEYENCE
** PLC�ʐM�p�����[�^�[
*/
// 1.IP�A�h���X
typedef struct {
    int			  iIPAddr1;				// IP�A�h���X1
    int			  iIPAddr2;				// IP�A�h���X2
    int			  iIPAddr3;				// IP�A�h���X3
    int			  iIPAddr4;				// IP�A�h���X4
}MC_IPADDRESS;


typedef struct {
    int 			iEnable;			// Enable
    MC_IPADDRESS	PLCIPAddr;			// IP�A�h���X
    MC_IPADDRESS	MyIPAddr;			// IP�A�h���X
    int 			iPortNo;			// �|�[�g�ԍ�
    int				iConnectCycleTime;	// �Đڑ���������
    int				iReplyTimeOut;		// �����҂��^�C���A�E�g
}MC_COMM_PARAM;


/*
** ������Ethernet�ʐM�p�����[�^�[
*/
typedef struct {
	int iIP_Addr[4+1];				        // PLC IP Adderss
	int	iPortNo;							// Port No.
    int iMACAddress[6 + 1];				        // PLC IP Adderss
}ETH_COMM_CFG;

typedef enum {
    USB_DRV = 0,
    ETHERNET_DRV,
}DRIVER_TYPE;

typedef struct {
    int     iLogSaveCycle;
    int     iPCEnable[PM_MAX + 1];
    int     iWaveMax;

    unsigned     unsEEPRomLoadParam[PM_MAX + 1];

    DRIVER_TYPE   Driver;
}EQ_CONFIG;



class CEPDConfig
{
public:
    CEPDConfig();
    virtual ~CEPDConfig();

    int     ConfigFileLoad();
    int     ConfigFileSave();

    int     WaveCfgFileLoad();
    int     WaveCfgFileSave();

    int     PLCCfgFileLoad();
    int     PLCCfgFileSave();

public:
    EQ_CONFIG       m_tEPDConfig;
//    WAVE_CONFIG     m_tWaveConfig;
    WAVE_CONFIG     m_tWaveConfig2[PM_MAX];
    WAVE_CALIBRATE  m_tWaveCaribrate[PM_MAX];
    PLC_COMM_CFG    m_tPLCConfig;
    MC_COMM_PARAM	m_tKeyenceComm;

    ETH_COMM_CFG    m_tEPDEtherConfig[PM_MAX];
    int             m_iMyIPAddress[4+1];

};

