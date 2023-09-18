#include "board.h"

extern float waterDepth[WATERDEPTH_485_NUM];
static int dispWaterIndex=0;
static int dispWaterTotlNum=0;
int waterDepthState(int i);
//��ʾ��������71.bmp����������
void  dispWaterData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispWaterTotlNum=0;
		for(int i=0;i<WATERDEPTH_485_NUM;i++){
				if(sheet.waterDepth[i].workFlag==RT_TRUE){
						dispWaterTotlNum++;
				}
		}
		if(dispWaterTotlNum==0){//û�й����Ļ���
				//��ʾ��ҳ��
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_WATER_TOTALNUM_ADDR,buf,2);
				//��ʾid
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_WATER_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_WATER_DEPTH_ADDR,buf,2);
			  LCDWtite(DISP_DATA_WATER_REDUID_ADDR,buf,2);
			  LCDDispModbusState(0,DISP_DATA_WATER_STATE_ADDR);
		}
		else{
				//��ʾ��ҳ��
				buf[0]=0;
				buf[1]=dispWaterTotlNum;
				LCDWtite(DISP_DATA_WATER_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < WATERDEPTH_485_NUM; i++)//�����������±�
				{		
						if(sheet.waterDepth[i].workFlag==RT_TRUE){
							  if(j==dispWaterIndex){
									k=i;
								}
								j++;
						}
				}
				//��ʾidr
			  int len=0,reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.waterDepth[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_WATER_ID_ADDR,buf,len);
				
				
				len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.waterDepth[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_WATER_REDUID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.2fm",waterDepth[k]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_WATER_DEPTH_ADDR,buf,len);
				LCDDispModbusState(waterDepthState(k),DISP_DATA_WATER_STATE_ADDR);
		}
}

//��һ��Water����
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
//��һ��Water����
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
