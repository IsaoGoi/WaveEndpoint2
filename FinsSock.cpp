// FinsSock.cpp: CFinsSock クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "WaveEndPoint.h"
#include "FinsSock.h"
#include "afxmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static char	gsz_PLC_dbgmsg[80]={'\0'};
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFinsSock::CFinsSock()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox("Windows Socket Initial Error");
		return;
	}
	CEPDConfig *pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	/*--- ProfileよりPLCｱﾄﾞﾚｽ/Port No.を取得する ---*/
	strcpy_s(m_PLC_IPAddr,sizeof(m_PLC_IPAddr), pConfig->m_tPLCConfig.szPLC_IP_Addr);
	Port_No = pConfig->m_tPLCConfig.iPortNo;
	RespTimeOut = pConfig->m_tPLCConfig.iResponceTimeout;
	m_plc_head.dna = (unsigned char)pConfig->m_tPLCConfig.iPLC_Network_Addr;
	m_plc_head.da1 = (unsigned char)pConfig->m_tPLCConfig.iPLC_Node_Addr;
	m_plc_head.da2 = (unsigned char)pConfig->m_tPLCConfig.iPLC_Machine_No;
	m_plc_head.sna = (unsigned char)pConfig->m_tPLCConfig.iPC_Network_Addr;
	m_plc_head.sa1 = (unsigned char)pConfig->m_tPLCConfig.iPC_Node_Addr;
	m_plc_head.sa2 = (unsigned char)pConfig->m_tPLCConfig.iPC_No;
	MemoryType = FINS_MEM_AREA;

	/*--- 通信ﾍｯﾀﾞ SID初期化 ---*/
	m_plc_head.sid=0;

	/*--- Fins Read/Write ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	InitializeCriticalSection(&m_FinsPort);

	/*--- Fins Status Set/Get ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	InitializeCriticalSection(&m_StatusUpdate);

	cl_sock = INVALID_SOCKET;
	UdpStatus=UDP_Initialize;
}

CFinsSock::~CFinsSock()
{
	/*--- Fins Read/Write ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	DeleteCriticalSection(&m_FinsPort);
	/*--- Fins Status Set/Get ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	DeleteCriticalSection(&m_StatusUpdate);
}
BOOL CFinsSock::FinsInitial()
{
	char	wkBuf[16];

	cl_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (cl_sock == INVALID_SOCKET)
		return FALSE;


	memset(&cl_addr, 0, sizeof(cl_addr));
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_port   = (unsigned short)(htons(Port_No+1));

	memset(m_PC_IPAddr,'\0',sizeof(m_PC_IPAddr));
	strncpy_s(m_PC_IPAddr,sizeof(m_PC_IPAddr),m_PLC_IPAddr,(strlen(m_PLC_IPAddr)-1));
	sprintf_s(wkBuf,sizeof(wkBuf),"%d",(int)m_plc_head.sa1);
	strcat_s(m_PC_IPAddr,sizeof(m_PC_IPAddr),wkBuf);
//	cl_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cl_addr.sin_addr.s_addr = inet_addr(m_PC_IPAddr);
//	InetPton(cl_addr.sin_family, m_PC_IPAddr, &cl_addr.sin_addr.S_un.S_addr);

	if(bind(cl_sock,(LPSOCKADDR)&cl_addr,sizeof(cl_addr)) == SOCKET_ERROR)
		return(FALSE);

	memset(&sv_addr, 0, sizeof(sv_addr));
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port   = (unsigned short)(htons(Port_No));
	sv_addr.sin_addr.s_addr = inet_addr(m_PLC_IPAddr);
//	InetPton(sv_addr.sin_family, m_PLC_IPAddr, &sv_addr.sin_addr.S_un.S_addr);

	/*--- 再接続要求待ち ---*/
	PLC_SetUDPStatus(UDP_Running);


	return TRUE;
}
void CFinsSock::FinsEnd()
{
	if (cl_sock != INVALID_SOCKET)
		closesocket(cl_sock);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_ReadCommand(LPSTR szSendBuff,
**						 int *Sendlen,
**						 LPSTR szRecvBuff,
**						 int *RecvLen)
**
**		処理 : PLC Readｺﾏﾝﾄﾞ処理
**
**		入力 : LPVOID lp
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_ReadCommand(UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen){
	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry = 0;
	int					nrlen;

	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	*RecvLen = 0;

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);	
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_rcmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_rcmd.header.icf = ICF_COMMAND;	
	m_plc_rcmd.header.gct = GCT_CODE;	

	m_plc_rcmd.mrc= FINS_MRC;	
	m_plc_rcmd.src= FINS_READ;	
//	m_plc_rcmd.memtype= FINS_MEM_AREA;	
	m_plc_rcmd.memtype= MemoryType;	
	/*--- Read 開始ｱﾄﾞﾚｽ set ---*/
	m_plc_rcmd.start_hi= (unsigned char)((ReadAddr >> 8) & 0x0000ff);	
	m_plc_rcmd.start_mid= (unsigned char)(ReadAddr & 0x0000ff);	
	m_plc_rcmd.start_low= 0;	

	/*--- Read ﾚﾝｸﾞｽ set ---*/
	m_plc_rcmd.len_hi = (unsigned char)((Readlen >> 8) & 0x00ff);
	m_plc_rcmd.len_low = (unsigned char)(Readlen & 0x00ff);

	iRetry = 0;
READ_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_rcmd,sizeof(PLC_RCMD_FORM),0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != sizeof(PLC_RCMD_FORM)){

			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry < 2){
			goto READ_RETRY;
		}

		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_rcmd.header.sid ==	m_plc_rresp.header.sid) &&
				   (m_plc_rresp.resp1 == 0)				  &&
				   (m_plc_rresp.resp2 == 0)				  &&
				   (nrlen == (int)(sizeof(PLC_HEADER)+4+(Readlen*2)))){
						memcpy(szRecvBuff,(char *)m_plc_rresp.data,Readlen*2);
						*RecvLen= Readlen*2;
				}
				else{
					errcode=WSAGetLastError();
					sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(RCMD LastErr=%d,sid=%d,%d rsp1=%d,rsp2=%d,Len=%d,%d)",
											errcode,
											m_plc_rcmd.header.sid,
											m_plc_rresp.header.sid,
											m_plc_rresp.resp1,
											m_plc_rresp.resp2,
											nrlen,
											(int)(sizeof(PLC_HEADER)+4+(Readlen*2)));

					while(1){
						ret=fins_select(0,&readfds,NULL,NULL,&select_time);
						if(ret != SOCKET_ERROR && ret != 0){
							nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
							if(nrlen > 0 ){
								nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
								/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
								if((m_plc_rcmd.header.sid ==	m_plc_rresp.header.sid) &&
								   (m_plc_rresp.resp1 == 0)				  &&
								   (m_plc_rresp.resp2 == 0)				  &&
								   (nrlen == (int)(sizeof(PLC_HEADER)+4+(Readlen*2)))){
										memcpy(szRecvBuff,(char *)m_plc_rresp.data,Readlen*2);
										*RecvLen= Readlen*2;
									break;
								}
							}
							else
								break;
						}
						else
							break;
					}/* while */					
					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else{
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_RecvTimeOut);
		}
	}
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SocketError);
		}
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_WriteCommand(LPSTR szSendBuff,
**						 int *Sendlen,
**						 LPSTR szRecvBuff,
**						 int *RecvLen)
**
**		処理 : PLC Readｺﾏﾝﾄﾞ処理
**
**		入力 : LPVOID lp
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_WriteCommand(UINT WriteAddr,int Writelen,LPSTR szSendBuff){

	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry;
	int					nrlen;

	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_wcmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_wcmd.header.icf = ICF_COMMAND;	
	m_plc_wcmd.header.gct = GCT_CODE;	

	m_plc_wcmd.mrc= FINS_MRC;	
	m_plc_wcmd.src= FINS_WRITE;	
//	m_plc_wcmd.memtype= FINS_MEM_AREA;	
	m_plc_wcmd.memtype= MemoryType;	

	/*--- Write 開始ｱﾄﾞﾚｽ set ---*/
	m_plc_wcmd.start_hi= (unsigned char)((WriteAddr >> 8) & 0x0000ff);	
	m_plc_wcmd.start_mid= (unsigned char)(WriteAddr & 0x0000ff);	
	m_plc_wcmd.start_low= 0;	

	/*--- Write ﾚﾝｸﾞｽ set ---*/
	m_plc_wcmd.len_hi = (unsigned char)((Writelen >> 8) & 0x00ff);
	m_plc_wcmd.len_low = (unsigned char)(Writelen & 0x00ff);
	memcpy((char *)m_plc_wcmd.data,szSendBuff, Writelen*2);

	iRetry = 0;
WRITE_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_wcmd,sizeof(PLC_HEADER)+8+(Writelen*2),0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (int)(sizeof(PLC_HEADER)+8+(Writelen*2))){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry < 2){
			goto WRITE_RETRY;
		}
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_wcmd.header.sid != m_plc_wresp.header.sid) ||
				   (m_plc_wresp.resp1 != 0)				  ||
				   (m_plc_wresp.resp2 != 0)				  ||
				   (nrlen != (int)(sizeof(PLC_HEADER)+4))){

					errcode=WSAGetLastError();
					sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(WCMD LastErr=%d,sid=%d,%d rsp1=%d,rsp2=%d,Len=%d,%d)",
											errcode,
											m_plc_wcmd.header.sid,
											m_plc_wresp.header.sid,
											m_plc_wresp.resp1,
											m_plc_wresp.resp2,
											nrlen,
											(int)(sizeof(PLC_HEADER)+4));

					while(1){
						ret=fins_select(0,&readfds,NULL,NULL,&select_time);
						if(ret != SOCKET_ERROR && ret != 0){
							nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
							if(nrlen > 0 ){
								nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
								/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
								if((m_plc_wcmd.header.sid == m_plc_wresp.header.sid) &&
								   (m_plc_wresp.resp1 == 0)				  &&
								   (m_plc_wresp.resp2 == 0)				  &&
								   (nrlen == (int)(sizeof(PLC_HEADER)+4))){
									break;
								}
							}
							else
								break;
						}
						else
							break;
					}/* while */					


					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else{
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

			PLC_SetUDPStatus(UDP_RecvTimeOut);
		}
	}
