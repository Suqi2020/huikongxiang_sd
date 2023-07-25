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
const static char sign[]="[LCDKeyEvent]";



void dispInterFaceIndexFun(void);
//接口名称增加
void dispInterFaceIndexAdd(void);
//接口名称减少
void dispInterFaceIndexReduc(void);


void	LCDDispIP(void);
void	LCDDispUart(void);
void	LCDDispMCUID(void);
void  LCDDispNetOffline(void);
void  offLineIndexAdd(void);
void  dispCinaName(uint8_t *buf);
void  dispChinaNameIndexLow(void);
void  dispChinaNameIndexAdd(void);
void  LDCDispMosbusInfo(void);
void  keyModbusCfgSure(void);
void  modbDevReadIndexLow(void);
void  modbDevReadIndexAdd(void);
void	modbErrDevReadIndexLow(void);
void	LDCDispErrMosbusInfo(void);
void  modbErrDevReadIndexAdd(void);
void	delModbusDevbyID_p(void);
void	LCDDispModbusGet(void);
void	LCDDispErrModbusGet(void);
void	offLineIndexLow(void);
void  dispPortIndexLow(void);
void  dispPortIndexFun(void);
void  rstPortIndexFun(void);
void  dispPortIndexAdd(void);
void  dispOutputNameIDType(void);
void  dispLevelState(char level);
bool levelRead(void);
void levelSet(char level);
void  lcdInputConfig(void);
void	LCDClearSaveOK(void);
void	LCDClearRstOK(void);

void LCDDispSaveOK(void);

void LCDDispRstOK(void);
void getInputTotalNum(void);
void 	dispInput(void);
void 	lastInput(void);
void 	nextInput(void);
void delOneInput(void);
void  dispoutputReadInterf(void);
//输出设置读取interface接口下一个
void  dispoutpReadInterfNext(void);
void  dispoutpReadInterfLast(void);
void  dispOutputRead(void);
void  getOutputTotalNum(void);
void  dispoutpReadNext(void);
void  dispoutpReadLast(void);
void	lcdOutputConfig(void);
void	dispoutConfigLast(void);
void	dispoutputNameInterf(void);
void	dispoutConfigNext(void);
void	delOneOutput(void);
void  getAnaTotalNum(void);
void  dispReadAna(void);
void  lastReadAna(void);
void  nextReadAna(void);

void 	LCDClearClose(void);
void	LCDClearOpen(void);


void dispName(void);
void dispSubName(void);
void nextSubName(void);
void lastSubName(void);
void coverClose(void);
void coverOpen(void);
void coverLastDisp(void);
void coverNextDisp(void);
void coverNowDisp(void);
//void LCDDispNetErrState(void);
void surePassWord(void);
void LCDRstPw(void);

void dispChooseModbusName(void);

void  nextModName(void);

void  lastModName(void);
void  changeBmp(int num);
void  dispCurrentModData(void);

void  dispHuanliuData(void);
void  nextHuanliuIndex(void);
void  lastHuanliuIndex(void);
void  dispJufangData(void);
void  nextJufangIndex(void);
void  lastJufangIndex(void);
void  dispChenjiangData(void);
void  nextChenJiangIndex(void);
void  lastChenJiangIndex(void);
void  dispWaipoData(void);
void  lastWaipoIndex(void);
void  nextWaipoIndex(void);
void  dispCH4Data(void);
void  nextCH4Index(void);
void  lastCH4Index(void);
void  dispO2Data(void);
void  nextO2Index(void);
void  lastO2Index(void);
void  dispH2SData(void);
void  nextH2SIndex(void);
void  lastH2SIndex(void);
void  dispCOData(void);
void  nextCOIndex(void);
void  lastCOIndex(void);
void  dispWenshiduData(void);
void  nextWenshiduIndex(void);
void  lastWenshiduIndex(void);
void  dispWaterData(void);
void  nextWaterIndex(void);
void  lastWaterIndex(void);
void  dispLiefengData(void);
void  nextLiefengIndex(void);
void  lastLiefengIndex(void);
void  nextJinggaiIndex(void);
void  lastJinggaiIndex(void);
void  dispJinggaiData(void);
void  LCDDispSDState(uint8_t state);
void  LCDDispSoftVer(void);
void LCDDispSDSize();
void ledOnOff(uint8_t value)
{
		uint8_t buf[2];
	  buf[0]=0;
	  buf[1]=value;
	
	
		LCDWtite(0x0ff0,buf,1*2);

}

