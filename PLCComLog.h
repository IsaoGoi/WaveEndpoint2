#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ExtThread.h"



#define PLC_LOG_TBL_MAX		64
#define PLC_LOG_BUF_SIZE	256

#define PLC_LOG_FILE_SIZE		1024*1000
#define PLC_LOG_FILE_SAVEPATH	"PLC_COMM_LOG"
#define PLC_LOG_FILE_NAME		"PLC_LOG.TXT"

typedef enum {
	MSG_COMMAND,
	MSG_CMD_REPLY,
	MSG_EVENT,
	MSG_EVT_REPLY,
}PLCMSGTYPE;
	


typedef struct {
	BOOL			bSet;		// FALSE: 空き / TRUE: ﾃﾞｰﾀ有り
	int				iPC;
	BOOL			bRecv;		// FALSE: Send / TRUE: Recv
	PLCMSGTYPE		MsgType;
	int				iLen;
	HGLOBAL			HdlMsg;
	unsigned char	*pMsg;
	char			szDateTime[24];
}PLCCOM_LOG;



/*
** 通信ログスレッド定義
*/

class CPLCComLogThread : public CExtThread
{
public:
	CPLCComLogThread();
	virtual			~CPLCComLogThread() {;}
	virtual	void	StartWork();						/* Start Up(Over ride)*/
	virtual	void	DoWork();							/* ｽﾚｯﾄﾞ処理(Over ride)*/
	virtual	void	EndWork();							/* 後処理  (Over ride)*/

	void	SetLogQueue(int iPC,BOOL bRecv,PLCMSGTYPE Type,int iLen,unsigned char *pMsg);


private:
	PLCCOM_LOG	m_MsgLog[PLC_LOG_TBL_MAX];
	int			m_iWritePt;
	int			m_iReadPt;
	BOOL		m_bInitialized;
	CStdioFile	m_stdFile;

	void 		OutPutLog();
	void		ChangeLogFile();

	CRITICAL_SECTION	m_LogSetInterlock;

	CFileStatus			m_FileStatus;
	CString				m_szOutLogBuf;

};
