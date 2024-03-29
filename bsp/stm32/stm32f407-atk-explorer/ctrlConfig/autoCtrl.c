#include <board.h>
//阈值设置参考 《汇控箱modbus串口配置V0.4.doc》
/*
a、当输入多个设备命令时候，设备间的关系为逻辑与（即输入条件全部满足），
b、输出多个设备命令时候，设备间的关系为逻辑与（即输出的设备都需要执行）
c、当输入多个设备，并且输入设备间的关系是逻辑或的时候，需要多次单独设置即可
d、不支持删除正在配置中的单个设备，需要设置cancel键或者重启设备
e、sure cancel确定或取消本次(a/b/c/d)配置
f、list显示最多10组配置生效的逻辑列表
g、delete(1-10)删除配置过的逻辑列表
h、10组逻辑控制用指针形式指向阈值执行的flag
{ char *inputFlag[10];
	Char *outputFlag[10];
	Char workFlag;
}total[10];
//////////////////////////////////////////////////
autoctrl AI 温湿度  GYNJLXSD000000499 1 
autoctrl AI 温湿度  GYNJLXSD000000499 2 
autoctrl DI 温湿度  GYNJLXSD000000498 1
autoctrl DI 温湿度  GYNJLXSD000000498 2 
autoctrl MB 甲烷  GYNJLXSD000000491 1 
autoctrl MB 硫化氢  GYNJLXSD000000491 1 
autoctrl MB 环流  GYNJLXSD000000492 1 
autoctrl MB 环流  GYNJLXSD000000492 2 
autoctrl MB 环流  GYNJLXSD000000492 3 
autoctrl DO 水泵  GYNJLXSD000000499 1 
autoctrl DO 水泵  GYNJLXSD000000499 2
autoctrl V3O 水泵  GYNJLXSD000000419 1
autoctrl V5O 水泵  GYNJLXSD000000429 1
autoctrl SWITCH 水泵  GYNJLXSD000000439 1
//////////////////////////////////////////////////
*/
extern int mod_printf(const char *fmt, ...);
inoutDevStru inpoutpFlag __attribute__((at(0x2000D000)))={0};//输入输出超值的标记位  指定内存地址  防止更改代码后内存地址移位
////sheet.autoctrl[pindex].output[outputIndex].flag中指针指向了inpoutpFlag
//const static char sign[]="[autoCtrl]";
const char inName[INNAME_NUM][INOUTNAME_LEN]={"AI","DI","MB",};
//const char outName[OUTNAME_NUM][INOUTNAME_LEN]={"DO","V3O","V5O","SWITCH"};
//char outSwitchNum[OUTNAME_NUM]={DO_NUM,V33O_NUM,V5O_NUM,SWITCH_NUM};
const char outName[OUTNAME_NUM][INOUTNAME_LEN]={"DO","SSWITCH"};


const uint8_t outChinaName[OUTNAME_NUM][INOUTNAME_LEN]={{0x54,0x54,0x4c,0xb5,0xe7,0xc6,0xbd,0x00},\
																												{0xBC,0xCC,0xB5,0xE7,0xC6,0xF7,0x00,0x00}};

																										

char outSwitchNum[OUTNAME_NUM]={DO_NUM,SWITCH_NUM};
uint8_t inputIndex=0; //本次配置控制逻辑的输入坐标配置   被inputFlag 指针调用
uint8_t outputIndex=0;//本次配置控制逻辑的输出的坐标配置 被outputFlag指针调用

uint8_t pindex=0;//总共配置过这么多控制条数 每配置或者删除完一条以及上电时候 需要重新读取pindex 
//如果是正在配置界面 并没有配置完成  点击删除 此时不能更新pindex
bool configFlag=false;//做个标记来判断是不是正在配置
//检查一下没有设置过的Index 
int findCtrlIndex()
{
		for(int i=0;i<CRTL_TOTAL_NUM;i++){
				if(sheet.autoctrl[i].workFlag!=1){
					  
						return i;//找到没有用过的 
				}
		}
		return 255;
}
bool firstReadFlag=false;