void rstLCD()
{
		uint8_t buf[4]={0x55,0xAA,0x5A,0xA5};
	
		LCDWtite(0x0004,buf,4);

}
extern int modbusChosIndex;
#ifndef     ANA_MASK
void lcdAnaConfig(void);
void  delOneAna(void);
#endif
//按键触发总接口
void  keyReturn(uint16_t keyAddr)
{
	  uint8_t *buf=NULL;
	  buf=rt_malloc(50);
	  bool ret;
	 // int nameLen=0;
		switch(keyAddr)
		{
			
			case KEY_SURE_CHOOSE_MODBUS_ADDR:
				changeBmp(71+modbusChosIndex);//71.bmp为环流数据展示
			  dispCurrentModData();
				break;
			case KEY_LAST_CHOOSE_MODBUS_ADDR:
				lastModName();
			  dispChooseModbusName();
				break;
			case KEY_NEXT_CHOOSE_MODBUS_ADDR:
				nextModName();
			  dispChooseModbusName();
				break;
			case KEY_MODUBS_DATA_ADDR:
				dispChooseModbusName();
				break ;	
			case KEY_DATA_HUANLIU_LAST_ADDR:
				lastHuanliuIndex();
				dispHuanliuData();
				break;
			case KEY_DATA_HUANLIU_NEXT_ADDR:
				nextHuanliuIndex();
				dispHuanliuData();
				break;
			case	KEY_DATA_JUFANG_LAST_ADDR:
				lastJufangIndex();
				dispJufangData();
				break;
			case 	KEY_DATA_JUFANG_NEXT_ADDR:
				nextJufangIndex();
				dispJufangData();
				break;
			case	KEY_DATA_CEHNJIANG_LAST_ADDR:
				lastChenJiangIndex();
				dispChenjiangData();
				break;
			case	KEY_DATA_CEHNJIANG_NEXT_ADDR:
				nextChenJiangIndex();
				dispChenjiangData();
				break;
			case	KEY_DATA_FANGWAIPO_LAST_ADDR:
				lastWaipoIndex();
				dispWaipoData();
				break;
      case 	KEY_DATA_FANGWAIPO_NEXT_ADDR:
				nextWaipoIndex();
				dispWaipoData();
				break;

			case	KEY_DATA_CH4_LAST_ADDR:
				lastCH4Index();
			  dispCH4Data();
				break;
      case 	KEY_DATA_CH4_NEXT_ADDR:
				nextCH4Index();
			  dispCH4Data();
				break;
			case	KEY_DATA_O2_LAST_ADDR:
				lastO2Index();
			  dispO2Data();
				break;
      case 	KEY_DATA_O2_NEXT_ADDR:
				nextO2Index();
			  dispO2Data();
				break;
			case	KEY_DATA_H2S_LAST_ADDR:
				lastH2SIndex();
			  dispH2SData();
				break;
      case 	KEY_DATA_H2S_NEXT_ADDR:
				nextH2SIndex();
			  dispH2SData();
				break;
			case	KEY_DATA_CO_LAST_ADDR:
				lastCOIndex();
			  dispCOData();
				break;
      case 	KEY_DATA_CO_NEXT_ADDR:
				nextCOIndex();
			  dispCOData();
				break;
			case	KEY_DATA_WENSHIDU_LAST_ADDR:
				lastWenshiduIndex();
			  dispWenshiduData();
				break;
      case 	KEY_DATA_WENSHIDU_NEXT_ADDR:
				nextWenshiduIndex();
				dispWenshiduData();
				break;
			case	KEY_DATA_WATER_LAST_ADDR:
				lastWaterIndex();
				dispWaterData();
				break;
      case 	KEY_DATA_WATER_NEXT_ADDR:
				nextWaterIndex();
				dispWaterData();
				break;
			case	KEY_DATA_LIEFENG_LAST_ADDR:
				lastLiefengIndex();
				dispLiefengData();
				break;
      case 	KEY_DATA_LIEFENG_NEXT_ADDR:
				nextLiefengIndex();
				dispLiefengData();
				break;
			case	KEY_DATA_COVER_LAST_ADDR:
				lastJinggaiIndex();
				dispJinggaiData();
				break;
      case 	KEY_DATA_COVER_NEXT_ADDR:
				nextJinggaiIndex();
				dispJinggaiData();
				break;
			case KEY_MODBUS_CFG_NAME_ADDR://点击传感器设置名称显示框调出 模糊界面  显示到1380
				//5A A5 1182 1380 C9CF BAA3 B9C8 D4AA BFC6 BCBC FFFF
        dispCinaName(buf);
				break;
			case KEY_MODBUS_LASTNAME_ADDR:
				dispChinaNameIndexLow();
				dispCinaName(buf);
				break;
			case KEY_MODBUS_NEXTNAME_ADDR:
				dispChinaNameIndexAdd();
				dispCinaName(buf);
				break;
			case KEY_MODBUS_SURENAME_ADDR:// 显示到1360
				break;	
			case KEY_IP_READ_ADDR:
				LCDDispIP();
				break;
			case KEY_PORT_READ_ADDR:
				LCDDispUart();
				break;
			case KEY_MCUID_READ_ADDR:
				LCDDispMCUID();
				break;
			case KEY_SAVE_ADDR://保存
				
				rt_kprintf("%sflash save OK\n",sign);
				stm32_flash_erase(FLASH_IP_SAVE_ADDR, sizeof(packFlash));//每次擦除128k字节数据 存储时候需要一起存储
				stm32_flash_write(FLASH_IP_SAVE_ADDR,(uint8_t*)&packFlash,sizeof(packFlash));
				stm32_flash_write(FLASH_MODBUS_SAVE_ADDR,(uint8_t*)&sheet,sizeof(sheet));
			  LCDDispSaveOK();
		
				break;
			case KEY_RESET_ADDR://复位
				
			  //LCDDispRstOK();
			  rstLCD();
			  rt_thread_mdelay(500);
			  rt_hw_cpu_reset();
				break;
			case KEY_MODBUS_CFG_WATCH_ADDR:

				LDCDispMosbusInfo();
				break;
			case KEY_MODBUS_CFG_SURE_ADDR:

						
				keyModbusCfgSure();
				break;
			case KEY_MODBUSDISP_LAST_ADDR:
				modbDevReadIndexLow();
				LDCDispMosbusInfo();
				break;

			case  KEY_MODBUSDISP_NEXT_ADDR:
				modbDevReadIndexAdd();
				LDCDispMosbusInfo();
			  break;
			

//////////////////////////////////////////////////

			case KEY_MODBUSDISP_ERRLAST_ADDR:
				modbErrDevReadIndexLow();
				LDCDispErrMosbusInfo();
				break;
			case KEY_MODBUSDISP_ERRNEXT_ADDR:
				modbErrDevReadIndexAdd();
				LDCDispErrMosbusInfo();
				break;
			case  KEY_MODBUSDISP_DEL_ADDR:
				delModbusDevbyID_p();
			  LCDDispModbusGet();
			  LDCDispMosbusInfo();
				break;
			case  KEY_NETERROR_ADDR:
				//rt_kprintf("%s按键按下\n",sign);
				LCDDispNetOffline();
//		  	LCDDispNetErrState();
				break;
			case  KEY_MODBUSERR_ADDR:{
				LCDDispErrModbusGet();
				LDCDispErrMosbusInfo();
				void  LCDDispErrMosbusState();
				LCDDispErrMosbusState();}
				break;

			case  KEY_SWITCH_INTERFACE_ADDR:
				rt_kprintf("%sKEY_SWITCH_INTERFACE_ADDR \n",sign);
				dispInterFaceIndexFun();
			  dispPortIndexFun();
				break;
			case KEY_SWITCH_PORT_ADDR:
				break;
			case KEY_SWITCH_LEVEL_ADDR:
				break;
			
			case KEY_SWITCH_SURE_ADDR:
				break;
			
			case KEY_SWITCH_RETURN_ADDR:
				break;
			case  KEY_INPUT_ADDR:
				LCDRstDispSameID(DISP_INPUT_SAME_ID_MSG_ADDR);
			  break;
			case  KEY_OUTPUT_ADDR:
				LCDRstDispSameID(DISP_OUTPUT_SAME_ID_MSG_ADDR);
			  break;
			case  KEY_MODUBS_ADDR:
				LCDRstDispSameID(DISP_MODBUS_SAME_ID_MSG_ADDR);
			  break;

			case KEY_SWITCHINTERF_SURE_ADDR://do nothing
				break;
			case KEY_SWITCHINTERF_NEXT_ADDR:
				rt_kprintf("%sKEY_SWITCHINTERF_NEXT_ADDR \n",sign);
				dispInterFaceIndexAdd();
			  dispInterFaceIndexFun();
				rstPortIndexFun();
			  dispPortIndexFun();
			  dispOutputNameIDType();
			  ret=levelRead();
			  dispLevelState(ret);
			  levelSet(ret);
				break;
			case KEY_SWITCHINTERF_LAST_ADDR:
				rt_kprintf("%sKEY_SWITCHINTERF_LAST_ADDR \n",sign);
				dispInterFaceIndexReduc();
			  dispInterFaceIndexFun();
			  rstPortIndexFun();
			  dispPortIndexFun();
			  dispOutputNameIDType();
			  ret=levelRead();
			  dispLevelState(ret);
			  levelSet(ret);
				break;
			case KEY_SWITCHINTERF_RETURN_ADDR://
				
				break;
			case KEY_SWITCHPORT_SURE_ADDR:
				break;
			case KEY_SWITCHPORT_NEXT_ADDR:
				dispPortIndexAdd();
			  dispPortIndexFun();
			  dispOutputNameIDType();
				ret=levelRead();
			  dispLevelState(ret);
			  levelSet(ret);
				break;
			case KEY_SWITCHPORT_LAST_ADDR:
				dispPortIndexLow();
			  dispPortIndexFun();
			  dispOutputNameIDType();
				ret=levelRead();
			  dispLevelState(ret);
			  levelSet(ret);
				break;
			case KEY_SWITCHPORT_RETURN_ADDR:
				//dispOutputNameIDType();
				break;
			case KEY_SWITCHLEVEL_ON_ADDR:
				rt_kprintf("%s ON\n",sign);
			  dispLevelState(1);
			  levelSet(1);
				break;
			case KEY_SWITCHLEVEL_OFF_ADDR:
				rt_kprintf("%s OFF\n",sign);
			  dispLevelState(0);
			  levelSet(0);
				break;
			case KEY_SWITCHLEVEL_SURE_ADDR:
				break;
			case KEY_SWITCHLEVEL_RETURN_ADDR:
				break;
			///////////input_start///////////////
			case KEY_INPUTCFG_SURE_ADDR:
				lcdInputConfig();
				break;
			case KEY_INPUTCFG_LOOK_ADDR:
				getInputTotalNum();
				dispInput();
				break;
			case  KEY_INPUT_LAST_ADDR:
				lastInput();
				dispInput();
				break;
			case  KEY_INPUT_NEXT_ADDR:
				nextInput();
				dispInput();
				break;
			case  KEY_INPUT_DEL_ADDR:
				delOneInput();
			  getInputTotalNum();
				dispInput();
				break;
			case  KEY_INPUT_RETURN_ADDR:
				LCDRstDispSameID(DISP_INPUT_SAME_ID_MSG_ADDR);
				break;

			case KEY_OUTPUT_SURE_ADDR://
				lcdOutputConfig();
				break;
			case KEY_OUTPUT_LAST_ADDR:
				dispoutConfigLast();
				dispoutputNameInterf();
				break;
			case KEY_OUTPUT_NEXT_ADDR:
				dispoutConfigNext();
				dispoutputNameInterf();
				break;
			case KEY_OUTPUT_INTERFACE_ADDR:
				dispoutputNameInterf();
				break;
			case  KEY_OUTPUT_READ_LAST_ADDR:
				dispoutpReadLast();
			  dispOutputRead();
				break;
			case  KEY_OUTPUT_READ_NEXT_ADDR:
				dispoutpReadNext();
			  dispOutputRead();
				break;

			case  KEY_OUTPUT_READ_DELETE_ADDR:
			  delOneOutput();
				getOutputTotalNum();
//				dispoutputReadInterf();
			  dispOutputRead();
				break;
			case  KEY_OUTPUT_LOOK_ADDR:
				getOutputTotalNum();
				dispoutputReadInterf();
			  dispOutputRead();
				break;
			//井盖
			case KEY_COVER_OPEN_ADDR:
				coverOpen();
				break;
			case KEY_COVER_CLOSE_ADDR:
				coverClose();
				break;			
			case KEY_COVER_LAST_ADDR:
				coverLastDisp();
				break;			
			case KEY_COVER_NEXT_ADDR:
				coverNextDisp();
				break;			
			case KEY_COVER_DISPLAY_ADDR:
				coverNowDisp();
				break;
			case KEY_PASSWD_ENTER_ADDR:
				LCDRstPw();
				break;
			case KEY_PASSWD_SURE_ADDR:
				surePassWord();
				break;
			case  KEY_LED_ON_ADDR:
				ledOnOff(0);
				break;
			case  KEY_LED_OFF_ADDR:
				ledOnOff(1);
				break;
			case  KEY_OUTPUT_READ_INTERFACE_ADDR://进去接口
				dispoutputReadInterf();
				dispOutputRead();
				break;
			case  KEY_OUTPUT_READ_LAST_P_ADDR://接口滚动  
				dispoutpReadInterfLast();
			  dispoutputReadInterf();
			  dispOutputRead();
				break;
			case  KEY_OUTPUT_READ_NEXT_P_ADDR:    //接口滚动 
				dispoutpReadInterfNext();
			  dispoutputReadInterf();
			  dispOutputRead();
				break;

			case  KEY_OUTPUT_READ_RETURN_P_ADDR:
				break;
			case KEY_RETURN_ADDR:

				LCDClearSaveOK();
				LCDClearRstOK();
				LCDClearClose();
				LCDClearOpen();
				break;
			case KEY_SOFT_VER_ADDR:
				LCDDispSoftVer();
				break;
			case KEY_SD_STATE_ADDR:
				LCDDispSDState((uint8_t)gbSDExit );
			  LCDDispSDSize();
				break;
			///////////output_end///////////////
//#define        KEY_ANA_SUBNAME_INTERFACE_ADDR     0x522C
//#define        KEY_ANA_SURE_ADDR     0x522E
	#ifndef     ANA_MASK
			case   KEY_ANA_LOOK_ADDR :
				getAnaTotalNum();
			  dispReadAna();
				break;
			case  KEY_ANA_NAME_INTERFACE_ADDR:
				dispName();
				break;
			case  KEY_ANA_SUBNAME_INTERFACE_ADDR:
				dispSubName();
				break;
			case  KEY_ANA_SURE_ADDR:
				lcdAnaConfig();
				break;
			case	KEY_ANA_LAST_ADDR:
				lastSubName();
				dispSubName();
				break;
			
			case  KEY_ANA_NEXT_ADDR:
				nextSubName();
				dispSubName();
				break;
			case   KEY_ANAREAD_LAST_ADDR:
				lastReadAna();
			  dispReadAna();
				break;
			case   KEY_ANAREAD_NEXT_ADDR:
				nextReadAna();
			  dispReadAna();
				break;
			case   KEY_ANAREAD_DEL_ADDR:
				delOneAna();
				getAnaTotalNum();
			  dispReadAna();
				break;
		#endif
		//开关控制end
		}
		rt_free(buf);
		buf=RT_NULL;
		
}




