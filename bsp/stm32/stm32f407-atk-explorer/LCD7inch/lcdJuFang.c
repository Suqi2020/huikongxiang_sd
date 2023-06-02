#include  "board.h"




extern  CIRCURStru  cirCurStru_p[CIRCULA_485_NUM];


static int dispJufangIndex=0;
int dispJufangTotlNum=0;

extern partDischargeStru partDiscStru_p[PARTDISCHAG_485_NUM];

//显示环流界面71.bmp的所有数据
void  dispJufangData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
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
			


		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispJufangTotlNum;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.partDischag[dispJufangIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].amplitudeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].amplitudeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].amplitudeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].freqA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].freqB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].freqC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].dischargeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].dischargeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[dispJufangIndex].dischargeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,len);
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
