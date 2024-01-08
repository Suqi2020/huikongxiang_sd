#include  "board.h"


extern threeAxisStru threeAxisp[THREEAXIS_485_NUM];




static int dispWaipoIndex=0;
static int dispWaipoTotlNum=0;

//判断value值如果为负就格式化负数 否则为正
void sprintf_threeAxismg(char *buf,uint16_t value)
{
				if((UINT16_BELOW_ZERO&value)==0){
					sprintf(buf,"%umg",value);
				}
				else{
					sprintf(buf,"-%umg",~value+1);
				}
}		

//显示防外破界面71.bmp的所有数据
void  dispWaipoData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispWaipoTotlNum=0;
		for(int i=0;i<THREEAXIS_485_NUM;i++){
				if(sheet.threeAxiss[i].workFlag==RT_TRUE){
					  dispWaipoTotlNum++;
				}
		}
		if(dispWaipoTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_FANGWAIPO_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO_ACCX_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO_ACCY_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO_ACCZ_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO_TEMP_ADDR,buf,2);
			 // LCDWtite(DISP_DATA_FANGWAIPO_REDUID_ADDR,buf,2);
        LCDDispModbusState(-1,DISP_DATA_FANGWAIPO_STATE_ADDR);

		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispWaipoTotlNum;
				LCDWtite(DISP_DATA_FANGWAIPO_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < THREEAXIS_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							  if(j==dispWaipoIndex){
									k=i;
								}
								j++;
						}
				}
				
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.threeAxiss[k].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_FANGWAIPO_ID_ADDR,buf,len);
				
				sprintf_threeAxismg((char *)buf,threeAxisp[k].acclrationX);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCX_ADDR,buf,len);
				sprintf_threeAxismg((char *)buf,threeAxisp[k].acclrationY);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCY_ADDR,buf,len);
				sprintf_threeAxismg((char *)buf,threeAxisp[k].acclrationZ);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCZ_ADDR,buf,len);
				sprintf((char *)buf,"%0.1f℃",threeAxisp[k].temp);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_TEMP_ADDR,buf,len);
        LCDDispModbusState(threeAxisp[k].respStat,DISP_DATA_FANGWAIPO_STATE_ADDR);
		}

}

//上一组防外破数据
void  nextWaipoIndex()
{
	if(dispWaipoTotlNum==0)
		return;
	else{
		dispWaipoIndex++;
		if(dispWaipoIndex>=dispWaipoTotlNum)
				dispWaipoIndex=0;
	}	
}
//下一组防外破数据
void  lastWaipoIndex()
{
	if(dispWaipoTotlNum==0)
		return;
	else{
		if(dispWaipoIndex==0)
			dispWaipoIndex=dispWaipoTotlNum-1;
		else
			dispWaipoIndex--;
	}
}



