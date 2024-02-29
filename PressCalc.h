/*==========================================================================
 *
 *  File:       PressCalc.cpp / PressCalc.h
 *  Content:    ˆ³—Í’l•ÏŠ·
 *
 ***************************************************************************/
//#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
//#endif	/* __cplusplus */
extern void		CalcPressValue(double dVal , int iExt ,double *pResult);
extern void		CalcPressExt(double dValue, double *pResultVal ,int *pResultExt);
extern "C" float ConvertTMPressure(float fPressure,int iPressureType);
extern "C" float ConvertTMPressure_to_Volt(float fPressure,int iPressureType);
extern "C" void ConvertPM_IonG_to_Press(unsigned short usPressure,float *pfBase,int *piExp);
extern "C" void ConvertPM_Press_to_IonG(float fBase,int iExp,unsigned short *pusPressure);
extern "C" void Set_TM_PressureTable();
//#ifdef __cplusplus
}
#define	UNIT_TORR	0
#define	UNIT_PA		1

//#endif	/* __cplusplus */
