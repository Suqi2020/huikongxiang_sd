#include "board.h"

extern float waterDepth[WATERDEPTH_485_NUM];
static int dispWaterIndex=0;
int dispWaterTotlNum=0;
//显示环流界面71.bmp的所有数据
void  dispWaterData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispWaterTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_WATER_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_WATER_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_WATER_DEPTH_ADDR,buf,2);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispWaterTotlNum;
				LCDWtite(DISP_DATA_WATER_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.waterDepth[dispWaterIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_WATER_ID_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",waterDepth[dispWaterIndex]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_WATER_DEPTH_ADDR,buf,len);
		}
}

//上一组Water数据
void  nextWaterIndex()
{
		if(dispWaterTotlNum==0)
				return;
		else{
				dispWaterTotlNum++;
				if(dispWaterIndex>=dispWaterTotlNum)
						dispWaterIndex=0;
		}	
}
//下一组Water数据
void  lastWaterIndex()
{
		if(dispWaterTotlNum==0)
				return;
		else{
			if(dispWaterIndex==0)
					dispWaterIndex=dispWaterTotlNum-1;
			else
					dispWaterIndex--;
		}
}
