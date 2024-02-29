#pragma once

typedef enum {
/* 0 */UDP_Initialize,
/* 1 */UDP_Running,
/* 2 */UDP_SendError,
/* 3 */UDP_RecvDataError,
/* 4 */UDP_RecvTimeOut,
/* 5 */UDP_SocketError
}UDP_STATUS;
#define	MAX_DATA_LEN	2048

/* 通信ﾍｯﾀﾞ構造 */
typedef struct{
/* 0 */unsigned char icf;		/* ICF: ｲﾝﾌｫﾒｰｼｮﾝ･ｺﾝﾄﾛｰﾙ･ﾌｨｰﾙﾄﾞ		*/
/* 1 */unsigned char rsv;		/* RSV: 予約ﾌｨｰﾙﾄﾞ(00H固定)			*/
/* 2 */unsigned char gct;		/* GCT: 許容ｹﾞｰﾄｳｪｲ通過数(02H固定)	*/
/* 3 */unsigned char dna;		/* DNA: 送信先ﾈｯﾄﾜｰｸｱﾄﾞﾚｽ(1-127)	*/
/* 4 */unsigned char da1;		/* DA1: 送信先ﾉｰﾄﾞｱﾄﾞﾚｽ(1-126)		*/
/* 5 */unsigned char da2;		/* DA2: 送信先号機ｱﾄﾞﾚｽ				*/
/* 6 */unsigned char sna;		/* SNA: 送信元ﾈｯﾄﾜｰｸｱﾄﾞﾚｽ			*/
/* 7 */unsigned char sa1;		/* SA1: 送信元ﾈｯﾄﾜｰｸｱﾄﾞﾚｽ			*/
/* 8 */unsigned char sa2;		/* SA1: 送信元号機ｱﾄﾞﾚｽ				*/
/* 9 */unsigned char sid;		/* SID: ｻｰﾋﾞｽID						*/
}PLC_HEADER;

/* 送信ﾃﾞｰﾀ構造 */
typedef struct{
/* 0 */	PLC_HEADER		header;			/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;			/* MRC	=1 固定				*/
/*11 */	unsigned char	src;			/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	memtype;		/* ﾒﾓﾘｴﾘｱｺｰﾄﾞ				*/
/*13 */	unsigned char	start_hi;		/* 読み出しｱﾄﾞﾚｽ-High		*/
/*14 */	unsigned char	start_mid;		/* 読み出しｱﾄﾞﾚｽ-Mid		*/
/*15 */	unsigned char	start_low;		/* 読み出しｱﾄﾞﾚｽ-Low(00H固定)*/
/*16 */	unsigned char	len_hi;			/* 読み出し数-High			*/
/*17 */	unsigned char	len_low;		/* 読み出し数-Low			*/
}PLC_RCMD_FORM;

/* 受信ﾃﾞｰﾀ構造 */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 固定				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responceｺｰﾄﾞ-High		*/
/*13 */	unsigned char	resp2;				/* Responceｺｰﾄﾞ-Low			*/
/*14 */	unsigned char	data[MAX_DATA_LEN];	/* 読み出しﾃﾞｰﾀ				*/
}PLC_RRSP_FORM;

typedef struct{
/* 0 */	PLC_HEADER		header;			/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;			/* MRC	=1 固定				*/
/*11 */	unsigned char	src;			/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	memtype;		/* ﾒﾓﾘｴﾘｱｺｰﾄﾞ				*/
/*13 */	unsigned char	start_hi;		/* 書き込みｱﾄﾞﾚｽ-High		*/
/*14 */	unsigned char	start_mid;		/* 書き込みｱﾄﾞﾚｽ-Mid		*/
/*15 */	unsigned char	start_low;		/* 書き込みｱﾄﾞﾚｽ-Low(00H固定)*/
/*16 */	unsigned char	len_hi;			/* 書き込み数-High			*/
/*17 */	unsigned char	len_low;		/* 書き込み数-Low			*/
/*18 */	unsigned char	data[MAX_DATA_LEN];	/* 書き込みﾃﾞｰﾀ			*/
}PLC_WCMD_FORM;



/* 受信ﾃﾞｰﾀ構造 */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 固定				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responceｺｰﾄﾞ-High		*/
/*13 */	unsigned char	resp2;				/* Responceｺｰﾄﾞ-Low			*/
}PLC_WRSP_FORM;


