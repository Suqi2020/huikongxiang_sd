#include  "board.h"


extern threeAxisStru threeAxisp[THREEAXIS_485_NUM];




static int dispWaipoIndex=0;
int dispWaipoTotlNum=0;



//显示防外破界面71.bmp的所有数据
void  dispWaipoData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
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

		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispWaipoTotlNum;
				LCDWtite(DISP_DATA_FANGWAIPO_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.threeAxiss[dispWaipoIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_FANGWAIPO_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%u",threeAxisp[dispWaipoIndex].acclrationX);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCX_ADDR,buf,len);
				sprintf((char *)buf,"%u",threeAxisp[dispWaipoIndex].acclrationY);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCY_ADDR,buf,len);
				sprintf((char *)buf,"%u",threeAxisp[dispWaipoIndex].acclrationZ);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_ACCZ_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",threeAxisp[dispWaipoIndex].temp);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_FANGWAIPO_TEMP_ADDR,buf,len);
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
