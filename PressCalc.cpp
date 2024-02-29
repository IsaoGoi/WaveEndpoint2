/*==========================================================================
 *
 *  File:       PressCalc.cpp / PressCalc.h
 *  Content:    圧力値変換
 *
 ***************************************************************************/
#include "pch.h"
#include "WaveEndPoint.h"
#include "presscalc.h"

/*
 *  CalcPressValue
 *
 *  仮数/指数 -> 圧力値に換算.
 *
 */
// #define	UNIT_TORR	0
// #define	UNIT_PA		1



extern "C" void CalcPressValue(double dVal , int iExt ,double *pResult)
{
	if(dVal == (double)0.0)	
		*pResult = (double)0.0;
	else if(iExt < 0){
		for(;iExt!=0;iExt++)
			dVal /= 10;
		*pResult = dVal;
	}
	else if(iExt > 0){
		for(;iExt!=0;iExt--)
			dVal *= 10;
		*pResult = dVal;
	}
	else		*pResult = dVal;
}
/*
 *  CalcPressExt
 *
 *  圧力値 -> 仮数/指数 に換算.
 *
 */
extern "C" void CalcPressExt(double dValue, double *pResultVal ,int *pResultExt)
{
	if(dValue == (double)0.0){	
		*pResultVal = (double)0.0;
		*pResultExt = 0;
	}
	else if(dValue < 1.0){
		for(*pResultExt=0;dValue < 1.0;(*pResultExt)--)
			dValue *= 10;
		*pResultVal = dValue;
	}
	else if(dValue >= 10.0){
		for(*pResultExt=0;dValue >= 10.0;(*pResultExt)++)
			dValue /= 10;
		*pResultVal = dValue;
	}
	else{
		*pResultVal = dValue;
		*pResultExt = 0;
	}
}

typedef struct {
	float data_min;
	float data_max;
	float press_min;
	float press_max;
}PRESSCONVTBL;

static	int	m_iCalcTableMax=8;
static PRESSCONVTBL TMPressConvertTable_Pa[20]={
//    			   Data Min    Data Max        Min Pressure      Max Pressure
/*0: 7.5V - 8.5V */{4500.0f ,  5100.0f ,         10000 ,          100000    },
/*1: 6.5V - 7.5V */{3900.0f ,  4500.0f ,          1000 ,           10000    },
/*2: 5.5V - 6.5V */{3300.0f ,  3900.0f ,           100 ,            1000    },
/*3: 4.5V - 5.5V */{2700.0f ,  3300.0f ,            10 ,             100    },
/*4: 3.5V - 4.5V */{2100.0f ,  2700.0f ,             1 ,              10    },
/*5: 2.5V - 3.5V */{1500.0f ,  2100.0f ,            0.1f,              1    },
/*6: 2.2V - 2.5V */{1320.0f ,  1500.0f ,            0.05f ,            0.1f  },
/*7: 1.5V - 2.2V */{ 900.0f ,  1320.0f ,            0.01f ,            0.05f},
};

static PRESSCONVTBL TMPressConvertTable_Torr[20]={
//    			   Data Min    Data Max        Min Pressure      Max Pressure
/*0: 7.5V - 8.5V */{4500.0f ,  5100.0f ,             75 ,             750    },
/*1: 6.5V - 7.5V */{3900.0f ,  4500.0f ,           7.5f ,              75    },
/*2: 5.5V - 6.5V */{3300.0f ,  3900.0f ,          0.75f ,             7.5f    },
/*3: 4.5V - 5.5V */{2700.0f ,  3300.0f ,         0.075f ,            0.75f    },
/*4: 3.5V - 4.5V */{2100.0f ,  2700.0f ,        0.0075f ,           0.075f    },
/*5: 2.5V - 3.5V */{1500.0f ,  2100.0f ,       0.00075f ,          0.0075f    },
/*6: 2.2V - 2.5V */{1320.0f ,  1500.0f ,       0.00038f ,         0.00075f    },
/*7: 1.5V - 2.2V */{ 900.0f ,  1320.0f ,      0.000075f ,         0.00038f   },
};