//指针指向局放的阈值
static bool ctrlCirculaConf(char **argv)
{
		//uint8_t subname = atoi16(argv[4],10);
		uint8_t limit   = atoi16(argv[5],10);
		int i;
		if(!((limit==0)||(limit==1))){
				mod_printf("ERR:autoCirculaConf argv[5] limit %s should be 0 1\n");
				return false;
		}

		for(i=0;i<CIRCULA_485_NUM;i++){
					if(rt_strcmp(sheet.cirCula[i].ID,argv[3])==0){//找到相同ID的

							if(strcmp(argv[4],"earthCurA")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurALowFlag;
						
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurAUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"earthCurB")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurBLowFlag;
											
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurBUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"earthCurC")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurCLowFlag;
											
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurCUpFlag;
											//inputIndex++;
											return true;
											//return true;
									}
							}
							
					}
			}
		return false;
}


//指针指向局放的阈值
static bool crtlPartDischagConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:partDischagConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<PARTDISCHAG_485_NUM;i++){
					if(rt_strcmp(sheet.partDischag[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"pdA")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"pdB")==0){//B
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"pdC")==0){//C
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeCLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeCUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqA")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqB")==0){//B
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqC")==0){//C
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqCLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqCUpFlag;
											return true;
									}
							}	
							else if(strcmp(argv[4],"dischargeDateA")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"dischargeDateB")==0){//B
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"dischargeDateC")==0){//C
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeCLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeCUpFlag;
											return true;
									}
							}
					}
			}
			return false;
}
//指针指向防沉降的阈值
static bool ctrlPressSettlConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlPressSettlConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<PRESSSETTL_485_NUM;i++){
						if(rt_strcmp(sheet.pressSetl[i].ID,argv[3])==0){//找到相同ID的
								if(strcmp(argv[4],"temperature")==0){//A
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].tempLowFlag;
											  
											  return true;// true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].tempUpFlag;
											  return true;
										}
								}
								else if(strcmp(argv[4],"height")==0){//A
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].heightLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].heightUpFlag;
												
											  return true;
										}
								}
						}
			}
			return false;
}

//指针指向三轴加速度的参数
static bool ctrlThreeAxisConf(char **argv)
{
//			uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlThreeAxisConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<THREEAXIS_485_NUM;i++){
						if(rt_strcmp(sheet.threeAxiss[i].ID,argv[3])==0){//找到相同ID的
								if(strcmp(argv[4],"temperature")==0){//A
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].tempLowFlag;
											  
											  return true;// true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].tempUpFlag;
												
											  return true; 
										}
								}
								else if(strcmp(argv[4],"accelerationX")==0){//A
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accXLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accXUpFlag;
												
											  return true;
										}
								}
								else if(strcmp(argv[4],"accelerationY")==0){//B
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accYLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accYUpFlag;
												
											  return true;
										}
								}							
								else if(strcmp(argv[4],"accelerationZ")==0){//C
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accZLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accZUpFlag;
												
											  return true;
										}
								}
						}
			}
			return false;
}
			
						
#ifdef USE_4GAS
//指针指向甲烷的阈值
static bool ctrlCh4Conf(char **argv)
{

			//uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlCh4Conf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<CH4_485_NUM;i++){
						if(rt_strcmp(sheet.ch4[i].ID,argv[3])==0){//找到相同ID的
								if(strcmp(argv[4],"methane")==0){//A
									  if(limit==0){//下限
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCh4[i].ch4LowFlag;
											  
											  return true;// true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCh4[i].ch4UpFlag;
												
											  return true;
										}
								}
						}
			}
			return false;
}
//指针指向氧气的阈值
static bool ctrlO2Conf(char **argv)
{
			//uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlO2Conf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<O2_485_NUM;i++){
					if(rt_strcmp(sheet.o2[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"oxy")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusO2[i].o2LowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusO2[i].o2UpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}
//指针指向硫化氢的阈值
static bool ctrlH2sConf(char **argv)
{

//			uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlH2sConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<H2S_485_NUM;i++){
					if(rt_strcmp(sheet.h2s[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"hydrogenSulfide")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusH2s[i].h2sLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusH2s[i].h2sUpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}
//指针指向CO的阈值
static bool ctrlCoConf(char **argv)
{
			//uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlCoConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<CO_485_NUM;i++){
					if(rt_strcmp(sheet.co[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"monoxide")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCo[i].coLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCo[i].coUpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}					

#endif

//指针指向温湿度的阈值
static bool ctrlTemphumConf(char **argv)
{
//			uint8_t subname = atoi16(argv[4],10);
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlTempHumConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<TEMPHUM_485_NUM;i++){
					if(rt_strcmp(sheet.tempHum[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"temperature")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].tempLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].tempUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"humidity")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].humLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].humUpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}					
//指针指向水位传感器的阈值
static bool  ctrlWaterConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlWaterConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<WATERDEPTH_485_NUM;i++){
					if(rt_strcmp(sheet.waterDepth[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"depth")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusWaterDepth[i].depthLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusWaterDepth[i].depthUpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}													
//指针指向水位传感器的阈值
static bool  ctrlCrackMeterConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrl crackMeter argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<CRACKMETER_485_NUM;i++){
					if(rt_strcmp(sheet.crackMeter[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"temperature")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].tempLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].tempUpFlag;
											
											return true;
									}
							}
							else 	if(strcmp(argv[4],"distance")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].distancLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].distancUpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}					

