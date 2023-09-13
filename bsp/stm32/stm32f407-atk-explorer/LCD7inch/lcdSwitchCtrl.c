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
const static char sign[]="[LCDSwtich]";
extern char outName[OUTNAME_NUM][INOUTNAME_LEN];
int switchInterfaceIndex=0;
void  dispLevelState(char level);
bool DOLevel[DO_NUM]={0};
bool SWITCHLevel[SWITCH_NUM]={0};



//向接口框显示接口名称
void dispInterFaceIndexFun()
{
		rt_kprintf("%\n ");
		for(int i=0;i<INOUTNAME_LEN;i++){//ACUID_LEN
			 	rt_kprintf("%0x ",outName[switchInterfaceIndex][i]);
		}
		rt_kprintf("%\n ");
		LCDWtite(DISP_SWITCH_INTERFACE_ADDR,			(uint8_t *)outName[switchInterfaceIndex],INOUTNAME_LEN);
		//LCDWtite(DISP_SWITCHINTERF_INTERFACE_ADDR,(uint8_t *)outName[switchInterfaceIndex],INOUTNAME_LEN);
}
//接口名称增加
void dispInterFaceIndexAdd()
{
	switchInterfaceIndex++;
	if(switchInterfaceIndex==OUTNAME_NUM)
		switchInterfaceIndex=0;
}
//接口名称减少
void dispInterFaceIndexReduc()
{

	if(switchInterfaceIndex==0){
		switchInterfaceIndex=OUTNAME_NUM-1;
	}
  else
		switchInterfaceIndex--;
}


extern char outSwitchNum[OUTNAME_NUM];
//int SwitchPortIndexX=0;//-OUTNAME_NUM
int SwitchPortIndexY=0;//outSwitchNum[0]数量

void rstPortIndexFun()
{
		SwitchPortIndexY=0;
}
//显示port接口
void dispPortIndexFun()
{
		uint8_t buf[2];
	  buf[0]=0;
	  buf[1]=SwitchPortIndexY+1;
	
	
		LCDWtite(DISP_SWITCHPORT_PORT_ADDR,buf,1*2);

		LCDWtite(DISP_SWITCH_PORT_ADDR,buf,1*2);
	
}
void dispPortIndexAdd()
{
	SwitchPortIndexY++;
	if(SwitchPortIndexY==outSwitchNum[switchInterfaceIndex])

		SwitchPortIndexY=0;
	
	
}
void dispPortIndexLow()
{
		if(SwitchPortIndexY==0)
			SwitchPortIndexY=outSwitchNum[switchInterfaceIndex]-1;
		else
			SwitchPortIndexY--;
}




//void printfDOList()
//{
//		for(int j=0;j<DO_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
//				if(packFlash.digoutput[j].workFlag==RT_TRUE){//打开
//						rt_kprintf("%s digOutput ",sign);
//						rt_kprintf("%s ",packFlash.digoutput[j].name);
//						rt_kprintf("%s ",packFlash.digoutput[j].devID);
//						rt_kprintf("%s ",packFlash.digoutput[j].model);
//						rt_kprintf("%d \n",packFlash.digoutput[j].port);
//				}
//		}
//}


//void printfPower3V3List()
//{
//		for(int j=0;j<V33O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
//				if(packFlash.v33output[j].workFlag==RT_TRUE){//打开
//						rt_kprintf("%s power3V3 ",sign);
//						rt_kprintf("%s ",packFlash.v33output[j].name);
//						rt_kprintf("%s ",packFlash.v33output[j].devID);
//						rt_kprintf("%s ",packFlash.v33output[j].model);
//						rt_kprintf("%d \n",packFlash.v33output[j].port);
//				}
//		}
//}
////打印5V的输出配置列表
//void printfPower5VList()
//{
//		for(int j=0;j<V5O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
//				if(packFlash.v5output[j].workFlag==RT_TRUE){//打开
//						rt_kprintf("%s power5V ",sign);
//						rt_kprintf("%s ",packFlash.v5output[j].name);
//						rt_kprintf("%s ",packFlash.v5output[j].devID);
//						rt_kprintf("%s ",packFlash.v5output[j].model);
//						rt_kprintf("%d \n",packFlash.v5output[j].port);
//				}
//		}
//}

