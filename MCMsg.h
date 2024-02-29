/*
**------------------------------------------------**
**	  MC �v���g�R�����b�Z�[�W�f�[�^��`
**------------------------------------------------**
*/


#if !defined(MCMSG_H)
#define MCMSG_H

#define		MC_PCNO				0xff
#define		MC_TIMER			0x00012
// #define		MC_D_REG			0x4420

/*
** 1.MC �g�����U�N�V������Ԓ�`
*/
typedef enum{
	MC_MSG_IDLE = 0,				// Message Idle
	MC_MSG_WAITREPLY ,				// Wait Reply Message
	MC_MSG_COMPLETE ,				// Complete
	MC_MSG_ERROR,					// Error End
	MC_MSG_TIMEOUT,					// �����҂��^�C���A�E�g
}MC_MSGSTATUS;


/*
** 2.MC �`���w�b�_�[��`
*/
typedef struct {
	unsigned short	usSubHeader;	// �T�u�w�b�_
	unsigned char	uchNetworkNo;	// �l�b�g���[�N�ԍ�(o)
	unsigned char	uchPCNo;		// PC No.(o)
	unsigned short	usUnitIO;		// ���j�b�gIO�ԍ�(o)
	unsigned char	uchUnitSt;		// ���j�b�g�ǔԍ�(x)
	unsigned short	usDataLehgth;	// �f�[�^��
}MC_HEADER;
typedef struct {
	unsigned short	usCommand;		// �R�}���h
	unsigned short	usSubCommand;	// �T�u�R�}���h
	unsigned char	uchDevice;		// �f�o�C�X
	unsigned short	usDeviceAddr;	// �f�o�C�X���ڽ
	unsigned short	usDeviceLength;	// �f�o�C�X���ڽ
}MC_COMMAND;


/*
** 3.MC ���M�`��Body��`
*/
typedef struct {
	MC_HEADER		MC_Header;
	unsigned short	usTimer;		// �Ď��^�C�}
	MC_COMMAND		MC_Command;
	unsigned short	usSendData[2048+32];	// ���M�f�[�^
}MC_SEND_BUFFER;

/*
** 4.MC ��M�`��Body��`
*/

typedef struct {
	MC_HEADER		MC_Header;
	unsigned short	usResult;			// �I���R�[�h
	unsigned short	usRecvData[2048+32];	// ���M�f�[�^
}MC_RECV_BUFFER;

typedef enum {
	/* 0 */MC_NOT_CONECT,
	/* 1 */MC_CONNECTED,
	/* 2 */MC_SENDERR,
	/* 3 */MC_RECVERR,
}MELSEC_STATUS;

#endif