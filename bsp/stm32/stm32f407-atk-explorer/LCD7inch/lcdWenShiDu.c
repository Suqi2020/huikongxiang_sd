#include  "board.h"


extern thStru thum[TEMPHUM_485_NUM];



static int dispWenshiduIndex=0;
static int dispWenshiduTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispWenshiduData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispWenshiduTotlNum=0;
	 for(int i=0;i<TEMPHUM_485_NUM;i++){
		if(sheet.tempHum[i].workFlag==RT_TRUE){
			    dispWenshiduTotlNum++;
			}
	}
		if(dispWenshiduTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_WENSHIDU_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_WENSHIDU_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_WENSHIDU_TEMP_ADDR,buf,2);
				LCDWtite(DISP_DATA_WENSHIDU_HUMI_ADDR,buf,2);
			  LCDWtite(DISP_DATA_WENSHIDU_REDUID_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_WENSHIDU_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispWenshiduTotlNum;
				LCDWtite(DISP_DATA_WENSHIDU_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < TEMPHUM_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.tempHum[i].workFlag==RT_TRUE){
							  if(j==dispWenshiduIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0,reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.tempHum[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_WENSHIDU_ID_ADDR,buf,len);
				len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.tempHum[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_WENSHIDU_REDUID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.1f℃",thum[k].temp);
				len=strlen((char *)buf);
//				buf[len++]=sheShiDu[0];
//				buf[len++]=sheShiDu[1];
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_WENSHIDU_TEMP_ADDR,buf,len);
				sprintf((char *)buf,"%0.1f%%RH",thum[k].hum);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_WENSHIDU_HUMI_ADDR,buf,len);
				LCDDispModbusState(thum[k].respStat,DISP_DATA_WENSHIDU_STATE_ADDR);
		}

}

//上一组环流数据
void  nextWenshiduIndex()
{
	if(dispWenshiduTotlNum==0)
		return;
	else{
		dispWenshiduIndex++;
		if(dispWenshiduIndex>=dispWenshiduTotlNum)
				dispWenshiduIndex=0;
	}	
}
//下一组环流数据
void  lastWenshiduIndex()
{
	if(dispWenshiduTotlNum==0)
		return;
	else{
		if(dispWenshiduIndex==0)
			dispWenshiduIndex=dispWenshiduTotlNum-1;
		else
			dispWenshiduIndex--;
	}
}
