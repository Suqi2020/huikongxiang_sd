#include "board.h"


const  char     UartName[UART_NUM][6] ={"port1", "port2",  "port3","port4","port5","port6","port7","port8","port9"};//重映射一个名称
//const  uartEnum UartNum[UART_NUM]     ={USE_UART3,USE_UART6,USE_UART4};//重映射一个名称
extern int mod_printf(const char *fmt, ...);
		
//打印所有增加的modbus设备列表
void printModbusDevList()
{
	  mod_printf("printf modbus device list\n");
		for(int i=0;i<MODBUS_NUM;i++){
			switch(i)
			{
				case CIRCULA:
					for(int j=0;j<CIRCULA_485_NUM;j++){//核对有没有配置过
							if(sheet.cirCula[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.cirCula[j].ID);
									mod_printf("%s ",sheet.cirCula[j].model);
									mod_printf("%s ",UartName[sheet.cirCula[j].useUartNum]);
									mod_printf("%d ",sheet.cirCula[j].slaveAddr);
									mod_printf("%d \n",sheet.cirCulaColTime);
							}
					}
				break;
				case PARTDISCHAG:
					for(int j=0;j<PARTDISCHAG_485_NUM;j++){//核对有没有配置过
							if(sheet.partDischag[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
									mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.partDischag[j].ID);
									mod_printf("%s ",sheet.partDischag[j].model);
									mod_printf("%s ",UartName[sheet.partDischag[j].useUartNum]);
									mod_printf("%d ",sheet.partDischag[j].slaveAddr);
									mod_printf("%d \n",sheet.partDischagColTime);
							}
					}
				break;
				case PRESSSETTL:
					for(int j=0;j<PRESSSETTL_485_NUM;j++){//核对有没有配置过
							if(sheet.pressSetl[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.pressSetl[j].ID);
									mod_printf("%s ",sheet.pressSetl[j].model);
									mod_printf("%s ",UartName[sheet.pressSetl[j].useUartNum]);
									mod_printf("%d ",sheet.pressSetl[j].slaveAddr);
									mod_printf("%d \n",sheet.pressSetlColTime);
							}
					}
				break;
				case THREEAXIS:
					for(int j=0;j<THREEAXIS_485_NUM;j++){//核对有没有配置过
							if(sheet.threeAxiss[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.threeAxiss[j].ID);
									mod_printf("%s ",sheet.threeAxiss[j].model);
									mod_printf("%s ",UartName[sheet.threeAxiss[j].useUartNum]);
									mod_printf("%d ",sheet.threeAxiss[j].slaveAddr);
									mod_printf("%d \n",sheet.threeAxissColTime);
							}
					}
				break;
	#ifdef USE_4GAS 
				case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//核对有没有配置过
							if(sheet.ch4[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.ch4[j].ID);
									mod_printf("%s ",sheet.ch4[j].model);
									mod_printf("%s ",UartName[sheet.ch4[j].useUartNum]);
									mod_printf("%d ",sheet.ch4[j].slaveAddr);
									mod_printf("%d \n",sheet.ch4ColTime);//sheet.ch4ColTime);
							}
					}
				break;
				case O2:
					for(int j=0;j<O2_485_NUM;j++){//核对有没有配置过
							if(sheet.o2[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.o2[j].ID);
									mod_printf("%s ",sheet.o2[j].model);
									mod_printf("%s ",UartName[sheet.o2[j].useUartNum]);
									mod_printf("%d ",sheet.o2[j].slaveAddr);
									mod_printf("%d \n",sheet.o2ColTime);//sheet.o2ColTime);
							}
					}
				break;
				case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//核对有没有配置过
							if(sheet.h2s[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.h2s[j].ID);
									mod_printf("%s ",sheet.h2s[j].model);
									mod_printf("%s ",UartName[sheet.h2s[j].useUartNum]);
									mod_printf("%d ",sheet.h2s[j].slaveAddr);
									mod_printf("%d \n",sheet.h2sColTime);//sheet.h2sColTime);
							}
					}
				break;
				case CO:
					for(int j=0;j<CO_485_NUM;j++){//核对有没有配置过
							if(sheet.co[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.co[j].ID);
									mod_printf("%s ",sheet.co[j].model);
									mod_printf("%s ",UartName[sheet.co[j].useUartNum]);
									mod_printf("%d ",sheet.co[j].slaveAddr);
									mod_printf("%d \n",sheet.coColTime);
							}
					}
				break;
	#endif
				case TEMPHUM:
					for(int j=0;j<TEMPHUM_485_NUM;j++){//核对有没有配置过
							if(sheet.tempHum[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s ",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
									mod_printf("%s ",sheet.tempHum[j].ID);
									mod_printf("%s ",sheet.tempHum[j].model);
									mod_printf("%s ",UartName[sheet.tempHum[j].useUartNum]);
									mod_printf("%d ",sheet.tempHum[j].slaveAddr);
									mod_printf("%d \n",sheet.tempHumColTime);
							}
					}
				break;
				case WATERDEPTH:
					for(int j=0;j<WATERDEPTH_485_NUM;j++){//核对有没有配置过
							if(sheet.waterDepth[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
			
									mod_printf("%s ",sheet.waterDepth[j].ID);
									mod_printf("%s ",sheet.waterDepth[j].model);
									mod_printf("%s ",UartName[sheet.waterDepth[j].useUartNum]);
									mod_printf("%d ",sheet.waterDepth[j].slaveAddr);
									mod_printf("%d \n",sheet.waterDepthColTime);
							}
					}
				break;
				case CRACKMETER:
					for(int j=0;j<CRACKMETER_485_NUM;j++){//核对有没有配置过
							if(sheet.crackMeter[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
			
									mod_printf("%s ",sheet.crackMeter[j].ID);
									mod_printf("%s ",sheet.crackMeter[j].model);
									mod_printf("%s ",UartName[sheet.crackMeter[j].useUartNum]);
									mod_printf("%d ",sheet.crackMeter[j].slaveAddr);
									mod_printf("%d \n",sheet.crackMeterColTime);
							}
					}
				break;
				case COVER://增加井盖
					for(int j=0;j<COVER_485_NUM;j++){//核对有没有配置过
							if(sheet.cover[j].workFlag==RT_TRUE){
									mod_printf("modbus ");
									mod_printf("%10s",modbusName[i]);
								  mod_printf("(%d)",modbusBps[i]);
			
									mod_printf("%s ",sheet.cover[j].ID);
									mod_printf("%s ",sheet.cover[j].model);
									mod_printf("%s ",UartName[sheet.cover[j].useUartNum]);
									mod_printf("%d ",sheet.cover[j].slaveAddr);
									mod_printf("%d \n",sheet.coverColTime);
							}
					}
				break;
				default:
				break;
			}
		}
}
//环流的配置
static int circulaConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.cirCulaColTime=atoi32(argv[6],10);
	for( i=0;i<CIRCULA_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.cirCula[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.cirCula[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del circula ok\n");
						  return 1;
					}
					else{
							sheet.cirCula[i].workFlag=RT_TRUE;//打开
					}
					sheet.cirCula[i].slaveAddr=slaveAddr;	
					sheet.cirCula[i].useUartNum=uartnum;
					rt_strcpy(sheet.cirCula[i].model,argv[3]);
					mod_printf("circula reconfig %d\n",i);
					mod_printf("circula OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==CIRCULA_485_NUM){//没有配置过
		  int j=0;
			for(j=0;j<CIRCULA_485_NUM;j++){
					if(sheet.cirCula[j].workFlag!=RT_TRUE){
							sheet.cirCula[j].workFlag=RT_TRUE;//打开
							sheet.cirCula[j].slaveAddr=slaveAddr;	
							sheet.cirCula[j].useUartNum=uartnum;
							rt_strcpy(sheet.cirCula[j].model,argv[3]);
							rt_strcpy(sheet.cirCula[j].ID,argv[2]);
							mod_printf("circula config %d\n",j);
						  mod_printf("circula OK\n");
							ret =1;
							break;
					}
			}
			if(j==CIRCULA_485_NUM){
					mod_printf("ERR totalNum=%d\n",CIRCULA_485_NUM);
			}
	}
	return ret;
}
//局放的配置
static int partDischagConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.partDischagColTime=atoi32(argv[6],10);
	for( i=0;i<PARTDISCHAG_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.partDischag[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.partDischag[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del partDischarge\n");
							return 1;
					}
					else{
							sheet.partDischag[i].workFlag=RT_TRUE;//打开
					}
					sheet.partDischag[i].slaveAddr=slaveAddr;	
					sheet.partDischag[i].useUartNum=uartnum;
					rt_strcpy(sheet.partDischag[i].model,argv[3]);
					mod_printf("partDischag reconfig %d\n",i);
					mod_printf("partDischag OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==PARTDISCHAG_485_NUM){//没有配置过
		  int j=0;
			for( j=0;j<PARTDISCHAG_485_NUM;j++){
					if(sheet.partDischag[j].workFlag!=RT_TRUE){
							sheet.partDischag[j].workFlag=RT_TRUE;//打开
							sheet.partDischag[j].slaveAddr=slaveAddr;	
							sheet.partDischag[j].useUartNum=uartnum;
							rt_strcpy(sheet.partDischag[j].model,argv[3]);
							rt_strcpy(sheet.partDischag[j].ID,argv[2]);
							mod_printf("partDischag config %d\n",j);
						  mod_printf("partDischag OK\n");
							ret =1;
							break;
					}
			}
			if(j==PARTDISCHAG_485_NUM){
					mod_printf("ERR totalNum=%d\n",PARTDISCHAG_485_NUM);
			}
	}
	return ret;
}
//沉降仪的配置
static int pressSettlConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.pressSetlColTime=atoi32(argv[6],10);
	for( i=0;i<PRESSSETTL_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.pressSetl[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.pressSetl[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del pressSettle\n");
						  return 1;
					}
					else{
							sheet.pressSetl[i].workFlag=RT_TRUE;//打开
					}
					sheet.pressSetl[i].slaveAddr=slaveAddr;	
					sheet.pressSetl[i].useUartNum=uartnum;
					rt_strcpy(sheet.pressSetl[i].model,argv[3]);
					mod_printf("pressSettl reconfig %d\n",i);
					mod_printf("pressSettl OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==PRESSSETTL_485_NUM){//没有配置过
		  int j=0;
			for(j=0;j<PRESSSETTL_485_NUM;j++){
					if(sheet.pressSetl[j].workFlag!=RT_TRUE){
							sheet.pressSetl[j].workFlag=RT_TRUE;//打开
							sheet.pressSetl[j].slaveAddr=slaveAddr;	
							sheet.pressSetl[j].useUartNum=uartnum;
							rt_strcpy(sheet.pressSetl[j].model,argv[3]);
							rt_strcpy(sheet.pressSetl[j].ID,argv[2]);
							mod_printf("pressSettl config %d\n",j);
						  mod_printf("pressSettl OK\n");
							ret =1;
							break;
					}
			}
			if(j==PRESSSETTL_485_NUM){
					mod_printf("ERR totalNum=%d\n",PRESSSETTL_485_NUM);
			}
	}
	return ret;
}
//crackmeter 配置
static int crackMeterConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.crackMeterColTime=atoi32(argv[6],10);
	for( i=0;i<CRACKMETER_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.crackMeter[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.crackMeter[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del cover\n");
						  return 1;
					}
					else{
							sheet.crackMeter[i].workFlag=RT_TRUE;//打开
					}
					sheet.crackMeter[i].slaveAddr=slaveAddr;	
					sheet.crackMeter[i].useUartNum=uartnum;
					rt_strcpy(sheet.crackMeter[i].model,argv[3]);
					mod_printf("cover reconfig %d\n",i);
					mod_printf("cover OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==CRACKMETER_485_NUM){//没有配置过
		  int j=0;
			for(j=0;j<CRACKMETER_485_NUM;j++){
					if(sheet.crackMeter[j].workFlag!=RT_TRUE){
							sheet.crackMeter[j].workFlag=RT_TRUE;//打开
							sheet.crackMeter[j].slaveAddr=slaveAddr;	
							sheet.crackMeter[j].useUartNum=uartnum;
							rt_strcpy(sheet.crackMeter[j].model,argv[3]);
							rt_strcpy(sheet.crackMeter[j].ID,argv[2]);
							mod_printf("crackMeter config %d\n",j);
						  mod_printf("crackMeter OK\n");
							ret =1;
							break;
					}
			}
			if(j==CRACKMETER_485_NUM){
					mod_printf("ERR totalNum=%d\n",CRACKMETER_485_NUM);
			}
	}
	return ret;
}



static int coverConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.coverColTime=atoi32(argv[6],10);
	for( i=0;i<COVER_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.cover[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.cover[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del crackMeter\n");
						  return 1;
					}
					else{
							sheet.cover[i].workFlag=RT_TRUE;//打开
					}
					sheet.cover[i].slaveAddr=slaveAddr;	
					sheet.cover[i].useUartNum=uartnum;
					rt_strcpy(sheet.cover[i].model,argv[3]);
					mod_printf("crackMeter reconfig %d\n",i);
					mod_printf("crackMeter OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==COVER_485_NUM){//没有配置过
		  int j=0;
			for(j=0;j<COVER_485_NUM;j++){
					if(sheet.cover[j].workFlag!=RT_TRUE){
							sheet.cover[j].workFlag=RT_TRUE;//打开
							sheet.cover[j].slaveAddr=slaveAddr;	
							sheet.cover[j].useUartNum=uartnum;
							rt_strcpy(sheet.cover[j].model,argv[3]);
							rt_strcpy(sheet.cover[j].ID,argv[2]);
							mod_printf("cover config %d\n",j);
						  mod_printf("cover OK\n");
							ret =1;
							break;
					}
			}
			if(j==COVER_485_NUM){
					mod_printf("ERR totalNum=%d\n",COVER_485_NUM);
			}
	}
	return ret;
}
//三轴的配置
static int threeAxisConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.threeAxissColTime =atoi32(argv[6],10);
	for( i=0;i<THREEAXIS_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.threeAxiss[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.threeAxiss[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del threeAxis\n");
						  return 1;
					}
					else{
							sheet.threeAxiss[i].workFlag=RT_TRUE;//打开
					}
					sheet.threeAxiss[i].slaveAddr=slaveAddr;	
					sheet.threeAxiss[i].useUartNum=uartnum;
					rt_strcpy(sheet.threeAxiss[i].model,argv[3]);
					mod_printf("threeaxis reconfig %d\n",i);
					mod_printf("threeaxis OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==THREEAXIS_485_NUM){//没有配置过
		  int j;
			for(j=0;j<THREEAXIS_485_NUM;j++){
					if(sheet.threeAxiss[j].workFlag!=RT_TRUE){
							sheet.threeAxiss[j].workFlag=RT_TRUE;//打开
							sheet.threeAxiss[j].slaveAddr=slaveAddr;	
							sheet.threeAxiss[j].useUartNum=uartnum;
							rt_strcpy(sheet.threeAxiss[j].model,argv[3]);
							rt_strcpy(sheet.threeAxiss[j].ID,argv[2]);
							mod_printf("threeaxis config %d\n",j);
						  mod_printf("threeaxis OK\n");
							ret =1;
							break;
					}
			}
			if(j==THREEAXIS_485_NUM){
					mod_printf("ERR totalNum=%d\n",THREEAXIS_485_NUM);
			}
	}
	return ret;
}
#ifdef USE_4GAS
//ch4的配置
static int ch4Conf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.ch4ColTime=atoi32(argv[6],10);
	for( i=0;i<CH4_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.ch4[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.ch4[i].workFlag=RT_FALSE;//关闭
							mod_printf("del ch4\n");
						  return 1;
					}
					else{
							sheet.ch4[i].workFlag=RT_TRUE;//打开
					}
					sheet.ch4[i].slaveAddr=slaveAddr;	
					sheet.ch4[i].useUartNum=uartnum;
					rt_strcpy(sheet.ch4[i].model,argv[3]);
					mod_printf("ch4 reconfig %d\n",i);
					mod_printf("ch4 OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==CH4_485_NUM){//没有配置过
		  int j;
			for(int j=0;j<CH4_485_NUM;j++){
					if(sheet.ch4[j].workFlag!=RT_TRUE){
							sheet.ch4[j].workFlag=RT_TRUE;//打开
							sheet.ch4[j].slaveAddr=slaveAddr;	
							sheet.ch4[j].useUartNum=uartnum;
							rt_strcpy(sheet.ch4[j].model,argv[3]);
							rt_strcpy(sheet.ch4[j].ID,argv[2]);
							mod_printf("ch4 config %d\n",j);
						  mod_printf("ch4 OK\n");
							ret =1;
							break;
					}
			}
			if(j==CH4_485_NUM){
					mod_printf("ERR totalNum=%d\n",CH4_485_NUM);
			}
	}
	return ret;
}
//co的配置
static int coConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.coColTime=atoi32(argv[6],10);
	for( i=0;i<CO_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.co[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.co[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del co\n");
						  return 1;
					}
					else{
							sheet.co[i].workFlag=RT_TRUE;//打开
					}
					sheet.co[i].slaveAddr=slaveAddr;	
					sheet.co[i].useUartNum=uartnum;
					rt_strcpy(sheet.co[i].model,argv[3]);
					mod_printf("co reconfig %d\n",i);
					mod_printf("co OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==CO_485_NUM){//没有配置过
		  int j;
			for(j=0;j<CO_485_NUM;j++){
					if(sheet.co[j].workFlag!=RT_TRUE){
							sheet.co[j].workFlag=RT_TRUE;//打开
							sheet.co[j].slaveAddr=slaveAddr;	
							sheet.co[j].useUartNum=uartnum;
							rt_strcpy(sheet.co[j].model,argv[3]);
							rt_strcpy(sheet.co[j].ID,argv[2]);
							mod_printf("co config %d\n",j);
						  mod_printf("co OK\n");
							ret =1;
							break;
					}
			}
			if(j==CO_485_NUM){
					mod_printf("ERR totalNum=%d\n",CO_485_NUM);
			}
	}
	return ret;
}
//h2s的配置
static int h2sConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.h2sColTime=atoi32(argv[6],10);
	for( i=0;i<H2S_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.h2s[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.h2s[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del h2s\n");
						  return 1;
					}
					else{
							sheet.h2s[i].workFlag=RT_TRUE;//打开
					}
					sheet.h2s[i].slaveAddr=slaveAddr;	
					sheet.h2s[i].useUartNum=uartnum;
					rt_strcpy(sheet.h2s[i].model,argv[3]);
					mod_printf("h2s reconfig %d\n",i);
					mod_printf("h2s OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==H2S_485_NUM){//没有配置过
		  int j=0;
			for(j=0;j<H2S_485_NUM;j++){
					if(sheet.h2s[j].workFlag!=RT_TRUE){
							sheet.h2s[j].workFlag=RT_TRUE;//打开
							sheet.h2s[j].slaveAddr=slaveAddr;	
							sheet.h2s[j].useUartNum=uartnum;
							rt_strcpy(sheet.h2s[j].model,argv[3]);
							rt_strcpy(sheet.h2s[j].ID,argv[2]);
							mod_printf("h2s config %d\n",j);
						  mod_printf("h2s OK\n");
							ret =1;
							break;
					}
			}
			if(j==H2S_485_NUM){
					mod_printf("ERR totalNum=%d\n",H2S_485_NUM);
			}
	}
	return ret;
}
//o2的配置
static int o2Conf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.cirCulaColTime=atoi32(argv[6],10);
	sheet.o2ColTime=atoi32(argv[6],10);
	for( i=0;i<O2_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.o2[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.o2[i].workFlag=RT_FALSE;//关闭
						  mod_printf("del o2\n");
						  return 1;
					}
					else{
							sheet.o2[i].workFlag=RT_TRUE;//打开
					}
					sheet.o2[i].slaveAddr=slaveAddr;	
					sheet.o2[i].useUartNum=uartnum;
					rt_strcpy(sheet.o2[i].model,argv[3]);
					mod_printf("o2 reconfig %d\n",i);
					mod_printf("o2 OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==O2_485_NUM){//没有配置过
		  int j;
			for(j=0;j<O2_485_NUM;j++){
					if(sheet.o2[j].workFlag!=RT_TRUE){
							sheet.o2[j].workFlag=RT_TRUE;//打开
							sheet.o2[j].slaveAddr=slaveAddr;	
							sheet.o2[j].useUartNum=uartnum;
							rt_strcpy(sheet.o2[j].model,argv[3]);
							rt_strcpy(sheet.o2[j].ID,argv[2]);
							mod_printf("o2 config %d\n",j);
						  mod_printf("o2 OK\n");
							ret =1;
							break;
					}
			}
			if(j==O2_485_NUM){
					mod_printf("ERR totalNum=%d\n",O2_485_NUM);
			}
	}
	return ret;
}
#endif
//水位的配置
static int waterDepthConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.waterDepthColTime=atoi32(argv[6],10);
	for( i=0;i<WATERDEPTH_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.waterDepth[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.waterDepth[i].workFlag=RT_FALSE;//关闭
							mod_printf("del waterdepth\n");
						  return 1;
					}
					else{
							sheet.waterDepth[i].workFlag=RT_TRUE;//打开
					}
					sheet.waterDepth[i].slaveAddr=slaveAddr;	
					sheet.waterDepth[i].useUartNum=uartnum;
					rt_strcpy(sheet.waterDepth[i].model,argv[3]);
					mod_printf("waterDepth reconfig %d\n",i);
					mod_printf("waterDepth OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==WATERDEPTH_485_NUM){//没有配置过
		  int j;
			for(j=0;j<WATERDEPTH_485_NUM;j++){
					if(sheet.waterDepth[j].workFlag!=RT_TRUE){
							sheet.waterDepth[j].workFlag=RT_TRUE;//打开
							sheet.waterDepth[j].slaveAddr=slaveAddr;	
							sheet.waterDepth[j].useUartNum=uartnum;
							rt_strcpy(sheet.waterDepth[j].model,argv[3]);
							rt_strcpy(sheet.waterDepth[j].ID,argv[2]);
							mod_printf("waterDepth config %d\n",j);
						  mod_printf("waterDepth OK\n");
							ret =1;
							break;
					}
			}
			if(j==WATERDEPTH_485_NUM){
					mod_printf("ERR totalNum=%d\n",WATERDEPTH_485_NUM);
			}
	}
	return ret;
}
//温湿度的配置
static int tempHumConf(int uartnum,char *argv[])
{
	int i=0;
	int ret=0;
	int slaveAddr=atoi32(argv[5],10);
	sheet.tempHumColTime=atoi32(argv[6],10);
	for( i=0;i<TEMPHUM_485_NUM;i++){//核对有没有配置过
			if(rt_strcmp(sheet.tempHum[i].ID,argv[2])==0){//配置过
					if((slaveAddr==0)||(slaveAddr==255)){//关闭
							sheet.tempHum[i].workFlag=RT_FALSE;//关闭
							mod_printf("del temp humi\n");
						  return 1;
					}
					else{
							sheet.tempHum[i].workFlag=RT_TRUE;//打开
					}
					sheet.tempHum[i].slaveAddr=slaveAddr;	
					sheet.tempHum[i].useUartNum=uartnum;
					rt_strcpy(sheet.tempHum[i].model,argv[3]);
					mod_printf("tempHum reconfig %d\n",i);
					mod_printf("tempHum OK\n");
					ret =1;
					break;
			}
	}
	if((slaveAddr==0)||(slaveAddr==255)){
			return 0;
	}
	if(i==TEMPHUM_485_NUM){//没有配置过
		  int j;
			for( j=0;j<TEMPHUM_485_NUM;j++){
					if(sheet.tempHum[j].workFlag!=RT_TRUE){
							sheet.tempHum[j].workFlag=RT_TRUE;//打开
							sheet.tempHum[j].slaveAddr=slaveAddr;	
							sheet.tempHum[j].useUartNum=uartnum;
							rt_strcpy(sheet.tempHum[j].model,argv[3]);
							rt_strcpy(sheet.tempHum[j].ID,argv[2]);
							mod_printf("tempHum config %d\n",j);
						  mod_printf("tempHum OK\n");
							ret =1;
							break;
					}
			}
			if(j==TEMPHUM_485_NUM){
					mod_printf("ERR totalNum=%d\n",TEMPHUM_485_NUM);
			}
	}
	return ret;
}
//modbus设备的配置
static int modbusConf(int modbusnum,int uartnum,char *argv[])
{
	  int ret=0;
	  //int slaveAddr=atoi32(argv[5],10);//提取设备地址 前面已经核对过
//		mod_printf("conf:%d %d\n",modbusnum,uartnum);
		switch(modbusnum)
		{
			case CIRCULA:
				ret =circulaConf(uartnum,argv);
			break;
			case PARTDISCHAG:
				ret=partDischagConf(uartnum,argv);
			break;
			case PRESSSETTL:
				ret=pressSettlConf(uartnum,argv);
			break;
			case THREEAXIS:
				ret=threeAxisConf(uartnum,argv);
			break;
#ifdef USE_4GAS
			case CH4:
				ret=ch4Conf(uartnum,argv);
			break;
			case O2:
				ret=o2Conf(uartnum,argv);
			break;
			case H2S:
				ret=h2sConf(uartnum,argv);
      break;
      case CO:
				ret=coConf(uartnum,argv);
      break;
#endif
      case TEMPHUM:
				ret=tempHumConf(uartnum,argv);
      break;
      case WATERDEPTH:
				ret=waterDepthConf(uartnum,argv);
			break;
			case CRACKMETER:
				ret=crackMeterConf(uartnum,argv);
			break;
			case COVER:
				ret=coverConf(uartnum,argv);
			default:
			break;
		}
		return ret;
}
//配置前检查ID有没有重复的 有的话删除重复的ID
//int modbusConfIDCheck(char *inputID)
//{
//		for(int i=0;i<MODBUS_NUM;i++){
//			switch(i)
//			{
//				case CIRCULA:
//					for(int j=0;j<CIRCULA_485_NUM;j++){//核对有没有配置过
//							if(sheet.cirCula[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.cirCula[j].ID,inputID)==0){
//											mod_printf("del cirCula same ID\n");
//											sheet.cirCula[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case PARTDISCHAG:
//					for(int j=0;j<PARTDISCHAG_485_NUM;j++){//核对有没有配置过
//							if(sheet.partDischag[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.partDischag[j].ID,inputID)==0){
//											mod_printf("del partDischag same ID\n");
//											sheet.partDischag[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case PRESSSETTL:
//					for(int j=0;j<PRESSSETTL_485_NUM;j++){//核对有没有配置过
//							if(sheet.pressSetl[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.pressSetl[j].ID,inputID)==0){
//											mod_printf("del pressSetl same ID\n");
//											sheet.pressSetl[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case THREEAXIS:
//					for(int j=0;j<THREEAXIS_485_NUM;j++){//核对有没有配置过
//							if(sheet.threeAxiss[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.threeAxiss[j].ID,inputID)==0){
//											mod_printf("del threeAxiss same ID\n");
//											sheet.threeAxiss[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//	#ifdef 	 USE_4GAS 	
//				case CH4:
////					for(int j=0;j<CH4_485_NUM;j++){//核对有没有配置过
////							if(sheet.ch4[j].workFlag==RT_TRUE){
////									if(rt_strcmp(sheet.ch4[j].ID,inputID)==0){
////											mod_printf("del ch4 same ID\n");
////											sheet.ch4[j].workFlag=RT_FALSE;
////										  return 1;
////									}
////								}
////					}
//				break;
//				case O2:
////					for(int j=0;j<O2_485_NUM;j++){//核对有没有配置过
////							if(sheet.o2[j].workFlag==RT_TRUE){
////									if(rt_strcmp(sheet.o2[j].ID,inputID)==0){
////											mod_printf("del o2 same ID\n");
////											sheet.o2[j].workFlag=RT_FALSE;
////										  return 1;
////									}
////							}
////					}
//				break;
//				case H2S:
////					for(int j=0;j<H2S_485_NUM;j++){//核对有没有配置过
////							if(sheet.h2s[j].workFlag==RT_TRUE){
////									if(rt_strcmp(sheet.h2s[j].ID,inputID)==0){
////											mod_printf("del h2s same ID\n");
////											sheet.h2s[j].workFlag=RT_FALSE;
////										  return 1;
////									}
////							}
////					}
//				break;
//				case CO:
////					for(int j=0;j<CO_485_NUM;j++){//核对有没有配置过
////							if(sheet.co[j].workFlag==RT_TRUE){
////									if(rt_strcmp(sheet.co[j].ID,inputID)==0){
////											mod_printf("del co same ID\n");
////											sheet.co[j].workFlag=RT_FALSE;
////										  return 1;
////									}
////							}
////					}
//				break;
//	#endif
//				case TEMPHUM:
//					for(int j=0;j<TEMPHUM_485_NUM;j++){//核对有没有配置过
//							if(sheet.tempHum[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.tempHum[j].ID,inputID)==0){
//											mod_printf("del TEMPHUM same ID\n");
//											sheet.tempHum[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case WATERDEPTH:
//					for(int j=0;j<WATERDEPTH_485_NUM;j++){//核对有没有配置过
//							if(sheet.waterDepth[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.waterDepth[j].ID,inputID)==0){
//											mod_printf("del waterDepth same ID\n");
//											sheet.waterDepth[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case CRACKMETER:
//					for(int j=0;j<CRACKMETER_485_NUM;j++){//核对有没有配置过
//							if(sheet.crackMeter[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.crackMeter[j].ID,inputID)==0){
//											mod_printf("del crackMeter same ID\n");
//											sheet.crackMeter[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				case COVER:
//					for(int j=0;j<COVER_485_NUM;j++){//核对有没有配置过
//							if(sheet.cover[j].workFlag==RT_TRUE){
//									if(rt_strcmp(sheet.cover[j].ID,inputID)==0){
//											mod_printf("del cover same ID\n");
//											sheet.cover[j].workFlag=RT_FALSE;
//										  return 1;
//									}
//							}
//					}
//				break;
//				default:
//				break;
//			}
//		}
//		
//		
//		
//		return 0;
//}
//相同种类的传感器必须挂到同一个port下边
//每种传感器最后一次输入的采集时间为最终采集时间
//modbus   局放 port2 2 60
//使用前提条件必须启用upKeepStateTask中参数以及回调函数初始化才能使用
//核对名称 核对地址 核对port才能写入
//modbus+设备名称(波特率)+ID+model+端口(port1-port4)+设备地址(0/255-关闭设备)+采集间隔(秒)
static void modbus(int argc, char *argv[])
{
	  extern void timeStop(upDataTimEnum num);
	  extern void timeInit(upDataTimEnum num,int value,int firstCnt);
	  int i,j;
		if ((argc != 7)&&(argc != 2))
		{
				mod_printf("ERR input argc\n");
				goto ERR;
		}
		if(devIDOKCheck(argv[2])!=true){
			if((strcmp(argv[5],"0")!=0)&&(strcmp(argv[5],"255")!=0)){
				mod_printf("ERR input the same ID\n");
			  return;
			}
		}
		if(argc==2){
				if(0==rt_strcmp((char *)"list", argv[1])){
						printModbusDevList();
						return;
				}
		}
		else{
				uint8_t slaveaddr=atoi16(argv[5],10);
				int setTime =atoi16(argv[6],10);
				if(setTime<60){
						mod_printf("err:argv[6] 采集时间>60 now is%d\n",setTime);
						goto ERR;
				}
				if(slaveaddr>255){
						mod_printf("err:argv[5] between 0 and 255 %d\n",argv[5]);
						goto ERR;
				}
				for(i=0;i<MODBUS_NUM;i++){
						if(0==rt_strcmp((char *)modbusName[i], argv[1])){
								for(j=0;j<UART_NUM;j++){
										if(0==rt_strcmp((char *)UartName[j], argv[4])){//串口 合法
												break;
										}
								}
								break;
						}
				}
				if(i>=MODBUS_NUM){
						mod_printf("err:argv[1]\n");
						for(int j=0;j<MODBUS_NUM;j++)
								mod_printf("%s ",modbusName[j]);
						mod_printf("\n ");
						goto ERR;
				}
				if(j>=UART_NUM){
						mod_printf("err:argv[2]\n");
						for(int j=0;j<UART_NUM;j++)
								mod_printf("%s ",UartName[j]);
						mod_printf("\n ");
						goto ERR;
				}
				if(1==modbusConf(i,j,argv)){//配置modbus参数到表中
						return;//正确跳出
				}
				else{
						mod_printf("modbus exit delete first\n");
				}
	
		}
		
		ERR:
		mod_printf("for example:modbus+设备名称(波特率)+ID+model+端口(port1-port%d)+设备地址(0/255-关闭设备)+采集间隔(秒)\n",UART_NUM);
		mod_printf("NOTE:括号内对参数进行解释,不需要输入\n");
		mod_printf("modbus list\n");
}
//FINSH_FUNCTION_EXPORT(modbus, offline finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(modbus,port slaveaddr config);//FINSH_FUNCTION_EXPORT_CMD


#if 1
//检查ID号是否重复  true -无重复  false-重复
//函数作用 为了保值ID配置的唯一性
bool  devIDOKCheck(char *inputID)
{
		for(int i=0;i<MODBUS_NUM;i++){
			switch(i)
			{
				case CIRCULA:
					for(int j=0;j<CIRCULA_485_NUM;j++){//核对有没有配置过
							if(sheet.cirCula[j].workFlag==RT_TRUE){
									if(strcmp(inputID,sheet.cirCula[j].ID)==0)
										return false;
							}
					}
				break;
				case PARTDISCHAG:
					for(int j=0;j<PARTDISCHAG_485_NUM;j++){//核对有没有配置过
							if(sheet.partDischag[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.partDischag[j].ID)==0)
										return false;
							}
					}
				break;
				case PRESSSETTL:
					for(int j=0;j<PRESSSETTL_485_NUM;j++){//核对有没有配置过
							if(sheet.pressSetl[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.pressSetl[j].ID)==0)
										return false;
							}
					}
				break;
				case THREEAXIS:
					for(int j=0;j<THREEAXIS_485_NUM;j++){//核对有没有配置过
							if(sheet.threeAxiss[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.threeAxiss[j].ID)==0)
										return false;
							}
					}
				break;
#ifdef USE_4GAS
				case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//核对有没有配置过
							if(sheet.ch4[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.ch4[j].ID)==0)
										return false;
							}
					}
				break;
				case O2:
					for(int j=0;j<O2_485_NUM;j++){//核对有没有配置过
							if(sheet.o2[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.o2[j].ID)==0)
										return false;
							}
					}
				break;
				case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//核对有没有配置过
							if(sheet.h2s[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.h2s[j].ID)==0)
										return false;
							}
					}
				break;
				case CO:
					for(int j=0;j<CO_485_NUM;j++){//核对有没有配置过
							if(sheet.co[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.co[j].ID)==0)
										return false;
							}
					}
				break;
#endif
				case TEMPHUM:
					for(int j=0;j<TEMPHUM_485_NUM;j++){//核对有没有配置过
							if(sheet.tempHum[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.tempHum[j].ID)==0)
										return false;
							}
					}
				break;
				case WATERDEPTH:
					for(int j=0;j<WATERDEPTH_485_NUM;j++){//核对有没有配置过
							if(sheet.waterDepth[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.waterDepth[j].ID)==0)
										return false;
							}
					}
				break;
				case CRACKMETER:
					for(int j=0;j<CRACKMETER_485_NUM;j++){//核对有没有配置过
							if(sheet.crackMeter[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.crackMeter[j].ID)==0)
										return false;
							}
					}
				break;
				case COVER:
					for(int j=0;j<COVER_485_NUM;j++){//核对有没有配置过
							if(sheet.cover[j].workFlag==RT_TRUE){
								if(strcmp(inputID,sheet.cover[j].ID)==0)
										return false;
							}
					}
				break;
				default:
				break;
			}	
		}

		int i;
