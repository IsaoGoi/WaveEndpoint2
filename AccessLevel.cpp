#include "pch.h"
#include "WaveEndPoint.h"
#include  "AccessLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


char	m_szUserName[32];



CAccessLevel::CAccessLevel()
{
	m_iCurrUserNo = -1;
	m_AccessLevel = NONELEVEL;

}
CAccessLevel::~CAccessLevel()
{
	FILE	*fp;
	int		rv;
	errno_t	iErr;

	iErr = fopen_s(&fp,AUTOLOGOFF_FILE_NAME,"wb");
	if(iErr==0){
		rv = (int)fwrite((char *)&m_iAutoLogOffTime,(int)sizeof(int),1,fp);
		if(!rv)
			m_iAutoLogOffTime = 0;
		fclose(fp);
	}

}
void CAccessLevel::UserInit()
{
	BOOL	bInitialize = FALSE;
	int		i,j;
	FILE	*fp;
	int		rv;
	errno_t	iErr;

	for(i=0;i<USER_MAX;i++)
		memset((char *)&User_Tbl[i],'\0',sizeof(USER_TBL));		// 2015/03/13


	iErr = fopen_s(&fp,USER_FILE_NAME,"rb");
	if(iErr==0){
		rv = (int)fread((char *)&User_Tbl,(int)sizeof(USER_TBL),USER_MAX,fp);
		if(!rv)
			bInitialize = TRUE;
		fclose(fp);

#if 0
		for(i=0;i<USER_MAX;i++){
			for(j=0;j<10;j++){
				if(User_Tbl[i].iScreenAccess[j] > 1 || User_Tbl[i].iScreenAccess[j] < 0)
					User_Tbl[i].iScreenAccess[j] = 0;
			}
		}
#endif
	}
	else
		bInitialize = TRUE;
	
	if(bInitialize){
		for(i=0;i<USER_MAX;i++){
			User_Tbl[i].bEnable = FALSE;
			User_Tbl[i].AccessLevel = NONELEVEL;
			memset(User_Tbl[i].szUserName,'\0',USER_NAME_LEN);
			memset(User_Tbl[i].szPassword,'\0',PASSWORD_LEN);
#if 0
			for(j=0;j<10;j++)
				User_Tbl[i].iScreenAccess[j] = FALSE;
#endif
			for(j=0;j<32;j++)	
				User_Tbl[i].iReserve[j] = FALSE;

		}/* for */
	}

	iErr = fopen_s(&fp,AUTOLOGOFF_FILE_NAME,"rb");
	if(iErr==0){
		rv = (int)fread((char *)&m_iAutoLogOffTime,(int)sizeof(int),1,fp);
		if(!rv)
			m_iAutoLogOffTime = 0;
		fclose(fp);
	}
	else
		m_iAutoLogOffTime = 0;


}