//	Sleep(20);
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SocketError);
		}
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_CIORead(LPSTR szSendBuff,
**						 int *Sendlen,
**						 LPSTR szRecvBuff,
**						 int *RecvLen)
**
**		処理 : PLC CIO Readｺﾏﾝﾄﾞ処理
**
**		入力 : LPVOID lp
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_CIORead(UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen){
	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry;
	int					nrlen;
	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	*RecvLen = 0;

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);	
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_rcmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_rcmd.header.icf = ICF_COMMAND;	
	m_plc_rcmd.header.gct = GCT_CODE;	

	m_plc_rcmd.mrc= FINS_MRC;	
	m_plc_rcmd.src= FINS_READ;	
	m_plc_rcmd.memtype= FINS_MEM_HR_W;	
	/*--- Read 開始ｱﾄﾞﾚｽ set ---*/
	m_plc_rcmd.start_hi= (unsigned char)((ReadAddr >> 8) & 0x0000ff);	
	m_plc_rcmd.start_mid= (unsigned char)(ReadAddr & 0x0000ff);	
	m_plc_rcmd.start_low= 0;	

	/*--- Read ﾚﾝｸﾞｽ set ---*/
	m_plc_rcmd.len_hi = (unsigned char)((Readlen >> 8) & 0x00ff);
	m_plc_rcmd.len_low = (unsigned char)(Readlen & 0x00ff);

	iRetry =0;
