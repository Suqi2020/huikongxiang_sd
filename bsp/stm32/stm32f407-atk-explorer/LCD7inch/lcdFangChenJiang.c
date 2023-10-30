
#include  "board.h"

extern pressSettlStru pressSettle[PRESSSETTL_485_NUM];


static int dispChenjiangIndex=0;
static int dispChenjiangTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispChenjiangData()
{
	
	  uint8_t buf[MODBID_LEN+2]={0};
		dispChenjiangTotlNum=0;
		for(int i=0;i<PRESSSETTL_485_NUM;i++){
				if(sheet.pressSetl[i].workFlag==RT_TRUE){
						dispChenjiangTotlNum++;
				}
		}
		if(dispChenjiangTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_CEHNJIANG_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG_TEMP_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG_HEIGHT_ADDR,buf,2);
        LCDDispModbusState(-1,DISP_DATA_CEHNJIANG_STATE_ADDR);

		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispChenjiangTotlNum;
				LCDWtite(DISP_DATA_CEHNJIANG_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < PRESSSETTL_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.pressSetl[i].workFlag==RT_TRUE){
							  if(j==dispChenjiangIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.pressSetl[k].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CEHNJIANG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.1fmm",pressSettle[k].height.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_HEIGHT_ADDR,buf,len);
				sprintf((char *)buf,"%0.1f℃",pressSettle[k].temp);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_TEMP_ADDR,buf,len);
        LCDDispModbusState(pressSettle[k].respStat,DISP_DATA_CEHNJIANG_STATE_ADDR);
		}

}

//上一组环流数据
void  nextChenJiangIndex()
{
		if(dispChenjiangTotlNum==0)
				return;
		else{
				dispChenjiangIndex++;
				if(dispChenjiangIndex>=dispChenjiangTotlNum)
						dispChenjiangIndex=0;
		}	
}
//下一组环流数据
void  lastChenJiangIndex()
{
		if(dispChenjiangTotlNum==0)
				return;
		else{
			if(dispChenjiangIndex==0)
					dispChenjiangIndex=dispChenjiangTotlNum-1;
			else
					dispChenjiangIndex--;
		}
}






