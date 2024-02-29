// FinsSock.cpp: CFinsSock クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "WaveEndPoint.h"
#include "KeyenceSock.h"
#include "afxmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	KEYENCE_MY_PORT_NO	20000


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CKeyenceSock::CKeyenceSock()
{
	MC_COMM_PARAM		tCFG_PLCComm;
	int		i;
	BYTE	bIPAddr[4 + 1];
	char* p1, * p2;
	CEPDConfig* pConfig;


	pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	tCFG_PLCComm = pConfig->m_tKeyenceComm;

	sprintf(m_PLC_IPAddr,"%d.%d.%d.%d", tCFG_PLCComm.PLCIPAddr.iIPAddr1, tCFG_PLCComm.PLCIPAddr.iIPAddr2, tCFG_PLCComm.PLCIPAddr.iIPAddr3, tCFG_PLCComm.PLCIPAddr.iIPAddr4);
	sprintf(m_PC_IPAddr, "%d.%d.%d.%d", tCFG_PLCComm.MyIPAddr.iIPAddr1, tCFG_PLCComm.MyIPAddr.iIPAddr2, tCFG_PLCComm.MyIPAddr.iIPAddr3, tCFG_PLCComm.MyIPAddr.iIPAddr4);
	Port_No = tCFG_PLCComm.iPortNo;


	/*--- Fins Read/Write ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	InitializeCriticalSection(&m_FinsPort);

	/*--- Fins Status Set/Get ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	InitializeCriticalSection(&m_StatusUpdate);

	cl_sock = INVALID_SOCKET;
	UdpStatus= MC_NOT_CONECT;

}

CKeyenceSock::~CKeyenceSock()
{

	/*--- Fins Read/Write ｸﾘﾁｶﾙｾｸｼｮﾝ ---*/
	DeleteCriticalSection(&m_FinsPort);

	DeleteCriticalSection(&m_StatusUpdate);
}
BOOL CKeyenceSock::MelsecInitial()
{
	char	wkBuf[16];
	char	szIOName[40];
	int		rv;

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	m_ioCommStatus = m_pIODB->IOPointGetHandle("CTC.KeyenceStatus");


	cl_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (cl_sock == INVALID_SOCKET)
		return FALSE;


	memset(&cl_addr, 0, sizeof(cl_addr));
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_port   = (unsigned short)(htons(KEYENCE_MY_PORT_NO));

	cl_addr.sin_addr.s_addr = inet_addr(m_PC_IPAddr);
	
	if (bind(cl_sock, (LPSOCKADDR)&cl_addr, sizeof(cl_addr)) == SOCKET_ERROR) {
		rv = GetLastError();
		return(FALSE);
	}
	memset(&sv_addr, 0, sizeof(sv_addr));
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port   = (unsigned short)(htons(Port_No));
	sv_addr.sin_addr.s_addr = inet_addr(m_PLC_IPAddr);


	/*--- 再接続要求待ち ---*/
	PLC_SetUDPStatus(MC_CONNECTED);


	return TRUE;
}
void CKeyenceSock::MelsecEnd()
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
BOOL CKeyenceSock::PLC_ReadCommand(int iDevType,UINT ReadAddr,int Readlen,LPSTR szRecvBuff,int *RecvLen){
	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry = 0;
	int					nrlen;
	int					iRecvMsgTotalSize;
	unsigned char		uchSendBuff[2048];	// for debug

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
	select_time.tv_usec = 10000;

	/*
	** Messageヘッダ作成
	*/
	m_SendBuff.MC_Header.usSubHeader	= 0x5000;	// サブヘッダ
	m_SendBuff.MC_Header.uchNetworkNo	= 0x00;	// ネットワーク番号(o)
	m_SendBuff.MC_Header.uchPCNo		= 0xFF;		// PC No.(o)
	m_SendBuff.MC_Header.usUnitIO		= 0xFF03;	// ユニットIO番号(o)
	m_SendBuff.MC_Header.uchUnitSt		= 0x00;		// ユニット局番号(x)
	m_SendBuff.usTimer					= MC_TIMER;
//	if(nCommand == MC_WRITE_CMD)
//		m_SendBuff.MC_Header.usDataLehgth = 2 + 10 + (nMsgSize*2);
//	else
	m_SendBuff.MC_Header.usDataLehgth = 2 + 10;

	m_SendBuff.MC_Command.usCommand = (unsigned short)MC_READ_CMD;
	m_SendBuff.MC_Command.usSubCommand = (unsigned short)MC_SUBCMD_WORD;
	m_SendBuff.MC_Command.uchDevice = (unsigned char)iDevType;
	m_SendBuff.MC_Command.usDeviceAddr = (unsigned short)ReadAddr;
	m_SendBuff.MC_Command.usDeviceLength = Readlen;

	iRecvMsgTotalSize = Readlen*2;

	uchSendBuff[0] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader >> 8) & 0xff;
	uchSendBuff[1] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader & 0xff);
	uchSendBuff[2] = m_SendBuff.MC_Header.uchNetworkNo;
	uchSendBuff[3] = m_SendBuff.MC_Header.uchPCNo;
	uchSendBuff[4] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO >> 8) & 0xff;
	uchSendBuff[5] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO & 0xff);
	uchSendBuff[6] = m_SendBuff.MC_Header.uchUnitSt;
	*((unsigned short *)(uchSendBuff+7)) = m_SendBuff.MC_Header.usDataLehgth;

	*((unsigned short *)(uchSendBuff+9)) = m_SendBuff.usTimer;
	*((unsigned short *)(uchSendBuff+11)) = m_SendBuff.MC_Command.usCommand;
	*((unsigned short *)(uchSendBuff+13)) = m_SendBuff.MC_Command.usSubCommand;
