#include  "board.h"



extern  crackMeterStru crackMeter[CRACKMETER_485_NUM];

static int dispLiefengIndex=0;
static int dispLiefengTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispLiefengData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispLiefengTotlNum=0;
		for(int i=0;i<CRACKMETER_485_NUM;i++){
				if(sheet.crackMeter[i].workFlag==RT_TRUE){
					  dispLiefengTotlNum++;
				}
		}
		if(dispLiefengTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_LIEFENG_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_LIEFENG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_LIEFENG_TEMP_ADDR,buf,2);
				LCDWtite(DISP_DATA_LIEFENG_DISTANC_ADDR,buf,2);
			  LCDWtite(DISP_DATA_LIEFENG_REDUID_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_LIEFENG_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispLiefengTotlNum;
				LCDWtite(DISP_DATA_LIEFENG_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < CRACKMETER_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.crackMeter[i].workFlag==RT_TRUE){
							  if(j==dispLiefengIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0,reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.crackMeter[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len ;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_LIEFENG_ID_ADDR,buf,len);
				len=0;
				for(int i=reduLen-3;i<reduLen;i++,len++){
							buf[len]=sheet.crackMeter[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_LIEFENG_REDUID_ADDR,buf,len);
				sprintf((char *)buf,"%0.1f℃",crackMeter[k].temp.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_LIEFENG_TEMP_ADDR,buf,len);
				sprintf((char *)buf,"%0.2fmm",crackMeter[k].distanc.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_LIEFENG_DISTANC_ADDR,buf,len);
				LCDDispModbusState(crackMeter[k].respStat,DISP_DATA_LIEFENG_STATE_ADDR);
		}

}

//上一组环流数据
void  nextLiefengIndex()
{
	if(dispLiefengTotlNum==0)
		return;
	else{
		dispLiefengIndex++;
		if(dispLiefengIndex>=dispLiefengTotlNum)
				dispLiefengIndex=0;
	}	
}
//下一组环流数据
void  lastLiefengIndex()
{
	if(dispLiefengTotlNum==0)
		return;
	else{
		if(dispLiefengIndex==0)
			dispLiefengIndex=dispLiefengTotlNum-1;
		else
			dispLiefengIndex--;
	}
}
