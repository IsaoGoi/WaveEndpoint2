
// WaveEndPoint.h : WaveEndPoint アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "Alarm.h"
#include "AccessLevel.h"
#include "EPDConfig.h"
#include "Recipe.h"
#include "FinsSock.h"
#include "IODB.h"
#include "IOEdit.h"
#include "EthernetLog.h"
#include "EthDriver.h"
#include "WaveData_Eth.h"
#include "WaveData_USB.h"
#include "CCalcWaveCfg.h"
#include "EPDData.h"
#include "DialogX.h"
#include "DlgAlarm.h"
#include "AlarmPopup.h"
#include "ChildView.h"
#include "PLCMonitor.h"
#include "DlgNumEdit.h"
#include "DlgEnumEdit.h"
#include "DlgStringEdit.h"
#include "WaveSamplingSave.h"
#include "CCalcWaveCfg.h"
#include "EPDThread.h"
#include "CEPDLog.h"
#include "EPDSimulation.h"
#include "PLCComLog.h"
#include "MainFrm.h"
#include "EthDriver.h"
#include "EthTimer.h"
#include "PCMonitorTask.h"
#include "MCExt.h"
#include "MCMsg.h"
#include "KeyenceSock.h"
#include "KeyenceMonitor.h"




// CWaveEndPointApp:
// このクラスの実装については、WaveEndPoint.cpp を参照してください
//

class CWaveEndPointApp : public CWinApp
{
public:
	CWaveEndPointApp() noexcept;



	HGLOBAL			    hPLCBuffHdl;
	unsigned short*		m_usPLCMonData;
	unsigned short*		GetPLCMonitorDataAddr() { return m_usPLCMonData; }
	int					m_iCfgScreenNo;

	char				m_szUnitID[PM_MAX][32 + 1];


	CIODB* m_pIODB;
	CFinsSock* m_FinsSock;
	CPLCMonitor* m_PLCMonitor;
	CDlgNumEdit* m_pNumEditPage;
	CDlgEnumEdit* m_pEnumEditPage;
	CDlgStringEdit* m_pStringEditPage;
	CWaveSamplingSave* m_pWaveSamplingSave;
	CEPDSimulation*		m_pEPDSimulation;
	CAlarm				*m_pAlarm;
	CDlgAlarm*			m_pDlgAlarm;

	CIODB*		GetIODB()		{ return m_pIODB; }
	CFinsSock*	GetFinsSock()	{ return m_FinsSock; }
	CWaveSamplingSave* GetWaveSamplingSave() { return m_pWaveSamplingSave; }
	CEPDSimulation*	   GetEPDSimulation() { return m_pEPDSimulation; }
	CAlarm* GetAlarmClass() { return(m_pAlarm); }
	CDlgAlarm* GetDlgAlarm() { return(m_pDlgAlarm); }

	/*
	** Keyence通信
	*/
	HGLOBAL			    hKeyenceBuffHdl;
	unsigned short* m_usKeyenceMonData;

	unsigned short* GetKeyenceMonitorDataAddr() { return m_usKeyenceMonData; }

	CKeyenceSock* m_pKeyenceSock;
	CKeyenceSock* GetKeyenceSock() { return m_pKeyenceSock; }
	CKeyenceMonitor* m_pKeyenceMonitor;



	int			GetConfigItemNo()			{ return(m_iCfgScreenNo); }
	void		SetConfigItemNo(int itemNo) { m_iCfgScreenNo = itemNo; }

	void		ChangeMainScreen() { ((CMainFrame*)m_pMainWnd)->m_wndView.m_pButtonPage->ChangeMainScreen(); }


	CRITICAL_SECTION	m_Recipe_Interlock;
// オーバーライド
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	CEPDConfig*		GetEPDConfig()	{ return(m_pEPDConfig);}
	CWaveData_Eth*	GetWaveData_Eth()	{ return(m_pWaveData_Eth); }
	CWaveData_USB*  GetWaveData_USB() { return(m_pWaveData_USB); }

	CEPDThread*		GetEPDThread(int iPM) { return(m_pEPDThread[iPM]); }

	int				m_iCurrRecipe[PM_MAX + 1];

#if 0
	int				m_iDel_RunEPDLogStatus;
	int				m_iDel_RunRCPLogStatus;
	int				m_iDel_RunCFGLogStatus;
	CFileFind		m_RunEPDLog_filefind;
	CFileFind		m_RunRCPLog_filefind;
	CFileFind		m_RunCFGLog_filefind;
	void			CheckRunEPDLog();
	void			CheckRunRCPLog();
	void			CheckRunCFGLog();
#endif
	int				m_iRowDataSaveMode[PM_MAX + 1];


// データ
	CEPDConfig* m_pEPDConfig;
	CWaveData_Eth*	m_pWaveData_Eth;
	CWaveData_USB*  m_pWaveData_USB;

	CPCRcpCtrl* m_pPCRcpCtrl;
	CPCRcpCtrl* GetPCRcpCtrl() { return(m_pPCRcpCtrl); }

	CWaveCfgCtrl* m_pWaveCfgCtrl;
	CWaveCfgCtrl* GetWaveCfgCtrl() { return(m_pWaveCfgCtrl); }

	CEPDThread* m_pEPDThread[PM_MAX];
	CEPDLog*	m_pEPDLog;
	CEPDLog*	GetEPDLogClass() {	return(m_pEPDLog);}

	CPLCComLogThread	*m_pPLCLogThread;
	CPLCComLogThread* GetPLCLogClass() { return(m_pPLCLogThread); }



	// 実装
	void		LoadEPDUnitID();
	void		SaveEPDUnitID();

	CAccessLevel* m_pAccessLevel;
	CAccessLevel* GetAccessLevelClass() { return(m_pAccessLevel); }
	BOOL			CheckAccessLevel(ACCESSLEVEL level);
	ACCESSLEVEL		GetAccessLevel() { return(m_pAccessLevel->GetCurrLevel()); }
	CTime		m_tLogOnTime;
	int			m_iLogOn;
	void		AutoLogOff_Start();
	void		AutoLogOff_Stop();

	// Row Log Delete Function
	void RawLog_CalcDate();
	void RowLog_FindOld1stLog();
	void RowLog_FindOldNextLog();
	void CheckRowLog();

	void EthNetLog_CalcDate();
	void EthNetLog_FindOld1stLog();
	void EthNetLog_FindOldNextLog();
	void CheckEthNetLog();

	void PLCLog_CalcDate();
	void PLCLog_FindOld1stLog();
	void PLCLog_FindOldNextLog();
	void CheckPLCLog();

	typedef enum {
		FUNC_ROWLOG_CALCMONTH,
		FUNC_ROWLOG_FIND,
		FUNC_ROWLOG_NEXT,
	}FUNC_DEL_LOG;

	int				m_iIdleProcessCnt;
	FUNC_DEL_LOG	m_RowLogDeleteFunc;
	CString			m_szCalcDate;
	CFileFind		m_RowLogfilefind;

	FUNC_DEL_LOG	m_EthNetLogDeleteFunc;
	CString			m_szEthNetCalcDate;
	CFileFind		m_EthNetLogfilefind;

	FUNC_DEL_LOG	m_PLCLogDeleteFunc;
	CString			m_szPLCCalcDate;
	CFileFind		m_PLCLogfilefind;


	void GetCurrUser(char* pszUserName) { m_pAccessLevel->GetCurrUser(pszUserName); }
	void ButtonPageUpdate();

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CWaveEndPointApp theApp;
