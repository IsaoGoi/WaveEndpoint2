#pragma once

#include "MCMsg.h"



class CKeyenceSock 
{
public:
	CKeyenceSock();
	virtual ~CKeyenceSock();

	BOOL MelsecInitial();
	void MelsecEnd();
	BOOL PLC_ReadCommand(int iDevType,UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen);
	BOOL PLC_WriteCommand(int iDevType, UINT WriteAddr,int Writelen,LPSTR szSendBuff);
	BOOL PLC_BitWriteCommand(UINT WriteAddr,int iOnOff);
	void PLC_GetUDPStatus(MELSEC_STATUS	*status);
	void PLC_SetUDPStatus(MELSEC_STATUS udp_status);

public:
	MC_RECV_BUFFER	m_RecvBuff;			// ��M�o�b�t�@
	MC_SEND_BUFFER	m_SendBuff;			// ���M�o�b�t�@
	unsigned char	m_ucRecvBuff[2048+16];
	unsigned char	m_ucRecvData[2048+16];

	CIODB*			m_pIODB;
	int				m_ioCommStatus;

	int				m_iMCRecvLength;


	int melsock_select(int nfds,fd_set* readfds,fd_set* writefds,fd_set* exceptfds,const struct timeval* timeout);
	SOCKET				cl_sock;	/* ����M����Socket					*/
	CRITICAL_SECTION	m_FinsPort;					/* Socket �r������p����پ����		*/
	CRITICAL_SECTION	m_StatusUpdate;				/* PLC�ʐM��ԍX�V�p����پ����		*/
	SOCKADDR_IN			cl_addr;
	SOCKADDR_IN			sv_addr;

	UINT				Port_No;					/* UDP Port No.			*/
	char				m_PLC_IPAddr[32];			/* PLC IP ���ڽ			*/
	char				m_PC_IPAddr[32];			/* PLC IP ���ڽ			*/



	UINT			RespTimeOut;

	long			m_lMelsecRecvEvent;

	MELSEC_STATUS	UdpStatus;
	int				m_iReplyTimer;
};