int CAccessLevel::EditUser(int iUserNo,char *pszUserName,char *pszPassword,ACCESSLEVEL AccessLevel)
{
	int	i;
	FILE	*fp;
	CString	szEditName('\0',32);
	CString	szChkName('\0',32);
	CString	szUserIDName('\0',32);
	errno_t	iErr;

	if(pszUserName[0] == '\0' || pszUserName[0] == ' ')
		return(USER_ERR_USERNAME);
	if(pszPassword[0] == '\0' || pszPassword[0] == ' ')
		return(USER_ERR_PASSWORD);

	// User–¼ d•¡–¼ƒ`ƒFƒbƒN
	szEditName = pszUserName;
	szEditName.TrimRight();
	for(i=0;i<USER_MAX;i++){
		if(i == iUserNo)
			continue;
		if(User_Tbl[i].bEnable == FALSE)
			continue;

		szChkName = User_Tbl[i].szUserName;
		szChkName.TrimRight();
		if(szEditName == szChkName)
			return(USER_ERR_DUPLICATE);
	}/* for */
	User_Tbl[iUserNo].bEnable = TRUE;
	User_Tbl[iUserNo].AccessLevel = AccessLevel;
	strncpy(User_Tbl[iUserNo].szUserName,(LPCSTR)szEditName,USER_NAME_LEN);
	szChkName = pszPassword;
	szChkName.TrimRight();
	strncpy(User_Tbl[iUserNo].szPassword,(LPCSTR)szChkName,PASSWORD_LEN);

	// User ID d•¡–¼ƒ`ƒFƒbƒN
	szEditName = pszUserName;
	szEditName.TrimRight();
	for(i=0;i<USER_MAX;i++){
		if(i == iUserNo)
			continue;
		if(User_Tbl[i].bEnable == FALSE)
			continue;

		szChkName = User_Tbl[i].szUserName;
		szChkName.TrimRight();
		if(szEditName == szChkName)
			return(USER_ERR_USERID_DUPLICATE);
	}/* for */

	User_Tbl[iUserNo].bEnable = TRUE;
	User_Tbl[iUserNo].AccessLevel = AccessLevel;

	szEditName = pszUserName;
	szEditName.TrimRight();
	strncpy(User_Tbl[iUserNo].szUserName,(LPCSTR)szEditName,USER_NAME_LEN);

	szChkName = pszPassword;
	szChkName.TrimRight();
	strncpy(User_Tbl[iUserNo].szPassword,(LPCSTR)szChkName,PASSWORD_LEN);


#if 0
	for(i=0;i<10;i++)
		User_Tbl[iUserNo].iScreenAccess[i] = *(bScreenAccess+i); 
#endif
	iErr = fopen_s(&fp,USER_FILE_NAME,"wb");
	if(iErr==0){
		fwrite((char *)&User_Tbl,sizeof(USER_TBL),USER_MAX,fp);
		fclose(fp);
	}
	else
		AfxMessageBox("User Information Save Error");
	return(0);
}
void CAccessLevel::GetUser(int iUserNo,char *pszUserName,char *pszPassword,ACCESSLEVEL *AccessLevel)
{
	int		i;

	
	if(User_Tbl[iUserNo].bEnable == FALSE){
		*AccessLevel = NONELEVEL;

		memset(pszUserName,'\0',USER_NAME_LEN);
		memset(pszPassword,'\0',PASSWORD_LEN);

	}
	else{
		strncpy(pszUserName,User_Tbl[iUserNo].szUserName,USER_NAME_LEN);
		strncpy(pszPassword,User_Tbl[iUserNo].szPassword,PASSWORD_LEN);
		pszUserName[USER_NAME_LEN]='\0';
		pszPassword[PASSWORD_LEN]='\0';

		*AccessLevel = User_Tbl[iUserNo].AccessLevel;
	}
}