extern "C" float ConvertTMPressure(float fPressure,int iPressureUnit)
{
	int		i;
	float	Present_Pressure;
	float 	f_max1,f_max2,f_min1,f_min2;

	Present_Pressure = fPressure;

	if(iPressureUnit == UNIT_TORR){
		/*
		** Convert Table Serch
		*/
		for(i=0;i<7;i++){
			if(TMPressConvertTable_Torr[i].data_min <= Present_Pressure)
				break;
		}
		f_max1 = TMPressConvertTable_Torr[i].data_max;
		f_min1 = TMPressConvertTable_Torr[i].data_min;
		f_max2 = TMPressConvertTable_Torr[i].press_max;
		f_min2 = TMPressConvertTable_Torr[i].press_min;
	}
	else{
		for(i=0;i<7;i++){
			if(TMPressConvertTable_Pa[i].data_min <= Present_Pressure)
				break;
		}
		f_max1 = TMPressConvertTable_Pa[i].data_max;
		f_min1 = TMPressConvertTable_Pa[i].data_min;
		f_max2 = TMPressConvertTable_Pa[i].press_max;
		f_min2 = TMPressConvertTable_Pa[i].press_min;
	}
	Present_Pressure = (Present_Pressure-f_min1);
	if(Present_Pressure != 0.00){
		Present_Pressure /= (f_max1-f_min1);
		Present_Pressure *= (f_max2-f_min2);
		Present_Pressure += f_min2;
	}
	else
		Present_Pressure = f_min2;
	return(Present_Pressure);
}
extern "C" float ConvertTMPressure_to_Volt(float fPressure,int iPressureType)
{
	int	i;
	float Present_Pressure;
	float 			f_max1,f_max2,f_min1,f_min2;

	Present_Pressure = fPressure;

	if(iPressureType== UNIT_TORR){
		/*
		** Convert Table Serch
		*/
		for(i=0;i<7;i++){
			if(TMPressConvertTable_Torr[i].press_min <= Present_Pressure)
				break;
		}
		f_max1 = TMPressConvertTable_Torr[i].data_max;
		f_min1 = TMPressConvertTable_Torr[i].data_min;
		f_max2 = TMPressConvertTable_Torr[i].press_max;
		f_min2 = TMPressConvertTable_Torr[i].press_min;
	}
	else{
		for(i=0;i<7;i++){
			if(TMPressConvertTable_Pa[i].press_min <= Present_Pressure)
				break;
		}
		f_max1 = TMPressConvertTable_Pa[i].data_max;
		f_min1 = TMPressConvertTable_Pa[i].data_min;
		f_max2 = TMPressConvertTable_Pa[i].press_max;
		f_min2 = TMPressConvertTable_Pa[i].press_min;
	}
	Present_Pressure = (Present_Pressure-f_min2);
	if(Present_Pressure != 0.00){
		Present_Pressure /= (f_max2-f_min2);
		Present_Pressure *= (f_max1-f_min1);
		Present_Pressure += f_min1;
	}
	else
		Present_Pressure = f_min1;
	return(Present_Pressure);
}


// Ion Gauge ABC -> C.B -A 形式
extern "C" void ConvertPM_IonG_to_Press(unsigned short usPressure,float *pfBase,int *piExp)
{
	unsigned short ushort_val;
	
	ushort_val = usPressure % 1000;

	/* 指数部算出(100の位) */
	*piExp = (int)(ushort_val / 100);
	if(*piExp)
		*piExp*= -1;

	ushort_val %= 100;

	/* 仮数部算出 */
	*pfBase =(float)(ushort_val / 10);
	*pfBase /=10.0;
	ushort_val %= 10;
	*pfBase += ((float)ushort_val);
}
// Ion Gauge C.B -A -> ABC 形式
extern "C" void ConvertPM_Press_to_IonG(float fBase,int iExp,unsigned short *pusPressure)
{
	unsigned short ushort_val;

	ushort_val = (unsigned short)(fBase * 10.0);
	*pusPressure = ushort_val % 10;
	*pusPressure *= 10;

	*pusPressure += (ushort_val / 10);
	if(iExp <= -1)
		iExp *= -1;
	else
		iExp = 0;
	*pusPressure += (unsigned short)(iExp * 100);
}
extern "C" void Set_TM_PressureTable()
{
#if 0
	int		i;
	double	dVolt;

	CFileParam *pParam = ((CEagle_XPApp *)AfxGetApp())->GetFileParam();
	
	m_iCalcTableMax = pParam->m_pParamTM->PressTable.iTableMax;


	dVolt = (double)(6000.0f / 10.0f);	// 1Voltの値

	for(i=0;i<m_iCalcTableMax;i++){
		TMPressConvertTable_Pa[i].data_max = (float)(dVolt *  pParam->m_pParamTM->PressTable.dAD_Value[i]);
		TMPressConvertTable_Pa[i].data_min = (float)(dVolt *  pParam->m_pParamTM->PressTable.dAD_Value[i+1]);
		TMPressConvertTable_Pa[i].press_max = (float)(pParam->m_pParamTM->PressTable.dPressure_Pa[i]);
		TMPressConvertTable_Pa[i].press_min = (float)(pParam->m_pParamTM->PressTable.dPressure_Pa[i+1]);

		TMPressConvertTable_Torr[i].data_max = (float)(dVolt *  pParam->m_pParamTM->PressTable.dAD_Value[i]);
		TMPressConvertTable_Torr[i].data_min = (float)(dVolt *  pParam->m_pParamTM->PressTable.dAD_Value[i+1]);
		TMPressConvertTable_Torr[i].press_max = (float)(pParam->m_pParamTM->PressTable.dPressure_Torr[i]);
		TMPressConvertTable_Torr[i].press_min = (float)(pParam->m_pParamTM->PressTable.dPressure_Torr[i+1]);
	}
#endif
};