/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>

/*
=============================================================================================

    //Total RO  Size (Code + RO Data)               270044 ( 263.71kB)
    Total RW  Size (RW Data + ZI Data)             73696 (  71.97kB)
    Total ROM Size (Code + RO Data + RW Data)     270376 ( 264.04kB)
		[10:27:11.390]收←◆free
		total    : 57296
		used     : 13228
		maximum  : 23920
		available: 44068
		RAM:占用 71.97（变量编译后）+12.92（操作系统malloc）=84.88k 剩余43.03K  剩余量 33.6%
		ROM:占用:264.04k +参数保存占用最后一个128K 总共392.04k  剩余量61.8%
		

=============================================================================================
==============================================================================

    //Total RO  Size (Code + RO Data)               468200 ( 457.23kB)
    Total RW  Size (RW Data + ZI Data)             63632 (  62.14k//
    Total ROM Size (Code + RO Data + RW Data)     468508 ( 457.53kB)
		[16:05:24.710]收←◆free
		total    : 67360
		used     : 12124
		maximum  : 19636
		available: 55236
		
		RAM:占用 62.14k（变量编译后）+11.83（操作系统malloc）=74k 剩余54K  剩余量 42%
		ROM:占用:457.53kB +参数保存占用最后一个128K 总共571.8k  剩余量44.2%
==============================================================================

*/
//https://github.com/Suqi2020/huikongxiang_env.git
//0V1   20220919
//初始化  没有加入版本管理 
//0V3   20220920
// 处了串口4外其他的串口收发都可以  
//0V4   20220921
// 所有io口输入输出 adc uart收发已经测试  
//0V5   20220921
//去掉menuconfig 配置IO驱动   
//1、spi时钟42MHZ 已经确认
//2、w5500内部缓存多大32K的接收缓存
//0V6  W5500参考野火网络通信成功  20220923
//0V7  w5500 中断接收数据OK  20220927
//0V9  shell命令行OK        20220927
//V0.11  增加4个主要task    20220930
//			task1  w5500网络状态的维护，以及接收数据               w5500Task
//			task2  上行数据发送的维护（包括上行心跳 注册 数据打包 ）
//       		以及重发 丢给task3                              upKeepStateTask
//			task3  net网络端的发送                                 netDataSednTask                      
//			task4  接收完数据的解析与处理                          netDataRecTask
//			task1  w5500网络状态的维护，以及接收数据               w5500Task
//			task2  上行数据发送的维护（包括上行心跳 注册 数据打包 ）
//       				以及重发 丢给task3                              upKeepStateTask
//			task3  net网络端的发送                                 netDataSednTask                      
//			task4  接收完数据的解析与处理                          netDataRecTask
//			#define RT_CONSOLEBUF_SIZE 2048  //后期需要该小 512
//V0.12  上行心跳json和回复OK    																	20221008
//V0.13   加入dataup 和devreg easy timer													 20221009
//V0.14   手动测试3条上行数据OK  																		20221010
//        串口2346 modbus 串口1 debug 串口5 串口屏


