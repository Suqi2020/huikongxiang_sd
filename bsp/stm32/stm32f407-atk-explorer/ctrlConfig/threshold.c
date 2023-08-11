#include <board.h>
//阈值设置参考 《汇控箱modbus串口配置V0.4.doc》中 8、阈值配置（主要对modbus和模拟传感器进行配置）



//const static char sign[]="[threshold]";


extern int mod_printf(const char *fmt, ...);
//必须启用情况下才能得到阈值  即workFlag为true
void printfThresholdList()
{
	  int i;
	#ifndef     ANA_MASK
		for(int j=0;j<ANALOG_NUM;j++){//查一遍 找到 温湿度的模拟阈值
			  if(sheet.analog[j].workFlag==RT_TRUE){
						//if(rt_strcmp(sheet.analog[j].name,analogName[i])==0){
								mod_printf("threshold ");
								mod_printf("%s ",sheet.analog[j].name);
								mod_printf("%s ",sheet.analog[j].ID);
								mod_printf("2 ");
								mod_printf("%s ",sheet.analog[j].subName);
							  if(sheet.analog[j].subName==1){//温度
										
										mod_printf("%0.2f ",sheet.analogTempHum.tempLowLimit);
									  mod_printf("%0.2f \n",sheet.analogTempHum.tempUpLimit);
								}
								else if(sheet.analog[j].subName==2){//湿度
										
										mod_printf("%0.2f ",sheet.analogTempHum.humLowLimit);
									  mod_printf("%0.2f \n",sheet.analogTempHum.humUpLimit);

								}
						}
				
		}
		#endif
		
		for(i=0;i<CIRCULA_485_NUM;i++){
				if(sheet.cirCula[i].workFlag==RT_TRUE){//找到工作的设备  环流需要连续打印三次
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CIRCULA]);
						mod_printf("%s ",sheet.cirCula[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusCircul[i].cirCurALowLimit);
						mod_printf("%0.2f \n",sheet.modbusCircul[i].cirCurAUpLimit);
	
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CIRCULA]);
						mod_printf("%s ",sheet.cirCula[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%0.2f ",sheet.modbusCircul[i].cirCurBLowLimit);
						mod_printf("%0.2f \n",sheet.modbusCircul[i].cirCurBUpLimit);
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CIRCULA]);
						mod_printf("%s ",sheet.cirCula[i].ID);
						mod_printf("1 ");
						mod_printf("3 ");
						mod_printf("%0.2f ",sheet.modbusCircul[i].cirCurCLowLimit);
						mod_printf("%0.2f \n",sheet.modbusCircul[i].cirCurCUpLimit);
						mod_printf("\n");
				}
				
		}

		for(i=0;i<PARTDISCHAG_485_NUM;i++){
				if(sheet.partDischag[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].amplitudeALowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].amplitudeAUpLimit);
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].freqALowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].freqAUpLimit);
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("3 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].dischargeALowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].dischargeAUpLimit);


						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("4 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].amplitudeBLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].amplitudeBUpLimit);
						
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("5 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].freqBLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].freqBUpLimit);
						
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("6 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].dischargeBLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].dischargeBUpLimit);	


						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("7 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].amplitudeCLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].amplitudeCUpLimit);	
						
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("8 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].freqCLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].freqCUpLimit);	
						
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PARTDISCHAG]);
						mod_printf("%s ",sheet.partDischag[i].ID);
						mod_printf("1 ");
						mod_printf("9 ");
						mod_printf("%u ",sheet.modbusPartDisChg[i].dischargeCLowLimit);
						mod_printf("%u \n",sheet.modbusPartDisChg[i].dischargeCUpLimit);
						mod_printf("\n");
				}
		}
		for(i=0;i<PRESSSETTL_485_NUM;i++){
				if(sheet.pressSetl[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PRESSSETTL]);
						mod_printf("%s ",sheet.pressSetl[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusPreSettl[i].tempLowLimit);
						mod_printf("%0.2f \n",sheet.modbusPreSettl[i].tempUpLimit);
					
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[PRESSSETTL]);
						mod_printf("%s ",sheet.pressSetl[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%0.2f ",sheet.modbusPreSettl[i].heightLowLimit);
						mod_printf("%0.2f \n",sheet.modbusPreSettl[i].heightUpLimit);
						mod_printf("\n");

				}
		}

		for(i=0;i<THREEAXIS_485_NUM;i++){
				if(sheet.threeAxiss[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[THREEAXIS]);
						mod_printf("%s ",sheet.threeAxiss[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusThreAxis[i].tempLowLimit);
						mod_printf("%0.2f \n",sheet.modbusThreAxis[i].tempUpLimit);
		
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[THREEAXIS]);
						mod_printf("%s ",sheet.threeAxiss[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%0.2f ",sheet.modbusThreAxis[i].accXLowLimit);
						mod_printf("%0.2f \n",sheet.modbusThreAxis[i].accXUpLimit);
	
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[THREEAXIS]);
						mod_printf("%s ",sheet.threeAxiss[i].ID);
						mod_printf("1 ");
						mod_printf("3 ");
						mod_printf("%0.2f ",sheet.modbusThreAxis[i].accYLowLimit);
						mod_printf("%0.2f \n",sheet.modbusThreAxis[i].accYUpLimit);

						mod_printf("threshold ");
						mod_printf("%s ",modbusName[THREEAXIS]);
						mod_printf("%s ",sheet.threeAxiss[i].ID);
						mod_printf("1 ");
						mod_printf("4 ");
						mod_printf("%0.2f ",sheet.modbusThreAxis[i].accZLowLimit);
						mod_printf("%0.2f \n",sheet.modbusThreAxis[i].accZUpLimit);
						mod_printf("\n");
				}

		}

#ifdef  USE_4GAS

		for(i=0;i<CH4_485_NUM;i++){
				if(sheet.ch4[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CH4]);
						mod_printf("%s ",sheet.ch4[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusCh4[i].ch4LowLimit);
						mod_printf("%0.2f \n",sheet.modbusCh4[i].ch4UpLimit);
					  mod_printf("\n");
				}
		}
		for(i=0;i<O2_485_NUM;i++){
				if(sheet.ch4[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[O2]);
						mod_printf("%s ",sheet.o2[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusO2[i].o2LowLimit);
						mod_printf("%0.2f \n",sheet.modbusO2[i].o2UpLimit);
				
						mod_printf("\n");
				}
		}
		for(i=0;i<H2S_485_NUM;i++){
				if(sheet.ch4[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[H2S]);
						mod_printf("%s ",sheet.h2s[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusH2s[i].h2sLowLimit);
						mod_printf("%0.2f \n",sheet.modbusH2s[i].h2sUpLimit);
				
				    mod_printf("\n");
				}
		}
		for(i=0;i<CO_485_NUM;i++){
				if(sheet.co[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CO]);
						mod_printf("%s ",sheet.co[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusCo[i].coLowLimit);
						mod_printf("%0.2f \n",sheet.modbusCo[i].coUpLimit);
					  mod_printf("\n");
				}
				
		}
#endif
		for(i=0;i<TEMPHUM_485_NUM;i++){
				if(sheet.tempHum[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[TEMPHUM]);
						mod_printf("%s ",sheet.tempHum[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusTempHum[i].tempLowLimit);
						mod_printf("%0.2f \n",sheet.modbusTempHum[i].tempUpLimit);
					  mod_printf("\n");
				}
				//mod_printf("\n");
		}
		for(i=0;i<WATERDEPTH_485_NUM;i++){
				if(sheet.waterDepth[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[WATERDEPTH]);
						mod_printf("%s ",sheet.waterDepth[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusWaterDepth[i].depthLowLimit);
						mod_printf("%0.2f \n",sheet.modbusWaterDepth[i].depthUpLimit);
					  mod_printf("\n");
				}
				//mod_printf("\n");
		}
		for(i=0;i<CRACKMETER_485_NUM;i++){
				if(sheet.crackMeter[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CRACKMETER]);
						mod_printf("%s ",sheet.crackMeter[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%0.2f ",sheet.modbusCrackMeter[i].tempLowLimit);
						mod_printf("%0.2f \n",sheet.modbusCrackMeter[i].tempUpLimit);
					  mod_printf("\n");
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[CRACKMETER]);
						mod_printf("%s ",sheet.crackMeter[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%0.2f ",sheet.modbusCrackMeter[i].distancLowLimit);
						mod_printf("%0.2f \n",sheet.modbusCrackMeter[i].distancUpLimit);
					  mod_printf("\n");
				}
				//mod_printf("\n");
		}
		for(i=0;i<COVER_485_NUM;i++){
				if(sheet.cover[i].workFlag==RT_TRUE){//找到相同ID的
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[COVER]);
						mod_printf("%s ",sheet.cover[i].ID);
						mod_printf("1 ");
						mod_printf("1 ");
						mod_printf("%d ",sheet.modbusCover[i].inclineLowLimit);
						mod_printf("%d \n",sheet.modbusCover[i].inclineUpLimit);
					  mod_printf("\n");
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[COVER]);
						mod_printf("%s ",sheet.cover[i].ID);
						mod_printf("1 ");
						mod_printf("2 ");
						mod_printf("%d ",sheet.modbusCover[i].switch2LowLimit);
						mod_printf("%d \n",sheet.modbusCover[i].switch2UpLimit);
					  mod_printf("\n");
					
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[COVER]);
						mod_printf("%s ",sheet.cover[i].ID);
						mod_printf("1 ");
						mod_printf("3 ");
						mod_printf("%d ",sheet.modbusCover[i].vibrationLowLimit);
						mod_printf("%d \n",sheet.modbusCover[i].vibrationUpLimit);
					  mod_printf("\n");
						
						mod_printf("threshold ");
						mod_printf("%s ",modbusName[COVER]);
						mod_printf("%s ",sheet.cover[i].ID);
						mod_printf("1 ");
						mod_printf("4 ");
						mod_printf("%d ",sheet.modbusCover[i].switch1LowLimit);
						mod_printf("%d \n",sheet.modbusCover[i].switch1UpLimit);
					  mod_printf("\n");
				}
				//mod_printf("\n");
		}
		//打印modbus设置的阈值
}
//与modbus传感器不同之处在于 除了判断ID还需要判断子选项
//模拟传感器阈值配置
#ifndef     ANA_MASK
bool  analogThresholdConfig(int num,char *ID,int sensorSubName,float upLimit,float lowLimit)
{ 
		switch(num)
		{
			case  0://modbus 温湿度
				for(int i=0;i<ANALOG_NUM;i++){//
						if(rt_strcmp(sheet.analog[i].ID,ID)==0){//先核对ID
								if(sheet.analog[i].subName==sensorSubName){
									 if(sensorSubName==1){//温度
											sheet.analogTempHum.tempLowLimit=lowLimit;
											sheet.analogTempHum.tempUpLimit=upLimit;
											return true;
									 }
									 else if(sensorSubName==2){//湿度
											sheet.analogTempHum.humLowLimit=lowLimit;
											sheet.analogTempHum.humUpLimit =upLimit;	
											return true;
									 }
								}
						}
				}
			break;
		}
		return false;
}
#endif
//modbus设备  输入依次为 传感器种类 ID号 传感器类型  上限制  下限制
//modbus传感器阈值配置
bool   modbusThresholdConfig(int num,char *ID,int sensorSubName,float upLimit,float lowLimit)
{
	  int i=0;
		switch(num)
		{
			case CIRCULA:
				for(i=0;i<CIRCULA_485_NUM;i++){
						if(rt_strcmp(sheet.cirCula[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusCircul[i].cirCurAUpLimit  = upLimit;
									  sheet.modbusCircul[i].cirCurALowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusCircul[i].cirCurBUpLimit  = upLimit;
										sheet.modbusCircul[i].cirCurBLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==3){
										sheet.modbusCircul[i].cirCurCUpLimit  = upLimit;
										sheet.modbusCircul[i].cirCurCLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
				break;
			case PARTDISCHAG:
				for(i=0;i<PARTDISCHAG_485_NUM;i++){
						if(rt_strcmp(sheet.partDischag[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusPartDisChg[i].amplitudeAUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].amplitudeALowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusPartDisChg[i].freqAUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].freqALowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==3){
										sheet.modbusPartDisChg[i].dischargeAUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].dischargeALowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==4){
										sheet.modbusPartDisChg[i].amplitudeBUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].amplitudeBLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==5){
										sheet.modbusPartDisChg[i].freqBUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].freqBLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==6){
										sheet.modbusPartDisChg[i].dischargeBUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].dischargeBLowLimit = lowLimit;
									  return  true;
								}
								if(sensorSubName==7){
										sheet.modbusPartDisChg[i].amplitudeCUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].amplitudeCLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==8){
										sheet.modbusPartDisChg[i].freqCUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].freqCLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==9){
										sheet.modbusPartDisChg[i].dischargeCUpLimit  = upLimit;
									  sheet.modbusPartDisChg[i].dischargeCLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
				break;
			case PRESSSETTL:
				for(i=0;i<PRESSSETTL_485_NUM;i++){
						if(rt_strcmp(sheet.pressSetl[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusPreSettl[i].tempUpLimit  = upLimit;
									  sheet.modbusPreSettl[i].tempLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusPreSettl[i].heightUpLimit = upLimit;
										sheet.modbusPreSettl[i].heightLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
				break;
			case THREEAXIS:
				for(i=0;i<THREEAXIS_485_NUM;i++){
						if(rt_strcmp(sheet.threeAxiss[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusThreAxis[i].tempUpLimit  = upLimit;
									  sheet.modbusThreAxis[i].tempLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusThreAxis[i].accXUpLimit  = upLimit;
										sheet.modbusThreAxis[i].accXLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==3){
										sheet.modbusThreAxis[i].accYUpLimit  = upLimit;
										sheet.modbusThreAxis[i].accYLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==4){
										sheet.modbusThreAxis[i].accZUpLimit  = upLimit;
										sheet.modbusThreAxis[i].accZLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
				break;
#ifdef  USE_4GAS
			case CH4:
				for(i=0;i<CH4_485_NUM;i++){
						if(rt_strcmp(sheet.ch4[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusCh4[i].ch4UpLimit  = upLimit;
									  sheet.modbusCh4[i].ch4LowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
			case O2:
				for(i=0;i<O2_485_NUM;i++){
						if(rt_strcmp(sheet.o2[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusO2[i].o2UpLimit  = upLimit;
									  sheet.modbusO2[i].o2LowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
			case H2S:
				for(i=0;i<H2S_485_NUM;i++){
						if(rt_strcmp(sheet.h2s[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusH2s[i].h2sUpLimit  = upLimit;
									  sheet.modbusH2s[i].h2sLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
			case CO:
				for(i=0;i<CO_485_NUM;i++){
						if(rt_strcmp(sheet.co[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusCo[i].coUpLimit  = upLimit;
									  sheet.modbusCo[i].coLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
#endif
			case TEMPHUM:
				for(i=0;i<TEMPHUM_485_NUM;i++){
						if(rt_strcmp(sheet.tempHum[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusTempHum[i].tempUpLimit  = upLimit;
									  sheet.modbusTempHum[i].tempLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusTempHum[i].humUpLimit  = upLimit;
									  sheet.modbusTempHum[i].humLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
			case WATERDEPTH:
				for(i=0;i<WATERDEPTH_485_NUM;i++){
						if(rt_strcmp(sheet.waterDepth[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusWaterDepth[i].depthUpLimit  = upLimit;
									  sheet.modbusWaterDepth[i].depthLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
			default:
				
			case CRACKMETER:
				for(i=0;i<CRACKMETER_485_NUM;i++){
						if(rt_strcmp(sheet.crackMeter[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusCrackMeter[i].tempUpLimit  = upLimit;
									  sheet.modbusCrackMeter[i].tempLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusCrackMeter[i].distancUpLimit  = upLimit;
									  sheet.modbusCrackMeter[i].distancLowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}
			  break;
				
				
			case COVER:
				for(i=0;i<COVER_485_NUM;i++){
						if(rt_strcmp(sheet.cover[i].ID,ID)==0){//找到相同ID的
								if(sensorSubName==1){
										sheet.modbusCover[i].inclineUpLimit  = upLimit;
									  sheet.modbusCover[i].inclineLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==2){
										sheet.modbusCover[i].switch2UpLimit  = upLimit;
									  sheet.modbusCover[i].switch2LowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==3){
										sheet.modbusCover[i].vibrationUpLimit  = upLimit;
									  sheet.modbusCover[i].vibrationLowLimit = lowLimit;
									  return  true;
								}
								else if(sensorSubName==4){
										sheet.modbusCover[i].switch1UpLimit  = upLimit;
									  sheet.modbusCover[i].switch1LowLimit = lowLimit;
									  return  true;
								}
								else
										return  false;
						}
				}

				mod_printf("ERR:modbusThresholdConfig %d\n",num);
				break;
		}
		return false;
}
/*
命令(threshold+名称+ID+类型(1-modbus 2-analog)+选项(1.2.3 只有一种时候设置为1)+阈值上限+阈值下限)(阈值为0或者0xffffffff时此阈值不启用)
threshold温湿度 GYNJLXSD000000164 1 1 10 80
阈值命令详解如下
threshold：固定头部
名称：modbus和模拟传感器名称
类型：(1-modbus 2-analog)  为0或255时候删除对应的阈值设置
选项：传感器功能只有一种情况下值为1，有多种情况下依次递增(1.2.3)如下
     模拟温湿度：1-温度  2-湿度
	   modbus温湿度：1-温度 2-湿度
     modbus防外破：1-温度 2-x方向加速度 3-y方向加速度 4-z方向加速度
     modbus沉降仪：1-温度 2-高度
     modbus环流： 1-cirCurA 2-cirCurB 3-cirCurC
     modbus局放：1-pdA 2-freqA 3-dischargeDataA            
				   4-pdB 5-freqB 6-dischargeDataB				
7-pdC 8-freqC 9-dischargeDataC 
阈值上限：0-99999999(为0阈值不启用)
阈值下限：0-99999999(为0阈值不启用)

*/
//传感器（模拟和485）的阈值配置
static void threshold(int argc,char *argv[])
{
		float uplimit ;
		float lowlimit ;
		int sensorType;
		int sensorSubName;
	  if(0==rt_strcmp((char *)"list", argv[1])){
				printfThresholdList();
				return;
		}
		if(argc!=7){
				goto ERR;
		}
		sensorType= atoi32(argv[3],10);
		sensorSubName=atoi32(argv[4],10);
		if((sensorType!=1)&&(sensorType!=2)){
				mod_printf("sensorType argv[3] should be 1 or 2\n");
			  goto ERR;
		}
		if(sensorSubName>100){
				mod_printf("sensorSubName argv[4] should be <100\n");
			  goto ERR;
		}
		uplimit  = atof(argv[6]);
		lowlimit = atof(argv[5]);
		if(sensorType==1){//modbus传感器
			  int i=0;
			  for(i=0;i<MODBUS_NUM;i++){
						if(rt_strcmp(argv[1],modbusName[i])==0){
								if(true==modbusThresholdConfig(i,argv[2],sensorSubName,uplimit,lowlimit)){
									  mod_printf("threshold mb cfg OK\n");
										return;
								}
								else{
										goto ERR;
								}
						}
				}
		}
#ifndef     ANA_MASK
		if(sensorType==2){//analog传感器
			  int i=0;
			  for(i=0;i<ANALOGNAME_NUM;i++){
						if(rt_strcmp(argv[1],analogName[i])==0){
								if(true==analogThresholdConfig(i,argv[2],sensorSubName,uplimit,lowlimit)){
										mod_printf("%sthreshold ana cfg OK\n",sign);
										return;
								}
								else{
										goto ERR;
								}
						}
				}	
		}
#endif
		return;
		ERR:
		mod_printf("for example\n");
		mod_printf("[threshold 温湿度 GYNJLXSD000000164 1 1 10 80]\n");
		mod_printf("[阈值上下限为0时候 不启用阈值]\n");

}
MSH_CMD_EXPORT(threshold,threshold config);

