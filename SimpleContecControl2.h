#pragma once

#ifndef SimpleContecControl2_h
#define SimpleContecControl2_h

#include "../../EmgTool/SimpleContecControl.h"

/**
CSimpleContecControlでエラーが起こりにくくしたもの
*/
class CSimpleContecControl2 : public CSimpleContecControl {
public:
	virtual bool GetData() {
		long Ret,Ret2;
		nInputEventCount++;
		// Get Sampled Number (How many sampled data are there in the device's buffer?)
		long lDevSampleCount=0;
		Ret = AioGetAiSamplingCount(sContecDeviceID, &lDevSampleCount);
		if(Ret != 0){
			Ret2 = AioGetErrorString(Ret, cErrorString);
			return false;
		}
		if(lDevSampleCount <= 0){
			return false;
		}
		// 一度に読むデータはテンポラリバッファの大きさに制限
		// テンポラリバッファの大きさは入力バッファと同じ
		if(lDevSampleCount > nInputBufLength){
			lDevSampleCount = nInputBufLength;
		}
		// Get data
		Ret = AioGetAiSamplingDataEx(sContecDeviceID, &lDevSampleCount, &AiData[0]);
		if(Ret != 0){
			Ret2 = AioGetErrorString(Ret, cErrorString);
			return false;
		}

		// Store data
		int i,ch;
		int nCurrentQtNum;

		// 平均値を引く（直流をカットする）処理を追加：川瀬（2009/5/8）
		double mean = 0.0;
		const int def_num_movingav = 100;
		int num_movingav = (nCurrentStoredNum<def_num_movingav ? nCurrentStoredNum : def_num_movingav);
		if(bEmg){
			for(i=0;i<lDevSampleCount;i++){
				for(ch=0;ch<nInputChannels;ch++){
					
					//追加：川瀬（2009/5/8）
					for (int j=0; j < num_movingav; j++) {
						mean += pCurrentBuf[ch][nCurrentStoredNum-j-1];
					}
					mean /= def_num_movingav;
					//追加ここまで

					pCurrentBuf[ch][nCurrentStoredNum] = 
						/*
						AiData[i*nInputChannels+ch];//*/prefilter->PushInput(AiData[i*nInputChannels+ch]);
					pCurrentEmg[ch][nCurrentStoredNum] = 
						/*
						AiData[i*nInputChannels+ch];//*/prefilter->PushInput(AiData[i*nInputChannels+ch])-mean;
					dEmgAverageVal[ch] += fabs(pCurrentEmg[ch][nCurrentStoredNum]);
				}
				nEmgAverageNum++;
				if(nEmgAverageNum == 10){
					nCurrentQtNum = (int)(0.1*nCurrentStoredNum);
					for(ch=0;ch<nInputChannels;ch++){
						pCurrentQt[ch][nCurrentQtNum] = koikefilter[ch].PushInput(0.1*dEmgAverageVal[ch]);
						dEmgAverageVal[ch] = 0.0;
					}
					nEmgAverageNum = 0;
					
					// キャリブレーション処理を移動：川瀬（2008/11/28）
					// Calibration
					if(bEmgCalib){
						for(ch=0;ch<nInputChannels;ch++){
							// check minimum quasi-tension
							if(pCurrentQt[ch][nCurrentQtNum] < dQtMin[ch]){
								dQtMin[ch] = pCurrentQt[ch][nCurrentQtNum];
							}
							// check maximun quasi-tension
							if(pCurrentQt[ch][nCurrentQtNum] > dQtMax[ch]){
								dQtMax[ch] = pCurrentQt[ch][nCurrentQtNum];
							}
						}
					}
				}
				// コメントアウト：川瀬（2008/11/28）
				//// Calibration
				//if(bEmgCalib){
				//	for(ch=0;ch<nInputChannels;ch++){
				//		// check minimum quasi-tension
				//		if(pCurrentQt[ch][nCurrentQtNum] < dQtMin[ch]){
				//			dQtMin[ch] = pCurrentQt[ch][nCurrentQtNum];
				//		}
				//		// check maximun quasi-tension
				//		if(pCurrentQt[ch][nCurrentQtNum] > dQtMax[ch]){
				//			dQtMax[ch] = pCurrentQt[ch][nCurrentQtNum];
				//		}
				//	}
				//}
				nCurrentStoredNum++;
				nTotalStoredNum++;
				if(nCurrentStoredNum == nInputBufLength){
					SwapCurrentBuf();
				}
			}
			
		}else{
			for(i=0;i<lDevSampleCount;i++){
				for(ch=0;ch<nInputChannels;ch++){
					pCurrentBuf[ch][nCurrentStoredNum] = /*
						AiData[i*nInputChannels+ch];//*/prefilter->PushInput(AiData[i*nInputChannels+ch]);
					//AiData[i*nInputChannels+ch];
				}
				nCurrentStoredNum++;
				nTotalStoredNum++;
				if(nCurrentStoredNum == nInputBufLength){
					SwapCurrentBuf();
				}
			}
			
		}

		// Prepare next 	
		nInputSampleCount += lDevSampleCount;
		//	TRACE("EventCount:%04d deltaSample:%05d\n",nInputEventCount,lDevSampleCount);
		return true;
	}
	virtual double GetNormalizedQt(double qt, int ch){
		if(qt < dQtMin[ch]){
			return 0.0;
		}
		//return (qt-dQtMin[ch])/(dQtMax[ch]-dQtMin[ch])*0.1;
		
		// 20140603修正　ACTを0.1倍しないで出力
		return (qt-dQtMin[ch])/(dQtMax[ch]-dQtMin[ch]);
	}
};

#endif