//		bool  add=true;
////////////////////////////////////////////////模拟数据打包////////////////////////////////////////////////////
#ifndef     ANA_MASK
		for(i=0;i<ANALOG_NUM;i++){
			  add=true;
				if(i>=1){
						for(j=0;j<i;j++){
								if(rt_strcmp(sheet.analog[i].ID,sheet.analog[j].ID)==0){
										add=false;//找到了重复的 跳过此次i的计数
									  break;
								}
						}
				}
				if(add==false){
					  continue;//跳过此次循环 执行下一次
				}
				if(sheet.analog[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
						cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.analog[i].model));
						cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(sheet.analog[i].name));
						cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.analog[i].ID));
				}
		 }
#endif
////////////////////////////////////////////////数字输入打包////////////////////////////////////////////////////
		 for(i=0;i<DI_NUM;i++){
				if(packFlash.diginput[i].workFlag==RT_TRUE){
						if(strcmp(inputID,packFlash.diginput[i].devID)==0)
								return false;
	
				}
		 }
////////////////////////////////////////////////数字输出打包////////////////////////////////////////////////////
		 for(i=0;i<DO_NUM;i++){
				if(packFlash.digoutput[i].workFlag==RT_TRUE){
						if(strcmp(inputID,packFlash.digoutput[i].devID)==0)
								return false;
				}
		 }
////////////////////////////////////////////////输出3V3打包////////////////////////////////////////////////////
//		 for(i=0;i<V33O_NUM;i++){
//				if(packFlash.v33output[i].workFlag==RT_TRUE){
//						if(strcmp(inputID,packFlash.v33output[i].devID)==0)
//								return false;
//				}
//		 }
//////////////////////////////////////////////////输出5V注册打包////////////////////////////////////////////////////
//		 for(i=0;i<V5O_NUM;i++){
//				if(packFlash.v5output[i].workFlag==RT_TRUE){
//						if(strcmp(inputID,packFlash.v5output[i].devID)==0)
//								return false;
//				}
//		 }
////////////////////////////////////////////////输出12V注册打包////////////////////////////////////////////////////
		 for(i=0;i<SWITCH_NUM;i++){
				if(packFlash.switchoutput[i].workFlag==RT_TRUE){
						if(strcmp(inputID,packFlash.switchoutput[i].devID)==0)
								return false;
				}
		 }
		 return true;
}
#endif