////打印12V的输出列表
//void printfPower12VList()
//{
//		for(int j=0;j<SWITCH_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
//				if(packFlash.v5output[j].workFlag==RT_TRUE){//打开
//						rt_kprintf("%s power12V ",sign);
//						rt_kprintf("%s ",packFlash.switchoutput[j].name);
//						rt_kprintf("%s ",packFlash.switchoutput[j].devID);
//						rt_kprintf("%s ",packFlash.switchoutput[j].model);
//						rt_kprintf("%d \n",packFlash.switchoutput[j].port);
//				}
//		}
//}
//显示输出控制的名称 ID号等
//注意：需要与outSwitchNum[OUTNAME_NUM]={DO_NUM,V33O_NUM,V5O_NUM,SWITCH_NUM};
//      对应起来
void  dispOutputNameIDType()
{
		 uint8_t *buf=NULL;
	  buf=rt_malloc(50);
//	  int index=0;
	  bool findFlag=false;
		switch(switchInterfaceIndex)
		{
			case 0://DO
					for(int k=0;k<DO_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
						if(packFlash.digoutput[k].port==SwitchPortIndexY+1){
							if(packFlash.digoutput[k].workFlag==RT_TRUE){//打开
//									rt_kprintf("%s digOutput ",sign);
//									rt_kprintf("%s ",packFlash.digoutput[k].name);
//									rt_kprintf("%s ",packFlash.digoutput[k].devID);
//									rt_kprintf("%s \n",packFlash.digoutput[k].model);
									int Len;
									Len=strlen(packFlash.digoutput[k].name);
									for(int i=0;i<Len;i++){
											buf[i]=packFlash.digoutput[k].name[i];
									}
									int j=0;
									while((Len+j)<NAME_LEN){
											buf[Len+j]=0xff;
											j++;
									}
									LCDWtite(DISP_SWITCH_NAME_ADDR,buf,NAME_LEN);
									Len=strlen(packFlash.digoutput[k].devID);
									for(int i=0;i<Len;i++){
										buf[i]=packFlash.digoutput[k].devID[i];
									}
									 j=0;
									while((Len+j)<MODBID_LEN-2){
										buf[Len+j]=0xff;
										j++;
										if(j>=2)
											break;
									}
									LCDWtite(DISP_SWITCH_ID_ADDR,buf,MODBID_LEN-2);//7寸屏显示18

									Len=strlen(packFlash.digoutput[k].model);
									for(int i=0;i<Len;i++){
										buf[i]=packFlash.digoutput[k].model[i];
									}
									j=0;
									while((Len+j)<MODEL_LEN){
										buf[Len+j]=0xff;
										j++;
										if(j>=2)
											break;
									}
		
									LCDWtite(DISP_SWITCH_TYPE_ADDR,buf,MODEL_LEN);//7寸屏显示18
									findFlag=true;
									
									
									//dispLevelState(DOLevel[k]);
							}
							break;
						}
					}
				break;
			
			case 1://SWITCH
				for(int k=0;k<SWITCH_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
					if(packFlash.switchoutput[k].port==SwitchPortIndexY+1){
						if(packFlash.switchoutput[k].workFlag==RT_TRUE){//打开
//								rt_kprintf("%s switchOutput ",sign);
//								rt_kprintf("%s ",packFlash.switchoutput[k].name);
//								rt_kprintf("%s ",packFlash.switchoutput[k].devID);
//								rt_kprintf("%s \n",packFlash.switchoutput[k].model);
								int Len;
								Len=strlen(packFlash.switchoutput[k].name);
								for(int i=0;i<Len;i++){
										buf[i]=packFlash.switchoutput[k].name[i];
								}
								int j=0;
								while((Len+j)<NAME_LEN){
										buf[Len+j]=0xff;
										j++;
								}
								LCDWtite(DISP_SWITCH_NAME_ADDR,buf,NAME_LEN);
								Len=strlen(packFlash.switchoutput[k].devID);
								for(int i=0;i<Len;i++){
									buf[i]=packFlash.switchoutput[k].devID[i];
								}
								 j=0;
								while((Len+j)<MODBID_LEN-2){
									buf[Len+j]=0xff;
									j++;
									if(j>=2)
										break;
								}
								LCDWtite(DISP_SWITCH_ID_ADDR,buf,MODBID_LEN-2);//7寸屏显示18

								Len=strlen(packFlash.switchoutput[k].model);
								for(int i=0;i<Len;i++){
									buf[i]=packFlash.switchoutput[k].model[i];
								}
								j=0;
								while((Len+j)<MODEL_LEN){
									buf[Len+j]=0xff;
									j++;
									if(j>=2)
										break;
								}

								LCDWtite(DISP_SWITCH_TYPE_ADDR,buf,MODEL_LEN);//7寸屏显示18
								findFlag=true;
								//dispLevelState(SWITCHLevel[k]);
						}
						break;
					}
				}

				break;
		}
		if(findFlag==false){
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_SWITCH_NAME_ADDR,buf,2);
				LCDWtite(DISP_SWITCH_ID_ADDR,buf,2);
				LCDWtite(DISP_SWITCH_TYPE_ADDR,buf,2);
		}
		rt_free(buf);
		buf=RT_NULL;
}

