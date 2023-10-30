#include  "board.h"




extern  CIRCURStru  cirCurStru_p[CIRCULA_485_NUM];


static int dispJufangIndex=0;
static int dispJufangTotlNum=0;

extern partDischargeStru partDiscStru_p[PARTDISCHAG_485_NUM];

//显示环流界面71.bmp的所有数据
void  dispJufangData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispJufangTotlNum=0;
		for(int i=0;i<PARTDISCHAG_485_NUM;i++){
			 if(sheet.partDischag[i].workFlag==RT_TRUE){
				    dispJufangTotlNum++;
				}
		}
		if(dispJufangTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,2);
			  LCDWtite(DISP_DATA_JUFANG_REDUID_ADDR,buf,2);
			  LCDDispModbusState(-1,DISP_DATA_JUFANG_STATE_ADDR);


		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispJufangTotlNum;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < PARTDISCHAG_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.partDischag[i].workFlag==RT_TRUE){
							  if(j==dispJufangIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.partDischag[k].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].amplitudeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,len);
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].amplitudeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,len);
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].amplitudeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,len);
				sprintf((char *)buf,"%uHz",partDiscStru_p[k].freqA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,len);
				sprintf((char *)buf,"%uHz",partDiscStru_p[k].freqB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,len);
				sprintf((char *)buf,"%uHz",partDiscStru_p[k].freqC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,len);
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].dischargeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,len);
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].dischargeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,len);
				sprintf((char *)buf,"%uPc",partDiscStru_p[k].dischargeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,len);
				LCDDispModbusState(partDiscStru_p[k].respStat,DISP_DATA_JUFANG_STATE_ADDR);
		}

}




//上一组环流数据
void  nextJufangIndex()
{
	if(dispJufangTotlNum==0)
		return;
	else{
		dispJufangIndex++;
		if(dispJufangIndex>=dispJufangTotlNum)
				dispJufangIndex=0;
	}	
}
//下一组环流数据
void  lastJufangIndex()
{
	if(dispJufangTotlNum==0)
		return;
	else{
		if(dispJufangIndex==0)
			dispJufangIndex=dispJufangTotlNum-1;
		else
			dispJufangIndex--;
	}
}


void  dispJufangData_tjw()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispJufangTotlNum=0;
		for(int i=0;i<PARTDISCHAG_485_NUM;i++){
			 if(sheet.partDischag[i].workFlag==RT_TRUE){
				    dispJufangTotlNum++;
				}
		}
		if(dispJufangTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,2);
			  LCDWtite(DISP_DATA_JUFANG_REDUID_ADDR,buf,2);
			
			  LCDDispModbusState(-1,DISP_DATA_JUFANG_STATE_ADDR);

				LCDWtite(DISP_DATA_JUFANG_ID1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_A1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_B1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_C1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_A1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_B1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_C1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C1_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_REDUID1_ADDR,buf,2);
			  LCDDispModbusState(-1,DISP_DATA_JUFANG_STATE1_ADDR);
		}
		
		
		else{
				//显示总页数
	
				int j=0,k1=0,k2=0;
				for (int i = 0; i < PARTDISCHAG_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.partDischag[i].workFlag==RT_TRUE){
				
						k1=i;

						//显示idr
						int len=0,reduLen=0;
						for(len=0;len<MODBID_LEN;len++){
								buf[len]=sheet.partDischag[k1].ID[len];
								if(buf[len]==0){
									  reduLen=len;
										break;
								}
						}
						buf[len++]	=0xff;  
						buf[len++]  =0xff; 
						LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,len);
						len=0;
						for(int i=reduLen-3;i<reduLen;i++,len++){
								buf[len]=sheet.partDischag[k1].ID[i];
						}
						buf[len++]	=0xff;  
						buf[len++]  =0xff; 
						LCDWtite(DISP_DATA_JUFANG_REDUID_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].amplitudeA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].amplitudeB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].amplitudeC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k1].freqA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k1].freqB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k1].freqC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].dischargeA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].dischargeB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k1].dischargeC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,len);
						LCDDispModbusState(partDiscStru_p[k1].respStat,DISP_DATA_JUFANG_STATE_ADDR);
						break;
					}
				}
				for (int i =k1+1; i < PARTDISCHAG_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.partDischag[i].workFlag==RT_TRUE){
				
						k2=i;

						//显示idr
						int len=0,reduLen=0;
						for(len=0;len<MODBID_LEN;len++){
								buf[len]=sheet.partDischag[k2].ID[len];
								if(buf[len]==0){
									  reduLen=len;
										break;
								}
						}
						buf[len++]	=0xff;  
						buf[len++]  =0xff; 
						LCDWtite(DISP_DATA_JUFANG_ID1_ADDR,buf,len);
						len=0;
						for(int i=reduLen-3;i<reduLen;i++,len++){
								buf[len]=sheet.partDischag[k2].ID[i];
						}
						buf[len++]	=0xff;  
						buf[len++]  =0xff; 
						LCDWtite(DISP_DATA_JUFANG_REDUID1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].amplitudeA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_A1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].amplitudeB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_B1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].amplitudeC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_PD_C1_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k2].freqA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_A1_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k2].freqB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_B1_ADDR,buf,len);
						sprintf((char *)buf,"%uHz",partDiscStru_p[k2].freqC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_FREQ_C1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].dischargeA);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_A1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].dischargeB);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_B1_ADDR,buf,len);
						sprintf((char *)buf,"%uPc",partDiscStru_p[k2].dischargeC);
						len=strlen((char *)buf);
						buf[len++]=0xff;
						buf[len++]=0xff;
						LCDWtite(DISP_DATA_JUFANG_DISCHG_C1_ADDR,buf,len);
						LCDDispModbusState(partDiscStru_p[k2].respStat,DISP_DATA_JUFANG_STATE1_ADDR);
						break;
					}
				}
		}

}
