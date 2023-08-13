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
//         增加启动软件定时器的配置 rt_config.h #define RT_USING_TIMER_SOFT 0 提高了定时器优先级 避免被task打乱
//         增加log存储时候写入时间，同步之前显示20年1月1日 0:0:0
//         [20-1-1-0:0:0]ACU RESET>>>>>>>>>>>>>>
//         [20-1-1-0:0:0][main]20230607  ver=03.08   20230613
//         加入SD卡存储log最大数为10天 超过需要自动删除早起的log
//V3.10    增加log日志读取时间戳名称后进行比较函数  找到最小值进行删除
//         ringbuf大小定义不能用 *乘法 会导致异常
//V3.11    增加回调钩子函数来打印log  增加同步LCD rtc时钟失败情况下  每隔10秒来继续同步时钟
//V3.12    修正局放显示异常			增加dispJufangTotlNum++;     20230616
//V3.13    增加记录rtc时间为utc时间减去8小时刚好为北京时间的0点
//         增加lcd显示版本号和SD卡异常状态        20230619
//V3.14    增加定义 defined(USER_VECT_TAB_ADDRESS)  修改VECT_TAB_OFFSET 为0x20000实现跳转
//V3.15    修改PRINTF_BUF_LEN 为2k 原来为6k 减小上电后延时等待串口屏的时间  
//         在BootLoader中加入延时等待 可以显示烧录程序时候擦除界面
//V3.16    增加环流数据存储超过30条的情况下删除txt文件命令
//         增加局放测试                           20230627
//V3.17    修复局放数据写入SD过程出现的问题       20230628
//V3.18    调整flash分区 前64k为BootLoader+64k用户数据+896kb的app  20230629
//V3.19    采用cjson自带的打包json格式容易导致内存溢出（无提示）打包数据量很少只有10个字节左右
//         更改打包注册信息格式为sprintf
//V3.20    更改打印信息从idletask中移到sdRTCTask.C中来打印 
//				 因idletask中使用延时函数rt_thread_delay()，rt_sem_take() 等可能会导致线程挂起的函数都不能使用。
//         并且，由于 malloc、free 等内存相关的函数内部使用了信号量作为临界区保护，因此在钩子函数内部也不允许调用此类函数！
//V3.21    增加下行数据协议中数据长度大于NetRxBuffer导致内存错误的保护  
//         修正心跳打包多占用了52字节ram 没有释放掉  20230704
//         插拔红色板子网线旁边的显示屏的5pin接口会导致程序某些线程死掉 SOCK_CLOSED
//V3.22    去除V33 V5输出     																	20230705
//V3.23    增加除局放和环流之外的其他modbus传感器的数据读取     20230710
//V3.24    增加sd容量显示界面                                   20230713
//V3.25    采用USE_RINGBUF代替mq mq串口发送丢数据               20230717
//V3.26    修复LCD读取数据前显示数据个数为0                     20230717
//V3.27    修复lsock连接稳定后net正常  否则会出现上线、掉线的情况           
//         修复lcd通讯延时时候由100ms改为200ms 以前由邮箱发送等待时间较短 现在改为ringbuf发送 等待数据接收时间较长（硬延时）
//                                                              20230720
//V3.28    增加屏幕配置ID成功提示                               20230721
//V4.00    硬件更改增加8路扩展485串口														20230725
//V4.01    增加5路485串口配置LCD端 一共9路串口                  20230802
//V4.02    调整上电同步RTC时钟策略 5秒内同步20次
//         增加mqtt协议 tcpserver和emqx不能同时开启来测试       20230804
//V4.03    修复一些数据包打包成mqtt格式的错误 
//				 提高tcp层断线情况下mqtt层重连的速度									20230808
//V4.04    增加协议切换  支持LCD屏幕切换TCP和MQTT协议 切换后需要保存重启 
//         增加LCD屏幕无操作2-3分钟内 切回home页面              20230809
//V4.05    修改LCD串口接收ringbuf为lcdqueue 
//         统一modbus数据采集保存函数为同一个函数               20230811
//V4.06    测试585扩展ic 更改调试函数            20230813
/*
		RW_IRAM2 0x20000000 0x00020000  {  ; RW data
		 .ANY (+RW +ZI)
		}
		RW_IRAM1 0x10000000 0x00010000  {  ; RW data
		 .ANY (ccmram)
		}
扇区 0 0x0800 0000 – 0x0800 03FF 16KB
扇区 1 0x0800 4000 – 0x0800 7FFF 16KB
扇区 2 0x0800 8000 – 0x0800 BFFF 16KB
扇区 3 0x0800 C000 – 0x0800 FFFF 16KB
扇区 4 0x0801 0000 – 0x0801 FFFF 64KB        
扇区 5 0x0802 0000 – 0x0803 FFFF 128KB
扇区 6 0x0804 0000 – 0x0805 FFFF 128KB
…… …… ……
扇区 11 0x080E 0000 – 0x080F FFFF 128KB
bootloader 占用扇区0-扇区3   64k
扇区4   存放用户数据   64k
总ram 128k+64kccm
*/
//          
