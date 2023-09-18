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
//extern char outName[OUTNAME_NUM][INOUTNAME_LEN];
static char nowIndex=0;
static char totalIndex=0;
static char index[COVER_485_NUM]={0};
extern bool  coverUartOnOff(char *ID,bool onOffFlag);
uint8_t coverLevel[COVER_485_NUM]={0};//记录上次井盖开关状态  重启后丢失 保存的ram里
void coverDispOnOff(uint8_t flag);

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
	 coverDispOnOff(coverLevel[index[nowIndex]]);

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
	 coverDispOnOff(coverLevel[index[nowIndex]]);

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
   coverDispOnOff(coverLevel[index[nowIndex]]);
}






void coverDispOnOff(uint8_t flag)
{
	uint8_t buf[10];
	int len=0;
	switch (flag){
		case 0:
			buf[len++]='O';
			buf[len++]='N';
			break;
		case 1:
			buf[len++]='O';
			buf[len++]='F';
			buf[len++]='F';
			break;
		case 2:
		buf[len++]='E';
		buf[len++]='R';
		buf[len++]='R';
			break;
		default:
			
		break;
	}
		buf[len++]=0xff;
		buf[len++]=0xff;
		LCDWtite(TEXT_COVER_OPENCLOSERSP_ADDR,buf,len);
}
//开锁  开锁成功on  关锁成功 off  开关锁无回应 err
void  coverOpen(bool openFlag)
{


	 int ret=2;
	if(coverUartOnOff(sheet.cover[index[nowIndex]].ID,openFlag)==true){//打开成功
	//if(1){
			if(openFlag==true){
			  //strcpy((char *)buf,"ON");//拷贝注意内存溢出  会拷贝0进去
				ret=0;
			}
			else{
				ret=1;
			}
		}
		else{
				ret=2;
			}
		coverLevel[index[nowIndex]]=ret;
		coverDispOnOff(ret);
}
//关锁
//void coverClose()
//{
//	 LCDClearOpen();
//	 LCDClearClose();
//	 uint8_t buf[12];

//		if(coverUartOnOff(sheet.cover[index[nowIndex]].ID,false)==true)//关闭成功
//			strcpy((char *)buf,"OFF SUCC");
//		else
//			strcpy((char *)buf,"OFF FAIL");
//	 buf[9]=0xff;
//	 buf[10]=0xff;
//		LCDWtite(TEXT_COVER_CLOSERESP_ADDR,buf,10);
//}