void  dispWaipoData_tjw()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispWaipoTotlNum=0;
		for(int i=0;i<THREEAXIS_485_NUM;i++){
				if(sheet.threeAxiss[i].workFlag==RT_TRUE){
					  dispWaipoTotlNum++;
				}
		}
		if(dispWaipoTotlNum==0){//没有工作的环流

				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO1_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO1_ACCX_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO1_ACCY_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO1_ACCZ_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO1_TEMP_TJW_ADDR,buf,2);
			
				LCDWtite(DISP_DATA_FANGWAIPO2_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO2_ACCX_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO2_ACCY_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO2_ACCZ_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO2_TEMP_TJW_ADDR,buf,2);
			
				LCDWtite(DISP_DATA_FANGWAIPO3_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO3_ACCX_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO3_ACCY_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO3_ACCZ_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO3_TEMP_TJW_ADDR,buf,2);
				
				LCDWtite(DISP_DATA_FANGWAIPO4_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO4_ACCX_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO4_ACCY_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO4_ACCZ_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_FANGWAIPO4_TEMP_TJW_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_FANGWAIPO1_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_FANGWAIPO2_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_FANGWAIPO3_STATE_TJW_ADDR);
				LCDDispModbusState(-1,DISP_DATA_FANGWAIPO4_STATE_TJW_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispWaipoTotlNum;
				LCDWtite(DISP_DATA_FANGWAIPO_TOTALNUM_ADDR,buf,2);
				int k1=0,k2=0,k3=0,k4=0;
				for (int i = 0; i < THREEAXIS_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							  k1=i;
								int len=0,reduLen=0;
								for(len=0;len<MODBID_LEN;len++){
										buf[len]=sheet.threeAxiss[i].ID[len];
										if(buf[len]==0){
											  reduLen=len;
												break;
										}
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO1_ID_TJW_ADDR,buf,len);
								len=0;
								for(int i=reduLen-3;i<reduLen;i++,len++){
										buf[len]=sheet.threeAxiss[k1].ID[i];
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO1_REDUID_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationX);
							//	sprintf((char *)buf,"%umg",threeAxisp[i].acclrationX);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO1_ACCX_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationY);//sprintf((char *)buf,"%umg",threeAxisp[i].acclrationY);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO1_ACCY_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationZ);//sprintf((char *)buf,"%umg",threeAxisp[i].acclrationZ);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO1_ACCZ_TJW_ADDR,buf,len);
								sprintf((char *)buf,"%0.1f℃",threeAxisp[i].temp);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO1_TEMP_TJW_ADDR,buf,len);
								LCDDispModbusState(threeAxisp[i].respStat,DISP_DATA_FANGWAIPO1_STATE_TJW_ADDR);
							  break;
						}
						
				}
				for (int i = k1+1; i < THREEAXIS_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							  k2=i;
								int len=0,reduLen=0;
								for(len=0;len<MODBID_LEN;len++){
										buf[len]=sheet.threeAxiss[i].ID[len];
										if(buf[len]==0){
											  reduLen=len;
												break;
										}
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO2_ID_TJW_ADDR,buf,len);
								len=0;
								for(int i=reduLen-3;i<reduLen;i++,len++){
										buf[len]=sheet.threeAxiss[k2].ID[i];
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO2_REDUID_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationX);//sprintf((char *)buf,"%umg",threeAxisp[i].acclrationX);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO2_ACCX_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationY);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO2_ACCY_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationZ);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO2_ACCZ_TJW_ADDR,buf,len);
								sprintf((char *)buf,"%0.1f℃",threeAxisp[i].temp);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO2_TEMP_TJW_ADDR,buf,len);
								LCDDispModbusState(threeAxisp[i].respStat,DISP_DATA_FANGWAIPO2_STATE_TJW_ADDR);
							  break;
						}
						
				}
				for (int i = k2+1; i < THREEAXIS_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							  k3=i;
								int len=0,reduLen=0;
								for(len=0;len<MODBID_LEN;len++){
										buf[len]=sheet.threeAxiss[i].ID[len];
										if(buf[len]==0){
											  reduLen=len;
												break;
										}
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO3_ID_TJW_ADDR,buf,len);
								len=0;
								for(int i=reduLen-3;i<reduLen;i++,len++){
										buf[len]=sheet.threeAxiss[k3].ID[i];
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO3_REDUID_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationX);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO3_ACCX_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationY);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO3_ACCY_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationZ);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO3_ACCZ_TJW_ADDR,buf,len);
								sprintf((char *)buf,"%0.1f℃",threeAxisp[i].temp);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO3_TEMP_TJW_ADDR,buf,len);
								LCDDispModbusState(threeAxisp[i].respStat,DISP_DATA_FANGWAIPO3_STATE_TJW_ADDR);
							  break;
						}
						
				}
				for (int i = k3+1; i < THREEAXIS_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							  k4=i;
								int len=0,reduLen=0;
								for(len=0;len<MODBID_LEN;len++){
										buf[len]=sheet.threeAxiss[i].ID[len];
										if(buf[len]==0){
											  reduLen=len;
												break;
										}
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO4_ID_TJW_ADDR,buf,len);
								len=0;
								for(int i=reduLen-3;i<reduLen;i++,len++){
										buf[len]=sheet.threeAxiss[k4].ID[i];
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_FANGWAIPO4_REDUID_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationX);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO4_ACCX_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationY);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO4_ACCY_TJW_ADDR,buf,len);
								sprintf_threeAxismg((char *)buf,threeAxisp[i].acclrationZ);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO4_ACCZ_TJW_ADDR,buf,len);
								sprintf((char *)buf,"%0.1f℃",threeAxisp[i].temp);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_FANGWAIPO4_TEMP_TJW_ADDR,buf,len);
								LCDDispModbusState(threeAxisp[i].respStat,DISP_DATA_FANGWAIPO4_STATE_TJW_ADDR);
							  break;
						}
						
				}
				//显示idr
			 
		}

}