//	uchSendBuff[15] = m_SendBuff.MC_Command.usDeviceAddr & 0xFF;
//	uchSendBuff[16] = (m_SendBuff.MC_Command.usDeviceAddr >> 8) & 0xFF;
//	uchSendBuff[17] = 0;
	uchSendBuff[15] = ReadAddr & 0xFF;
	uchSendBuff[16] = (ReadAddr >> 8) & 0xFF;
	uchSendBuff[17] = (ReadAddr >> 16) & 0xFF;;
	uchSendBuff[18] = m_SendBuff.MC_Command.uchDevice;

	*((unsigned short *)(uchSendBuff+19)) = m_SendBuff.MC_Command.usDeviceLength;

	iRetry = 0;
READ_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=melsock_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)uchSendBuff,(m_SendBuff.MC_Header.usDataLehgth)+9,0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (m_SendBuff.MC_Header.usDataLehgth)+9){

			errcode=WSAGetLastError();

			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=melsock_select(0,&readfds,NULL,NULL,&select_time);
//		nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);

//		if(ret==0 && ++iRetry < 2 && nrlen <=0){
		if(ret==0 && ++iRetry < 2){
			goto READ_RETRY;
		}
		if(ret == SOCKET_ERROR || ret == 0){
			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

//		if((ret != SOCKET_ERROR && ret != 0) || nrlen > 0){
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)m_ucRecvData,sizeof(m_ucRecvBuff), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				m_RecvBuff.MC_Header.usSubHeader = m_ucRecvData[0];
				m_RecvBuff.MC_Header.usSubHeader <<= 8;
				m_RecvBuff.MC_Header.usSubHeader |= m_ucRecvData[1];

				m_RecvBuff.MC_Header.uchNetworkNo = m_ucRecvData[2];
				m_RecvBuff.MC_Header.uchPCNo      = m_ucRecvData[3];

				m_RecvBuff.MC_Header.usUnitIO = m_ucRecvData[4];
				m_RecvBuff.MC_Header.usUnitIO <<= 8;
				m_RecvBuff.MC_Header.usUnitIO |= m_ucRecvData[5];

				m_RecvBuff.MC_Header.uchUnitSt      = m_ucRecvData[6];

				m_RecvBuff.MC_Header.usDataLehgth = *((unsigned short *)&m_ucRecvData[7]);
				m_RecvBuff.usResult = *((unsigned short *)&m_ucRecvData[9]);

				// 正常終了
				if(m_RecvBuff.usResult==0){
					if(m_RecvBuff.MC_Header.usDataLehgth > 2){
						memcpy((char *)szRecvBuff,&m_ucRecvData[11],(m_RecvBuff.MC_Header.usDataLehgth-2));
					}
					*RecvLen = m_RecvBuff.MC_Header.usDataLehgth-2;
				}
				else{
					PLC_SetUDPStatus(MC_SENDERR);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
			else {
				PLC_SetUDPStatus(MC_SENDERR);
				LeaveCriticalSection(&m_FinsPort);
				return(FALSE);
			}
		}
		else{
			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}
	}
	else{
		errcode=WSAGetLastError();
			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
	}
	LeaveCriticalSection(&m_FinsPort);

	PLC_SetUDPStatus(MC_CONNECTED);
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
BOOL CKeyenceSock::PLC_WriteCommand(int iDevType,UINT WriteAddr,int Writelen,LPSTR szSendBuff)
{

	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry = 0;
	int					nrlen;
	unsigned char		uchSendBuff[2048];	// for debug

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
	select_time.tv_usec = 10000;

	/*
	** Messageヘッダ作成
	*/
	m_SendBuff.MC_Header.usSubHeader	= 0x5000;	// サブヘッダ
	m_SendBuff.MC_Header.uchNetworkNo	= 0x00;	// ネットワーク番号(o)
	m_SendBuff.MC_Header.uchPCNo		= 0xFF;		// PC No.(o)
	m_SendBuff.MC_Header.usUnitIO		= 0xFF03;	// ユニットIO番号(o)
	m_SendBuff.MC_Header.uchUnitSt		= 0x00;		// ユニット局番号(x)
	m_SendBuff.usTimer					= MC_TIMER;
//	if(nCommand == MC_WRITE_CMD)
//		m_SendBuff.MC_Header.usDataLehgth = 2 + 10 + (nMsgSize*2);
//	else
	m_SendBuff.MC_Header.usDataLehgth = 2 + 10 + (Writelen*2);

	m_SendBuff.MC_Command.usCommand = (unsigned short)MC_WRITE_CMD;
	m_SendBuff.MC_Command.usSubCommand = (unsigned short)MC_SUBCMD_WORD;
	m_SendBuff.MC_Command.uchDevice = (unsigned char)iDevType;
//	m_SendBuff.MC_Command.uchDevice = (unsigned char)MC_W_DEVCODE;
	m_SendBuff.MC_Command.usDeviceAddr = (unsigned short)WriteAddr;
	m_SendBuff.MC_Command.usDeviceLength = Writelen;

	uchSendBuff[0] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader >> 8) & 0xff;
	uchSendBuff[1] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader & 0xff);
	uchSendBuff[2] = m_SendBuff.MC_Header.uchNetworkNo;
	uchSendBuff[3] = m_SendBuff.MC_Header.uchPCNo;
	uchSendBuff[4] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO >> 8) & 0xff;
	uchSendBuff[5] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO & 0xff);
	uchSendBuff[6] = m_SendBuff.MC_Header.uchUnitSt;
	*((unsigned short *)(uchSendBuff+7)) = m_SendBuff.MC_Header.usDataLehgth;
	*((unsigned short *)(uchSendBuff+9)) = m_SendBuff.usTimer;

	*((unsigned short *)(uchSendBuff+11)) = m_SendBuff.MC_Command.usCommand;
	*((unsigned short *)(uchSendBuff+13)) = m_SendBuff.MC_Command.usSubCommand;
	uchSendBuff[15] = m_SendBuff.MC_Command.usDeviceAddr & 0xFF;
	uchSendBuff[16] = (m_SendBuff.MC_Command.usDeviceAddr >> 8) & 0xFF;
	uchSendBuff[17] = 0;
	uchSendBuff[18] = m_SendBuff.MC_Command.uchDevice;

	*((unsigned short *)(uchSendBuff+19)) = m_SendBuff.MC_Command.usDeviceLength;


	memcpy((char *)&uchSendBuff[21],(char *)szSendBuff,(Writelen*2));