//指针指向传感器的阈值
static bool  ctrlCoverConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrl cover argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<COVER_485_NUM;i++){
					if(rt_strcmp(sheet.cover[i].ID,argv[3])==0){//找到相同ID的
							if(strcmp(argv[4],"incline")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].inclineLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].inclineUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"switch2")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch2LowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch2UpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"vibration")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].vibrationLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].vibrationUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"switch1")==0){//A
									if(limit==0){//下限
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch1LowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch1UpFlag;
											
											return true;
									}
							}
					}
			}
			return false;
}					
//输入的指针配置
void autoctrlInputcfg(char*argv[])
{
	  int i;
	  uint8_t limit ;
	  bool ret=false;
		for(i=0;i<sizeof(inName)/sizeof(inName[0]);i++){
				if(rt_strcmp(inName[i],argv[1])==0){//find input
						if(inputIndex>=CRTL_IN_NUM){
								mod_printf("err:autoctrl input num is %d\n",CRTL_IN_NUM);
							  return;
						}
						switch(i){
							case 0://AI
								
							
							#ifndef     ANA_MASK
								for(j=0;j<ANALOG_NUM;j++){
										if(rt_strcmp(argv[2],sheet.analog[j].name)==0){//find  name
												if(rt_strcmp(argv[3],sheet.analog[j].ID)==0){//find ID
													 //uint8_t subname = atoi16(argv[4],10);
													  uint8_t limit   = atoi16(argv[5],10);

														rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].typeName,argv[1]);
														rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].senseName,argv[2]);
														rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].ID,argv[3]);
														rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].subName,argv[4]);
														sheet.autoctrl[pindex].input[inputIndex].limit=limit;