BOOL CAccessLevel::DeleteUser(int iUserNo)
{
	FILE	*fp;
	errno_t	iErr;
	int		i;

	User_Tbl[iUserNo].bEnable = FALSE;
	User_Tbl[iUserNo].AccessLevel = NONELEVEL;
	memset(User_Tbl[iUserNo].szUserName,'\0',USER_NAME_LEN);
	memset(User_Tbl[iUserNo].szPassword,'\0',PASSWORD_LEN);

	iErr = fopen_s(&fp,USER_FILE_NAME,"wb");
	if(iErr==0){
		fwrite((char *)&User_Tbl,sizeof(USER_TBL),USER_MAX,fp);
		fclose(fp);
	}
	else{	
		AfxMessageBox("User Information Save Error");
		return(FALSE);
	}
	return(TRUE);
}
int CAccessLevel::LogOn(char *pszUserID,char *pszPassword)
{
	int		i;
	int		int_val;
	int		IOHdl;
	CIODB	*pIODB;

	pIODB = ((CWaveEndPointApp *)AfxGetApp())->GetIODB();
	IOHdl = pIODB->IOPointGetHandle("CTC.AccessLevel");

	CString	szCurrID('\0',20);
	CString	szChkName('\0',20);


	// Super User Check
	szCurrID = pszUserID;
	szCurrID.TrimRight();
	szChkName = pszPassword;
	szChkName.TrimRight();
	if(szCurrID == _T("Cosmo") && szChkName == _T("YAC")){
		m_iCurrUserNo = SUPER_USER_ID;
		m_AccessLevel = SUPER;

		int_val = (int)m_AccessLevel;
		pIODB->IOPointWrite(IOHdl,&int_val);
		
		return(USER_SUCCESS);
	}
	
	for(i=0;i<USER_MAX;i++){
		if(User_Tbl[i].bEnable == FALSE)
			continue;

		szChkName = User_Tbl[i].szUserName;
		szChkName.TrimRight();
		if(szCurrID == szChkName)
			break;
	}/* for */
	if(i==USER_MAX)
		return(USER_ERR_USERNAME);

	szCurrID = pszPassword;
	szCurrID.TrimRight();
	szChkName = User_Tbl[i].szPassword;
	szChkName.TrimRight();
	if(szCurrID != szChkName)
		return(USER_ERR_PASSWORD);
			
	m_iCurrUserNo = i;
	m_AccessLevel = User_Tbl[i].AccessLevel;

	int_val = (int)m_AccessLevel;
	pIODB->IOPointWrite(IOHdl,&int_val);

	strcpy(m_szUserName,pszUserID);

	((CWaveEndPointApp*)AfxGetApp())->AutoLogOff_Start();
	return(USER_SUCCESS);
}
void CAccessLevel::LogOff(int iAuto)
{
	int		int_val;
	int		IOHdl;
	CIODB	*pIODB;


	pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	IOHdl = pIODB->IOPointGetHandle("CTC.AccessLevel");
	
	GetCurrUser(m_szUserName);
	
	m_iCurrUserNo = -1;
	m_AccessLevel = NONELEVEL;

	int_val = (int)m_AccessLevel;
	pIODB->IOPointWrite(IOHdl,&int_val);

	((CWaveEndPointApp*)AfxGetApp())->AutoLogOff_Stop();

}
void CAccessLevel::GetCurrUser(char *pszUserName)
{
	if(m_iCurrUserNo == SUPER_USER_ID)
		strcpy(pszUserName,"SuperUser");
	
	else if(m_iCurrUserNo == -1){
		memset(pszUserName,' ',USER_NAME_LEN);
		pszUserName[USER_NAME_LEN] = '\0';
	}else
		strncpy(pszUserName,User_Tbl[m_iCurrUserNo].szUserName,USER_NAME_LEN);
}
ACCESSLEVEL CAccessLevel::GetUserAccessLevel(char *pszUserName)
{
	int	i;
	
	CString	szCurrName('\0',20);
	CString	szChkName('\0',20);

	szCurrName = pszUserName;
	szCurrName.TrimRight();

	if(m_iCurrUserNo == SUPER_USER_ID)
		return(SUPER);

	for(i=0;i<USER_MAX;i++){
		if(User_Tbl[i].bEnable == FALSE)
			continue;

		szChkName = User_Tbl[i].szUserName;
		szChkName.TrimRight();
		if(szCurrName == szChkName)
			break;
	}/* for */
	if(i==USER_MAX)
		return(NONELEVEL);
	return(User_Tbl[i].AccessLevel);
}
int CAccessLevel::InportUser(char *pszUserName,char *pszPassword,ACCESSLEVEL AccessLevel)
{
	int	i;
	FILE	*fp;
	CString	szEditName('\0',32);
	CString	szChkName('\0',32);
	CString	szUserIDName('\0',32);
	errno_t	iErr;
	int	iEmptyUserNo = -1;
	int	bUpdate = FALSE;

	for(i=0;i<USER_MAX;i++){
		if(User_Tbl[i].bEnable == FALSE){
			if(iEmptyUserNo == -1)
				iEmptyUserNo = i;
			continue;
		}

		/*
		** User ID‚Å“o˜^Ï‚ÝUser‚ð’T‚·
		*/
		szEditName = pszUserName;
		szEditName.TrimRight();

		szChkName = User_Tbl[i].szUserName;
		szChkName.TrimRight();
		if(szEditName == szChkName){

			User_Tbl[i].AccessLevel = AccessLevel;
			szChkName = pszPassword;
			szChkName.TrimRight();
			strncpy(User_Tbl[i].szPassword,(LPCSTR)szChkName,PASSWORD_LEN);
			bUpdate = TRUE;
			break;
		}
	}/* for */
	// V‹K“o˜^
	if(i==USER_MAX && iEmptyUserNo != -1){

		User_Tbl[iEmptyUserNo].bEnable = TRUE;
		User_Tbl[iEmptyUserNo].AccessLevel = AccessLevel;

		szChkName = pszUserName;
		szChkName.TrimRight();
		strncpy(User_Tbl[iEmptyUserNo].szUserName,(LPCSTR)szChkName,USER_ID_LEN);
		szChkName = pszPassword;
		szChkName.TrimRight();
		strncpy(User_Tbl[iEmptyUserNo].szPassword,(LPCSTR)szChkName,PASSWORD_LEN);

		bUpdate = TRUE;
	}


	if(bUpdate){
		iErr = fopen_s(&fp,USER_FILE_NAME,"wb");
		if(iErr==0){
			fwrite((char *)&User_Tbl,sizeof(USER_TBL),USER_MAX,fp);
			fclose(fp);
		}
	}
	return(0);
}