//	for(i=0;i<Writelen;i++){
//		
//	}/* for */

	iRetry = 0;
READ_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=melsock_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)uchSendBuff,(m_SendBuff.MC_Header.usDataLehgth)+9,0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (m_SendBuff.MC_Header.usDataLehgth)+9){

			errcode=WSAGetLastError();

			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=melsock_select(0,&readfds,NULL,NULL,&select_time);
//		nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);

//		if(ret==0 && ++iRetry < 2 && nrlen <=0){
		if(ret==0 && ++iRetry < 2){
			goto READ_RETRY;
		}
		if(ret == SOCKET_ERROR || ret == 0){
			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

//		if((ret != SOCKET_ERROR && ret != 0) || nrlen > 0){
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)m_ucRecvData,sizeof(m_ucRecvBuff), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				m_RecvBuff.MC_Header.usSubHeader = m_ucRecvData[0];
				m_RecvBuff.MC_Header.usSubHeader <<= 8;
				m_RecvBuff.MC_Header.usSubHeader |= m_ucRecvData[1];

				m_RecvBuff.MC_Header.uchNetworkNo = m_ucRecvData[2];
				m_RecvBuff.MC_Header.uchPCNo      = m_ucRecvData[3];

				m_RecvBuff.MC_Header.usUnitIO = m_ucRecvData[4];
				m_RecvBuff.MC_Header.usUnitIO <<= 8;
				m_RecvBuff.MC_Header.usUnitIO |= m_ucRecvData[5];

				m_RecvBuff.MC_Header.uchUnitSt      = m_ucRecvData[6];

				m_RecvBuff.MC_Header.usDataLehgth = *((unsigned short *)&m_ucRecvData[7]);
				m_RecvBuff.usResult = *((unsigned short *)&m_ucRecvData[9]);

				// 正常終了
				if(m_RecvBuff.usResult!=0){
//					if(m_RecvBuff.MC_Header.usDataLehgth > 2){
//						memcpy((char *)szRecvBuff,&m_ucRecvData[11],(m_RecvBuff.MC_Header.usDataLehgth-2));
//					}
//					*RecvLen = m_RecvBuff.MC_Header.usDataLehgth-2;
//				}
//				else{
					PLC_SetUDPStatus(MC_RECVERR);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
		}
		else{
			PLC_SetUDPStatus(MC_RECVERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}
	}
	else{
		errcode=WSAGetLastError();
			PLC_SetUDPStatus(MC_RECVERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
	}
	LeaveCriticalSection(&m_FinsPort);

	PLC_SetUDPStatus(MC_CONNECTED);
	return(TRUE);
}
BOOL CKeyenceSock::PLC_BitWriteCommand(UINT WriteAddr,int iOnOff)
{
	int					errcode;
	int					ret;
	fd_set				readfds;
	fd_set				writefds;
	struct timeval		select_time;
	int					addrLen=sizeof(sv_addr);
	int					iRetry = 0;
	int					nrlen;
	unsigned char		uchSendBuff[2048];	// for debug

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
	select_time.tv_usec = 10000;

	/*
	** Messageヘッダ作成
	*/
	m_SendBuff.MC_Header.usSubHeader	= 0x5000;	// サブヘッダ
	m_SendBuff.MC_Header.uchNetworkNo	= 0x00;	// ネットワーク番号(o)
	m_SendBuff.MC_Header.uchPCNo		= 0xFF;		// PC No.(o)
	m_SendBuff.MC_Header.usUnitIO		= 0xFF03;	// ユニットIO番号(o)
	m_SendBuff.MC_Header.uchUnitSt		= 0x00;		// ユニット局番号(x)
	m_SendBuff.usTimer					= MC_TIMER;
//	if(nCommand == MC_WRITE_CMD)
//		m_SendBuff.MC_Header.usDataLehgth = 2 + 10 + (nMsgSize*2);
//	else
	m_SendBuff.MC_Header.usDataLehgth = 2 + 10;

	m_SendBuff.MC_Command.usCommand = (unsigned short)MC_BIT_WRITE_CMD;
	m_SendBuff.MC_Command.usSubCommand = (unsigned short)MC_SUBCMD_BIT;
	m_SendBuff.MC_Command.uchDevice = (unsigned char)MC_B_DEVCODE;
	m_SendBuff.MC_Command.usDeviceAddr = (unsigned short)WriteAddr;
	m_SendBuff.MC_Command.usDeviceLength = 1;

	uchSendBuff[0] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader >> 8) & 0xff;
	uchSendBuff[1] = (unsigned char)(m_SendBuff.MC_Header.usSubHeader & 0xff);
	uchSendBuff[2] = m_SendBuff.MC_Header.uchNetworkNo;
	uchSendBuff[3] = m_SendBuff.MC_Header.uchPCNo;
	uchSendBuff[4] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO >> 8) & 0xff;
	uchSendBuff[5] = (unsigned char)(m_SendBuff.MC_Header.usUnitIO & 0xff);
	uchSendBuff[6] = m_SendBuff.MC_Header.uchUnitSt;
	*((unsigned short *)(uchSendBuff+7)) = m_SendBuff.MC_Header.usDataLehgth;
	*((unsigned short *)(uchSendBuff+9)) = m_SendBuff.usTimer;

	*((unsigned short *)(uchSendBuff+11)) = m_SendBuff.MC_Command.usCommand;
	*((unsigned short *)(uchSendBuff+13)) = m_SendBuff.MC_Command.usSubCommand;
	uchSendBuff[15] = (unsigned char)m_SendBuff.MC_Command.usDeviceLength;
	uchSendBuff[16] = m_SendBuff.MC_Command.usDeviceAddr & 0xFF;
	uchSendBuff[17] = (m_SendBuff.MC_Command.usDeviceAddr >> 8) & 0xFF;
	uchSendBuff[18] = 0;
	uchSendBuff[19] = m_SendBuff.MC_Command.uchDevice;
	if(iOnOff)
		uchSendBuff[20] = 1;
	else
		uchSendBuff[20] = 0;

	iRetry = 0;