//													  if(!((subname==0)||(subname==1)||(subname==2))){
//																mod_printf("%sERR:argv[4] subname %s should be 0 1 2\n",sign,inName[i]);
//														}
														if(!((limit==0)||(limit==1))){
																mod_printf("%sERR:argv[5] limit %s should be 0 1\n",sign,inName[i]);
														}
														if(sheet.analog[j].subName==1){//find subname 这里用数字替代
															  if(rt_strcmp(argv[4],"temperature")==0){//温度
																		if(limit==0){//下限
																				sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.tempLowFlag;
																		}
																		else{//上限
																			  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.tempUpFlag;
																		}
																		inputIndex++;
																}
																if(rt_strcmp(argv[4],"humidity")==0){//湿度
																		if(limit==0){//下限
																				sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.humLowFlag;
																		}
																		else{//上限
																			  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.humUpFlag;
																		}
																		inputIndex++;
																}
																configFlag=true;
																mod_printf("%saotuctrl AI OK\n",sign);
																break;
														}
												}
										}
								}
								#endif
								break;
							case 1://DI
								limit   = atoi16(argv[5],10);
								rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].typeName,argv[1]);
								rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].senseName,argv[2]);
								rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].ID,argv[3]);
								rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].subName,"useless");
								sheet.autoctrl[pindex].input[inputIndex].limit=limit;
								if(!((limit==0)||(limit==1))){
										mod_printf("ERR:di conf argv[5] limit %s should be 0 1\n");
										return ;
								}
								for(i=0;i<DI_NUM;i++){
										if(0==rt_strcmp((char *)packFlash.diginput[i].devID,argv[3])){
												//packFlash.input[i].levelFlag=limit;//通过levelFlag来置位 lowFlag upFlag
											  if(limit==0)
														sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.digInput[i].lowFlag;
												else
														sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.digInput[i].upFlag;
												inputIndex++;
												configFlag=true;
												mod_printf("aotuctrl DI OK\n");
										}
								}
								break;
							case 2://MB
								for(i=0;i<MODBUS_NUM;i++){
										if(0==rt_strcmp((char *)modbusName[i], argv[2])){
												limit   = atoi16(argv[5],10);
												//subName = atoi16(argv[4],10);
												rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].typeName,argv[1]);
												rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].senseName,argv[2]);
												rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].ID,argv[3]);
												rt_strcpy(sheet.autoctrl[pindex].input[inputIndex].subName,argv[4]);
												//sheet.autoctrl[pindex].input[inputIndex].subName=subName;
												sheet.autoctrl[pindex].input[inputIndex].limit=limit;
												switch(i){
													case CIRCULA:
															// if(true==autoCirculaConf(argv))
															ret=ctrlCirculaConf(argv);	
													break;
													case PARTDISCHAG:
														  ret=crtlPartDischagConf(argv);
													break;
													case PRESSSETTL:
															ret=ctrlPressSettlConf(argv);
													break;
													case THREEAXIS:
															ret=ctrlThreeAxisConf(argv);
													break;
										#ifdef USE_4GAS
													case CH4:
														  ret=ctrlCh4Conf(argv);
													break;
													case O2:
														  ret=ctrlO2Conf(argv);
													break;
													case H2S:
														  ret=ctrlH2sConf(argv);
													break;
													case CO:
															ret=ctrlCoConf(argv);
													break;
										#endif
													case TEMPHUM:
														ret=ctrlTemphumConf(argv);
													break;
													case WATERDEPTH:
														ret=ctrlWaterConf(argv);
													break;
													case CRACKMETER:
														ret=ctrlCrackMeterConf(argv);
													break;
													case COVER:
														ret=ctrlCoverConf(argv);
													break;
													default:
														mod_printf("ERR:argv[2]>%d\n",WATERDEPTH);
													break;
												}
												
												if(ret==true){
														configFlag=true;
													  inputIndex++;
														mod_printf("aotuctrl MB OK\n");
												}
												break;
										}
								}
								break;
						}
//						for(j=0;j<ANALOG_NUM;j++)
						return;
				}
		}	
}

