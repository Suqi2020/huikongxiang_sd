
#include  "board.h"

extern float ch4[CH4_485_NUM];
static int dispCH4Index=0;
int dispCH4TotlNum=0;
//显示环流界面71.bmp的所有数据
void  dispCH4Data()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispCH4TotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_CH4_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_CH4_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_CH4_VALUE_ADDR,buf,2);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispCH4TotlNum;
				LCDWtite(DISP_DATA_CH4_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.ch4[dispCH4Index].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CH4_ID_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",ch4[dispCH4Index]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CH4_VALUE_ADDR,buf,len);
		}
}

//上一组ch4数据
void  nextCH4Index()
{
		if(dispCH4TotlNum==0)
				return;
		else{
				dispCH4Index++;
				if(dispCH4Index>=dispCH4TotlNum)
						dispCH4Index=0;
		}	
}
//下一组ch4数据
void  lastCH4Index()
{
		if(dispCH4TotlNum==0)
				return;
		else{
			if(dispCH4Index==0)
					dispCH4Index=dispCH4TotlNum-1;
			else
					dispCH4Index--;
		}
}





extern float o2[O2_485_NUM];
static int dispO2Index=0;
int dispO2TotlNum=0;
//显示环流界面71.bmp的所有数据
void  dispO2Data()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispO2TotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_O2_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_O2_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_O2_VALUE_ADDR,buf,2);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispO2TotlNum;
				LCDWtite(DISP_DATA_O2_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.o2[dispO2Index].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_O2_ID_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",o2[dispO2Index]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_O2_VALUE_ADDR,buf,len);
		}
}

//上一组O2数据
void  nextO2Index()
{
		if(dispO2TotlNum==0)
				return;
		else{
				dispO2Index++;
				if(dispO2Index>=dispO2TotlNum)
						dispO2Index=0;
		}	
}
//下一组O2数据
void  lastO2Index()
{
		if(dispO2TotlNum==0)
				return;
		else{
			if(dispO2Index==0)
					dispO2Index=dispO2TotlNum-1;
			else
					dispO2Index--;
		}
}




extern float h2s[H2S_485_NUM];
static int dispH2SIndex=0;
int dispH2STotlNum=0;
//显示H2S界面71.bmp的所有数据
void  dispH2SData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispH2STotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_H2S_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_H2S_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_H2S_VALUE_ADDR,buf,2);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispH2STotlNum;
				LCDWtite(DISP_DATA_H2S_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.h2s[dispH2SIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_H2S_ID_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",h2s[dispH2SIndex]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_H2S_VALUE_ADDR,buf,len);
		}
}

//上一组环流数据
void  nextH2SIndex()
{
		if(dispH2STotlNum==0)
				return;
		else{
				dispH2SIndex++;
				if(dispH2SIndex>=dispH2STotlNum)
						dispH2SIndex=0;
		}	
}
//下一组环流数据
void  lastH2SIndex()
{
		if(dispH2STotlNum==0)
				return;
		else{
			if(dispH2SIndex==0)
					dispH2SIndex=dispH2STotlNum-1;
			else
					dispH2SIndex--;
		}
}




extern float co[CO_485_NUM];
static int dispCOIndex=0;
int dispCOTotlNum=0;
//显示环流界面71.bmp的所有数据
void  dispCOData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		if(dispCOTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_CO_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_CO_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_CO_VALUE_ADDR,buf,2);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispCOTotlNum;
				LCDWtite(DISP_DATA_CO_TOTALNUM_ADDR,buf,2);
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.co[dispCOIndex].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CO_ID_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",co[dispCOIndex]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CO_VALUE_ADDR,buf,len);
		}
}

//上一组CO数据
void  nextCOIndex()
{
		if(dispCOTotlNum==0)
				return;
		else{
				dispCOIndex++;
				if(dispCOIndex>=dispCOTotlNum)
						dispCOIndex=0;
		}	
}
//下一组CO数据
void  lastCOIndex()
{
		if(dispCOTotlNum==0)
				return;
		else{
			if(dispCOIndex==0)
					dispCOIndex=dispCOTotlNum-1;
			else
					dispCOIndex--;
		}
}
