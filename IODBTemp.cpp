#include "pch.h"
#include "WaveEndPoint.h"
#include "IODBTemp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CIODBTemplate::CIODBTemplate()
{
	m_iTemplateMax = 0;
	m_iTemplateCount = 0;
}
CIODBTemplate::~CIODBTemplate()
{
	int	i,j;
	for(i=0;i<m_iTemplateCount;i++){
		if((m_pTemplate + i)->io_type == IO_ENUM || (m_pTemplate + i)->io_type == IO_BIT){
			for(j=0;j<40;j++){
				if((m_pTemplate + i)->pEnumString[j])
					free((m_pTemplate + i)->pEnumString[j]);
				else
					break;
			}/* for */
		}
	}/* for */
	if(m_hTempHdl){
		GlobalUnlock(m_hTempHdl);
		GlobalFree(m_hTempHdl);
	}
}
BOOL CIODBTemplate::LoadIOTemp(LPCSTR pszfilename)
{
	FILE	*fp;
	char	szBuff[768];
	BOOL	bRet=TRUE;
	errno_t	iErr;

	iErr = fopen_s(&fp,pszfilename,"rt");
	if(iErr){
		MessageBox(NULL,_T("Template File Open Error"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}
	while(fgets(szBuff,512,fp) != NULL){

		if(!(strncmp(szBuff,_T("TemplateMax"),11)))
			bRet = IOTemplateMax(szBuff);		
		else if(!(strncmp(szBuff,"IOTemplateCreate",16)))
			bRet = IOTemplateCreate(szBuff);		
		if(bRet == FALSE)
			break;
	}/* while */
	fclose(fp);
	return(TRUE);
}
int CIODBTemplate::GetIOTemplateHdl(LPCSTR pszName)
{
	int	IOHdl = INVALID_HDL;
	int	i;

	for(i=0;i<m_iTemplateCount;i++){
		if(!strcmp((m_pTemplate + i)->szTemplateName,pszName)){
			IOHdl = (m_pTemplate + i)->Handle;
			break;
		}
	}/* for */
	return(IOHdl);
}
IO_TYPE CIODBTemplate::GetIOType(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(IO_NONE);
	return((m_pTemplate + (Handle-1))->io_type);
}
unsigned CIODBTemplate::GetBitMask(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(IO_NONE);
	return((m_pTemplate + (Handle-1))->uintMask);
}
unsigned CIODBTemplate::GetShift(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(IO_NONE);
	return((m_pTemplate + (Handle-1))->uintShift);
}

BOOL CIODBTemplate::GetIOScale(int Handle,IO_SCALE *pScale)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);
	if((m_pTemplate + (Handle-1))->io_type == IO_ENUM)
		return(FALSE);
	*pScale = (m_pTemplate + (Handle-1))->io_scale;
	return(TRUE);
}
BOOL CIODBTemplate::SetIOScale(int Handle,IO_SCALE *pScale)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);
	if((m_pTemplate + (Handle-1))->io_type == IO_ENUM)
		return(FALSE);
	(m_pTemplate + (Handle-1))->io_scale = *pScale;
	return(TRUE);
}
BOOL CIODBTemplate::IOTemplateMax(char *pBuff)
{
	char	*p;
	char	*context;

	p = strtok_s(pBuff," ",&context);
	if(p){
		p = strtok_s(NULL," ",&context);
		if(p){
			m_iTemplateMax = atoi(p);
		}
	}
	if(m_iTemplateMax < 500)
		m_iTemplateMax = 500;

	m_hTempHdl = GlobalAlloc(GPTR,sizeof(IOTemplate)*m_iTemplateMax);
	if(m_hTempHdl == NULL){
		m_iTemplateMax = 0;
		MessageBox(NULL,_T("Template Memory Not Enough"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}
	m_pTemplate = (IOTemplate *)GlobalLock(m_hTempHdl);
	return(TRUE);
}
BOOL CIODBTemplate::IOTemplateCreate(char *pBuff)
{
	char	*p;
	char	*szEnum;
	CString	szMessage('\0',64);
	int		i,j;
	size_t	len;
	char	*context;

	if(m_iTemplateMax <= m_iTemplateCount){
		MessageBox(NULL,_T("Too many Template Create"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}
	p = strtok_s(pBuff," ",&context);
	if(!p){
		MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}	
	// Templalate Name取得
	p = strtok_s(NULL," ",&context);
	if(!p){
		MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}	
	strncpy((m_pTemplate + m_iTemplateCount)->szTemplateName,p,32);

	// データ型取得
	p = strtok_s(NULL," ",&context);
	if(!p){
		MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}	
	if(!strcmp(p,"Char"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_CHAR;
	else if(!strcmp(p,"Uchar"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_UCHAR;
	else if(!strcmp(p,"Short"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_SHORT;
	else if(!strcmp(p,"Ushort"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_USHORT;
	else if(!strcmp(p,"Integer"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_INT;
	else if(!strcmp(p,"Unsigned"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_UINT;
	else if(!strcmp(p,"Long"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_LONG;
	else if(!strcmp(p,"Ulong"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_ULONG;
	else if(!strcmp(p,"Float"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_FLOAT;
	else if(!strcmp(p,"Double"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_DOUBLE;
	else if (!strcmp(p, "DoubleE"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_DOUBLE_E;
	else if(!strcmp(p,"Enum"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_ENUM;
	else if(!strcmp(p,"String"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_STRING;
	else if(!strcmp(p,"Bit"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_BIT;
	else if(!strcmp(p,"Mask"))
		(m_pTemplate + m_iTemplateCount)->io_type = IO_MASK;
	else{
		szMessage.Format("Unknown Template Data Type <%s>",p);
		MessageBox(NULL,szMessage,_T("IOTemplate"),MB_OK);
		return(FALSE);
	}

	// 単位文字列取得
	p = strtok_s(NULL," ",&context);
	if(!p){
		MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
		return(FALSE);
	}
	if(!strcmp(p,"None"))
		(m_pTemplate + m_iTemplateCount)->szUnit[0] = '\0';
	else
		strncpy((m_pTemplate + m_iTemplateCount)->szUnit,p,16);

	// スケール/Enum文字列取得
	if((m_pTemplate + m_iTemplateCount)->io_type == IO_STRING){
		p = strtok_s(NULL," {",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		(m_pTemplate + m_iTemplateCount)->iStringSize = atoi(p);
		if((m_pTemplate + m_iTemplateCount)->iStringSize < 0)
			(m_pTemplate + m_iTemplateCount)->iStringSize = 8;
		else if((m_pTemplate + m_iTemplateCount)->iStringSize > 128)
			(m_pTemplate + m_iTemplateCount)->iStringSize = 128;
		

		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		if(!strncmp(p,"Ushort_LH",9))
			(m_pTemplate + m_iTemplateCount)->sz_type = SZ_TYPE_USHORT_LH;
		else if(!strncmp(p,"Ushort_HL",9))
			(m_pTemplate + m_iTemplateCount)->sz_type = SZ_TYPE_USHORT_HL;
		else
			(m_pTemplate + m_iTemplateCount)->sz_type = SZ_TYPE_STRING;

	}
	// 2005/09/02 Bit 対応
	else if((m_pTemplate + m_iTemplateCount)->io_type == IO_BIT){
		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		i=atoi(p);
		(m_pTemplate + m_iTemplateCount)->uintMask = (unsigned)(0x0001 << i);

		(m_pTemplate + m_iTemplateCount)->iEnumMax = 0;
		for(i=0;i<3;i++)
			(m_pTemplate + m_iTemplateCount)->pEnumString[i] = NULL;
		if(context[0] != '{')
			p = strtok_s(NULL,"{",&context);
		for(i=0;i<2 && p;i++){
			while(p){
				p = strtok_s(NULL,"{\",}\n",&context);
				if(p){
					len = strlen(p);
					for(j=0;j<(int)len;j++){
						if(*(p+j)!=' ')
							break;
					}
					if(j <(int) len)
						break;
				}
			}
			if(p){
				szEnum = (char *)malloc(20);
				if(szEnum == NULL){
					MessageBox(NULL,_T("Template Bit String Alloc Error"),_T("IOTemplate"),MB_OK);
					return(FALSE);
				}
				strncpy(szEnum,p,17);
				(m_pTemplate + m_iTemplateCount)->pEnumString[i] = szEnum;
				(m_pTemplate + m_iTemplateCount)->iEnumMax++;
			}
		}/* for */
		(m_pTemplate + m_iTemplateCount)->pEnumString[i] = NULL;
	
	}
	// 2005/09/02 Bit 対応
	else if((m_pTemplate + m_iTemplateCount)->io_type == IO_MASK){
		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		sscanf(p,"%x",&((m_pTemplate + m_iTemplateCount)->uintMask));

		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		i=atoi(p);
		(m_pTemplate + m_iTemplateCount)->uintShift = i;

	
	}
	// スケール/Enum文字列取得
	else if((m_pTemplate + m_iTemplateCount)->io_type != IO_ENUM){
		p = strtok_s(NULL," {",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		(m_pTemplate + m_iTemplateCount)->io_scale.d_phy_low = atof(p);
		
		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		(m_pTemplate + m_iTemplateCount)->io_scale.d_phy_high = atof(p);

		p = strtok_s(NULL," ",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		(m_pTemplate + m_iTemplateCount)->io_scale.d_log_low = atof(p);

		p = strtok_s(NULL," }",&context);
		if(!p){
			MessageBox(NULL,_T("Template Format Error"),_T("IOTemplate"),MB_OK);
			return(FALSE);
		}
		(m_pTemplate + m_iTemplateCount)->io_scale.d_log_high = atof(p);

		/*
		** 書式
		*/
		p = strtok_s(NULL, " }\n", &context);
		if (p)
			strcpy((m_pTemplate + m_iTemplateCount)->szDecimalFormat, p);
		else {
			if ((m_pTemplate + m_iTemplateCount)->io_type == IO_FLOAT)
				strcpy((m_pTemplate + m_iTemplateCount)->szDecimalFormat, "%6.2f");
			else if ((m_pTemplate + m_iTemplateCount)->io_type == IO_DOUBLE)
				strcpy((m_pTemplate + m_iTemplateCount)->szDecimalFormat, "%7.3f");
		}
	}
	else{
		(m_pTemplate + m_iTemplateCount)->iEnumMax = 0;
		for(i=0;i<40;i++)
			(m_pTemplate + m_iTemplateCount)->pEnumString[i] = NULL;
		p = strtok_s(NULL,"{",&context);
		for(i=0;i<40 && p;i++){
			while(p){
				p = strtok_s(NULL,"{\",}\n",&context);
				if(p){
					len = strlen(p);
					for(j=0;j<(int)len;j++){
						if(*(p+j)!=' ')
							break;
					}
					if(j < (int)len)
						break;
				}
			}
			if(p){
				szEnum = (char *)malloc(33);
				if(szEnum == NULL){
					MessageBox(NULL,_T("Template Enum String Alloc Error"),_T("IOTemplate"),MB_OK);
					return(FALSE);
				}
				strncpy(szEnum,p,32);
				(m_pTemplate + m_iTemplateCount)->pEnumString[i] = szEnum;
				(m_pTemplate + m_iTemplateCount)->iEnumMax++;
			}
		}/* for */
		(m_pTemplate + m_iTemplateCount)->pEnumString[i] = NULL;
	}
	(m_pTemplate + m_iTemplateCount)->Handle = m_iTemplateCount+1;
	m_iTemplateCount++;
	return(TRUE);
}
BOOL CIODBTemplate::GetIOUnit(int Handle,	LPSTR pszunit)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);
	strncpy(pszunit,(m_pTemplate+(Handle-1))->szUnit,16);
	return(TRUE);
}
BOOL CIODBTemplate::SetIOUnit(int Handle,	LPCSTR pszunit)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);
	strncpy((m_pTemplate+(Handle-1))->szUnit,pszunit,16);
	return(TRUE);
}
BOOL CIODBTemplate::SetIOEnumString(int Handle,int item,char *pszEnumString)
{
	int i;

	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);

	if((m_pTemplate + (Handle-1))->iEnumMax < item)
		return(FALSE);

	strncpy((m_pTemplate+(Handle-1))->pEnumString[item],pszEnumString,16);

	return(TRUE);
}
BOOL CIODBTemplate::GetIOEnumString(int Handle,char **pszEnumString)
{
	int i;

	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);

	for(i=0;i<(m_pTemplate + (Handle-1))->iEnumMax;i++)
		pszEnumString[i] = (m_pTemplate+(Handle-1))->pEnumString[i];
	for(;i<40;i++)
		pszEnumString[i] = NULL;
	
	return(TRUE);
}
int  CIODBTemplate::GetIOEnumMax(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(FALSE);
	return((m_pTemplate+(Handle-1))->iEnumMax);
}
int  CIODBTemplate::SetIOEnumMax(int Handle,int iEmnumMax)
{
	if(iEmnumMax > 32)
		return(FALSE);
	(m_pTemplate+(Handle-1))->iEnumMax = iEmnumMax;
	return(TRUE);
}
int CIODBTemplate::GetIOStrSize(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(0);
	if((m_pTemplate + (Handle-1))->io_type != IO_STRING)
		return(0);
	return((m_pTemplate + (Handle-1))->iStringSize);
}
STRING_TYPE  CIODBTemplate::GetIOStrType(int Handle)
{
	if((m_pTemplate + (Handle-1))->Handle != Handle)
		return(SZ_TYPE_STRING);
	if((m_pTemplate + (Handle-1))->io_type != IO_STRING)
		return(SZ_TYPE_STRING);
	return((m_pTemplate + (Handle-1))->sz_type);
}
char* CIODBTemplate::GetIODecimalFormat(int Handle)
{
	if ((m_pTemplate + m_iTemplateCount)->io_type != IO_FLOAT && (m_pTemplate + m_iTemplateCount)->io_type == IO_DOUBLE)
		return(NULL);

	return((m_pTemplate + (Handle - 1))->szDecimalFormat);
}
