#pragma once

#define		USER_FILE_NAME		"UserDef.Dat"
#define		RCPPASS_FILE_NAME	"rcpdef.Dat"
#define		AUTOLOGOFF_FILE_NAME	"AutoLogoff.Dat"

#define		USER_MAX	100

#define		USER_NAME_LEN	20
#define		USER_ID_LEN		8
#define		PASSWORD_LEN	8
#define		DIVISION_LEN	8

#define USER_SUCCESS				0
#define USER_ERR_USERNAME			1
#define USER_ERR_PASSWORD			2
#define USER_ERR_DUPLICATE			3
#define USER_ERR_USERID				4
#define USER_ERR_USERID_DUPLICATE	5


#define SUPER_USER_ID		101

typedef enum{
	NONELEVEL = 0,
	OPERATOR,
	ENGINEER,
	SUPER,
}ACCESSLEVEL;



typedef struct{
	BOOL		bEnable;
	char		szUserName[USER_NAME_LEN+1];
	char		szPassword[PASSWORD_LEN+1];
	ACCESSLEVEL	AccessLevel;
	int			iReserve[32+1];
}USER_TBL;



class CAccessLevel
{
// コンストラクション
public:
	CAccessLevel();
	~CAccessLevel();

	void UserInit();
	int  EditUser(int iUserNo,char *pszUserName,char *pszPassword,ACCESSLEVEL AccessLevel);
	void  GetUser(int iUserNo,char *pszUserName,char *pszPassword,ACCESSLEVEL *AccessLevel);
	BOOL DeleteUser(int iUserNo);
	int  InportUser(char *pszUserName,char *pszPassword,ACCESSLEVEL AccessLevel);


	int LogOn(char *pszUserID,char *pszPassword);
	void LogOff(int iAuto);
	void GetCurrUser(char *pszUserName);
	ACCESSLEVEL GetCurrLevel() {return(m_AccessLevel);}
	ACCESSLEVEL GetUserAccessLevel(char *pszUserName);

	int			m_iAutoLogOffTime;
	int			m_iCurrUserNo;
	ACCESSLEVEL	m_AccessLevel;

private:
	USER_TBL	User_Tbl[USER_MAX];


};