CIO_READ_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_rcmd,sizeof(PLC_RCMD_FORM),0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != sizeof(PLC_RCMD_FORM)){

			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry<2){
			goto CIO_READ_RETRY;
		}
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_rcmd.header.sid ==	m_plc_rresp.header.sid) &&
				   (m_plc_rresp.resp1 == 0)				  &&
				   (m_plc_rresp.resp2 == 0)				  &&
				   (nrlen == (int)(sizeof(PLC_HEADER)+4+(Readlen*2)))){
						memcpy(szRecvBuff,(char *)m_plc_rresp.data,Readlen*2);
						*RecvLen= Readlen*2;
				}
				else{
					errcode=WSAGetLastError();
					sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(RCMD LastErr=%d,sid=%d,%d rsp1=%d,rsp2=%d,Len=%d,%d)",
											errcode,
											m_plc_rcmd.header.sid,
											m_plc_rresp.header.sid,
											m_plc_rresp.resp1,
											m_plc_rresp.resp2,
											nrlen,
											(int)(sizeof(PLC_HEADER)+4+(Readlen*2)));

					while(1){
						ret=fins_select(0,&readfds,NULL,NULL,&select_time);
						if(ret != SOCKET_ERROR && ret != 0){
							nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
							if(nrlen > 0 ){
								nrlen = recvfrom(cl_sock,(char *)&m_plc_rresp,sizeof(PLC_RRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
								/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
								if((m_plc_rcmd.header.sid ==	m_plc_rresp.header.sid) &&
								   (m_plc_rresp.resp1 == 0)				  &&
								   (m_plc_rresp.resp2 == 0)				  &&
								   (nrlen == (int)(sizeof(PLC_HEADER)+4+(Readlen*2)))){
										memcpy(szRecvBuff,(char *)m_plc_rresp.data,Readlen*2);
										*RecvLen= Readlen*2;
									break;
								}
							}
							else
								break;
						}
						else
							break;
					}/* while */					
					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else{
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_RecvTimeOut);
		}
	}
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SocketError);
		}
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_CIOWrite(LPSTR szSendBuff,
**						 int *Sendlen,
**						 LPSTR szRecvBuff,
**						 int *RecvLen)
**
**		処理 : PLC Readｺﾏﾝﾄﾞ処理
**
**		入力 : LPVOID lp
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_CIOWrite(UINT WriteAddr,int Writelen,LPSTR szSendBuff){

	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry;
	int					nrlen;

	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_wcmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_wcmd.header.icf = ICF_COMMAND;	
	m_plc_wcmd.header.gct = GCT_CODE;	

	m_plc_wcmd.mrc= FINS_MRC;	
	m_plc_wcmd.src= FINS_WRITE;	
	m_plc_wcmd.memtype= FINS_MEM_HR_W;	

	/*--- Write 開始ｱﾄﾞﾚｽ set ---*/
	m_plc_wcmd.start_hi= (unsigned char)((WriteAddr >> 8) & 0x0000ff);	
	m_plc_wcmd.start_mid= (unsigned char)(WriteAddr & 0x0000ff);	
	m_plc_wcmd.start_low= 0;	

	/*--- Write ﾚﾝｸﾞｽ set ---*/
	m_plc_wcmd.len_hi = (unsigned char)((Writelen >> 8) & 0x00ff);
	m_plc_wcmd.len_low = (unsigned char)(Writelen & 0x00ff);
	memcpy((char *)m_plc_wcmd.data,szSendBuff, Writelen*2);

	iRetry =0;
CIO_WRITE_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_wcmd,sizeof(PLC_HEADER)+8+(Writelen*2),0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (int)(sizeof(PLC_HEADER)+8+(Writelen*2))){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry < 2){
			goto CIO_WRITE_RETRY;
		}
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_wcmd.header.sid != m_plc_wresp.header.sid) ||
				   (m_plc_wresp.resp1 != 0)				  ||
				   (m_plc_wresp.resp2 != 0)				  ||
				   (nrlen != (int)(sizeof(PLC_HEADER)+4))){

					errcode=WSAGetLastError();
					sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(WCMD LastErr=%d,sid=%d,%d rsp1=%d,rsp2=%d,Len=%d,%d)",
											errcode,
											m_plc_wcmd.header.sid,
											m_plc_wresp.header.sid,
											m_plc_wresp.resp1,
											m_plc_wresp.resp2,
											nrlen,
											(int)(sizeof(PLC_HEADER)+4));

					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else{
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

			PLC_SetUDPStatus(UDP_RecvTimeOut);
		}
	}