//V0.15		rs485_公众环流 modbus 初步调式读取数据成功，并能实现上传 20221011
//        rt_sprintf float类型有问题  使用sprintf代替 
//V0.16   READ WRITE modbus OK   																			20221012
//V0.17   //迅速切换其它485接口来使用 方法：只需要修改串口发送接口 和中断接收接口即可
//         rs485Circula.c-cirCurrUartSend(uint8_t *buf,int len) 
//         和drv_uart.c-USART2_IRQHandler中 -rt_mq_send(&cirCurrmque, &Res, 1);    20221012
//V0.18    modbus读不到数据填充0       2022012
//V0.19    局放读取可以  加入modbus读取保护  每次发送后再读取             
//         增加软件定时器 粗略具备初始化 开始 停止功能 需要1秒的基准 
//         增加modbus设备重启后出现的干扰过滤（非modbus读取的数据 丢弃）
//         具备定时器同时到的情况下错开发送功能                          20221013
//V0.20    加入16K代码测试  MAX_SOCK_NUM 为1时候 最大发送16k网络数据  
//         TX_RX_MAX_BUF_SIZE为实际发送缓存buf大小(与MAX_SOCK_NUM反比)                       20221017
//V0.21    加入统计代码，粗略计算掉线次数和掉线时长 命令 offline                    20221018
//V0.22    增加沉降仪代码 默认波特率9600 文档有误      20221018
//V0.23    增加三轴代码 默认波特率9600 需要修改三轴的地址为01  同沉降仪  20221019
//V0.24 	 增加支持modbus设备选择串口 UART2(1) UART3(2) UART6(3) UART1(debug) UART4(4)  未测试  20221020
//V0.25    增加4路485和网络故障log打印输出                                     20221021
//V0.26    增加ip_from==IP_FROM_DHCP 或者自定义来实现网络ip配置 （交换机自动获取ip失败） 20221025
//V0.27    1、加入utf8格式打包json格式  nodepad++设置为utf8 以及 c++中设置 misc controls --no-multibyte-chars
//            增加json格式中文字符要在dataPack.c中增加 已经把文件用nodepad++更改为utf8格式
//         2、修复接收数后死机问题 接收完json数据没有释放掉                      20221026
//         3、增加json格式打包devRegJsonPack  用json自带代替sprintf 增加数据包 465Byte增加到586Byte 谨慎使用
//V0.28    增加配置多个modbus到同一个串口上  需要同类型的放到一起  比如 局放和环流  三轴和沉降仪   20221027
//V0.29    增加配置多个文件夹到keil中      20221028
//V0.30    增加flash存储  最后4K           20221028
//V0.31    增加at指令来配置modbus  example[modbus 接地环流 uart5 12]
//				 mobus+设备名称+端口+设备地址(0-关闭设备)+采集时间(秒)           20221029
//V0.32    增加mdbus配置时候 检查不同类型设备用同一个端口
//         检查同类型设备用同一个设备地址用同一个端口
//         实现ip地址的配置和存储到flash中            20221031
//V0.33//4种气体在一起读取 所以前三个不使用 只在CO定时器地方读取并打包发送  关闭时候只需要关闭CO就可以把所有气体全部关闭
//         CO默认气体是4  为了方便打包把气体数据打包在一起    20221101
//V0.34    加入温湿度和水位读取 每个传感器只支持一种       20221102  
//V0.35    更改flash读写 调用drv_falsh_f4.c库函数          20221103
//V0.36    更改modbus读取为通用傻瓜式读取 mcu不介入具体数字含义  20221103
//V0.37    加入错误检测 devid重复 检查挂在同一个串口下边的modbus设备有同样的设备地址  
//         上电打印modbus配置过的设备      
//          开启128kflash空间来存储配置modbus设备(擦除最小单位是128K) 目前最大支持200个       20221104
//V0.38    局放GY-JF100-C01和别的设备挂载在同一个串口的情况下连续读取设备会导致局放读取失败 
//         故连续读取设备情况下需要延时2秒继续读取                   20221105
//V0.39    修改存储参数命令统一为 flash save                      
//         增加模拟配置                                            20221107
//         存在bug  uart 的MX_USART2_UART_Init(packFlash.port[i].bps); 中
//V0.40    修改傻瓜式为指定传感器       20221108
//V0.41    所用到传感器json格式已经打包完成   20221109
//V0.42    接入传感器测试 修复bug    增加模拟量配置目前支持温湿度未完成 20221110
//V0.43    加入传感器响应状态上传  1-传感器响应  0-传感器不响应  20221111
//V0.44    修复log显示问题 以及显示波特率      20221117
//V0.45    增加7寸触屏显示组件地址定义         20221201
//V0.46    增加7寸触屏部分接口函数 增加串口5调试 增加lcdtask 队列       20221205
//V0.47    增加显示网络、串口、mcuid 、传感器类型选择功能         20221208
//V0.48    增加传感器显示 删除界面 增加重启 界面         20221209
//V0.49    增加网络掉线次数显示 显示的是上线过再掉线的时长  一开始上电没有接网线情况下不会显示此次掉线时长  20221212
//v0.50    增加传感器配置界面  暂时不稳定   20221213
//V0.51    增加传感器配置界面 测试OK    20221214
//V0.52    增加故障传感器显示界面    20221215
//V0.53    加入故障传感器界面显示 翻页正常    20221216
//V0.54    修复格式化RTC为负值的情况 %d改为u%  20221230
//V0.55    增加utf8格式的传感器名称显示  2023-1-4
//V0.56    修改json打包格式 identify 2023-1-5
//V0.57    modbus传感器4种气体 氧气硫化氢一氧化碳甲烷使用同一个ID
//         1、去掉串口和LCD配置传感器时候4种气体的的deviceID查重
//         2、统一4种气体定时器为co定时器
//         3、更改4种气体打包逻辑  2023-1-7
//V0.58    utcTime_ms()打包json格式错误
//         sprintf(sprinBuf,"%u",utcTime_ms()); 
//         更改为		sprintf(sprinBuf,"%llu",utcTime_ms());
//         更改uint32_t utcTime_ms() 为 uint64_t utcTime_ms()  2023-1-10
//V0.59    修改串口配置modbus传感器情况下不能删除传感器  2023-1-11
//V0.60		 增加串口digitalinput配置以及存储              2023-1-12
//V0.61    增加串口digital output 配置以及存储 参考文档《汇控箱modbus串口配置V0.3》 2023-1-17
//V0.62    增加digital output input 配置超过8个情况下错误提示功能  2023-1-18
//V0.63    增加analog传感器配置    2023-02-02
//V0.64    增加传感器threshold配置   2023-02-05
//V0.65    增加3v3 5v 12voutput供电配置 20230206
//V0.66    增加逻辑控制 扩展结构体 增加保存指针到flash中
//V0.67    输入输出配置增加完成 未测试 20230212
//V0.68    增加autoctrltask 增加 autoCtrlRun和ctrlOutSetIO函数
//V0.69    增加modbus ai传感器输入阈值检测置位标记 以及diIOOutSetFlag() di输入检测置位标记
//V0.70    增加autoctrl的配置以及测试 20230215
//V0.71    增加test IO输入 输出执行情况
//V0.72    所有的对外接口的AT命令全部实现 包括边缘控制 简单测试了逻辑控制的最大数目 20230217
//V0.73    解决tasklog屏蔽时候出现内存泄漏问题 需要等待任务挂起才能删除任务
//         更改模拟温湿度读取数值不准确  已经实现并且测试
//V0.74    修复配置threadshold 时候高低值放反的问题 
//V0.75    修复自动配置出现的bug
//V0.76    增加开关控制的接口显示V3O SWITCH V5O
//V0.77    增加开关控制对接LCD屏，测试OK
//V0.78    增加电平显示窗口默认显示电平值  20230312
//V0.79    输入设置配置测试OK
//V0.80    输出设置显示OK 
//         除了modbus传感器，输入输出以及模拟传感器的配置按照端口号来的
//         输入新的配置如果端口号相同会导致原来的失效
//V0.81    增加模拟传感器读取成功
//V0.82    增加模拟传感器配置删除成功
//V0.83    加入子站读取模拟和485传感器数据成功
//V0.84    设置读取传感器时间测试完成
//V0.85    增加对子站通讯json格式 读取和配置阈值完成  20230323
//V0.86    增加485和模拟告警上报    20230324
//V0.87    完善所有的modbus告警上报 20230325
//v0.88    增加数字输入输出的开机上报与读取 20230327
//V0.89    增加子站下发重启保存命令  20230328
//V0.90    增加crackMeter的modbus测试显示 后期厂家可能修改协议格式  
//         修复lcd显示的出错，控件地址更改    20230330
//V0.91    增加自动控制json数据解析 20230404
//V0.92    裂缝仪厂家更改为标准modbus协议 代码加入测试 20230407
//V1.00    去掉uartMutex互斥量  增加读取485数据时候的互斥保护（2个线程调用） 20230418
//V1.01    在uint16 send(SOCKET s, const uint8 * buf, uint16 len)中增加#if(MAX_SOCK_NUM!=8) break freesize一直为2048
//         更改中断接收为查询接收每隔100ms查询一次 中断接收在大数据量的时候会分包 测试8k的数据包会分包 
//         目前最大8K数据包 大于8K会导致内存紧张
//V1.02    修复逻辑控制远程配置错误  更改autoCfgSure();位置
//         修复下行设置逻辑控制超过20条导致程序死机 logCrtlAddResp(cJSON *Json)加入逻辑控制总数判断
//V1.03    修复串口数据过多导致rt_smem_free err的问题  LENTH <MSGPOOL_LEN 导致buf溢出  
//         增加一分钟刷新一次传感器状态和网络状态的显示 LCDTask.C中
//V1.04    增加WDT
//V1.05    增加LCD显示复位和保存成功窗口
//V1.06    修复内存溢出导致的(rt_object_get_type(&mq->parent.parent) == RT_Object_Class_MessageQueue) assertion failed at function:rt_mq_recv,
//					内存溢出代码 
//			                 if(offLine.times<sizeof(offLine.relayTimer)/sizeof(offLine.relayTimer[0]))（增加）
//                          {offLine.times++;
//					               	offLine.relayTimer[offLine.times]=rt_tick_get()/1000;}
//         增加LCDWtite 互斥信号量保护
//         LCDDataSend中发送完毕增加5ms延时 将2个相邻数据包分包 否则lcd屏识别不出
//V1.07    加入timeQuckIncSet() 给传感器一个初始值，快速测试传感器状态 2分钟内可以监测出故障传感器并显示
//V1.08    增加井盖LCD显示控制  
//V3.01    增加SD卡读写
//         SDIO_TRANSFER_CLK_DIV  设置为2 原本为0  导致读写sd速度过快  84/2+2 =21mhz 不能大于25Mhz
//V3.02    采用安富来的rafts驱动代替野火的  写入65000条 测试OK  20230519 
//V3.03    SDIO_TRANSFER_CLK_DIV改为0 提高写入速度 达到3538992字节/279秒=12.38KB/秒
//V3.04    增加sd写入时候根据时间保存 以ID号建立二级目录 保存RTC记录。
//V3.05    使用STM32F4中的CCM的另外64K内存  __attribute__((at(0x10000000)));
//V3.06    更改ccm用宏定义  #define  CCMRAM __attribute__((section("ccmram"))) 
//         修改link文件 
//V3.07    W5500发送最大只能支持2kbuf 怎么设置来支持16kbuf  需要初始化txsize[MAX_SOCK_NUM]   以及socket数字需要与其对应
//         如果设置MAX_SOCK_NUM=1     SOCK_TCPC必须为0
//         加入w5500SpiMutex 互斥保护 发送和接收分别再两个进程中 会同时操作spi     20230607
//V3.08    调整气体打包上传好H2S附带co的id的错误 修改更改串口后导致串口读取异常的问题 20230609
//V3.09    增加存储log到sd卡功能，增加log打印和存储的单独task
//         增加上电后从显示屏读取RTC时钟函数，从服务器同步rtc后会和lcd显示屏比较 如果相差3秒 同步lcd时钟
//         修改sd卡内写入文件记录的创建时间函数 
//         增加启动软件定时器的配置 #define RT_USING_TIMER_SOFT 0
//         增加log存储时候写入时间，同步之前显示20年1月1日 0:0:0
//         [20-1-1-0:0:0]ACU RESET>>>>>>>>>>>>>>
//         [20-1-1-0:0:0][main]20230607  ver=03.08
//         加入SD卡存储log最大数为10天 超过需要自动删除早起的log
/*
		RW_IRAM2 0x20000000 0x00020000  {  ; RW data
		 .ANY (+RW +ZI)
		}
		RW_IRAM1 0x10000000 0x00010000  {  ; RW data
		 .ANY (ccmram)
		}
*/
//          
#define APP_VER       ((3<<8)+9)//0x0105 表示1.5版本
//注：本代码中json格式解析非UTF8_格式代码（GB2312格式中文） 会导致解析失败
//    打印log如下 “[dataPhrs]err:json cannot phrase”  20230403
const char date[]="20230613";

