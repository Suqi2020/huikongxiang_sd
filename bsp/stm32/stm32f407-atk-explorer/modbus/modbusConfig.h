#ifndef __MODBUSCONFIG_H__
#define __MODBUSCONFIG_H__
//#include "uartReconf.h"
#include  "board.h"

/*
		AIR_ENVIRONMENT(1, "空气环境"),  //包括温湿度 气体
    WATER_LEVEL(2, "水位"),
    DISCHARGE(3, "局放"),
    GROUNDING_CURRENT(4, "环流"),
    OPTICAL_FIBER(5, "光纤测温"),
    VIBRATION(6, "外破"),
    SETTLEMENT(7, "沉降"),
    SWITCH_CONTROL(8, "风机/水泵");
		CRACK_TESTER(9,"裂缝仪");
		MANHOLE_COVER(10,"井盖");
*/
#define  MODL_LEN     8


typedef  enum{
		AIR_ENVIRONMENT=1,//(1, "空气环境"),  //包括温湿度 气体
    WATER_LEVEL,//(2, "水位"),
    DISCHARGE,//(3, "局放"),
    GROUNDING_CURRENT,//(4, "环流"),
    OPTICAL_FIBER,//(5, "光纤测温"),
    VIBRATION,//(6, "外破"),
    SETTLEMENT,//(7, "沉降"),
    SWITCH_CONTROL,//(8, "风机/水泵");
	  CRACK_TESTER,//(9,"裂缝仪");
	  MANHOLE_COVER//(10,"井盖");
}regTypeEnum;



typedef struct{
		bool 	workFlag;
 		uint16_t  	useUartNum;
		uint16_t  	slaveAddr; //0-255
		char  ID[MODBID_LEN];
		char  model[MODL_LEN];

}modbusStru;
//modbus传感器温湿度
typedef struct{
		float tempUpLimit;
		float tempLowLimit;
		float humUpLimit;
		float humLowLimit;

}tempHumStru_p;
typedef struct{
		
	  uint32_t  tempUpFlag;//阈值超限的标记
	  uint32_t  tempLowFlag;
	  uint32_t  humUpFlag;
	  uint32_t  humLowFlag;
}tempHumFlagStru;
//modbus传感器防外破
typedef struct{
		float tempUpLimit;
	  float tempLowLimit;
	  uint16_t accXUpLimit;
	  uint16_t accXLowLimit;
	  uint16_t accYUpLimit;
	  uint16_t accYLowLimit;
	  uint16_t accZUpLimit;
	  uint16_t accZLowLimit;
}threeAxisStru_p;
typedef struct{
		uint32_t  tempUpFlag;//阈值超限的标记
		uint32_t  tempLowFlag;
		uint32_t  accXUpFlag;
		uint32_t  accXLowFlag;
		uint32_t  accYUpFlag;
		uint32_t  accYLowFlag;
		uint32_t  accZUpFlag;
		uint32_t  accZLowFlag;
}threeAxisFlagStru;
//modbus传感 沉降仪
typedef struct{
	  float tempUpLimit;
		float tempLowLimit;
	  float heightUpLimit;
	  float heightLowLimit;
}pressSettlStru_p;
//modbus传感裂缝仪阈值
typedef struct{
	  float tempUpLimit;
		float tempLowLimit;
	  float distancUpLimit;
	  float distancLowLimit;
}crackMeterStru_p;
//modbus井盖阈值
typedef struct{
	  uint8_t inclineUpLimit;
	  uint8_t inclineLowLimit;
	  uint8_t switch1UpLimit;
	  uint8_t switch1LowLimit;
	  uint8_t vibrationUpLimit;
	  uint8_t vibrationLowLimit;
	  uint8_t switch2UpLimit;
	  uint8_t switch2LowLimit;
}coverStru_p;
typedef struct{
		uint32_t  tempUpFlag;//阈值超限的标记
		uint32_t  tempLowFlag;
		uint32_t  heightUpFlag;
		uint32_t  heightLowFlag;
}pressSettlFlagStru;


typedef struct{
		uint32_t  tempUpFlag;//阈值超限的标记
		uint32_t  tempLowFlag;
		uint32_t  distancUpFlag;
		uint32_t  distancLowFlag;
}crackMeterFlagStru;