//	Sleep(20);
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SocketError);
		}
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_CIOWrite(LPSTR szSendBuff,
**						 int *Sendlen,
**						 LPSTR szRecvBuff,
**						 int *RecvLen)
**
**		処理 : PLC Readｺﾏﾝﾄﾞ処理
**
**		入力 : LPVOID lp
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_CIOBitWrite(UINT WriteAddr,int iBit,LPSTR szSendBuff)
{

	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry;
	int					nrlen;

	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_wcmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_wcmd.header.icf = ICF_COMMAND;	
	m_plc_wcmd.header.gct = GCT_CODE;	

	m_plc_wcmd.mrc= FINS_MRC;	
	m_plc_wcmd.src= FINS_WRITE;	
	m_plc_wcmd.memtype= FINS_MEM_HR_BIT;	

	/*--- Write 開始ｱﾄﾞﾚｽ set ---*/
	m_plc_wcmd.start_hi= (unsigned char)((WriteAddr >> 8) & 0x0000ff);	
	m_plc_wcmd.start_mid= (unsigned char)(WriteAddr & 0x0000ff);	
	m_plc_wcmd.start_low= iBit;	

	/*--- Write ﾚﾝｸﾞｽ set ---*/
	m_plc_wcmd.len_hi = (unsigned char)0;
	m_plc_wcmd.len_low = (unsigned char)1;
	memcpy((char *)m_plc_wcmd.data,szSendBuff, 1);

	iRetry = 0;
CIO_BITWRITE_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_wcmd,sizeof(PLC_HEADER)+8+1,0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (int)(sizeof(PLC_HEADER)+8+1)){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry < 2){
			goto CIO_BITWRITE_RETRY;
		}
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_wresp,sizeof(PLC_WRSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_wcmd.header.sid != m_plc_wresp.header.sid) ||
				   (m_plc_wresp.resp1 != 0)				  ||
				   (m_plc_wresp.resp2 != 0)				  ||
				   (nrlen != (int)(sizeof(PLC_HEADER)+4))){

					errcode=WSAGetLastError();
					sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(WCMD LastErr=%d,sid=%d,%d rsp1=%d,rsp2=%d,Len=%d,%d)",
											errcode,
											m_plc_wcmd.header.sid,
											m_plc_wresp.header.sid,
											m_plc_wresp.resp1,
											m_plc_wresp.resp2,
											nrlen,
											(int)(sizeof(PLC_HEADER)+4));

					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else{
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);

			PLC_SetUDPStatus(UDP_RecvTimeOut);
		}
	}