//io数据显示
void  dispLevelState(char level)
{
	  uint8_t buf[10]={0};
		int len=0;
		if(level==0){
				buf[len++]='O';
			  buf[len++]='F';
				buf[len++]='F';
				buf[len++]=0XFF;
				buf[len++]=0XFF;
		}
		else{
				buf[len++]='O';
			  buf[len++]='N';
				buf[len++]=0XFF;
				buf[len++]=0XFF;
		}
		LCDWtite(DISP_SWITCH_LEVEL_ADDR,buf,len);
}


void digOutputONFun(char num);
void digOutputOFFFun(char num);
void v33OutputONFun(char num);
void v33OutputOFFFun(char num);
void v5OutputONFun(char num);
void v5OutputOFFFun(char num);
void switchOutputONFun(char num);
void switchOutputOFFFun(char num);



void levelSet(char level)
{
	  
		switch(switchInterfaceIndex)
		{
			case 0://DO
				if(level==0){
						digOutputOFFFun(SwitchPortIndexY);
				}
				else{
						digOutputONFun(SwitchPortIndexY);
				}
				break;
			case 1://SWITCH
				if(level==0){
						switchOutputOFFFun(SwitchPortIndexY);
				}
				else{
						switchOutputONFun(SwitchPortIndexY);
				}
				break;
			
		}
}
bool digOutputReadFun(char num);
bool v33OutputReadFun(char num);
bool v5OutputReadFun(char num);
bool switchOutputReadFun(char num);

//不用配置do和switch 即可使用
bool levelRead()
{
	
	  rt_kprintf("levelread x=%d,y=%d\n",switchInterfaceIndex,SwitchPortIndexY);
		switch(switchInterfaceIndex)
		{
			case 0://DO
	
				return digOutputReadFun(SwitchPortIndexY);
				break;
			case 1://SWITCH
			 return switchOutputReadFun(SwitchPortIndexY);
				break;
			
		}
		return 0;
}
#if 0 //此函数必须在配置DO和SWITCH继电器成功的情况下才能使用  
bool levelRead()
{
	
	  rt_kprintf("levelread x=%d,y=%d\n",switchInterfaceIndex,SwitchPortIndexY);
		switch(switchInterfaceIndex)
		{
			case 0://DO
					for(int k=0;k<DO_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
						if(packFlash.digoutput[k].port==SwitchPortIndexY+1){
							if(packFlash.digoutput[k].workFlag==RT_TRUE){//打开
								return digOutputReadFun(SwitchPortIndexY);
							}
							break;
						}
					}
				break;
//			case 1://V33O
//				for(int k=0;k<V33O_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
//					if(packFlash.v33output[k].port==SwitchPortIndexY+1){
//						if(packFlash.v33output[k].workFlag==RT_TRUE){//打开
//								return v33OutputReadFun(SwitchPortIndexY);
//						}
//						break;
//					}
//				}
//				break;
//			case  2://V5O
//				for(int k=0;k<V5O_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
//					if(packFlash.v5output[k].port==SwitchPortIndexY+1){
//						if(packFlash.v5output[k].workFlag==RT_TRUE){//打开
//								return v5OutputReadFun(SwitchPortIndexY);
//						}
//						break;
//					}
//				}
//				break;
			case 1://SWITCH
				for(int k=0;k<SWITCH_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
					if(packFlash.switchoutput[k].port==SwitchPortIndexY+1){
						if(packFlash.switchoutput[k].workFlag==RT_TRUE){//打开
								return switchOutputReadFun(SwitchPortIndexY);
						}
						break;
					}
				}
				break;
			
		}
		return 0;
}


//此函数必须在配置DO和SWITCH继电器成功的情况下才能使用  
void levelSet(char level)
{
	  
		switch(switchInterfaceIndex)
		{
			case 0://DO
					for(int k=0;k<DO_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
						if(packFlash.digoutput[k].port==SwitchPortIndexY+1){
							if(packFlash.digoutput[k].workFlag==RT_TRUE)
							{//打开
								if(level==0){
										digOutputOFFFun(SwitchPortIndexY);
									  

								}
								else{
										digOutputONFun(SwitchPortIndexY);

								}

							}
							break;
						}
					}
				break;
			case 1://SWITCH
				for(int k=0;k<SWITCH_NUM;k++){//查一遍 找到 GYNJLXSD000000499  如果
					if(packFlash.switchoutput[k].port==SwitchPortIndexY+1){
						if(packFlash.switchoutput[k].workFlag==RT_TRUE)
							{//打开
								if(level==0){
										switchOutputOFFFun(SwitchPortIndexY);
								}
								else{
										switchOutputONFun(SwitchPortIndexY);
								}
		
						}
						break;
					}
				}
				break;
			
		}
}

#endif