extern  modbusStru LCDInput;
extern  uint32_t   LCDInputTime;


void lcdCopyInputName(uint8_t *rec);
//MODEL_LEN
void lcdCopyInputID(uint8_t *rec);
void lcdCopyInputModel(uint8_t *rec);
void lcdCopyInputPort(uint8_t *rec);
void	lcdCopyOutputName(uint8_t *rec);
void	lcdCopyOutputID(uint8_t *rec);
void	lcdCopyOutputModel(uint8_t *rec);
void	lcdCopyOutputPort(uint8_t *rec);



void lcdCopyAnaID(uint8_t *rec);
//拷贝输入的type到AnaInputLCD中
void lcdCopyAnaModel(uint8_t *rec);
//拷贝输入的port到AnadigInputLCD中
void lcdCopyAnaPort(uint8_t *rec);
//拷贝输入的time到AnaInput中
void lcdCopyAnaTime(uint8_t *rec);
bool checkPassWord(char *rec);
void LCDHidePssd(void);
extern rt_bool_t getUTCFlag;
//lcd 发来的配置解析
void LCDDispConfig(uint8_t *recBuf,int len)
{
	  
	

		
		
		if((uint16_t)((recBuf[0]<<8)+recBuf[1])!=LCD_HEAD){
				rt_kprintf("%s head err\n",sign);
				return;
		}			
		if((uint16_t)(recBuf[2]+2+1)!=len){//+2+1  头部长度2 数据长度1
			 // rt_kprintf("%s lenth err\n",sign);
			
			
			
				if((recBuf[4]<<8)+recBuf[5]== LCD_RUN){
						rt_kprintf("%sLCD resp\n",sign);
				}
						
				return;
		}	
		if(recBuf[3]!=LCD_READ){//+2+1  头部长度2 数据长度1
			  rt_kprintf("%s 0x83 err\n",sign);
				return;
		}				
		uint16_t CMD_ADDR=(uint16_t)(recBuf[4]<<8)+recBuf[5];
		switch(CMD_ADDR){
			case  LOCAL_IP1_ADDR:
				packFlash.netIpFlash.localIp[0]=recBuf[8];
				break;
			case LOCAL_IP2_ADDR:
				packFlash.netIpFlash.localIp[1]=recBuf[8];
				break;       
			case LOCAL_IP3_ADDR:
				packFlash.netIpFlash.localIp[2]=recBuf[8];
				break;        
			case LOCAL_IP4_ADDR:
				packFlash.netIpFlash.localIp[3]=recBuf[8];
				break;        
			case PHY_PORT_ADDR:
				packFlash.netIpFlash.macaddr=recBuf[8];
				break;        
			case REMOTE_IP1_ADDR:
				packFlash.netIpFlash.remoteIp[0]=recBuf[8];
				break;       
			case REMOTE_IP2_ADDR:
				packFlash.netIpFlash.remoteIp[1]=recBuf[8];
				break;        
			case REMOTE_IP3_ADDR:
				packFlash.netIpFlash.remoteIp[2]=recBuf[8];
				break;        
			case REMOTE_IP4_ADDR:
				packFlash.netIpFlash.remoteIp[3]=recBuf[8];
				break;    		 
			case   GATEWAY_IP1_ADDR:
				packFlash.netIpFlash.gateway[0]=recBuf[8];
				break;
			case   GATEWAY_IP2_ADDR:
				packFlash.netIpFlash.gateway[1]=recBuf[8];
				break;
			case   GATEWAY_IP3_ADDR:
				packFlash.netIpFlash.gateway[2]=recBuf[8];
				break;
			case   GATEWAY_IP4_ADDR:
				packFlash.netIpFlash.gateway[3]=recBuf[8];
				break;

			case REMOTE_PORT_ADDR:
				packFlash.netIpFlash.remotePort=(uint16_t)(recBuf[9]<<8)+recBuf[10];
				break;   		 

			case PORT1_ADDR:
				packFlash.uartBps[0]=(uint32_t)(recBuf[7]<<24)+(uint32_t)(recBuf[8]<<16)+(uint32_t)(recBuf[9]<<8)+recBuf[10];
				break;     		 			
			case PORT2_ADDR:
				packFlash.uartBps[1]=(uint32_t)(recBuf[7]<<24)+(uint32_t)(recBuf[8]<<16)+(uint32_t)(recBuf[9]<<8)+recBuf[10];
				break;     		 			
			case PORT3_ADDR:
				packFlash.uartBps[2]=(uint32_t)(recBuf[7]<<24)+(uint32_t)(recBuf[8]<<16)+(uint32_t)(recBuf[9]<<8)+recBuf[10];
				break;     		 			
			case PORT4_ADDR:
			//	packFlash.uartBps[3]=(uint32_t)(recBuf[7]<<24)+(uint32_t)(recBuf[8]<<16)+(uint32_t)(recBuf[9]<<8)+recBuf[10];
				break;     		 			

			//MCUID
			case MCUID_ADDR:
				for(int i=0;i<ACUID_LEN+1;i++){
					packFlash.acuId[i]=recBuf[7+i];
					if((recBuf[7+i]==0xff)||(recBuf[7+i]==0)){
							packFlash.acuId[i]=0;
							break;
					}
				}
				break;         			

			case MODBUS_CFG_ID_ADDR:
				for(int i=0;i<20;i++){
					LCDInput.ID[i]=recBuf[7+i];
					if((recBuf[7+i]==0xff)||(recBuf[7+i]==0)){
							LCDInput.ID[i]=0;
							break;
					}
				}
				if(devIDOKCheck(LCDInput.ID)!=true){//核对ID
						LCDDispSameID(DISP_MODBUS_SAME_ID_MSG_ADDR);
						rt_kprintf("%sERR:same ID\n",sign);
				}
				else{
						LCDRstDispSameID(DISP_MODBUS_SAME_ID_MSG_ADDR);
				}
				break;    		
			case MODBUS_CFG_TYPE_ADDR:
				for(int i=0;i<8;i++){
					LCDInput.model[i]=recBuf[7+i];
					if((recBuf[7+i]==0xff)||(recBuf[7+i]==0)){
							LCDInput.model[i]=0;
							break;
					}
				}
				break;  		
			case MODBUS_CFG_PORT_ADDR:
				LCDInput.useUartNum=recBuf[8]-1;
				break;  		
			case MODBUS_CFG_ADDR_ADDR:
				LCDInput.slaveAddr=recBuf[8];
				break;  		
			case MODBUS_CFG_TIME_ADDR:
				LCDInputTime=(uint32_t)(recBuf[7]<<24)+(uint32_t)(recBuf[8]<<16)+(uint32_t)(recBuf[9]<<8)+recBuf[10];
				break; 		
			case MODBUS_CFG_NAME2_ADDR:
				break;	
			case KEY_ADDR:
				keyReturn((uint16_t)(recBuf[7]<<8)+recBuf[8]);
				break;
			case DISP_INPUTCFG_NAME_ADDR:
				lcdCopyInputName(recBuf);
				break;
			case DISP_INPUTCFG_ID_ADDR:
				
				lcdCopyInputID(recBuf);

				break;
			case DISP_INPUTCFG_TYPE_ADDR:
				lcdCopyInputModel(recBuf);
				break;
			case DISP_INPUTCFG_PORT_ADDR:
				lcdCopyInputPort(recBuf);
				break;
			case DISP_OUTPUT_NAME_ADDR:
				lcdCopyOutputName(recBuf);
				break;
			case DISP_OUTPUT_ID_ADDR:
				lcdCopyOutputID(recBuf);
				break;
      case TEXT_PASSWD_ADDR://密码长度最大8位
				pwssWdRet=checkPassWord((char *)recBuf);
				LCDHidePssd();
				break;
 			case DISP_OUTPUT_TYPE_ADDR:
				lcdCopyOutputModel(recBuf);
				break;
			case DISP_OUTPUT_PORT_ADDR:
				lcdCopyOutputPort(recBuf);
				break;
			case LCD_RTC_ADDR:
				readRtc= copyBufToRtc((char *)recBuf);
			
				getUTCFlag=RT_TRUE;
				
			  break;
#ifndef     ANA_MASK
			case DISP_ANA_ID_ADDR:
				lcdCopyAnaID(recBuf);
				break;
			case DISP_ANA_TYPE_ADDR:
				lcdCopyAnaModel(recBuf);
				break;
			case DISP_ANA_PORT_ADDR:
				lcdCopyAnaPort(recBuf);
				break;
			case DISP_ANA_TIME_ADDR:
				lcdCopyAnaTime(recBuf);
				break;
#endif
		}
}

