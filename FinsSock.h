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

/* �ʐMͯ�ލ\�� */
typedef struct{
/* 0 */unsigned char icf;		/* ICF: ��̫Ұ��ݥ���۰٥̨����		*/
/* 1 */unsigned char rsv;		/* RSV: �\��̨����(00H�Œ�)			*/
/* 2 */unsigned char gct;		/* GCT: ���e�ްĳ���ʉߐ�(02H�Œ�)	*/
/* 3 */unsigned char dna;		/* DNA: ���M��ȯ�ܰ����ڽ(1-127)	*/
/* 4 */unsigned char da1;		/* DA1: ���M��ɰ�ޱ��ڽ(1-126)		*/
/* 5 */unsigned char da2;		/* DA2: ���M�捆�@���ڽ				*/
/* 6 */unsigned char sna;		/* SNA: ���M��ȯ�ܰ����ڽ			*/
/* 7 */unsigned char sa1;		/* SA1: ���M��ȯ�ܰ����ڽ			*/
/* 8 */unsigned char sa2;		/* SA1: ���M�����@���ڽ				*/
/* 9 */unsigned char sid;		/* SID: ���޽ID						*/
}PLC_HEADER;

/* ���M�ް��\�� */
typedef struct{
/* 0 */	PLC_HEADER		header;			/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;			/* MRC	=1 �Œ�				*/
/*11 */	unsigned char	src;			/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	memtype;		/* ��شر����				*/
/*13 */	unsigned char	start_hi;		/* �ǂݏo�����ڽ-High		*/
/*14 */	unsigned char	start_mid;		/* �ǂݏo�����ڽ-Mid		*/
/*15 */	unsigned char	start_low;		/* �ǂݏo�����ڽ-Low(00H�Œ�)*/
/*16 */	unsigned char	len_hi;			/* �ǂݏo����-High			*/
/*17 */	unsigned char	len_low;		/* �ǂݏo����-Low			*/
}PLC_RCMD_FORM;

/* ��M�ް��\�� */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 �Œ�				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responce����-High		*/
/*13 */	unsigned char	resp2;				/* Responce����-Low			*/
/*14 */	unsigned char	data[MAX_DATA_LEN];	/* �ǂݏo���ް�				*/
}PLC_RRSP_FORM;

typedef struct{
/* 0 */	PLC_HEADER		header;			/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;			/* MRC	=1 �Œ�				*/
/*11 */	unsigned char	src;			/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	memtype;		/* ��شر����				*/
/*13 */	unsigned char	start_hi;		/* �������ݱ��ڽ-High		*/
/*14 */	unsigned char	start_mid;		/* �������ݱ��ڽ-Mid		*/
/*15 */	unsigned char	start_low;		/* �������ݱ��ڽ-Low(00H�Œ�)*/
/*16 */	unsigned char	len_hi;			/* �������ݐ�-High			*/
/*17 */	unsigned char	len_low;		/* �������ݐ�-Low			*/
/*18 */	unsigned char	data[MAX_DATA_LEN];	/* ���������ް�			*/
}PLC_WCMD_FORM;



/* ��M�ް��\�� */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 �Œ�				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responce����-High		*/
/*13 */	unsigned char	resp2;				/* Responce����-Low			*/
}PLC_WRSP_FORM;


typedef struct{
/* 0 */	PLC_HEADER		header;			/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;			/* MRC	=7 �Œ�				*/
/*11 */	unsigned char	src;			/* SRC	(READ=1) /WRITE = 2	*/
/*12 */	unsigned char	year;			/* �N						*/
/*13 */	unsigned char	month;			/* ��						*/
/*14 */	unsigned char	day;			/* ��						*/
/*15 */	unsigned char	hour;			/* ��						*/
/*16 */	unsigned char	minute;			/* ��						*/
/*17 */	unsigned char	second;			/* �b						*/
/*18 */	unsigned char	week;			/* �j��						*/
}PLC_DATECMD_FORM;



/* ��M�ް��\�� */
typedef struct{
/* 0 */	PLC_HEADER		header;				/* �ʐMͯ��					*/
/*10 */	unsigned char	mrc;				/* MRC	=1 �Œ�				*/
/*11 */	unsigned char	src;				/* SRC	READ = 1 /WRITE = 2	*/
/*12 */	unsigned char	resp1;				/* Responce����-High		*/
/*13 */	unsigned char	resp2;				/* Responce����-Low			*/
}PLC_DATERSP_FORM;


/*
** �ʐMͯ�ގg�p���ޒ�`
*/
#define	ICF_COMMAND		0x80				/* ICF Code				*/
#define	GCT_CODE		0x02				/* GCT Code				*/

/*
** �ʐM����ޒ�`
*/
#define	FINS_MRC		0x01				/* MRC Code				*/
#define	FINS_READ		0x01				/* Read Request Command	*/
#define	FINS_WRITE		0x02				/* Write Request Command*/
#define FINS_MEM_AREA	0xA0				/* Fins Memory Area Code<EM>*/
#define FINS_MEM_DM		0x82				/* <#001>DM�ر�w��		*/
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
	SOCKET				cl_sock;	/* ����M����Socket					*/
	CRITICAL_SECTION	m_FinsPort;					/* Socket �r������p����پ����		*/
	CRITICAL_SECTION	m_StatusUpdate;				/* PLC�ʐM��ԍX�V�p����پ����		*/
	SOCKADDR_IN			cl_addr;
	SOCKADDR_IN			sv_addr;


	UINT				Port_No;					/* UDP Port No.			*/
	char				m_PLC_IPAddr[32];			/* PLC IP ���ڽ			*/
	char				m_PC_IPAddr[32];			/* PLC IP ���ڽ			*/
	PLC_HEADER			m_plc_head;					/* Fins�ʐMHeader		*/
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
