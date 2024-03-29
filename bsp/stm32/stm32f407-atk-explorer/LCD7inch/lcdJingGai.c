#include  "board.h"




extern coverStru cover[COVER_485_NUM];


static int dispJinggaiIndex=0;
static int dispJinggaiTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispJinggaiData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispJinggaiTotlNum=0;
		for(int i=0;i<COVER_485_NUM;i++){
			if(sheet.cover[i].workFlag==RT_TRUE){
			    dispJinggaiTotlNum++;
			}
		}
		if(dispJinggaiTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_COVER_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_COVER_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_COVER_INCLINE_ADDR,buf,2);
				LCDWtite(DISP_DATA_COVER_SWITCH2P_ADDR,buf,2);
				LCDWtite(DISP_DATA_COVER_VIBRATION_ADDR,buf,2);
				LCDWtite(DISP_DATA_COVER_SWITCH1P_ADDR,buf,2);
			  LCDWtite(DISP_DATA_COVER_REDUID_ADDR,buf,2);
	      LCDDispModbusState(-1,DISP_DATA_COVER_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispJinggaiTotlNum;
				LCDWtite(DISP_DATA_COVER_TOTALNUM_ADDR,buf,2);
				int j=0,k=0,reduLen=0;
				for (int i = 0; i < COVER_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.cover[i].workFlag==RT_TRUE){
							  if(j==dispJinggaiIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.cover[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_COVER_ID_ADDR,buf,len);
				len=0;
				for(int i=reduLen-3;i<reduLen;i++,len++){
							buf[len]=sheet.cover[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_COVER_REDUID_ADDR,buf,len);

						
						
				sprintf((char *)buf,"%d",cover[k].incline);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_COVER_INCLINE_ADDR,buf,len);
				sprintf((char *)buf,"%d",cover[k].switch2p);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_COVER_SWITCH2P_ADDR,buf,len);
				sprintf((char *)buf,"%d",cover[k].vibration);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_COVER_VIBRATION_ADDR,buf,len);
				sprintf((char *)buf,"%d",cover[k].switch1p);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_COVER_SWITCH1P_ADDR,buf,len);
				LCDDispModbusState(cover[k].respStat,DISP_DATA_COVER_STATE_ADDR);
		}

}

//上一组环流数据
void  nextJinggaiIndex()
{
	if(dispJinggaiTotlNum==0)
		return;
	else{
		dispJinggaiIndex++;
		if(dispJinggaiIndex>=dispJinggaiTotlNum)
				dispJinggaiIndex=0;
	}	
}
//下一组环流数据
void  lastJinggaiIndex()
{
	if(dispJinggaiTotlNum==0)
		return;
	else{
		if(dispJinggaiIndex==0)
			dispJinggaiIndex=dispJinggaiTotlNum-1;
		else
			dispJinggaiIndex--;
	}
}