typedef struct{
	  uint8_t inclineUpFlag;
	  uint8_t inclineLowFlag;
	  uint8_t switch2UpFlag;
	  uint8_t switch2LowFlag;
	  uint8_t vibrationUpFlag;
	  uint8_t vibrationLowFlag;
	
	  uint8_t switch1UpFlag;
	  uint8_t switch1LowFlag;
}coverFlagStru;
typedef struct
{
	  //环流值 放大了100倍
		float cirCurAUpLimit;
	  float cirCurALowLimit;
		float cirCurBUpLimit;
	  float cirCurBLowLimit;
		float cirCurCUpLimit;
	  float cirCurCLowLimit;
}circuStru_p;
typedef struct{
		uint32_t  cirCurAUpFlag;//阈值超限的标记
		uint32_t  cirCurALowFlag;
		uint32_t  cirCurBUpFlag;
		uint32_t  cirCurBLowFlag;
		uint32_t  cirCurCUpFlag;
		uint32_t  cirCurCLowFlag;
}circuFlagStru;

//modbus传感器co
typedef struct{
		float coUpLimit;
		float coLowLimit;
}coStru_p;
typedef struct{
		uint32_t  coUpFlag;//阈值超限的标记
		uint32_t  coLowFlag;
}coFlagStru;
//modbus传感器O2
typedef struct{
		float o2UpLimit;
		float o2LowLimit;
}o2Stru_p;
typedef struct{
	  uint32_t  o2UpFlag;//阈值超限的标记
		uint32_t  o2LowFlag;
}o2FlagStru;
//modbus传感器ch4
typedef struct{
		float ch4UpLimit;
		float ch4LowLimit;
}ch4Stru_p;
typedef struct{
	  uint32_t  ch4UpFlag;//阈值超限的标记
		uint32_t  ch4LowFlag;
}ch4FlagStru;
//modbus传感器h2s
typedef struct{
		float h2sUpLimit;
		float h2sLowLimit;
}h2sStru_p;
typedef  struct{
	  uint32_t  h2sUpFlag;//阈值超限的标记
		uint32_t  h2sLowFlag;
}h2sFlagStru;
//modbus传感器waterLev
typedef struct{
		float depthUpLimit;
		float depthLowLimit;
}depthStru_p;
typedef struct{
	  uint32_t  depthUpFlag;//阈值超限的标记
		uint32_t  depthLowFlag;
}depthFlagStru;

//modbus传感器partDischarg
typedef struct{
		uint32_t amplitudeAUpLimit;
		uint32_t amplitudeALowLimit;
		uint32_t freqAUpLimit;
		uint32_t freqALowLimit;
		uint32_t dischargeAUpLimit;
		uint32_t dischargeALowLimit;
	
		uint32_t amplitudeBUpLimit;
		uint32_t amplitudeBLowLimit;
		uint32_t freqBUpLimit;
		uint32_t freqBLowLimit;
		uint32_t dischargeBUpLimit;
		uint32_t dischargeBLowLimit;

		uint32_t amplitudeCUpLimit;
		uint32_t amplitudeCLowLimit;
		uint32_t freqCUpLimit;
		uint32_t freqCLowLimit;
		uint32_t dischargeCUpLimit;
		uint32_t dischargeCLowLimit;
}partDisChgStru_p;
typedef struct{
	  uint32_t  amplitudeAUpFlag;//阈值超限的标记
		uint32_t  amplitudeALowFlag;
	  uint32_t  amplitudeBUpFlag;
		uint32_t  amplitudeBLowFlag;
		uint32_t  amplitudeCUpFlag;
		uint32_t  amplitudeCLowFlag;
		
	  uint32_t  freqAUpFlag;//阈值超限的标记
		uint32_t  freqALowFlag;
	  uint32_t  freqBUpFlag;
		uint32_t  freqBLowFlag;
		uint32_t  freqCUpFlag;
		uint32_t  freqCLowFlag;

	  uint32_t  dischargeAUpFlag;//阈值超限的标记
		uint32_t  dischargeALowFlag;
	  uint32_t  dischargeBUpFlag;
		uint32_t  dischargeBLowFlag;
		uint32_t  dischargeCUpFlag;
		uint32_t  dischargeCLowFlag;
}partDisChgFlagStru;

//extern const uint8_t sheShiDu[2];

#define GAS_NUM               2
#define THREEAXIS_485_NUM     40
#define PRESSSETTL_485_NUM    40
#define CIRCULA_485_NUM   	  5
#define PARTDISCHAG_485_NUM   5
#define CRACKMETER_485_NUM    20
#define COVER_485_NUM         10
#ifdef  USE_4GAS
#define CH4_485_NUM   			  GAS_NUM
#define CO_485_NUM   				  GAS_NUM
#define H2S_485_NUM   			  GAS_NUM
#define O2_485_NUM   				  GAS_NUM
#endif
#define WATERDEPTH_485_NUM   	2
#define TEMPHUM_485_NUM   	  2
#ifdef  USE_4GAS
	#define TOTOLA_485_NUM     (THREEAXIS_485_NUM+\
														PRESSSETTL_485_NUM+\
														CIRCULA_485_NUM+\
														PARTDISCHAG_485_NUM+\
														CH4_485_NUM+\
														CO_485_NUM+\
														H2S_485_NUM+\
														O2_485_NUM+\
														WATERDEPTH_485_NUM+\
														TEMPHUM_485_NUM+\
														CRACKMETER_485_NUM+\
														COVER_485_NUM)