//static    rt_thread_t tid 	= RT_NULL;
static    rt_thread_t tidW5500 	  = RT_NULL;
static    rt_thread_t tidNetRec 	= RT_NULL;
static    rt_thread_t tidNetSend 	= RT_NULL;
static    rt_thread_t tidUpkeep 	= RT_NULL;
static    rt_thread_t tidLCD      = RT_NULL;
static    rt_thread_t tidAutoCtrl = RT_NULL;

static    rt_thread_t tidSaveLogSd =RT_NULL;


//互斥信号量定义
rt_mutex_t   read485_mutex=RT_NULL;//防止多个线程同事读取modbus数据
rt_mutex_t   lcdSend_mutex=RT_NULL;//防止多个线程同事往lcd发数据
rt_mutex_t   w5500Spi_mutex=RT_NULL;//防止多个线程同时操作spi
rt_mutex_t   sdWrite_mutex=RT_NULL;
rt_mutex_t   printf_mutex=RT_NULL;
//邮箱的定义
extern struct  rt_mailbox mbNetSendData;;
static char 	 mbSendPool[20];//发送缓存20条
static char 	 mbRecPool[20];//发送缓存20条
extern struct rt_mailbox mbNetRecData;

static char 	 sdWritePool[100];//发送缓存100条
extern struct rt_mailbox mbsdWriteData;


