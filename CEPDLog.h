#pragma once

#define		EPD_LOG_MAX		10000		// -> —e—Ê‚É‚æ‚é 100000

typedef struct {
	int		iPCNo;
	char	szGlassID[32+ 1];
	char	szYYYYMMDD[24 + 1];
	char	szhhmmss[24 + 1];
	char	szEPDLogPath[MAX_PATH+1];
	char	szRcpLogPath[MAX_PATH + 1];
	char	szCfgLogPath[MAX_PATH + 1];
}EPD_LOGPATH;

typedef struct {
	int					iLogCount;
	EPD_LOGPATH			tEPDLogPath[EPD_LOG_MAX + 1];
}EPD_LOG_TBL;


class CEPDLog
{
public:
	CEPDLog();
	virtual			~CEPDLog() { ; }

	void	InitialLogTable();
	int		AddEPDLog(int iPC, EPD_WAVEDATA2 *pEPDData, RECIPE *pRecipe, SET_WAVECFG *pWaveCfg);
	void	LoadEPDLogList();
	int		ReadEPDLog(int	iPos, EPD_WAVEDATA2* pEPDData, RECIPE* pRecipe, SET_WAVECFG* pWaveCfg);
	int		RemoveEPDLog();

	EPD_LOG_TBL		m_tEPDLogTable;
	EPD_LOG_TBL		m_tDisplayLogTable;
};