//	Sleep(20);
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR){
			errcode=WSAGetLastError();
			sprintf_s(gsz_PLC_dbgmsg,sizeof(gsz_PLC_dbgmsg),"(GetLastError = %d)",errcode);
			PLC_SetUDPStatus(UDP_SocketError);
		}
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_SendDateTime()
**
**		処理 : PLC 時刻合わせ処理
**
**		入力 : なし
**
**		出力 : Thread 終了ｺｰﾄﾞ
**
**----------------------------------------------------------------**
*/
BOOL CFinsSock::PLC_SendDateTime(){

	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	SYSTEMTIME			t_SysTime;
	int					iRetry;
	int					nrlen;

	/*--- Socket 有効ﾁｪｯｸ ---*/
	if(cl_sock == INVALID_SOCKET)
		return(FALSE);

	/*--- 前処理終了待ち ---*/
	EnterCriticalSection(&m_FinsPort);

	/*--- Socket集合体ｸﾘｱ ---*/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	/*--- Read/Write Select用Socket登録 ---*/
	FD_SET(cl_sock ,&readfds);
	FD_SET(cl_sock ,&writefds);
	select_time.tv_sec = 0;
	select_time.tv_usec = 100000;

	/*--- 送信ﾃﾞｰﾀ作成 ---*/
	m_plc_head.sid++;
	memcpy((char *)&m_plc_datecmd.header,(char *)&m_plc_head,sizeof(PLC_HEADER));

	if(m_plc_head.sid == 255)			/* SID 更新 */
		m_plc_head.sid =0;

	m_plc_datecmd.header.icf = ICF_COMMAND;	
	m_plc_datecmd.header.gct = GCT_CODE;	

	m_plc_datecmd.mrc= 7;	
	m_plc_datecmd.src= 2;	

	GetLocalTime(&t_SysTime);

    m_plc_datecmd.year   = Get1ByteBCD((t_SysTime.wYear-2000)); 
    m_plc_datecmd.month  = Get1ByteBCD(t_SysTime.wMonth);
    m_plc_datecmd.day    = Get1ByteBCD(t_SysTime.wDay);
    m_plc_datecmd.hour   = Get1ByteBCD(t_SysTime.wHour);
    m_plc_datecmd.minute = Get1ByteBCD(t_SysTime.wMinute);
    m_plc_datecmd.second = Get1ByteBCD(t_SysTime.wSecond);
    m_plc_datecmd.week = Get1ByteBCD(t_SysTime.wDayOfWeek);

	iRetry = 0;
DATE_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=fins_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)&m_plc_datecmd,sizeof(PLC_HEADER)+9,0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (int)(sizeof(PLC_HEADER)+9)){
			errcode=WSAGetLastError();
			PLC_SetUDPStatus(UDP_SendError);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=fins_select(0,&readfds,NULL,NULL,&select_time);
		if(ret==0 && ++iRetry<2){
			goto DATE_RETRY;
		}
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)&m_plc_datersp,sizeof(PLC_DATERSP_FORM), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)&m_plc_datersp,sizeof(PLC_DATERSP_FORM), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				if((m_plc_datecmd.header.sid != m_plc_datersp.header.sid) ||
				   (m_plc_datersp.resp1 != 0)				  ||
				   (m_plc_datersp.resp2 != 0)				  ||
				   (nrlen != (int)(sizeof(PLC_HEADER)+4))){

					PLC_SetUDPStatus(UDP_RecvDataError);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else{
				errcode=WSAGetLastError();
				PLC_SetUDPStatus(UDP_RecvDataError);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);	
			}
		}
		else
			PLC_SetUDPStatus(UDP_RecvTimeOut);
	}