/* 事件控制块 */
struct rt_event mqttAckEvent;
rt_bool_t gbSDExit=true;

#ifdef USE_WDT
struct rt_event WDTEvent;
#endif





//队列的定义

#if   USE_RINGBUF

#else
	struct  rt_messagequeue LCDmque;//= {RT_NULL} ;//创建LCD队列
	uint8_t LCDQuePool[LCD_BUF_LEN];  //创建lcd队列池
#endif




//struct  rt_messagequeue SDLogmque;//= {RT_NULL} ;//创建LCD队列
//uint8_t SDLogQuePool[100];  //创建lcd队列池
//任务的定义
extern  void   netDataRecTask(void *para);//网络数据接收
extern  void   netDataSendTask(void *para);//网络数据发送
extern  void   upKeepStateTask(void *para);//定时打包数据 后期可能加入定时读取modbus
extern  void   w5500Task(void *parameter);//w5500网络状态的维护
extern  void   hardWareDriverTest(void);
extern  void   LCDTask(void *parameter);
extern  void   autoCtrlTask(void *para);
extern  void   logSaveSDTask(void *para);
extern  void   WDTTask(void *parameter);
const  static char sign[]="[main]";
extern rt_bool_t gbNetState;

/* 定时器的控制块 */
static rt_timer_t timer1;

