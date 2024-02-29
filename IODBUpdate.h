#pragma once

#include "ExtThread.h"



class CIODBUpdate  : public CExtThread 
{
public:
	CIODBUpdate(CIODB *pIODB);
	virtual ~CIODBUpdate();
	virtual	void	StartWork();		/* Start Up(Over ride)*/
	virtual	void	DoWork();			/* Ω⁄Øƒﬁèàóù(Over ride)*/
	virtual	void	EndWork();			/* å„èàóù  (Over ride)*/

	void	IonGaugeCalculate(float fVal,unsigned short *usVal);
private:
	CIODB *m_pIODB;
	unsigned short *m_pReadBuffTop;
	unsigned short *m_pReadCIOTop;
	unsigned short *m_pMelsecBuffTop;
	unsigned short *m_pMelsecBuffTop2;
	unsigned short* m_pKeyenceBuffTop;
	CEPDConfig *m_pConfig;
	int PLCStatIOHdl;

};