#else
	#define TOTOLA_485_NUM     (THREEAXIS_485_NUM+\
														PRESSSETTL_485_NUM+\
														CIRCULA_485_NUM+\
														PARTDISCHAG_485_NUM+\
														WATERDEPTH_485_NUM+\
														TEMPHUM_485_NUM+\
														CRACKMETER_485_NUM+\
														COVER_485_NUM)
#endif
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//modbus+设备名称(波特率)+端口(port1-port4)+设备地址(0-关闭设备)+采集间隔(秒)
#ifdef  USE_4GAS
#define   MODBUS_NUM      (12) //加入井盖
#else 
#define   MODBUS_NUM      (8) //加入井盖
#endif
typedef struct{
	 void (* modbusRead)(void );
}modbusFunStru;
extern modbusFunStru modbusFun[MODBUS_NUM];
//////////////////////////////增加传感器需要修改下边 一一对应////////////////////////////////////////

//extern deviceFlashStru  modbusFlash[MODBUS_NUM];//需要存储到flash的设备配置
#ifdef  USE_4GAS
typedef enum{
    CIRCULA=0, 	PARTDISCHAG,			PRESSSETTL, 			THREEAXIS,		\
   	CH4,		O2		,H2S,			CO,			\
	  TEMPHUM,			WATERDEPTH,CRACKMETER,COVER
}modbNumEnum;
#else
typedef enum{
     CIRCULA=0, 	PARTDISCHAG,			PRESSSETTL, 			THREEAXIS,		\
	  TEMPHUM,			WATERDEPTH,CRACKMETER,COVER
}modbNumEnum;
#endif
#ifdef  USE_4GAS

#ifndef     ANA_MASK
typedef enum{
		CIRCULA_TIME=0,PARTDISCHAG_TIME,PRESSSETTL_TIME,THREEAXIS_TIME,\
	  CH4_TIME,O2_TIME,H2S_TIME,CO_TIME,\
	  TEMPHUM_TIME,WATERDEPTH_TIME,CRACKMETER_TIME,COVER_TIME,\
	  ANA_TEMPHUM_TIME,\
	HEART_TIME,REG_TIME
}upDataTimEnum;//需要与modbusName 名称一一对应 来实现代码精简高效
#else
typedef enum{
		CIRCULA_TIME=0,PARTDISCHAG_TIME,PRESSSETTL_TIME,THREEAXIS_TIME,\
	  CH4_TIME,O2_TIME,H2S_TIME,CO_TIME,\
	  TEMPHUM_TIME,WATERDEPTH_TIME,CRACKMETER_TIME,COVER_TIME,\
	HEART_TIME,REG_TIME
}upDataTimEnum;//需要与modbusName 名称一一对应 来实现代码精简高效
#endif
#else

#ifndef     ANA_MASK
typedef enum{
		CIRCULA_TIME=0,PARTDISCHAG_TIME,PRESSSETTL_TIME,THREEAXIS_TIME,\
	  TEMPHUM_TIME,WATERDEPTH_TIME,\
	  ANA_TEMPHUM_TIME,HEART_TIME,REG_TIME
}upDataTimEnum;//需要与modbusName 名称一一对应 来实现代码精简高效
#else
typedef enum{
		CIRCULA_TIME=0,PARTDISCHAG_TIME,PRESSSETTL_TIME,THREEAXIS_TIME,\
	  TEMPHUM_TIME,WATERDEPTH_TIME,\
	  HEART_TIME,REG_TIME
}upDataTimEnum;//需要与modbusName 名称一一对应 来实现代码精简高效
#endif
#endif
#ifdef  USE_4GAS

#define GAS_TIME    CO_TIME  //4种气体传感器使用 同一个定时器
#else
#define GAS_TIME    22      //使用一种混合气体传感器时候定义此处
#endif
#define CHINA_NAME_LEN  9
#ifdef  USE_4GAS
const static char  modbusName[MODBUS_NUM][NAME_LEN] ={"HuanLiu","JuFang","FangChenJiang","FangWaiPo","JiaWan","YangQi","LiuHuaQing","YiYangHuaTan","WenShiDu","ShuiWei","LieFengYi","JingGai"};


