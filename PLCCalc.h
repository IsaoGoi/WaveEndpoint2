/*
**
** OMRON 受信データ演算処理
**
*/

extern void DataCharToUINT(char *p,UINT *data);
extern void DataCharToUINT2(char *p,UINT *data);
extern void DataUINTToBin(UINT *data,char *p,int nBlock);
extern void DataUINTToChar(UINT *data,char *p,int nBlock);
extern void DataCharToDate(unsigned char *p,char *data);
extern void DataDWORDToChar(UINT *data,char *p,int nBlock);
extern void DataBCDToBin(UINT uBCD,UINT *data);
extern void DataBinToBCD(UINT uBin,UINT *bcd);