//输出的指针配置
//{"DO","V3O","V5O","SWITCH"};
void autoctrlOutputcfg(char*argv[])
{
		int i;
	
		int  limit   = atoi16(argv[5],10);
		if(!((limit==0)||(limit==1))){
				mod_printf("ERR:autoctrlOutputcfg argv[5] limit %s should be 0 1\n");
				return ;
		}
		
		for(i=0;i<sizeof(outName)/sizeof(outName[0]);i++){
				if(rt_strcmp(outName[i],argv[1])==0){//find output

						if(outputIndex>=CRTL_OUT_NUM){
								mod_printf("err:autoctrl output num is %d\n",CRTL_OUT_NUM);
							  return;
						}
						rt_strcpy(sheet.autoctrl[pindex].output[outputIndex].typeName,argv[1]);
						rt_strcpy(sheet.autoctrl[pindex].output[outputIndex].senseName,argv[2]);
						rt_strcpy(sheet.autoctrl[pindex].output[outputIndex].ID,argv[3]);
						rt_strcpy(sheet.autoctrl[pindex].output[outputIndex].subName,"useless");
						//sheet.autoctrl[pindex].output[outputIndex].subName=0;
					  sheet.autoctrl[pindex].output[outputIndex].limit=limit;
						switch(i)
						{
							case 0:
								for(i=0;i<DO_NUM;i++){
										if(0==rt_strcmp((char *)packFlash.digoutput[i].devID,argv[3])){
												//packFlash.digoutput[i].levelFlag=limit;//通过levelFlag来置位 lowFlag upFlag
											  if(limit==0)
														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.digOutput[i].lowFlag;
												else
														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.digOutput[i].upFlag;
												mod_printf("aotuctrl DO OK addr=0x%x\n",sheet.autoctrl[pindex].output[outputIndex].flag);
												outputIndex++;
												configFlag=true;
												
										}
								}
								break;
//							case 1:							
//								for(i=0;i<V33O_NUM;i++){
//										if(0==rt_strcmp((char *)packFlash.v33output[i].devID,argv[3])){
//												//packFlash.digoutput[i].levelFlag=limit;//通过levelFlag来置位 lowFlag upFlag
//											  if(limit==0)
//														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.v33Output[i].lowFlag;
//												else
//														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.v33Output[i].upFlag;
//												mod_printf("%saotuctrl V33O OK addr=0x%x\n",sign,sheet.autoctrl[pindex].output[outputIndex].flag);
//												outputIndex++;
//												configFlag=true;
//												
//										}
//								}
//								break;
//							case 2:
//								for(i=0;i<V5O_NUM;i++){
//										if(0==rt_strcmp((char *)packFlash.v5output[i].devID,argv[3])){
//												
//											  if(limit==0)
//														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.v5Output[i].lowFlag;
//												else
//														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.v5Output[i].upFlag;
//												mod_printf("%saotuctrl V5O OK  addr=0x%x\n",sign,sheet.autoctrl[pindex].output[outputIndex].flag);
//												outputIndex++;
//												configFlag=true;
//												
//										}
//								}
//								break;
							case 1:
								for(i=0;i<SWITCH_NUM;i++){
										if(0==rt_strcmp((char *)packFlash.switchoutput[i].devID,argv[3])){
												//packFlash.digoutput[i].levelFlag=limit;//通过levelFlag来置位 lowFlag upFlag
											  if(limit==0)
														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.switchOutput[i].lowFlag;
												else
														sheet.autoctrl[pindex].output[outputIndex].flag=&inpoutpFlag.switchOutput[i].upFlag;
												mod_printf("aotuctrl SWITCH OK  addr=0x%x\n",sheet.autoctrl[pindex].output[outputIndex].flag);
												outputIndex++;
												configFlag=true;
												
										}
								}
								break;
								default:

								break;
						}
						return;
				}
		}
	
}
//打印自动控制的配置列表
void printfCtrl()
{
		mod_printf("auto ctrl list\n");
		for(int i=0;i<CRTL_TOTAL_NUM;i++){
				//mod_printf("%s,autoctrl list",sign);
			  if(sheet.autoctrl[i].workFlag==true){
					  mod_printf("autoctrl the [%d] inctrl\n",i+1);
						for(int k=0;k<CRTL_IN_NUM;k++){
								if((sheet.autoctrl[i].input[k].flag!=NULL)&&((uint32_t)(uint32_t*)(sheet.autoctrl[i].input[k].flag)!=0xFFFFFFFF)){
										mod_printf("In:autoctrl ");
										mod_printf("%s ",sheet.autoctrl[i].input[k].typeName);
										mod_printf("%s ",sheet.autoctrl[i].input[k].senseName);
										mod_printf("%s ",sheet.autoctrl[i].input[k].ID);
										mod_printf("%s ",sheet.autoctrl[i].input[k].subName);
										mod_printf("%d \n",sheet.autoctrl[i].input[k].limit);
								}
						}
						for(int j=0;j<CRTL_OUT_NUM;j++){
							 							//	if((sheet.autoctrl[i].input[k].flag!=NULL)&&((uint32_t)(uint32_t*)(sheet.autoctrl[i].input[k].flag)!=0xFFFFFFFF)){
								if((sheet.autoctrl[i].output[j].flag!=NULL)&&((uint32_t)(uint32_t*)sheet.autoctrl[i].output[j].flag!=0xFFFFFFFF)){
										mod_printf("Out:autoctrl ");
										mod_printf("%s ",sheet.autoctrl[i].output[j].typeName);
										mod_printf("%s ",sheet.autoctrl[i].output[j].senseName);
										mod_printf("%s ",sheet.autoctrl[i].output[j].ID);
										mod_printf("%s ",sheet.autoctrl[i].output[j].subName);
										mod_printf("%d \n",sheet.autoctrl[i].output[j].limit);
								}
						}	
						mod_printf("\n");
				}
		}
}
//每次上电或者被调用时候读取
void checkIndex()
{
		  if(firstReadFlag==false){
				firstReadFlag=true;
				pindex=findCtrlIndex();//每次上电后读取一次即可
		}
}
//确定配置
void  autoCfgSure()
{
		sheet.autoctrl[pindex].workFlag=true;//本条配置生效
		pindex =findCtrlIndex();//确定后刷新坐标
		configFlag=false;
		inputIndex=0; 
		outputIndex=0;
		mod_printf("aotuctrl sure OK\n");
}
void delAutoCfg(int num)
{
		sheet.autoctrl[num-1].workFlag=false;//本条配置失效
		for(int j=0;j<CRTL_IN_NUM;j++)
			 sheet.autoctrl[num-1].input[j].flag=NULL;
		for(int j=0;j<CRTL_OUT_NUM;j++)
			 sheet.autoctrl[num-1].output[j].flag=NULL;
		if(configFlag==false)
				pindex =findCtrlIndex();//删除后刷新坐标
		mod_printf("aotuctrl delete OK\n");
}
//自动控制输入函数  串口输出命令首先调用此函数解析
//以上逻辑配置时候如需要删除单个输入或者输出设备，选项为0即可，
//必须从后往前（图形界面显示）一个一个删除（逻辑控制列表的指针为空）或者重启设备，cancel命令也可以取消本次整体配置
static void autoctrl(char argc,char*argv[])
{

    checkIndex();
		if(rt_strcmp("list",argv[1])==0){
				printfCtrl();
				return;
		}//打印
		if(rt_strcmp("delete",argv[1])==0){
			  
			  int num=atoi16(argv[2],10);
			  if( num==0){
						mod_printf("argv[2] should not 0\n");
				}
			  mod_printf("%d %d\n",num,pindex);
			  delAutoCfg(num);
				return;
		}//打印删除按钮

	  if(pindex==255){
				mod_printf("ERR:aotuctrl is full,the total number is %d\n",CRTL_TOTAL_NUM);
			  return;
		}
		if(rt_strcmp("sure",argv[1])==0){//确定本次配置按钮  确定完了只能删除 不能取消
			  autoCfgSure();
				return;
		}
		int k;
		if(rt_strcmp("cancel",argv[1])==0){//取消本次配置按钮
			  for( k=0;k<CRTL_IN_NUM;k++){
						sheet.autoctrl[pindex].input[inputIndex].flag=NULL;
				}
				for( k=0;k<CRTL_OUT_NUM;k++){
						sheet.autoctrl[pindex].output[outputIndex].flag=NULL;
				}
				pindex =findCtrlIndex();//确定后刷新坐标
			  configFlag=false;
				inputIndex=0; 
				outputIndex=0;
				mod_printf("aotuctrl cancel OK\n");
				return;
		}

		if(pindex>=CRTL_TOTAL_NUM){
				mod_printf("err:autoctrl totoal num is %d\n",CRTL_TOTAL_NUM);
		}
		autoctrlInputcfg(argv); //检查是不是输入配置
		autoctrlOutputcfg(argv);//检查是不是输出配置
}
MSH_CMD_EXPORT(autoctrl,autoctrl config);