READ_RETRY:
	/*--- 送信可ﾁｪｯｸ ---*/
	ret=melsock_select(0,NULL,&writefds,NULL,&select_time);
	if(ret != SOCKET_ERROR && ret != 0){
		int nlen = sendto(cl_sock, (char *)uchSendBuff,(m_SendBuff.MC_Header.usDataLehgth)+9,0,(LPSOCKADDR)&sv_addr,sizeof(sv_addr));
		if(nlen != (m_SendBuff.MC_Header.usDataLehgth)+9){

			errcode=WSAGetLastError();

			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

		/*--- 受信可ﾁｪｯｸ ---*/
		ret=melsock_select(0,&readfds,NULL,NULL,&select_time);
//		nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);

//		if(ret==0 && ++iRetry < 2 && nrlen <=0){
		if(ret==0 && ++iRetry < 2){
			goto READ_RETRY;
		}
		if(ret == SOCKET_ERROR || ret == 0){
			PLC_SetUDPStatus(MC_SENDERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}

//		if((ret != SOCKET_ERROR && ret != 0) || nrlen > 0){
		if(ret != SOCKET_ERROR && ret != 0){
			nrlen = recvfrom(cl_sock,(char *)m_ucRecvBuff,sizeof(m_ucRecvBuff), MSG_PEEK,(LPSOCKADDR)&sv_addr, &addrLen);
			if(nrlen > 0 ){
				nrlen = recvfrom(cl_sock,(char *)m_ucRecvData,sizeof(m_ucRecvBuff), 0,(LPSOCKADDR)&sv_addr, &addrLen);
				/*--- 受信ﾃﾞｰﾀﾁｪｯｸ ---*/
				m_RecvBuff.MC_Header.usSubHeader = m_ucRecvData[0];
				m_RecvBuff.MC_Header.usSubHeader <<= 8;
				m_RecvBuff.MC_Header.usSubHeader |= m_ucRecvData[1];

				m_RecvBuff.MC_Header.uchNetworkNo = m_ucRecvData[2];
				m_RecvBuff.MC_Header.uchPCNo      = m_ucRecvData[3];

				m_RecvBuff.MC_Header.usUnitIO = m_ucRecvData[4];
				m_RecvBuff.MC_Header.usUnitIO <<= 8;
				m_RecvBuff.MC_Header.usUnitIO |= m_ucRecvData[5];

				m_RecvBuff.MC_Header.uchUnitSt      = m_ucRecvData[6];

				m_RecvBuff.MC_Header.usDataLehgth = *((unsigned short *)&m_ucRecvData[7]);
				m_RecvBuff.usResult = *((unsigned short *)&m_ucRecvData[9]);

				// 正常終了
				if(m_RecvBuff.usResult!=0){
//					if(m_RecvBuff.MC_Header.usDataLehgth > 2){
//						memcpy((char *)szRecvBuff,&m_ucRecvData[11],(m_RecvBuff.MC_Header.usDataLehgth-2));
//					}
//					*RecvLen = m_RecvBuff.MC_Header.usDataLehgth-2;
//				}
//				else{
					PLC_SetUDPStatus(MC_RECVERR);
					LeaveCriticalSection(&m_FinsPort);
					return(FALSE);	
				}
			}
		}
		else{
			PLC_SetUDPStatus(MC_RECVERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
		}
	}
	else{
		errcode=WSAGetLastError();
			PLC_SetUDPStatus(MC_RECVERR);
			LeaveCriticalSection(&m_FinsPort);
			return(FALSE);	
	}
	LeaveCriticalSection(&m_FinsPort);

	PLC_SetUDPStatus(MC_CONNECTED);
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
void CKeyenceSock::PLC_GetUDPStatus(MELSEC_STATUS *udp_status){
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
void CKeyenceSock::PLC_SetUDPStatus(MELSEC_STATUS udp_status){
	


	EnterCriticalSection(&m_StatusUpdate);
	if(UdpStatus != udp_status){

		UdpStatus = udp_status;
	
		UINT	uint_val = (UINT)UdpStatus;
		m_pIODB->IOPointWrite(m_ioCommStatus,&uint_val);

	}
	LeaveCriticalSection(&m_StatusUpdate);
}
int CKeyenceSock::melsock_select(int nfds,fd_set* readfds,fd_set* writefds,fd_set* exceptfds,const struct timeval* timeout)
{
	int	rv;
	int	i;
	fd_set				rfds;
	fd_set				wfds;

	for(i=0;i<100;i++){
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
			return(rv);
		}
		if(rv == SOCKET_ERROR){
			break;
		}
		Sleep(20);
	}/* for */
	Sleep(1000);
	return(rv);

}