void LCDHidePssd()
{
	 uint8_t buf[10]="********";
	 buf[8]=0xff;
	 buf[9]=0xff;
	
	 LCDWtite(TEXT_PASSWD_ADDR,buf,10);
	
}
void LCDClearSaveOK()
{
	 uint8_t buf[2];
	 buf[0]=0xff;
	 buf[1]=0xff;
	
	 LCDWtite(KEY_SAVEOK_ADDR,buf,10);
}

void LCDDispSaveOK()
{
	 uint8_t buf[12]="SAVE SUCC";
	 buf[11]=0xff;
	 buf[10]=0xff;
	
	 LCDWtite(KEY_SAVEOK_ADDR,buf,11);
}

void LCDClearRstOK()
{
	 uint8_t buf[2];
	 buf[0]=0xff;
	 buf[1]=0xff;
	
	 LCDWtite(KEY_RESETOK_ADDR,buf,4);
}

void LCDDispRstOK()
{
	 uint8_t buf[13]="RESET SUCC";
	 int len =strlen((char *)buf);
	 buf[len++]=0xff;
	 buf[len++]=0xff;
	
	 LCDWtite(KEY_RESETOK_ADDR,buf,len);
}




void LCDDispSameID(uint16_t addr)
{
		uint8_t buf[14]="ERR:SAME ID";
	  
		buf[12]=0xff;
		buf[13]=0xff;
	
	 LCDWtite(addr,buf,14);
}
void LCDRstDispSameID(uint16_t addr)
{
		uint8_t buf[4]="";
		buf[0]=0xff;
		buf[1]=0xff;
	
	 LCDWtite(addr,buf,4);
}
void LCDDispIDcfgOK(uint16_t addr)
{
		uint8_t buf[8]="SET OK";
		buf[6]=0xff;
		buf[7]=0xff;
	
	 LCDWtite(addr,buf,8);
}

