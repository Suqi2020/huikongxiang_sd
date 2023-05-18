/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include "7inchRegist.h"
const static char sign[]="[LCDCover]";
extern char outName[OUTNAME_NUM][INOUTNAME_LEN];
static char nowIndex=0;
static char totalIndex=0;
static char index[COVER_485_NUM]={0};
extern bool  coverUartOnOff(char *ID,bool onOffFlag);


void LCDClearOpen()
{
	 uint8_t buf[2];
	 buf[0]=0xff;
	 buf[1]=0xff;
	
	 LCDWtite(TEXT_COVER_OPENRESP_ADDR,buf,2);
}
void LCDClearClose()
{
	 uint8_t buf[2];
	 buf[0]=0xff;
	 buf[1]=0xff;
	
	 LCDWtite(TEXT_COVER_CLOSERESP_ADDR,buf,2);
}
//登记当前所有index
static void signIndex()
{
	    totalIndex=0;
			for(int j=0;j<COVER_485_NUM;j++){//核对有没有配置过
				if(sheet.cover[j].workFlag==true){
					 
					 index[totalIndex]=j;
					 totalIndex++;
				}
		}
}
//界面显示
static void coverDisp()
{
	  uint8_t  buf[MODBID_LEN+5];	
	//显示ID
		int Len=strlen(sheet.cover[index[nowIndex]].ID);
		for(int i=0;i<Len;i++){
				buf[i]=sheet.cover[index[nowIndex]].ID[i];
		}
		int j1=0;
		while((Len+j1)<MODBID_LEN-2){
				buf[Len+j1]=0xff;
				j1++;
			  if(j1>=2)
					break;
		}
	  LCDWtite(TEXT_COVER_ID_ADDR,buf,MODBID_LEN-2);//7寸屏显示18

		//显示addr
		buf[0]=0;
		buf[1]=sheet.cover[index[nowIndex]].slaveAddr;
		LCDWtite(DATA_COVER_ADDR_ADDR,buf,2);
		
		//显示totalIndex
		buf[0]=0;
		buf[1]=totalIndex;
		LCDWtite(DATA_COVER_TOTALPAGE_ADDR,buf,2);
		
		
		//显示nowIndex
		buf[0]=0;
		buf[1]=nowIndex+1;
		LCDWtite(DATA_COVER_NOWPAGE_ADDR,buf,2);

}
void coverNowDisp()
{
	 signIndex();//得到所有的index总数
   if(totalIndex==0)
		 return;
	 nowIndex=0;
	 coverDisp();
	 LCDClearOpen();
	 LCDClearClose();
}
//下一个井盖
void coverNextDisp()
{
	 if(totalIndex==0)
		 return;
	 nowIndex++;
	 if(nowIndex>=totalIndex)
		 nowIndex=0;
	 coverDisp();
	 LCDClearOpen();
	 LCDClearClose();
}
//上一个井盖
void coverLastDisp()
{
	  if(totalIndex==0)
		  return;
	 if(nowIndex==0)
		 nowIndex=totalIndex-1;
	 else
		 nowIndex--;
	 coverDisp();
	 LCDClearOpen();
	 LCDClearClose();
}




//开锁
void coverOpen()
{
		LCDClearOpen();
	 LCDClearClose();
	 uint8_t buf[10];

		if(coverUartOnOff(sheet.cover[index[nowIndex]].ID,true)==true){//打开成功
			strcpy((char *)buf,"ON SUCC");//拷贝注意内存溢出  会拷贝0进去
		}
		else{
			strcpy((char *)buf,"ON FAIL");//拷贝注意内存溢出  会拷贝0进去
		}
	  buf[7]=0xff;
	  buf[8]=0xff;
		LCDWtite(TEXT_COVER_OPENRESP_ADDR,buf,9);
}
//关锁
void coverClose()
{
	 LCDClearOpen();
	 LCDClearClose();
	 uint8_t buf[12];

		if(coverUartOnOff(sheet.cover[index[nowIndex]].ID,false)==true)//关闭成功
			strcpy((char *)buf,"OFF SUCC");
		else
			strcpy((char *)buf,"OFF FAIL");
	 buf[9]=0xff;
	 buf[10]=0xff;
		LCDWtite(TEXT_COVER_CLOSERESP_ADDR,buf,10);
}
