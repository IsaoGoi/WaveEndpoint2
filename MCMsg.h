/*
**------------------------------------------------**
**	  MC プロトコルメッセージデータ定義
**------------------------------------------------**
*/


#if !defined(MCMSG_H)
#define MCMSG_H

#define		MC_PCNO				0xff
#define		MC_TIMER			0x00012
// #define		MC_D_REG			0x4420

/*
** 1.MC トランザクション状態定義
*/
typedef enum{
	MC_MSG_IDLE = 0,				// Message Idle
	MC_MSG_WAITREPLY ,				// Wait Reply Message
	MC_MSG_COMPLETE ,				// Complete
	MC_MSG_ERROR,					// Error End
	MC_MSG_TIMEOUT,					// 応答待ちタイムアウト
}MC_MSGSTATUS;


/*
** 2.MC 伝文ヘッダー定義
*/
typedef struct {
	unsigned short	usSubHeader;	// サブヘッダ
	unsigned char	uchNetworkNo;	// ネットワーク番号(o)
	unsigned char	uchPCNo;		// PC No.(o)
	unsigned short	usUnitIO;		// ユニットIO番号(o)
	unsigned char	uchUnitSt;		// ユニット局番号(x)
	unsigned short	usDataLehgth;	// データ長
}MC_HEADER;
typedef struct {
	unsigned short	usCommand;		// コマンド
	unsigned short	usSubCommand;	// サブコマンド
	unsigned char	uchDevice;		// デバイス
	unsigned short	usDeviceAddr;	// デバイスｱﾄﾞﾚｽ
	unsigned short	usDeviceLength;	// デバイスｱﾄﾞﾚｽ
}MC_COMMAND;


/*
** 3.MC 送信伝文Body定義
*/
typedef struct {
	MC_HEADER		MC_Header;
	unsigned short	usTimer;		// 監視タイマ
	MC_COMMAND		MC_Command;
	unsigned short	usSendData[2048+32];	// 送信データ
}MC_SEND_BUFFER;

/*
** 4.MC 受信伝文Body定義
*/

typedef struct {
	MC_HEADER		MC_Header;
	unsigned short	usResult;			// 終了コード
	unsigned short	usRecvData[2048+32];	// 送信データ
}MC_RECV_BUFFER;

typedef enum {
	/* 0 */MC_NOT_CONECT,
	/* 1 */MC_CONNECTED,
	/* 2 */MC_SENDERR,
	/* 3 */MC_RECVERR,
}MELSEC_STATUS;

#endif