typedef struct{
/* 0 */	PLC_HEADER		header;			/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;			/* MRC	=7 固定				*/
/*11 */	unsigned char	src;			/* SRC	(READ=1) /WRITE = 2	*/
/*12 */	unsigned char	year;			/* 年						*/
/*13 */	unsigned char	month;			/* 月						*/
/*14 */	unsigned char	day;			/* 日						*/
/*15 */	unsigned char	hour;			/* 時						*/
/*16 */	unsigned char	minute;			/* 分						*/
/*17 */	unsigned char	second;			/* 秒						*/
/*18 */	unsigned char	week;			/* 曜日						*/
}PLC_DATECMD_FORM;



/* 受信ﾃﾞｰﾀ構造 */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* 通信ﾍｯﾀﾞ					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 固定				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responceｺｰﾄﾞ-High		*/
/*13 */	unsigned char	resp2;				/* Responceｺｰﾄﾞ-Low			*/
}PLC_DATERSP_FORM;


/*
** 通信ﾍｯﾀﾞ使用ｺｰﾄﾞ定義
*/
#define	ICF_COMMAND		0x80				/* ICF Code				*/
#define	GCT_CODE		0x02				/* GCT Code				*/

/*
** 通信ｺﾏﾝﾄﾞ定義
*/
#define	FINS_MRC		0x01				/* MRC Code				*/
#define	FINS_READ		0x01				/* Read Request Command	*/
#define	FINS_WRITE		0x02				/* Write Request Command*/
#define FINS_MEM_AREA	0xA0				/* Fins Memory Area Code<EM>*/
#define FINS_MEM_DM		0x82				/* <#001>DMｴﾘｱ指定		*/
//#define FINS_MEM_CIO_W	0xB0				/* CIO Word				*/
//#define FINS_MEM_CIO_BIT 0x30				/* CIO Bit				*/
#define FINS_MEM_HR_W	0xB2				/* HR Word				*/
#define FINS_MEM_HR_BIT 0x32				/* HR Bit				*/



class CFinsSock 
{
public:
	CFinsSock();
	virtual ~CFinsSock();

	BOOL FinsInitial();
	void FinsEnd();
	BOOL PLC_ReadCommand(UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen);
	BOOL PLC_WriteCommand(UINT WriteAddr,int Writelen,LPSTR szSendBuff);
	BOOL PLC_CIORead(UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen);
	BOOL PLC_CIOWrite(UINT WriteAddr,int Writelen,LPSTR szSendBuff);
	BOOL PLC_CIOBitWrite(UINT WriteAddr,int iBit,LPSTR szSendBuff);

	BOOL PLC_SendDateTime();
	void PLC_GetUDPStatus(UDP_STATUS *udp_status);
	void PLC_SetUDPStatus(UDP_STATUS udp_status);


private:
	int fins_select(int nfds,fd_set* readfds,fd_set* writefds,fd_set* exceptfds,const struct timeval* timeout);
	char Get1ByteBCD(int iVal);
	SOCKET				cl_sock;	/* 送受信共通Socket					*/
	CRITICAL_SECTION	m_FinsPort;					/* Socket 排他制御用ｸﾘﾁｶﾙｾｸｼｮﾝ		*/
	CRITICAL_SECTION	m_StatusUpdate;				/* PLC通信状態更新用ｸﾘﾁｶﾙｾｸｼｮﾝ		*/
	SOCKADDR_IN			cl_addr;
	SOCKADDR_IN			sv_addr;


	UINT				Port_No;					/* UDP Port No.			*/
	char				m_PLC_IPAddr[32];			/* PLC IP ｱﾄﾞﾚｽ			*/
	char				m_PC_IPAddr[32];			/* PLC IP ｱﾄﾞﾚｽ			*/
	PLC_HEADER			m_plc_head;					/* Fins通信Header		*/
	PLC_RCMD_FORM		m_plc_rcmd;
	PLC_RRSP_FORM		m_plc_rresp;
	PLC_WCMD_FORM		m_plc_wcmd;
	PLC_WRSP_FORM		m_plc_wresp;
	PLC_DATECMD_FORM	m_plc_datecmd;
	PLC_DATERSP_FORM	m_plc_datersp;
	UINT				RespTimeOut;
	UDP_STATUS			UdpStatus;
	unsigned char		MemoryType;

};