#define PASSWORD_LEN   8
static char passWord1[PASSWORD_LEN]="gy2023";
static char passWord2[PASSWORD_LEN]="666666";
bool pwssWdRet=false;
bool checkPassWord(char *rec)
{
	  char passWord_p[PASSWORD_LEN]={0};
		for(int i=0;i<PASSWORD_LEN;i++){
				passWord_p[i]=rec[7+i];
				if(((uint8_t )rec[7+i]==0xff)||(rec[7+i]==0)){
						passWord_p[i]=0;
						break;
				}
		}
		if(strcmp(passWord1,passWord_p)==0){
				return true;
		}
		if(strcmp(passWord2,passWord_p)==0){
				return true;
		}
		return false;
}





//void LCDDispErrPasswd()
//{
//		uint8_t buf[14]="ERROR!";
//		buf[7]=0xff;
//		buf[8]=0xff;
//	
//	  LCDWtite(TEXT_ERR_PASSWD_DISP_ADDR,buf,9);
//}
////LCD复位错误的pwd信息提示
void LCDRstPw()
{
		uint8_t buf[4]="";
		buf[0]=0xff;
		buf[1]=0xff;
	

	  LCDWtite(TEXT_PASSWD_ADDR,buf,2);
	
}



//password 确认按键
void  surePassWord()
{
	  //uint8_t buf[10]={0X5A,0XA5,0X07,0X82,0,0X84,0X5A,0X01,0,2};//最后一个0002 表示切换到02图
		if(pwssWdRet==true){	
				rt_kprintf("%sPassword ok\n",sign);
			  LCDRstPw();
//				extern rt_mutex_t   lcdSend_mutex;
//				rt_mutex_take(lcdSend_mutex,RT_WAITING_FOREVER);
//				extern  void LCDDataSend(uint8_t *buf,int lenth);
//				LCDDataSend(buf,sizeof(buf));
//				rt_mutex_release(lcdSend_mutex);
			  changeBmp(2);
			  pwssWdRet=false;
		}
		else{
			  changeBmp(3);
				rt_kprintf("%sPassword err\n",sign);
//			  LCDDispErrPasswd();
		}
}