//显示环流界面71.bmp的所有数据
void  dispChenjiangData_tjw()
{
	  dispChenjiangTotlNum=0;
	  uint8_t buf[MODBID_LEN+2]={0};
		for(int i=0;i<PRESSSETTL_485_NUM;i++){
				if(sheet.pressSetl[i].workFlag==RT_TRUE){
						dispChenjiangTotlNum++;
				}
		}
		if(dispChenjiangTotlNum==0){//没有工作的环流
				//显示总页数

				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG1_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG1_TEMP_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG1_HEIGHT_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG2_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG2_TEMP_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG2_HEIGHT_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG3_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG3_TEMP_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG3_HEIGHT_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG4_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG4_TEMP_TJW_ADDR,buf,2);
			
				LCDWtite(DISP_DATA_CEHNJIANG4_HEIGHT_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG1_REDUID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG2_REDUID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG3_REDUID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG4_REDUID_TJW_ADDR,buf,2);			
				
				LCDDispModbusState(-1,DISP_DATA_CEHNJIANG1_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_CEHNJIANG2_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_CEHNJIANG3_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_CEHNJIANG4_STATE_TJW_ADDR);

		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispChenjiangTotlNum;
				LCDWtite(DISP_DATA_CEHNJIANG_TOTALNUM_ADDR,buf,2);
				int k1=0,k2=0,k3=0,k4=0;
				for (int i = 0; i < PRESSSETTL_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.pressSetl[i].workFlag==RT_TRUE){
				
									k1=i;
									int len=0,reduLen=0;
									for(len=0;len<MODBID_LEN;len++){
											buf[len]=sheet.pressSetl[k1].ID[len];
											if(buf[len]==0){
												  reduLen=len;
													break;
											}
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG1_ID_TJW_ADDR,buf,len);

									len=0;
									for(int i=reduLen-3;i<reduLen;i++,len++){
											buf[len]=sheet.pressSetl[k1].ID[i];
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG1_REDUID_TJW_ADDR,buf,len);
			
									
									
									
									sprintf((char *)buf,"%0.1fmm",pressSettle[k1].height.flotVal);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG1_HEIGHT_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.1f℃",pressSettle[k1].temp);
									len=strlen((char *)buf);
			
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG1_TEMP_TJW_ADDR,buf,len);

									LCDDispModbusState(pressSettle[k1].respStat,DISP_DATA_CEHNJIANG1_STATE_TJW_ADDR);
									break;
						}
				}
				for (int i = k1+1; i < PRESSSETTL_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.pressSetl[i].workFlag==RT_TRUE){
									k2=i;
									int len=0,reduLen=0;
									for(len=0;len<MODBID_LEN;len++){
											buf[len]=sheet.pressSetl[k2].ID[len];
											if(buf[len]==0){
												  reduLen=len;
													break;
											}
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG2_ID_TJW_ADDR,buf,len);
									len=0;
									for(int i=reduLen-3;i<reduLen;i++,len++){
											buf[len]=sheet.pressSetl[k2].ID[i];
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG2_REDUID_TJW_ADDR,buf,len);

									sprintf((char *)buf,"%0.1fmm",pressSettle[k2].height.flotVal);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG2_HEIGHT_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.1f℃",pressSettle[k2].temp);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG2_TEMP_TJW_ADDR,buf,len);

									LCDDispModbusState(pressSettle[k2].respStat,DISP_DATA_CEHNJIANG2_STATE_TJW_ADDR);
									break;
						}
				}
				for (int i = k2+1; i < PRESSSETTL_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.pressSetl[i].workFlag==RT_TRUE){
									k3=i;
								  int len=0,reduLen=0;
									for(len=0;len<MODBID_LEN;len++){
											buf[len]=sheet.pressSetl[k3].ID[len];
											if(buf[len]==0){
												  reduLen=len;
													break;
											}
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG3_ID_TJW_ADDR,buf,len);
									len=0;
									for(int i=reduLen-3;i<reduLen;i++,len++){
											buf[len]=sheet.pressSetl[k3].ID[i];
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG3_REDUID_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.1fmm",pressSettle[k3].height.flotVal);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG3_HEIGHT_TJW_ADDR,buf,len);
				
									sprintf((char *)buf,"%0.1f℃",pressSettle[k3].temp);
									len=strlen((char *)buf);

									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG3_TEMP_TJW_ADDR,buf,len);
									LCDDispModbusState(pressSettle[k3].respStat,DISP_DATA_CEHNJIANG3_STATE_TJW_ADDR);
									break;
						}
				}
				for (int i = k3+1; i < PRESSSETTL_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.pressSetl[i].workFlag==RT_TRUE){
									k4=i;
							  	int len=0,reduLen=0;
									for(len=0;len<MODBID_LEN;len++){
											buf[len]=sheet.pressSetl[k4].ID[len];
											if(buf[len]==0){
												  reduLen=len;
													break;
											}
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG4_ID_TJW_ADDR,buf,len);
									len=0;
									for(int i=reduLen-3;i<reduLen;i++,len++){
											buf[len]=sheet.pressSetl[k4].ID[i];
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_CEHNJIANG4_REDUID_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.1fmm",pressSettle[k4].height.flotVal);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG4_HEIGHT_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.1f℃",pressSettle[k4].temp);
									len=strlen((char *)buf);


									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_CEHNJIANG4_TEMP_TJW_ADDR,buf,len);
									LCDDispModbusState(pressSettle[k4].respStat,DISP_DATA_CEHNJIANG4_STATE_TJW_ADDR);
									break;
						}
				}
				//显示idr


		}

}