//	Sleep(20);
	LeaveCriticalSection(&m_FinsPort);

	if(ret == SOCKET_ERROR || ret == 0){
		if(ret == SOCKET_ERROR)
			PLC_SetUDPStatus(UDP_SocketError);
		errcode=WSAGetLastError();
		return(FALSE);
	}
	PLC_SetUDPStatus(UDP_Running);
	return(TRUE);
}

/*
**----------------------------------------------------------------**
**
**  BOOL PLC_GetUDPStatus(UDP_STATUS *udp_status)
**
**		処理 : PLC 通信状態取得処理
**
**		入力 : UDP_STATUS *udp_status
**
**		出力 : なし
**
**----------------------------------------------------------------**
*/
void CFinsSock::PLC_GetUDPStatus(UDP_STATUS *udp_status){
	EnterCriticalSection(&m_StatusUpdate);
	*udp_status = UdpStatus;
	LeaveCriticalSection(&m_StatusUpdate);
}
/*
**----------------------------------------------------------------**
**
**  BOOL PLC_SetUDPStatus(UDP_STATUS udp_status)
**
**		処理 : PLC 通信状態更新
**
**		入力 : UDP_STATUS udp_status
**
**		出力 : なし
**
**----------------------------------------------------------------**
*/
void CFinsSock::PLC_SetUDPStatus(UDP_STATUS udp_status){
	
	EnterCriticalSection(&m_StatusUpdate);
	if(UdpStatus != udp_status){

		UdpStatus = udp_status;
	
		CIODB *pIODB = ((CWaveEndPointApp *)AfxGetApp())->GetIODB();
		int hdl = pIODB->IOPointGetHandle("CTC.PLCStatus");
		UINT	uint_val = (UINT)UdpStatus;
		pIODB->IOPointWrite(hdl,&uint_val);

	}
	LeaveCriticalSection(&m_StatusUpdate);
}

char CFinsSock::Get1ByteBCD(int iVal)
{
	unsigned uint_val,uData;
	unsigned dec,div;
	int		i;

	uData = 0;
	uint_val = (unsigned) iVal;
	for(dec = 10,div=0x10,i=0;i<2;i++,dec/=10,div >>=4){
		uData  += (uint_val / dec)*div;
		uint_val %= dec;
	}/* for */
	return((char)uData);
}
int CFinsSock::fins_select(int nfds,fd_set* readfds,fd_set* writefds,fd_set* exceptfds,const struct timeval* timeout)
{
	int	rv;
	int	i;
	fd_set				rfds;
	fd_set				wfds;

	for(i=0;i<30;i++){
		if(readfds != NULL){
			rfds = *readfds;
			rv = select(nfds,&rfds,NULL,exceptfds,timeout);
		}
		else if(writefds != NULL){
			wfds = *writefds;
			rv = select(nfds,NULL,&wfds,exceptfds,timeout);
		}
		else
			return(0);

		if(rv != SOCKET_ERROR && rv != 0){
////			if(readfds != NULL)
////				m_iErrorFlag = FALSE;
			return(rv);
		}
		if(rv == SOCKET_ERROR){
			break;
		}
		Sleep(20);
	}/* for */
#if 0
	if(m_iErrorFlag == FALSE){
		m_iNotCommStartTime = CTime::GetCurrentTime();
		m_iErrorFlag = TRUE;
	}
	else if(m_iErrorFlag == TRUE){
		m_iNotCommEndTime = CTime::GetCurrentTime();
		CTimeSpan ts = m_iNotCommEndTime - m_iNotCommStartTime;
			
		if(ts.GetTotalSeconds() >= 60L){
			m_iErrorFlag = FALSE;
			if(((CEagle_XPApp *)AfxGetApp())->CheckAlarmExist(916) == FALSE){
				((CEagle_XPApp *)AfxGetApp())->SetPCAlarm(916);
			}
		}
	}
#endif
	Sleep(1000);
	return(rv);
}