void changeBmp(int num)
{
		uint8_t buf[10]={0X5A,0XA5,0X07,0X82,0,0X84,0X5A,0X01,0,0};
		buf[9]=num;
		extern rt_mutex_t   lcdSend_mutex;
		rt_mutex_take(lcdSend_mutex,RT_WAITING_FOREVER);
		extern  void LCDDataSend(uint8_t *buf,int lenth);
		LCDDataSend(buf,sizeof(buf));
		rt_mutex_release(lcdSend_mutex);
}



//显示sd卡状态
void LCDDispSDState(uint8_t state )
{
	 uint8_t buf[2];
	 buf[0]=0;
	 buf[1]=state;

	 LCDWtite(SD_STATE_ADDR,(uint8_t *)buf,2);
}



void sdSize_p(int *tot,int *fre);
typedef union{
	float fVal;
	int   iVal;
}floIntUn;
floIntUn SDtotal,SDfree;
//显示卡的剩余量
void LCDDispSDSize()
{
		int tot_sect,fre_sect;
		sdSize_p(&tot_sect,&fre_sect);
	  SDtotal.fVal=(float)tot_sect/2/1024/1024;
	  SDfree.fVal =(float)fre_sect/2/1024/1024;
	
    uint8_t buf[4];
		buf[0]=(uint8_t)(SDtotal.iVal>>24);
		buf[1]=(uint8_t)(SDtotal.iVal>>16);
		buf[2]=(uint8_t)(SDtotal.iVal>>8);
		buf[3]=(uint8_t)(SDtotal.iVal>>0);
		LCDWtite(SD_TOTOAL_SIZE_ADDR,(uint8_t *)buf,4);
	

		buf[0]=(uint8_t)(SDfree.iVal>>24);
		buf[1]=(uint8_t)(SDfree.iVal>>16);
		buf[2]=(uint8_t)(SDfree.iVal>>8);
		buf[3]=(uint8_t)(SDfree.iVal>>0);
		LCDWtite(SD_REMAIN_SIZE_ADDR,(uint8_t *)buf,4);
	
}
//显示sd卡状态
//void LCDDispSDState(uint8_t state )
//{
//	 uint8_t buf[2];
//	 buf[0]=0;
//	 buf[1]=state;

//	 LCDWtite(SD_STATE_ADDR,(uint8_t *)buf,2);
//}

////显示sd卡状态
//void LCDDispSDState(uint8_t state )
//{
//	 uint8_t buf[2];
//	 buf[0]=0;
//	 buf[1]=state;

//	 LCDWtite(SD_STATE_ADDR,(uint8_t *)buf,2);
//}