const static uint8_t mdbusChinaName[MODBUS_NUM][CHINA_NAME_LEN]={0xBB,0xB7,0xC1,0xF7,0x00,0x00,0x00,0x00,0x00,\
																										0xBE,0xD6,0xB7,0xC5,0x00,0x00,0x00,0x00,0x00,\
																										0xB7,0xC0,0xB3,0xC1,0xBD,0xB5,0x00,0x00,0x00,\
																										0xB7,0xC0,0xCD,0xE2,0xC6,0xC6,0x00,0x00,0x00,\
																										0xBC,0xD7,0xCD,0xE9,0x00,0x00,0x00,0x00,0x00,\
																										0xD1,0xF5,0xC6,0xF8,0x00,0x00,0x00,0x00,0x00,\
																										0xC1,0xF2,0xBB,0xAF,0xC7,0xE2,0x00,0x00,0x00,\
																										0xD2,0xBB,0xD1,0xF5,0xBB,0xAF,0xCC,0xBC,0x00,\
																										0xCE,0xC2,0xCA,0xAA,0xB6,0xC8,0x00,0x00,0x00,\
																										0xCB,0xAE,0xCE,0xBB,0x00,0x00,0x00,0x00,0x00,\
																										0xC1,0xD1,0xB7,0xEC,0xD2,0xC7,0x00,0x00,0x00,\
																										0xBE,0xAE,0xB8,0xC7,0x00,0x00,0x00,0x00,0x00};
//const static char  modbusNameFold[MODBUS_NUM][NAME_LEN+6]={"0://HuanLiu","0://JuFang","0://FangChenJiang","0://FangWaiPo","0://JiaWan","0://YangQi","0://LiuHuaQing","0://YiYangHuaTan","0://WenShiDu","0://ShuiWei","0://LieFengYi","0://JingGai"};
const static char  modbusLog[MODBUS_NUM][100]={
	"earthCurA runCurA loadRatioA earthCurB runCurB loadRatioB earthCurC runCurC loadRatioC time(ms)\r\n",\
	"pdA freqA dischargeDateA pdB freqB dischargeDateB  pdC freqC dischargeDateC time(ms)\r\n",\
	"temp height time(ms)\r\n",\
	"temp accelerationX accelerationY accelerationZ time(ms)\r\n",\
	"methane time(ms)\r\n",\
	"oxygen time(ms)\r\n",\
	"hydrogenSulfide time(ms)\r\n",\
	"monoxide time(ms)\r\n",\
	"temperature humidity time(ms)\r\n",\
	"depth time(ms)\r\n",\
	"temp distance time(ms)\r\n",\
  "incline switch2(2盖) vibration switch1(1盖) time(ms)\r\n"\
};

	
const static int   modbusBps[MODBUS_NUM]      ={115200,   115200  ,9600,   9600,   9600,   9600,   9600,   9600,   9600,   9600,9600,9600};
#else
const static char  modbusName[MODBUS_NUM][NAME_LEN] ={"HuanLiu","JuFang","FangChenJiang","FangWaiPo","WenShiDu","ShuiWei","LieFengYi","JingGai"};
const static int   modbusBps[MODBUS_NUM]      ={115200,   115200  ,9600,   9600,     9600,   9600,9600,9600};

//const static char  modbusName[MODBUS_NUM][NAME_LEN] ={"接地环流","局放","防沉降","防外破","甲烷","氧气","硫化氢","一氧化碳","温湿度","水位","裂缝仪"};
//const static int   modbusBps[MODBUS_NUM]      ={115200,   115200  ,9600,   9600,   9600,   9600,   9600,   9600,   9600,   9600,9600};
//#else
//const static char  modbusName[MODBUS_NUM][NAME_LEN] ={"接地环流","局放","防沉降","防外破","温湿度","水位","裂缝仪"};
//const static int   modbusBps[MODBUS_NUM]      ={115200,   115200  ,9600,   9600,     9600,   9600,960};