//static int cnt = 0;
/* 定时器1超时函数 每100ms进去一次*/
//10秒提醒一次 uart offline状态
static void timeout1(void *parameter)
{
		static int count=0;
	  static int alarmTick=10;
		extern rt_bool_t gbNetState;
	  extern void timeInc();
	  extern void FatReadDirDelEarlyTxt();
	  extern bool fountFlag;
	  //extern void modbusWorkErrCheck(void);
	  count++;  
	  if(gbSDExit==false){
				if(count%(100)==0)//10秒提醒一下
						rt_kprintf("%s请插入TF卡并重启设备\n",sign);
		}
		if(count%TXT_LOG_TIME==(0)){
			  if(fountFlag==true)//挂载u盘成功
						FatReadDirDelEarlyTxt();//每隔TXT_LOG_TIME/10秒时间检查一次
		}
	  if(count%10==0)
				timeInc();
		if(gbNetState==RT_TRUE){
				if(count%10==0){
						HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				}
		}
		else
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		if(count%alarmTick==0){
			  alarmTick+=20;
			  if(alarmTick>=100){
						alarmTick=100;// 1 2 3 最终10秒提醒一次
				}

//				modbusWorkErrCheck();//modbus 错误工作状态打印
//				errConfigCheck();//	modbusWorkErrCheck();//errConfigCheck();
				//modbusPrintRead();
//				if(gbNetState ==RT_FALSE){
//						rt_kprintf("%sERR:网络故障\n",sign);
//				}
		}
}
char *strnum="1234.5678";
//double atof(const char *s);
extern void creatFolder(void);
//char testNum[4]={1,2,3,4};
void  outIOInit(void);

