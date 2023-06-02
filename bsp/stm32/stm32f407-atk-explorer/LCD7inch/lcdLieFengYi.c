#include  "board.h"



extern  crackMeterStru crackMeter[CRACKMETER_485_NUM];

static int dispLiefengIndex=0;
int dispLiefengTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispLiefengData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
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

		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispLiefengTotlNum;
				LCDWtite(DISP_DATA_LIEFENG_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.crackMeter[dispLiefengIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_LIEFENG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.2f",crackMeter[dispLiefengIndex].temp.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_LIEFENG_TEMP_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",crackMeter[dispLiefengIndex].distanc.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_LIEFENG_DISTANC_ADDR,buf,len);
				
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
