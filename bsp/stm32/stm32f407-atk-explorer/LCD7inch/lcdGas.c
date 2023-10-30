
#include  "board.h"

extern float ch4[CH4_485_NUM];
static int dispCH4Index=0;
static int dispCH4TotlNum=0;
extern int ch4State(int i);

const char o2SafeValue[] =">19.5%VOL";
const char coSafeValue[] ="<24ppm";
const char h2sSafeValue[]="<6ppm";
const char ch4afeValue[] ="<10%LEV";

//显示环流界面71.bmp的所有数据
void  dispCH4Data()
{
	  uint8_t buf[MODBID_LEN+2+10]={0};
		dispCH4TotlNum=0;
	  for(int i=0;i<CH4_485_NUM;i++){
			if(sheet.ch4[i].workFlag==RT_TRUE){
						dispCH4TotlNum++;
				}
		}
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
			  LCDWtite(DISP_DATA_CH4_REDUID_ADDR,buf,2);
        LCDDispModbusState(-1,DISP_DATA_CH4_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispCH4TotlNum;
				LCDWtite(DISP_DATA_CH4_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < CH4_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.ch4[i].workFlag==RT_TRUE){
							  if(j==dispCH4Index){
									k=i;
								}
								j++;
						}
				}

				//显示idr
			  int len=0;
				int reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.ch4[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CH4_ID_ADDR,buf,len);

				len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.ch4[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CH4_REDUID_ADDR,buf,len);
				
				
				
				sprintf((char *)buf,"%0.1f%%LEL",ch4[k]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CH4_VALUE_ADDR,buf,len);
				
				len=strlen((char *)ch4afeValue);
				memcpy(buf,ch4afeValue,len);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CH4_SAFEVAL_ADDR,buf,len);
				LCDDispModbusState(ch4State(k),DISP_DATA_CH4_STATE_ADDR);
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
int o2State(int i);
//显示环流界面71.bmp的所有数据
void  dispO2Data()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispO2TotlNum=0;
	  for(int i=0;i<O2_485_NUM;i++){
			if(sheet.o2[i].workFlag==RT_TRUE){
						dispO2TotlNum++;
				}
		}
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
				LCDWtite(DISP_DATA_CH4_REDUID_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_O2_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispO2TotlNum;
				LCDWtite(DISP_DATA_O2_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < O2_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.o2[i].workFlag==RT_TRUE){
							  if(j==dispO2Index){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0;
				int reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.o2[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_O2_ID_ADDR,buf,len);
				
	      len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.o2[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_O2_REDUID_ADDR,buf,len);
				
				sprintf((char *)buf,"%0.1f%%VOL",o2[k]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_O2_VALUE_ADDR,buf,len);
				len=0;
        memcpy(buf,o2SafeValue,strlen(o2SafeValue));
				len=strlen((char *)o2SafeValue);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_O2_SAFEVAL_ADDR,buf,len);
				LCDDispModbusState(o2State(k),DISP_DATA_O2_STATE_ADDR);
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
int h2sState(int i);
//显示H2S界面71.bmp的所有数据
void  dispH2SData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispH2STotlNum=0;
	  for(int i=0;i<H2S_485_NUM;i++){
			if(sheet.h2s[i].workFlag==RT_TRUE){
						dispH2STotlNum++;
				}
		}
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
				LCDWtite(DISP_DATA_CH4_REDUID_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_H2S_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispH2STotlNum;
				LCDWtite(DISP_DATA_H2S_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < H2S_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.h2s[i].workFlag==RT_TRUE){
							  if(j==dispH2SIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0,reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.h2s[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_H2S_ID_ADDR,buf,len);
	      len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.h2s[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_H2S_REDUID_ADDR,buf,len);
				sprintf((char *)buf,"%0.1fppm",h2s[k]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_H2S_VALUE_ADDR,buf,len);
				

				len=strlen((char *)h2sSafeValue);
				strcpy((char *)buf,h2sSafeValue);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_H2S_SAFEVAL_ADDR,buf,len);
				LCDDispModbusState(h2sState(k),DISP_DATA_H2S_STATE_ADDR);
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
int coState(int i);
//显示环流界面71.bmp的所有数据
void  dispCOData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispCOTotlNum=0;
	  for(int i=0;i<CO_485_NUM;i++){
			if(sheet.co[i].workFlag==RT_TRUE){
						dispCOTotlNum++;
				}
		}
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
			  LCDWtite(DISP_DATA_CH4_REDUID_ADDR,buf,2);
				LCDDispModbusState(-1,DISP_DATA_CO_STATE_ADDR);
		}
		else{
				//显示总页数
				buf[0]=0;
				buf[1]=dispCOTotlNum;
				LCDWtite(DISP_DATA_CO_TOTALNUM_ADDR,buf,2);
			
				int j=0,k=0;
				for (int i = 0; i < CO_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.co[i].workFlag==RT_TRUE){
							  if(j==dispCOIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0,reduLen=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.co[k].ID[len];
					  if(buf[len]==0){
							  reduLen=len;
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CO_ID_ADDR,buf,len);
	      len=0;
			  for(int i=reduLen-3;i<reduLen;i++,len++){
						buf[len]=sheet.co[k].ID[i];
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_CO_REDUID_ADDR,buf,len);
				sprintf((char *)buf,"%0.1fppm",co[k]);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CO_VALUE_ADDR,buf,len);
				
				len=strlen((char *)coSafeValue);
			  strcpy((char *)buf,coSafeValue);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_CO_SAFEVAL_ADDR,buf,len);
				LCDDispModbusState(coState(k),DISP_DATA_CO_STATE_ADDR);
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
