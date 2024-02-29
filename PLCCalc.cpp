/*
**
** OMRON 受信データ演算処理
**
*/
#include "pch.h"

/*
**----------------------------------------------------------------**
**
**  void CPortCtrl::DataCharToUINT(char *p,UINT *data)
**
**		処理 : 2ﾊﾞｲﾄ charﾃﾞｰﾀをUINT型に変換する。
**
**		入力 :  char *p		... 変換元 Charﾃﾞｰﾀ
**				UINT *dtaa	... 変換先ﾃﾞｰﾀｱﾄﾞﾚｽ
**
**		出力 : TRUE  .... Lot 情報正常受信
**			   FALSE .... ﾃﾞｰﾀ未受信
**
**----------------------------------------------------------------**
*/
void DataCharToUINT(char *p,UINT *data){
	UINT	uint_val;
	int		i;
	UINT	div;
	UINT	dec;

	uint_val = (UINT)*((unsigned char *)p);
	uint_val <<= 8;
	uint_val |= (UINT)*((unsigned char *)(p+1));

	*data = 0;
	for(dec = 1000,div=0x1000,i=0;i<4;i++,dec/=10,div >>=4){
		*data  += (uint_val / div)*dec;
		uint_val %= div;
	}/* for */
}
/*
**----------------------------------------------------------------**
**
**  void CPortCtrl::DataCharToUINT(char *p,UINT *data)
**
**		処理 : 2ﾊﾞｲﾄ charﾃﾞｰﾀをUINT型に変換する。
**
**		入力 :  char *p		... 変換元 Charﾃﾞｰﾀ
**				UINT *dtaa	... 変換先ﾃﾞｰﾀｱﾄﾞﾚｽ
**
**		出力 : TRUE  .... Lot 情報正常受信
**			   FALSE .... ﾃﾞｰﾀ未受信
**
**----------------------------------------------------------------**
*/
void DataCharToUINT2(char *p,UINT *data){
	UINT	uint_val;
	int		i;
	UINT	div;
	UINT	dec;

	uint_val = (UINT)*((unsigned char *)p);
	uint_val <<= 8;
	uint_val |= (UINT)*((unsigned char *)(p+1));

	*data = 0;
	if(uint_val & 0x8000){
		*data = uint_val;
	}
	else{
		for(dec = 1000,div=0x1000,i=0;i<4;i++,dec/=10,div >>=4){
			*data  += (uint_val / div)*dec;
			uint_val %= div;
		}/* for */
	}
}
/*
**----------------------------------------------------------------**
**
**  void CPortCtrl::DataCharToUINT(char *p,UINT *data)
**
**		処理 : 2ﾊﾞｲﾄ charﾃﾞｰﾀをUINT型に変換する。
**
**		入力 :  char *p		... 変換元 Charﾃﾞｰﾀ
**				UINT *dtaa	... 変換先ﾃﾞｰﾀｱﾄﾞﾚｽ
**
**		出力 : TRUE  .... Lot 情報正常受信
**			   FALSE .... ﾃﾞｰﾀ未受信
**
**----------------------------------------------------------------**
*/
void DataUINTToChar(UINT *data,char *p,int nBlock){
	UINT	uint_val;
	UINT	uData;
	int		i,j;
	UINT	div;
	UINT	dec;
	unsigned char	*p2;

	p2 = (unsigned char *)p;
	for(i=0;i<nBlock;i++){
		uint_val = *(data+i);
		uData = 0;
		for(dec = 1000,div=0x1000,j=0;j<4;j++,dec/=10,div >>=4){
			uData  += (uint_val / dec)*div;
			uint_val %= dec;
		}/* for */
		*p2 = (unsigned char)((uData & 0xff00) >> 8);
		p2++;
		*p2 = (unsigned char)(uData & 0x00ff);
		p2++;
	}	
}
/*
**----------------------------------------------------------------**
**
**  void CPortCtrl::DataCharToUINT(char *p,UINT *data)
**
**		処理 : 2ﾊﾞｲﾄ charﾃﾞｰﾀをUINT型に変換する。
**
**		入力 :  char *p		... 変換元 Charﾃﾞｰﾀ
**				UINT *dtaa	... 変換先ﾃﾞｰﾀｱﾄﾞﾚｽ
**
**		出力 : TRUE  .... Lot 情報正常受信
**			   FALSE .... ﾃﾞｰﾀ未受信
**
**----------------------------------------------------------------**
*/
void DataUINTToBin(UINT *data,char *p,int nBlock){
	UINT	uint_val;
	UINT	uData;
	int		i,j;
	UINT	div;
	UINT	dec;
	unsigned char	*p2;

	p2 = (unsigned char *)p;
	for(i=0;i<nBlock;i++){
		uData = *(data+i);
		*p2 = (unsigned char)((uData & 0xff00) >> 8);
		p2++;
		*p2 = (unsigned char)(uData & 0x00ff);
		p2++;
	}	
}
/*
**----------------------------------------------------------------**
**
**  void CPortCtrl::DataCharToDate(char *p,char *data)
**
**		処理 : 2ﾊﾞｲﾄ charﾃﾞｰﾀを日付文字列に変換する。
**
**		入力 :  char *p		... 変換元 Charﾃﾞｰﾀ
**				UINT *data	... 変換先ﾃﾞｰﾀｱﾄﾞﾚｽ
**
**		出力 : なし
**
**----------------------------------------------------------------**
*/
void DataCharToDate(unsigned char *p,char *data){
	sprintf(data,"%02.02X%02.02X%02.02X%02.02X%02.02X%02.02X",
			*p,
			*(p+1),
			*(p+2),
			*(p+3),
			*(p+4),
			*(p+5));
}
void DataDWORDToChar(UINT *data,char *p,int nBlock){
	UINT	uint_val;
	UINT	uData;
	int		i,j;
	UINT	div;
	UINT	dec;
	unsigned char	*p2;

	p2 = (unsigned char *)p;
	for(i=0;i<nBlock;i++){
		uint_val = *(data+i);
		uData = 0;

		uint_val %= 10000;
		for(dec = 1000,div=0x1000,j=0;j<4;j++,dec/=10,div >>=4){
			uData  += (uint_val / dec)*div;
			uint_val %= dec;
		}/* for */
		*p2 = (unsigned char)((uData & 0xff00) >> 8);
		p2++;
		*p2 = (unsigned char)(uData & 0x00ff);
		p2++;

		uint_val = *(data+i);
		uData = 0;
		uint_val /= 10000;
		for(dec = 1000,div=0x1000,j=0;j<4;j++,dec/=10,div >>=4){
			uData  += (uint_val / dec)*div;
			uint_val %= dec;
		}/* for */
		*p2 = (unsigned char)((uData & 0xff00) >> 8);
		p2++;
		*p2 = (unsigned char)(uData & 0x00ff);
		p2++;
	}	
}
void DataBCDToBin(UINT uBCD,UINT *data){
	UINT	uint_val;
	int		i;
	UINT	div;
	UINT	dec;

	uint_val = uBCD;

	*data = 0;
	for(dec = 1000,div=0x1000,i=0;i<4;i++,dec/=10,div >>=4){
		*data  += (uint_val / div)*dec;
		uint_val %= div;
	}/* for */
}
void DataBinToBCD(UINT uBin,UINT *bcd){
	UINT	uint_val;
	int		i;
	UINT	div;
	UINT	dec;

	uint_val = uBin;

	*bcd = 0;
	for(dec = 0x1000,div=1000,i=0;i<4;i++,div/=10,dec >>=4){
		*bcd  += (uint_val / div)*dec;
		uint_val %= div;
	}/* for */
}

