/*
** MC 通信パラメータフォーマット
*/



#define		MC_READ_CMD		0x401	// ワードデータ一括読み出し
#define		MC_WRITE_CMD	0x1401	// ワードデータ一括書き込み
#define		MC_BIT_WRITE_CMD 0x1402	// ビットデータランダム書き込み
#define		MC_SUBCMD_WORD	0x0000	// ワードデータ
#define		MC_SUBCMD_BIT	0x0001	// ビットデータ

#define		MC_W_DEVCODE		0xb4	// Wレジ デバイスコード
#define		MC_B_DEVCODE		0xa0	// Bレジ デバイスコード
#define		MC_D_DEVCODE		0xa8	// Dレジ デバイスコード
#define		MC_ZR_DEVCODE		0xb0	// ZRレジ デバイスコード

// Command Reply受信エラー
#define	MC_CMD_REPLYTIMEOUT		100
#define	MC_CMD_NODATA			200


