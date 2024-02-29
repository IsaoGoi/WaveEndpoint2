#pragma once

#define	INVALID_HDL		0

/*
** IODB Template
*/
typedef enum {
	IO_NONE,
	IO_CHAR,
	IO_UCHAR,
	IO_SHORT,
	IO_USHORT,
	IO_INT,
	IO_UINT,
	IO_LONG,
	IO_ULONG,
	IO_FLOAT,
	IO_DOUBLE,
	IO_DOUBLE_E,
	IO_ENUM,
	IO_STRING,
	IO_BIT,
	IO_MASK,
}IO_TYPE;

typedef struct{
	double	d_phy_low;
	double	d_phy_high;
	double	d_log_low;
	double	d_log_high;
}IO_SCALE;

typedef enum {
	SZ_TYPE_STRING,
	SZ_TYPE_USHORT_LH,
	SZ_TYPE_USHORT_HL,
}STRING_TYPE;



class  CIODBTemplate
{
public:
	CIODBTemplate();
	virtual	~CIODBTemplate();

protected:
	BOOL IOTemplateMax(char *p);
	BOOL IOTemplateCreate(char *p);
	
	HGLOBAL		m_hTempHdl;
	int			m_iTemplateMax;
	int			m_iTemplateCount;

	typedef struct{
		int			Handle;
		char		szTemplateName[32+1];		// テンプレート名
		IO_TYPE		io_type;					// データ型
		char		szUnit[16];					// 単位
		IO_SCALE	io_scale;					// スケール
		char		*pEnumString[40+1];			// Enum文字列
		int			iEnumMax;
		int			iStringSize;				// for String
		STRING_TYPE	sz_type;
		unsigned 	uintMask;					// 2005/09/02 Bit対応
		unsigned 	uintShift;					// 2005/09/02 Bit対応

		char		szDecimalFormat[32];		// Float/Double 小数部書式
	}IOTemplate;

	IOTemplate	*m_pTemplate;


public:
	BOOL LoadIOTemp(LPCSTR pszfilename);
	int GetIOTemplateHdl(LPCSTR pszName);
	IO_TYPE GetIOType(int Handle);
	unsigned GetBitMask(int Handle);
	unsigned GetShift(int Handle);
	BOOL GetIOScale(int Handle,IO_SCALE *pScale);
	BOOL SetIOScale(int Handle,	IO_SCALE *pScale);
	BOOL GetIOUnit(int Handle,	LPSTR pszunit);
	BOOL SetIOUnit(int Handle,	LPCSTR pszunit);
	BOOL GetIOEnumString(int Handle,char **pszEnumString);
	BOOL SetIOEnumString(int Handle,int item,char *pszEnumString);
	int  GetIOEnumMax(int Handle);
	int	 SetIOEnumMax(int Handle,int iEmnumMax);
	int CIODBTemplate::GetIOStrSize(int Handle);
	STRING_TYPE  GetIOStrType(int Handle);
	char* GetIODecimalFormat(int Handle);
};