#endif
//const static int   modbusType[MODBUS_NUM]     ={1,        1,       2,      2, 		 3,  			3,  		3,  		3,  		3,  		3};//想同类型的modbus设备名称相同
//extern char modbusName_UTF8[MODBUS_NUM][30];
extern const  char  modbusName_utf8[MODBUS_NUM][30];
typedef struct{
///////////////////////////////////////////////////
			modbusStru  cirCula[CIRCULA_485_NUM];
			modbusStru  partDischag[PARTDISCHAG_485_NUM];
			modbusStru  pressSetl[PRESSSETTL_485_NUM];
	    modbusStru  threeAxiss[THREEAXIS_485_NUM];
#ifdef  USE_4GAS
			modbusStru  ch4[CH4_485_NUM];
	    modbusStru  o2[O2_485_NUM];
			modbusStru  h2s[H2S_485_NUM];
			modbusStru  co[CO_485_NUM];
#endif
			modbusStru  tempHum[TEMPHUM_485_NUM];
			modbusStru  waterDepth[WATERDEPTH_485_NUM];
			modbusStru  crackMeter[CRACKMETER_485_NUM];
	    modbusStru  cover[COVER_485_NUM];
///////////////////////////////////////////////////
			uint32_t  cirCulaColTime;
			uint32_t  partDischagColTime;
			uint32_t  pressSetlColTime;
	    uint32_t  threeAxissColTime;
#ifdef  USE_4GAS

			uint32_t  ch4ColTime;
	    uint32_t  o2ColTime;
			uint32_t  h2sColTime;
			uint32_t  coColTime;//用co的定时器来采集信息  去掉其他三个气体定时器  合并打包上传
#endif
			uint32_t  tempHumColTime;
			uint32_t  waterDepthColTime;
			uint32_t  crackMeterColTime;
			uint32_t  coverColTime;
///////////////////////////////////////////////////
#ifndef     ANA_MASK
			analogStru analog[ANALOG_NUM];
#endif
///////////////////////////////////////////////////
//放入各种传感器上限下限值 start  跟modbusStru 一一对应
			circuStru_p      modbusCircul[CIRCULA_485_NUM];
			partDisChgStru_p modbusPartDisChg[PARTDISCHAG_485_NUM];
			pressSettlStru_p modbusPreSettl[PRESSSETTL_485_NUM];
			threeAxisStru_p  modbusThreAxis[THREEAXIS_485_NUM];
#ifdef  USE_4GAS
			ch4Stru_p        modbusCh4[CH4_485_NUM];
			o2Stru_p         modbusO2[O2_485_NUM];
			h2sStru_p        modbusH2s[H2S_485_NUM];
			coStru_p         modbusCo[CO_485_NUM];
#endif
			tempHumStru_p	   modbusTempHum[TEMPHUM_485_NUM];
			depthStru_p			 modbusWaterDepth[WATERDEPTH_485_NUM];
			crackMeterStru_p modbusCrackMeter[CRACKMETER_485_NUM];
			coverStru_p      modbusCover[COVER_485_NUM];
		#ifndef     ANA_MASK
			tempHumStru_p    analogTempHum;//不支持多路模拟温度传感器
		#endif
//放入各种传感器上限下限值 end
			
			autoCtrl_stru		 autoctrl[CRTL_TOTAL_NUM];
			
			//uint32_t         autoCrtl
			




	    
}deviceFlashStru;//存储到flash中

typedef struct{
		uint32_t lowFlag;//低电平
	  uint32_t upFlag;//高电平
}digputFlagStru;


typedef struct{
			circuFlagStru      modbusCircul[CIRCULA_485_NUM];
			partDisChgFlagStru modbusPartDisChg[PARTDISCHAG_485_NUM];
			pressSettlFlagStru modbusPreSettl[PRESSSETTL_485_NUM];
			threeAxisFlagStru  modbusThreAxis[THREEAXIS_485_NUM];
#ifdef  USE_4GAS
			ch4FlagStru        modbusCh4[CH4_485_NUM];
			o2FlagStru         modbusO2[O2_485_NUM];
			h2sFlagStru        modbusH2s[H2S_485_NUM];
			coFlagStru         modbusCo[CO_485_NUM];
#endif
			tempHumFlagStru	   modbusTempHum[TEMPHUM_485_NUM];
			depthFlagStru			 modbusWaterDepth[WATERDEPTH_485_NUM];
	    crackMeterFlagStru modbusCrackMeter[CRACKMETER_485_NUM];
	    coverFlagStru      modbusCover[COVER_485_NUM];
			tempHumFlagStru    analogTempHum;//不支持多路模拟温度传感器
		  digputFlagStru     digInput[DI_NUM];
	    digputFlagStru     digOutput[DO_NUM];
//	    digputFlagStru     v33Output[V33O_NUM];
//	    digputFlagStru     v5Output[V5O_NUM];
	    digputFlagStru     switchOutput[SWITCH_NUM];
}inoutDevStru;
//DI配置高有效
extern rt_err_t uartDataRec( uint8_t uartNum,uint8_t dat);
extern deviceFlashStru sheet;
extern inoutDevStru inpoutpFlag;


//extern rt_err_t uartDataRec( uartEnum uartNum,uint8_t dat);


















#endif




