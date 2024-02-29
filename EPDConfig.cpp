#include "pch.h"
#include "WaveEndPoint.h"


_TCHAR gpszEPDCfgFileName[16] = "EPDConfig.CFG";
_TCHAR gpszWaveCfgFileName[16] = "WaveConfig.CFG";
_TCHAR gpszPLCCfgFileName[16] = "OMRON.CFG";
_TCHAR gpszKeyenceCfgFileName[16] = "Keyence.CFG";
_TCHAR gpszEthCfgFileName[16] = "EthConfig.CFG";
_TCHAR gpszMyIPCfgFileName[16] = "MyIPConfig.CFG";


CEPDConfig::CEPDConfig()
{
}
CEPDConfig::~CEPDConfig()
{

}
/*
** 環境パラメータロード
*/
int CEPDConfig::ConfigFileLoad()
{
	CFile* fp = new CFile();
	int	rv = FALSE;
	int		bOpen;
	int		i;


	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(gpszEPDCfgFileName, CFile::modeRead);
	}
	CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

	//ファイルが存在しなければ初期値を
	//セットしてファイル作成
	if (bOpen == FALSE) {
		TRY
		{
			fp->Open(gpszEPDCfgFileName, CFile::modeCreate | CFile::modeWrite);
		}
		CATCH(CFileException, e)
		{
			delete fp;
			return(rv);
		}
		END_CATCH

		for (i = 0; i < PM_MAX; i++) {
			m_tEPDConfig.iPCEnable[i] = TRUE;
			m_tEPDConfig.unsEEPRomLoadParam[i] = 0;
		}
		m_tEPDConfig.iLogSaveCycle = 3;
		m_tEPDConfig.iWaveMax = 256;
		m_tEPDConfig.Driver= ETHERNET_DRV;

		TRY
		{
			fp->Write(&m_tEPDConfig,sizeof(EQ_CONFIG));
		}
		CATCH(CFileException, e)
		{
#ifdef _DEBUG
			afxDump << "File could not be write " << e->m_cause << "\n";
#endif
		}
		END_CATCH
			
		fp->Close();
		delete fp;
		return(rv);
	}
	// ファイルのロード
	TRY
	{
		fp->Read(&m_tEPDConfig,sizeof(EQ_CONFIG));
	rv = TRUE;
	}
	CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be read " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	fp->Close();
	delete fp;
	return(rv);
}
/*
** EPDパラメータ保存
*/
int CEPDConfig::ConfigFileSave()
{
	CFile* fp = new CFile();


	//装置環境ファイルのオープン
	TRY
	{
		fp->Open(gpszEPDCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

	TRY
	{
		fp->Write(&m_tEPDConfig,sizeof(EQ_CONFIG));
	}
	CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be write " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	fp->Close();
	delete fp;
	return(TRUE);
}
/*
** 環境パラメータロード
*/
int CEPDConfig::WaveCfgFileLoad()
{
	CFile* fp = new CFile();
	int	rv = FALSE;
	int		bOpen;
	int		i;
	int		iPM;

	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(gpszWaveCfgFileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
		if (bOpen == FALSE) {
			TRY
			{
				fp->Open(gpszWaveCfgFileName, CFile::modeCreate | CFile::modeWrite);
			}
				CATCH(CFileException, e)
			{
				delete fp;
				return(rv);
			}
			END_CATCH

				for (iPM = 0; iPM < PM_MAX; iPM++) {
					for (i = 0; i < WAVE_CNT_MAX; i++) {
						m_tWaveConfig2[iPM].fWaveLength[i] = 162.3038 + (float)(0.29645f * (float)(i + 1)) + (float)(0.00002513721082f * (float)(pow((i + 1), 2))) + (float)(-0.000000005551f * (float)(pow((i + 1), 3))) + (float)(0.000000000001891500767f * (float)(pow((i + 1), 4))) + (float)(-0.000000000000000409f * (float)(pow((i + 1), 5)));
						sprintf_s(m_tWaveConfig2[iPM].szWaveLenName[i], sizeof(m_tWaveConfig2[iPM].szWaveLenName[i]), "No%d.%5.1fnm", (i + 1), m_tWaveConfig2[iPM].fWaveLength[i]);
					}
					TRY
					{
						fp->Write(&m_tWaveConfig2[iPM],sizeof(WAVE_CONFIG));
					}
						CATCH(CFileException, e)
					{
#ifdef _DEBUG
						afxDump << "File could not be write " << e->m_cause << "\n";
#endif
					}
					END_CATCH
				}/* for */
			fp->Close();
		}
		else {
			for (iPM = 0; iPM < PM_MAX; iPM++) {
				// ファイルのロード
				TRY
				{
					fp->Read(&m_tWaveConfig2[iPM],sizeof(WAVE_CONFIG));
					rv = TRUE;
				}
					CATCH(CFileException, e)
				{
#ifdef _DEBUG
					afxDump << "File could not be read " << e->m_cause << "\n";
#endif
				}
				END_CATCH
			}/* for */
			fp->Close();
		}

	/*
	** EPD IP Address Load
	*/
	TRY
	{
		bOpen = fp->Open(gpszEthCfgFileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

	//ファイルが存在しなければ初期値を
	//セットしてファイル作成
	if (bOpen == FALSE) {
			TRY
			{
				fp->Open(gpszEthCfgFileName, CFile::modeCreate | CFile::modeWrite);
			}
				CATCH(CFileException, e)
			{
				delete fp;
				return(rv);
			}
			END_CATCH
				for (i = 0; i < PM_MAX; i++) {
					m_tEPDEtherConfig[i].iIP_Addr[0] = 192;
					m_tEPDEtherConfig[i].iIP_Addr[1] = 168;
					m_tEPDEtherConfig[i].iIP_Addr[2] = 0;
					m_tEPDEtherConfig[i].iIP_Addr[3] = 100;
					m_tEPDEtherConfig[i].iPortNo = 50000;
				}
			TRY
			{
				fp->Write(&m_tEPDEtherConfig,sizeof(ETH_COMM_CFG)*PM_MAX);
			}
				CATCH(CFileException, e)
			{
			}
			END_CATCH

				fp->Close();
	}
	else {
		// ファイルのロード
		TRY
		{
			fp->Read(&m_tEPDEtherConfig,sizeof(ETH_COMM_CFG) * PM_MAX);
			rv = TRUE;
		}
			CATCH(CFileException, e)
		{
		}
		END_CATCH

			fp->Close();
	}

	/*
	** My IP Address Load
	*/
	TRY
	{
		bOpen = fp->Open(gpszMyIPCfgFileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
		if (bOpen == FALSE) {
			TRY
			{
				fp->Open(gpszMyIPCfgFileName, CFile::modeCreate | CFile::modeWrite);
			}
				CATCH(CFileException, e)
			{
				delete fp;
				return(rv);
			}
			END_CATCH
				
			m_iMyIPAddress[0] = 192;
			m_iMyIPAddress[1] = 168;
			m_iMyIPAddress[2] = 0;
			m_iMyIPAddress[3] = 1;
			TRY
			{
				fp->Write(&m_iMyIPAddress,sizeof(m_iMyIPAddress));
			}
				CATCH(CFileException, e)
			{
			}
			END_CATCH

				fp->Close();
			delete fp;
			return(rv);
		}
	// ファイルのロード
	TRY
	{
		fp->Read(&m_iMyIPAddress,sizeof(m_iMyIPAddress));
		rv = TRUE;
	}
		CATCH(CFileException, e)
	{
	}
	END_CATCH

	fp->Close();
	delete fp;

	return(rv);
}
/*
** Waveパラメータ保存
*/
int CEPDConfig::WaveCfgFileSave()
{
	int		iPM;

	CFile* fp = new CFile();


	//装置環境ファイルのオープン
	TRY
	{
		fp->Open(gpszWaveCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

		for (iPM = 0; iPM < PM_MAX; iPM++) {

			TRY
			{
				fp->Write(&m_tWaveConfig2[iPM],sizeof(WAVE_CONFIG));
			}
			CATCH(CFileException, e)
			{
#ifdef _DEBUG
				afxDump << "File could not be write " << e->m_cause << "\n";
#endif
			}
			END_CATCH
		}/* for */
		fp->Close();

	/*
	** EPD IP Address Load
	*/
	TRY
	{
		fp->Open(gpszEthCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

	// ファイルのロード
	TRY
	{
		fp->Write(&m_tEPDEtherConfig,sizeof(ETH_COMM_CFG) * PM_MAX);
	}
	CATCH(CFileException, e)
	{
	}
	END_CATCH
	fp->Close();

	/*
	** My IP Address Load
	*/
	TRY
	{
		fp->Open(gpszMyIPCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

	TRY
	{
		fp->Write(&m_iMyIPAddress,sizeof(m_iMyIPAddress));
	}
	CATCH(CFileException, e)
	{
	}
	END_CATCH
	fp->Close();

	delete fp;
	return(TRUE);
}
int CEPDConfig::PLCCfgFileLoad()
{
	CFile* fp = new CFile();
	int	rv = FALSE;
	int		bOpen;
	int		i;


	//装置環境ファイルのオープン
	TRY
	{
		bOpen = fp->Open(gpszPLCCfgFileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
		if (bOpen == FALSE) {
			TRY
			{
				fp->Open(gpszPLCCfgFileName, CFile::modeCreate | CFile::modeWrite);
			}
				CATCH(CFileException, e)
			{
				delete fp;
				return(rv);
			}
			END_CATCH

			m_tPLCConfig.iPC_No = 0;
			m_tPLCConfig.iPC_Network_Addr = 1;
			m_tPLCConfig.iPC_Node_Addr = 45;

			strcpy(m_tPLCConfig.szPLC_IP_Addr, "200.1.1.2");
			m_tPLCConfig.iPLC_Machine_No = 0;

			m_tPLCConfig.iPLC_Network_Addr = 1;
			m_tPLCConfig.iPLC_Node_Addr = 2;
			m_tPLCConfig.iPortNo = 9600;
			m_tPLCConfig.iEnable = FALSE;

			TRY
			{
				fp->Write(&m_tPLCConfig,sizeof(PLC_COMM_CFG));
			}
				CATCH(CFileException, e)
			{
#ifdef _DEBUG
				afxDump << "File could not be write " << e->m_cause << "\n";
#endif
			}
			END_CATCH

				fp->Close();
			delete fp;
			return(rv);
		}
	// ファイルのロード
	TRY
	{
		fp->Read(&m_tPLCConfig,sizeof(PLC_COMM_CFG));
		rv = TRUE;
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be read " << e->m_cause << "\n";
#endif
	}
	END_CATCH

		fp->Close();

	/*
	** Keyence通信パラメータ
	*/
	TRY
	{
		bOpen = fp->Open(gpszKeyenceCfgFileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(rv);
	}
	END_CATCH

		//ファイルが存在しなければ初期値を
		//セットしてファイル作成
		if (bOpen == FALSE) {
			TRY
			{
				fp->Open(gpszKeyenceCfgFileName, CFile::modeCreate | CFile::modeWrite);
			}
				CATCH(CFileException, e)
			{
				delete fp;
				return(rv);
			}
			END_CATCH

			m_tKeyenceComm.iEnable = FALSE;
			m_tKeyenceComm.PLCIPAddr.iIPAddr1 = 200;
			m_tKeyenceComm.PLCIPAddr.iIPAddr2 = 1;
			m_tKeyenceComm.PLCIPAddr.iIPAddr3 = 1;
			m_tKeyenceComm.PLCIPAddr.iIPAddr4 = 100;

			m_tKeyenceComm.MyIPAddr.iIPAddr1 = 200;
			m_tKeyenceComm.MyIPAddr.iIPAddr2 = 1;
			m_tKeyenceComm.MyIPAddr.iIPAddr3 = 1;
			m_tKeyenceComm.MyIPAddr.iIPAddr4 = 40;

			m_tKeyenceComm.iPortNo = 0x1000;

			m_tKeyenceComm.iConnectCycleTime = 10;
			m_tKeyenceComm.iReplyTimeOut = 3;

			TRY
			{
				fp->Write(&m_tKeyenceComm,sizeof(MC_COMM_PARAM));
			}
				CATCH(CFileException, e)
			{
#ifdef _DEBUG
				afxDump << "File could not be write " << e->m_cause << "\n";
#endif
			}
			END_CATCH

				fp->Close();
			delete fp;
			return(rv);
		}
	// ファイルのロード
	TRY
	{
		fp->Read(&m_tKeyenceComm,sizeof(MC_COMM_PARAM));
		rv = TRUE;
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be read " << e->m_cause << "\n";
#endif
	}
	END_CATCH

		fp->Close();

	delete fp;
	return(rv);
}
/*
** PLCパラメータ保存
*/
int CEPDConfig::PLCCfgFileSave()
{
	CFile* fp = new CFile();


	//装置環境ファイルのオープン
	TRY
	{
		fp->Open(gpszPLCCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

		TRY
	{
		fp->Write(&m_tPLCConfig,sizeof(PLC_COMM_CFG));
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be write " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	fp->Close();


	/*
	** Keyence PLC
	*/
	TRY
	{
		fp->Open(gpszKeyenceCfgFileName, CFile::modeCreate | CFile::modeWrite);
	}
		CATCH(CFileException, e)
	{
		delete fp;
		return(FALSE);
	}
	END_CATCH

		TRY
	{
		fp->Write(&m_tKeyenceComm,sizeof(MC_COMM_PARAM));
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be write " << e->m_cause << "\n";
#endif
	}
	END_CATCH

		fp->Close();


	delete fp;
	return(TRUE);
}
