
#include  "board.h"

extern pressSettlStru pressSettle[PRESSSETTL_485_NUM];


static int dispChenjiangIndex=0;
int dispChenjiangTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispChenjiangData()
{
	
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispChenjiangTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_CEHNJIANG_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG_TEMP_ADDR,buf,2);
				LCDWtite(DISP_DATA_CEHNJIANG_HEIGHT_ADDR,buf,2);


		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispChenjiangTotlNum;
				LCDWtite(DISP_DATA_CEHNJIANG_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.pressSetl[dispChenjiangIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CEHNJIANG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.2f",pressSettle[dispChenjiangIndex].height.flotVal);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_HEIGHT_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",pressSettle[dispChenjiangIndex].temp);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CEHNJIANG_TEMP_ADDR,buf,len);

		}

}

//上一组环流数据
void  nextChenJiangIndex()
{
		if(dispChenjiangTotlNum==0)
				return;
		else{
				dispChenjiangIndex++;
				if(dispChenjiangIndex>=dispChenjiangTotlNum)
						dispChenjiangIndex=0;
		}	
}
//下一组环流数据
void  lastChenJiangIndex()
{
		if(dispChenjiangTotlNum==0)
				return;
		else{
			if(dispChenjiangIndex==0)
					dispChenjiangIndex=dispChenjiangTotlNum-1;
			else
					dispChenjiangIndex--;
		}
}