//bool  log_save_sdFlag =false;  
char  printVer[50];
int main(void)
{


    printf("\n%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  sprintf(printVer,"%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  rt_err_t result;
		stm32_flash_read(FLASH_IP_SAVE_ADDR,    (uint8_t*)&packFlash,sizeof(packFlash));
		stm32_flash_read(FLASH_MODBUS_SAVE_ADDR,(uint8_t*)&sheet,    sizeof(sheet));
	  outIOInit();
		if(packFlash.acuId[0]>=0x7F){
				rt_strcpy(packFlash.acuId,"000000000000001");//必须加上 执行cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		}    
		
	
		
		  /* 创建定时器 周期定时器 */
    timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 100,
                             RT_TIMER_FLAG_PERIODIC);
		if (timer1 != RT_NULL)
        rt_timer_start(timer1);
		//创建485设备用到的互斥 队列
		read485_mutex= rt_mutex_create("read485_mutex", RT_IPC_FLAG_FIFO);
		if (read485_mutex == RT_NULL)
    {
        printf("%screate read485_mutex failed\n",sign);
    }
		lcdSend_mutex= rt_mutex_create("lcdSend_mutex", RT_IPC_FLAG_FIFO);
		if (lcdSend_mutex == RT_NULL)
    {
        printf("%screate lcdSend_mutex failed\n",sign);
    }
		w5500Spi_mutex= rt_mutex_create("w5500Spi_mutex", RT_IPC_FLAG_FIFO);
		if (w5500Spi_mutex == RT_NULL)
    {
        printf("%screate w5500Spi_mutex failed\n",sign);
    }	
		sdWrite_mutex= rt_mutex_create("sdWrite_mutex", RT_IPC_FLAG_FIFO);
		if (sdWrite_mutex == RT_NULL)
    {
        printf("%screate sdWrite_mutex failed\n",sign);
    }		
		printf_mutex= rt_mutex_create("printf_mutex", RT_IPC_FLAG_FIFO);
		if (printf_mutex == RT_NULL)
    {
        printf("%screate sdWrite_mutex failed\n",sign);
    }		
#if   USE_RINGBUF

#else
		int ret = rt_mq_init(&LCDmque,"LCDrecBuf",&LCDQuePool[0],1,LCD_BUF_LEN,RT_IPC_FLAG_FIFO);       
		if (ret != RT_EOK)
		{
				printf("%sinit LCD msgque failed.\n",sign);
				return -1;
		}
#endif
		extern void 	uartMutexQueueCreate();
		uartMutexQueueCreate();
		
//	 ret = rt_mq_init(&SDLogmque,"SDLogmque",&SDLogQuePool[0],1,sizeof(SDLogQuePool),RT_IPC_FLAG_FIFO);       
//	if (ret != RT_EOK)
//	{
//			rt_kprintf("%sinit SDLogmque  failed.\n",sign);
//			return -1;
//	}
		
		


////////////////////////////////////邮箱//////////////////////////////////
		
    result = rt_mb_init(&mbsdWriteData,"sdsend",&sdWritePool[0],sizeof(sdWritePool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox mbsdWriteData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetRecData,"mbRec",&mbRecPool[0],sizeof(mbRecPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetRecData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetSendData,"mbSend",&mbSendPool[0],sizeof(mbSendPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetSend failed.\n",sign);
        return -1;
    }
#ifdef  USE_WDT   
		if (rt_event_init(&WDTEvent, "WDTEvent", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("%sinit WDTEvent failed.\n",sign);

    }
#endif
		

////////////////////////////////任务////////////////////////////////////
		tidSaveLogSd=  rt_thread_create("logSaveSDTask",logSaveSDTask,RT_NULL,512*2,6, 10 );
		if(tidSaveLogSd!=NULL){
				rt_thread_startup(tidSaveLogSd);													 
				printf("%sRTcreat tidSaveLogSd\r\n",sign);
		}
		tidLCD    =  rt_thread_create("LCD",LCDTask,RT_NULL,512*2,2, 10 );
		if(tidLCD!=NULL){
				rt_thread_startup(tidLCD);													 
				printf("%sRTcreat LCDStateTask \r\n",sign);
		}
		tidNetRec =  rt_thread_create("netRec",netDataRecTask,RT_NULL,512*2,3, 10 );
		if(tidNetRec!=NULL){
				rt_thread_startup(tidNetRec);													 
				printf("%sRTcreat netDataRecTask \r\n",sign);
		}
		tidNetSend =  rt_thread_create("netSend",netDataSendTask,RT_NULL,512*2,3, 10 );
		if(tidNetSend!=NULL){
				rt_thread_startup(tidNetSend);													 
				printf("%sRTcreat netDataSendTask \r\n",sign);
		}
		tidUpkeep 	=  rt_thread_create("upKeep",upKeepStateTask,RT_NULL,512*4,4, 10 );
		if(tidUpkeep!=NULL){
				rt_thread_startup(tidUpkeep);													 
				printf("%sRTcreat upKeepStateTask \r\n",sign);
		}
    tidW5500 =  rt_thread_create("w5500",w5500Task,RT_NULL,512*3,2, 10 );
		if(tidW5500!=NULL){
				rt_thread_startup(tidW5500);													 
				printf("%sRTcreat w5500Task task\r\n",sign);
		}
		tidAutoCtrl =  rt_thread_create("autoCtrl",autoCtrlTask,RT_NULL,512*2,5, 10 );
		if(tidAutoCtrl!=NULL){
				rt_thread_startup(tidAutoCtrl);													 
				printf("%sRTcreat autoCtrlTask\r\n",sign);
		}

		
#ifdef  USE_WDT
		extern IWDG_HandleTypeDef hiwdg;
		static    rt_thread_t tidWDT      = RT_NULL;
		tidWDT =  rt_thread_create("WDT",WDTTask,RT_NULL,256,10, 10 );
		if(tidWDT!=NULL){
				rt_thread_startup(tidWDT);													 
				rt_kprintf("%sRTcreat WDTTask\r\n",sign);
		}
		HAL_IWDG_Refresh(&hiwdg);
#endif	
		//队列初始化之后再开启串口中断接收
//		char test[50]={0};
//			uint32_t a=4294967290;
//			sprintf(test,"long:%u", a);//"monitoringTime":"1655172531937"
//			rt_kprintf("%s %s \r\n",sign,test);
//////////////////////////////结束//////////////////////////////////////
    while (0)//task用于测试 以及闪灯操作
    {
				hardWareDriverTest();
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				rt_thread_mdelay(250);
			  extern rt_bool_t gbNetState;
			  if(gbNetState==RT_TRUE){
					  rt_thread_mdelay(250);
						
				}
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				rt_thread_mdelay(250);
				if(gbNetState==RT_TRUE){
					  rt_thread_mdelay(250);	
				}
    }
}
//tasklog delete  线程挂起时候才能删除   
void  tasklog(int argc, char *argv[])
{
		if(argc!=2){
				goto ERR;
		}
		if(rt_strcmp("delete",argv[1])==0){
			  if((tidW5500->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidW5500);
				if((tidNetRec->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidNetRec);
				if((tidNetSend->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidNetSend);
				if((tidUpkeep->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidUpkeep);
				if((tidAutoCtrl->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidAutoCtrl);
			  rt_kprintf("%s[tasklog delete OK]\n",sign);
			  return;
		}
		ERR:
		rt_kprintf("%serr for example:\n",sign);
		rt_kprintf("%s[tasklog delete]\n",sign);
}
MSH_CMD_EXPORT(tasklog,tasklog del);//FINSH_FUNCTION_EXPORT_CMD

