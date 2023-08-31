/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __7INCHREGIST_H__
#define __7INCHREGIST_H__


#include "board.h"
//NET
#define   LOCAL_IP1_ADDR         0X1000
#define   LOCAL_IP2_ADDR         0X1002
#define   LOCAL_IP3_ADDR         0X1004
#define   LOCAL_IP4_ADDR         0X1006
#define   PHY_PORT_ADDR          0X1008
#define   REMOTE_IP1_ADDR        0X100A
#define   REMOTE_IP2_ADDR        0X100C
#define   REMOTE_IP3_ADDR        0X100E
#define   REMOTE_IP4_ADDR    		 0X1010
#define   REMOTE_PORT_ADDR   		 0X1012
#define   KEY_IP_SURE_ADDR			 0X1014
#define   KEY_IP_READ_ADDR			 0X1016




#define   GATEWAY_IP1_ADDR        0X1018
#define   GATEWAY_IP2_ADDR        0X101A
#define   GATEWAY_IP3_ADDR        0X101C
#define   GATEWAY_IP4_ADDR    		 0X101E
//波特率
#define   PORT1_ADDR     		 			0X1100
#define   PORT2_ADDR     		 			0X1102
#define   PORT3_ADDR     		 			0X1104
#define   PORT4_ADDR     		 			0X1106
#define   KEY_PORT_SURE_ADDR		  0X1108
#define   KEY_PORT_READ_ADDR		  0X110A



#define   PORT5_ADDR     		 			0X1110
#define   PORT6_ADDR     		 			0X1112
#define   PORT7_ADDR     		 			0X1114
#define   PORT8_ADDR     		 			0X1116
#define   PORT9_ADDR     		 			0X1118

//MCUID
#define   MCUID_ADDR         			0X1210
#define		KEY_MCUID_SURE_ADDR		  0X1202
#define		KEY_MCUID_READ_ADDR		  0X1204
//MODBUS
//#define		MODBUS_CFG_NAME_ADDR  		0x1360
#define		MODBUS_CFG_ID_ADDR    		0x1320
//#define		MODBUS_CFG_SORT_ADDR  		0X1300
#define		MODBUS_CFG_TYPE_ADDR  		0x1350
#define		MODBUS_CFG_PORT_ADDR  		0x1302
#define		MODBUS_CFG_ADDR_ADDR  		0x1304
#define		MODBUS_CFG_TIME_ADDR  		0x1306
#define		KEY_MODBUS_CFG_SURE_ADDR  0x1308
#define		KEY_MODBUS_CFG_DEL_ADDR   0x130A
#define		KEY_MODBUS_CFG_WATCH_ADDR 0x130C
#define		KEY_MODBUS_CFG_NAME_ADDR  0x130E
#define		MODBUS_CFG_NAME2_ADDR  		0x1380

#define   DISP_MODBUS_SAME_ID_MSG_ADDR          0X13A0


#define		KEY_MODBUS_LASTNAME_ADDR  0x1310
#define		KEY_MODBUS_NEXTNAME_ADDR  0x1312
#define		KEY_MODBUS_SURENAME_ADDR  0x1314
//modbusDisp
#define   MODBUSDISP_NAME_ADDR      0X1460
#define   MODBUSDISP_ID_ADDR        0X1420
//#define   MODBUSDISP_SORT_ADDR      0X1400
#define   MODBUSDISP_TYPE_ADDR      0X1450
#define   MODBUSDISP_PORT_ADDR      0X1402
#define   MODBUSDISP_ADDR_ADDR      0X1404
#define   MODBUSDISP_TIME_ADDR      0X1406
#define   MODBUSDISP_TOTALNUM_ADDR  0X1408
#define   MODBUSDISP_NOWNUM_ADDR    0X140A

#define   KEY_MODBUSDISP_LAST_ADDR  0X140C
#define   KEY_MODBUSDISP_NEXT_ADDR  0X140E
#define   KEY_MODBUSDISP_DEL_ADDR   0X1410

//netoffline 显示界面地址
#define		NET_ONLINE_ADDR  0X1600
#define		NET_RESP_ADDR       0X1602
//#define   NET_OFFLINE_RELAYTIME_ADDR   0X1604
//#define		NET_OFFLINE_LAST_ADDR        0X1606
//#define   NET_OFFLINE_NEXT_ADDR        0X1608



//troublemodbusDisp
#define   MODBUSDISP_ERRNAME_ADDR      0X1560
#define   MODBUSDISP_ERRID_ADDR        0X1520
#define   MODBUSDISP_ERRSORT_ADDR      0X1500
#define   MODBUSDISP_ERRTYPE_ADDR      0X1550
#define   MODBUSDISP_ERRPORT_ADDR      0X1502
#define   MODBUSDISP_ERRADDR_ADDR      0X1504
#define   MODBUSDISP_ERRTIME_ADDR      0X1506
#define   MODBUSDISP_ERRTOTALNUM_ADDR  0X1508
#define   MODBUSDISP_ERRNOWNUM_ADDR    0X150A

#define   KEY_MODBUSDISP_ERRLAST_ADDR  0X150C
#define   KEY_MODBUSDISP_ERRNEXT_ADDR  0X150E




//#define  TEST_CHOOSE_MODBUS_ADDR       0X2000
//#define  KEY_SURE_CHOOSE_MODBUS_ADDR       0X2010
//#define  KEY_LAST_CHOOSE_MODBUS_ADDR       0X2012
//#define  KEY_NEXT_CHOOSE_MODBUS_ADDR       0X2014



//#define  TEST_CHOOSE_MODBUS_ADDR       0X2000
//#define  KEY_SURE_CHOOSE_MODBUS_ADDR       0X2010
//#define  KEY_LAST_CHOOSE_MODBUS_ADDR       0X2012
//#define  KEY_NEXT_CHOOSE_MODBUS_ADDR       0X2014
#define   KEY_HUANLIU_CHOOSE_ADDR     0X20A0  //值靠后一点
#define   KEY_JUFANG_CHOOSE_ADDR      0X20A2
#define   KEY_FANGCHENJIANG_CHOOSE_ADDR   0X20A4
#define   KEY_FANGWAIPO_CHOOSE_ADDR   0X20A6
#define   KEY_JIAWAN_CHOOSE_ADDR      0X20A8
#define   KEY_YANGQI_CHOOSE_ADDR      0X20AA
#define   KEY_LIUHUAQING_CHOOSE_ADDR      0X20AC
#define   KEY_YIYANGHUATAN_CHOOSE_ADDR    0X20AE
#define   KEY_WENSHIDU_CHOOSE_ADDR     0X20B0
#define   KEY_SHUIWEI_CHOOSE_ADDR      0X20B2
#define   KEY_LIEFENGYI_CHOOSE_ADDR    0X20B4
#define   KEY_JINGGAI_CHOOSE_ADDR      0X20B6
//////////////////////////////////////////////////////////////
//开关控制界面
#define   KEY_SWITCH_INTERFACE_ADDR    0x5400
#define   KEY_SWITCH_PORT_ADDR         0x5402
#define   KEY_SWITCH_LEVEL_ADDR        0x5404
#define   KEY_SWITCH_SURE_ADDR         0x5406
#define   KEY_SWITCH_RETURN_ADDR       0x5408

//#define   DISP_SWITCH_INTERFACE_ADDR   0x5440
#define   DISP_SWITCH_PORT_ADDR        0x540C
#define   DISP_SWITCH_NAME_ADDR        0x5450
#define   DISP_SWITCH_ID_ADDR       	 0x5460
#define   DISP_SWITCH_TYPE_ADDR        0x5480
#define   DISP_SWITCH_LEVEL_ADDR       0x5414

//开关控制调出接口显示界面
#define   DISP_SWITCH_INTERFACE_ADDR  0x5410

#define   KEY_SWITCHINTERF_SURE_ADDR    		0x5418
#define   KEY_SWITCHINTERF_NEXT_ADDR        0x541A
#define   KEY_SWITCHINTERF_LAST_ADDR        0x541C
#define   KEY_SWITCHINTERF_RETURN_ADDR      0x541E
//开关控制调出port显示界面
#define   DISP_SWITCHPORT_PORT_ADDR         0x5420

#define   KEY_SWITCHPORT_SURE_ADDR    			0x5422
#define   KEY_SWITCHPORT_NEXT_ADDR        	0x5424
#define   KEY_SWITCHPORT_LAST_ADDR        	0x5426
#define   KEY_SWITCHPORT_RETURN_ADDR      	0x5428
//开关控制调出电平显示界面
#define   KEY_SWITCHLEVEL_ON_ADDR    				0x542A
#define   KEY_SWITCHLEVEL_OFF_ADDR    			0x542C
#define   KEY_SWITCHLEVEL_SURE_ADDR    			0x542E
#define   KEY_SWITCHLEVEL_RETURN_ADDR    		0x5430
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//输入界面

#define   KEY_INPUTCFG_LOOK_ADDR      	    0X5000
#define   KEY_INPUTCFG_SURE_ADDR        	  0X5002
#define   DISP_INPUTCFG_NAME_ADDR        	 	0X5004
#define   DISP_INPUTCFG_ID_ADDR        	 	  0X5010
#define   DISP_INPUTCFG_TYPE_ADDR        	  0X5030
#define   DISP_INPUTCFG_PORT_ADDR        	  0X5038

//输入查看界面
#define   DISP_INPUT_NAME_ADDR             0x5040
#define   DISP_INPUT_ID_ADDR               0x5050
#define   DISP_INPUT_TYPE_ADDR             0x5064
#define   DISP_INPUT_PORT_ADDR             0x5070
#define   DISP_INPUT_TOTALNUM_ADDR         0x5072
#define   DISP_INPUT_THENUM_ADDR           0x5074

#define   KEY_INPUT_LAST_ADDR              0x5076
#define   KEY_INPUT_NEXT_ADDR              0x5078
#define   KEY_INPUT_DEL_ADDR               0x507A
#define   KEY_INPUT_RETURN_ADDR            0x507C


#define   DISP_INPUT_SAME_ID_MSG_ADDR          0X5080
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//输出设置
//#define        DISP_OUTPUT_INTERFACE_ADDR     0x5110
#define        DISP_OUTPUT_NAME_ADDR     			0x5118
#define        DISP_OUTPUT_ID_ADDR     				0x5128
#define        DISP_OUTPUT_TYPE_ADDR     			0x5138
#define        DISP_OUTPUT_PORT_ADDR     			0x5140
#define        KEY_OUTPUT_INTERFACE_ADDR     	0x5142
#define        KEY_OUTPUT_SURE_ADDR     			0x5144
#define        KEY_OUTPUT_LOOK_ADDR     			0x5146
#define        KEY_OUTPUT_RETURN_ADDR     		0x5148

#define        DISP_OUTPUT_INTERFACE_ADDR   		0x514A
//#define        KEY_OUTPUT_SURE_P_ADDR     		0x5150
#define        KEY_OUTPUT_LAST_ADDR     				0x5152
#define        KEY_OUTPUT_NEXT_ADDR     				0x5154
#define        KEY_OUTPUT_RETURN_P_ADDR     		0x5156

#define        DISP_OUTPUT_READ_INTERFACE_ADDR   0x5158
#define        DISP_OUTPUT_READ_NAME_ADDR        0x5160
#define        DISP_OUTPUT_READ_ID_ADDR          0x5168
#define        DISP_OUTPUT_READ_TYPE_ADDR        0x5178
#define        DISP_OUTPUT_READ_PORT_ADDR        0x5180
#define        DISP_OUTPUT_READ_TOTALNUM_ADDR    0x5182
#define        DISP_OUTPUT_READ_THENUM_ADDR      0x5184
#define        KEY_OUTPUT_READ_INTERFACE_ADDR    0x5186
#define        KEY_OUTPUT_READ_LAST_ADDR    		 0x5188
#define        KEY_OUTPUT_READ_NEXT_ADDR     		 0x518A
#define        KEY_OUTPUT_READ_DELETE_ADDR     	 0x518C
#define        KEY_OUTPUT_READ_RETURN_ADDR     	 0x518E

#define        DISP_OUTPUT_READ_INTERFACE_P_ADDR     0x5190
#define        KEY_OUTPUT_READ_LAST_P_ADDR     			 0x5198
#define        KEY_OUTPUT_READ_NEXT_P_ADDR     			 0x519A
#define        KEY_OUTPUT_READ_RETURN_P_ADDR     		 0x519C
#define        DISP_OUTPUT_SAME_ID_MSG_ADDR          0X51A0
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//模拟传感器配置显示
#ifndef     ANA_MASK
#define        DISP_ANA_NAME_ADDR     	 					0x5200
#define        DISP_ANA_SUBNAME_ADDR     					0x5208
#define        DISP_ANA_ID_ADDR     		 					0x5210
#define        DISP_ANA_TYPE_ADDR        					0x5220
#define        DISP_ANA_PORT_ADDR        					0x5228
#define        DISP_ANA_TIME_ADDR                 0x522A
#define        KEY_ANA_NAME_INTERFACE_ADDR     		0x523C
#define        KEY_ANA_SUBNAME_INTERFACE_ADDR     0x522C
#define        KEY_ANA_SURE_ADDR     							0x522E
#define        KEY_ANA_LOOK_ADDR     							0x5230
//#define        KEY_ANA_RETURN_ADDR     0x5232

//#define             0x5208
#define        KEY_ANA_SURE_INTERFACE_ADDR     		0x5236
#define        KEY_ANA_LAST_ADDR     							0x5238
#define        KEY_ANA_NEXT_ADDR     							0x523A


#define        DISP_ANAREAD_NAME_ADDR    					0x5240
#define        DISP_ANAREAD_SUBNAME_ADDR 					0x5248
#define        DISP_ANAREAD_ID_ADDR     					0x5250
#define        DISP_ANAREAD_TYPE_ADDR     				0x5260
#define        DISP_ANAREAD_PORT_ADDR     				0x5268
#define        DISP_ANAREAD_TIME_ADDR     				0x526A
#define        DISP_ANAREAD_TOTALNUM_ADDR     		0x526C
#define        DISP_ANAREAD_THENUM_ADDR     			0x526E
#define        KEY_ANAREAD_LAST_ADDR     					0x5270
#define        KEY_ANAREAD_NEXT_ADDR     					0x5272
#define        KEY_ANAREAD_DEL_ADDR     					0x5274
#endif
//#define        KEY_ANAREAD_RETURN_ADDR     0x5276
//////////////////////////////////////////////////////////////
//阈值
#define        DISP_TH_TYPE_ADDR     							0x5300	//阈值类别
#define        DISP_TH_NAME_ADDR     							0x5308	//阈值名称
#define        DISP_TH_SUBNAME_ADDR     					0x5310	//阈值子选项
#define        DISP_TH_ID_ADDR     								0x5318	//阈值ID号
#define        DISP_TH_UP_LIMIT_ADDR     					0x5328	//阈值上限
#define        DISP_TH_DOWN_LIMIT_ADDR     				0x5330	//阈值下限


#define        KEY_TH_TYPE_ADDR     							0x5348	//阈值类别
#define        KEY_TH_NAME_ADDR     							0x534A	//阈值名称
//#define        DISP_TH_SUBNAME_ADDR     					0x534C	//阈值子选项
#define        KEY_TH_SURE_ADDR     							0x5338	//阈值确定
#define        KEY_TH_LOOK_ADDR     							0x533A	//阈值查看
#define        KEY_TH_TYPE_LAST_ADDR     					0x533C	//阈值类别上一个
#define        KEY_TH_TYPE_NEXT_ADDR     					0x533E	//阈值类别下一个
#define        KEY_TH_NAME_LAST_ADDR     					0x5340	//阈值名称上一个
#define        KEY_TH_NAME_NEXT_ADDR     					0x5342	//阈值名称下一个
#define        KEY_TH_SUBNAME_LAST_ADDR     			0x5344	//阈值子选项上一个
#define        KEY_TH_SUBNAME_NEXT_ADDR     			0x5346	//阈值子选项下一个


#define        DISP_TH_READ_TYPE_ADDR     				0x6300	//阈值读取类别
#define        DISP_TH_READ_NAME_ADDR     				0x6308	//阈值读取名称
#define        DISP_TH_READ_SUBNAME_ADDR     			0x6310	//阈值读取子选项
#define        DISP_TH_READ_ID_ADDR     					0x6318	//阈值读取ID号
#define        DISP_TH_READ_UP_LIMIT_ADDR     		0x6328	//阈值读取上限
#define        DISP_TH_READ_DOWN_LIMIT_ADDR     	0x6330	//阈值读取下限
#define        DISP_TH_READ_TOTAL_PAGE_ADDR     	0x633A	//阈值读取总页数
#define        DISP_TH_READ_THE_PAGE_ADDR     		0x633C	//阈值读取当前页

#define        KEY_TH_READ_TYPE_ADDR     					0x633E	//阈值读取类别
#define        KEY_TH_READ_NAME_ADDR     					0x6340	//阈值读取名称
#define        KEY_TH_READ_LAST_ADDR     					0x6342	//阈值读取上一页
#define        KEY_TH_READ_NEXT_ADDR     					0x6344	//阈值读取下一页
#define        KEY_TH_READ_RETURN_ADDR     				0x6346	//阈值读取返回
#define        KEY_TH_READ_DEL_ADDR     				  0x6354	//阈值读取删除

#define        KEY_TH_READ_NAME_LAST_ADDR     		0x6348	//阈值读取名称上一页
#define        KEY_TH_READ_NAME_NEXT_ADDR     		0x634A	//阈值读取名称下一页
#define        KEY_TH_READ_NAME_RETURN_ADDR     	0x634C	//阈值读取名称返回

#define        KEY_TH_READ_TYPE_LAST_ADDR     		0x634E	//阈值读取类别上一页
#define        KEY_TH_READ_TYPE_NEXT_ADDR     		0x6350	//阈值读取类别下一页
#define        KEY_TH_READ_TYPE_RETURN_ADDR     	0x6352	//阈值读类别返回
//////////////////////////////////////////////////////////////
//数据显示界面
//环流  71.bmp
#define 			DISP_DATA_HUANLIU_ID_ADDR           	0X7100
#define       DISP_DATA_HUANLIU_EARTH_A_ADDR      	0x7110
#define       DISP_DATA_HUANLIU_EARTH_B_ADDR				0X711A
#define       DISP_DATA_HUANLIU_EARTH_C_ADDR      	0x7120
#define       DISP_DATA_HUANLIU_RUN_A_ADDR        	0X712A
#define       DISP_DATA_HUANLIU_RUN_B_ADDR        	0X7130
#define       DISP_DATA_HUANLIU_RUN_C_ADDR        	0X713A
#define       DISP_DATA_HUANLIU_LOADR_A_ADDR        0X7140
#define       DISP_DATA_HUANLIU_LOADR_B_ADDR        0X714A
#define       DISP_DATA_HUANLIU_LOADR_C_ADDR        0X7150
#define 			DISP_DATA_HUANLIU_TOTALNUM_ADDR       0X715A
#define 			KEY_DATA_HUANLIU_LAST_ADDR            0X715C
#define 			KEY_DATA_HUANLIU_NEXT_ADDR            0X715E
//天井洼 start

#define  KEY_CHOOSE_HUAJING_TJW_ADDR             0X2020
#define  KEY_CHOOSE_FANGCHENJIANG_TJW_ADDR       0X2022
#define  KEY_CHOOSE_FANGWAIPO_TJW_ADDR           0X2024
#define  KEY_CHOOSE_HUANLIU_TJW_ADDR       			 0X2026
#define  KEY_CHOOSE_JINGGAI_TJW_ADDR       			 0X2028
#define  KEY_CHOOSE_LIEFENGYI_TJW_ADDR           0X202A
//天井洼 end
//天井洼 start
#define 			DISP_DATA_HUANLIU1_ID_TJW_ADDR           	0X7160
#define       DISP_DATA_HUANLIU1_EARTH_A_TJW_ADDR      	0x7170
#define       DISP_DATA_HUANLIU1_EARTH_B_TJW_ADDR				0X717A
#define       DISP_DATA_HUANLIU1_EARTH_C_TJW_ADDR      	0x7180
#define       DISP_DATA_HUANLIU1_CURRENT_TJW_ADDR      	0x718A
#define 			DISP_DATA_HUANLIU2_ID_TJW_ADDR           	0X7190
#define       DISP_DATA_HUANLIU2_EARTH_A_TJW_ADDR      	0x71A0
#define       DISP_DATA_HUANLIU2_EARTH_B_TJW_ADDR				0X71AA
#define       DISP_DATA_HUANLIU2_EARTH_C_TJW_ADDR      	0x71B0
#define       DISP_DATA_HUANLIU2_CURRENT_TJW_ADDR      	0x71BA
//天井洼 end
//局放  72.bmp
#define 			DISP_DATA_JUFANG_ID_ADDR           	0X7200
#define       DISP_DATA_JUFANG_PD_A_ADDR      		0x7210
#define       DISP_DATA_JUFANG_PD_B_ADDR			 		0X721A
#define       DISP_DATA_JUFANG_PD_C_ADDR      		0x7220
#define       DISP_DATA_JUFANG_FREQ_A_ADDR        0X722A
#define       DISP_DATA_JUFANG_FREQ_B_ADDR        0X7230
#define       DISP_DATA_JUFANG_FREQ_C_ADDR        0X723A
#define       DISP_DATA_JUFANG_DISCHG_A_ADDR      0X7240
#define       DISP_DATA_JUFANG_DISCHG_B_ADDR      0X724A
#define       DISP_DATA_JUFANG_DISCHG_C_ADDR      0X7250
#define 			DISP_DATA_JUFANG_TOTALNUM_ADDR      0X725A
#define 			KEY_DATA_JUFANG_LAST_ADDR           0X725C
#define 			KEY_DATA_JUFANG_NEXT_ADDR           0X725E


//沉降仪  73.bmp
#define 			DISP_DATA_CEHNJIANG_ID_ADDR            0X7300
#define       DISP_DATA_CEHNJIANG_TEMP_ADDR      		 0x7310
#define       DISP_DATA_CEHNJIANG_HEIGHT_ADDR			 	 0X732A
#define 			DISP_DATA_CEHNJIANG_TOTALNUM_ADDR      0X735A
#define 			KEY_DATA_CEHNJIANG_LAST_ADDR           0X735C
#define 			KEY_DATA_CEHNJIANG_NEXT_ADDR           0X735E


//天井洼 start
#define 			DISP_DATA_CEHNJIANG1_ID_TJW_ADDR             0X7360
#define       DISP_DATA_CEHNJIANG1_TEMP_TJW_ADDR      		 0x7370
#define       DISP_DATA_CEHNJIANG1_HEIGHT_TJW_ADDR			 	 0X737A
#define 			DISP_DATA_CEHNJIANG2_ID_TJW_ADDR             0X7380
#define       DISP_DATA_CEHNJIANG2_TEMP_TJW_ADDR      		 0x7390
#define       DISP_DATA_CEHNJIANG2_HEIGHT_TJW_ADDR			 	 0X739A
#define 			DISP_DATA_CEHNJIANG3_ID_TJW_ADDR             0X73A0
#define       DISP_DATA_CEHNJIANG3_TEMP_TJW_ADDR      		 0x73B0
#define       DISP_DATA_CEHNJIANG3_HEIGHT_TJW_ADDR			 	 0X73BA
#define 			DISP_DATA_CEHNJIANG4_ID_TJW_ADDR            0X73C0
#define       DISP_DATA_CEHNJIANG4_TEMP_TJW_ADDR      		 0x73D0
#define       DISP_DATA_CEHNJIANG4_HEIGHT_TJW_ADDR			 	 0X73DA
//天井洼 end

//防外破  74.bmp
#define 			DISP_DATA_FANGWAIPO_ID_ADDR           0X7400
#define       DISP_DATA_FANGWAIPO_ACCX_ADDR      		0x7410
#define       DISP_DATA_FANGWAIPO_ACCY_ADDR			 		0X741A
#define       DISP_DATA_FANGWAIPO_ACCZ_ADDR      		0x7420
#define       DISP_DATA_FANGWAIPO_TEMP_ADDR         0X742A
#define 			DISP_DATA_FANGWAIPO_TOTALNUM_ADDR     0X745A
#define 			KEY_DATA_FANGWAIPO_LAST_ADDR          0X745C
#define 			KEY_DATA_FANGWAIPO_NEXT_ADDR          0X745E

//天井洼 start
#define 			DISP_DATA_FANGWAIPO1_ID_TJW_ADDR            0X7430
#define       DISP_DATA_FANGWAIPO1_ACCX_TJW_ADDR      		0x7440
#define       DISP_DATA_FANGWAIPO1_ACCY_TJW_ADDR			 		0X744A
#define       DISP_DATA_FANGWAIPO1_ACCZ_TJW_ADDR      		0x7450
#define       DISP_DATA_FANGWAIPO1_TEMP_TJW_ADDR          0X74F0

#define 			DISP_DATA_FANGWAIPO2_ID_TJW_ADDR            0X7460
#define       DISP_DATA_FANGWAIPO2_ACCX_TJW_ADDR      		0x7470
#define       DISP_DATA_FANGWAIPO2_ACCY_TJW_ADDR			 		0X747A
#define       DISP_DATA_FANGWAIPO2_ACCZ_TJW_ADDR      		0x7480
#define       DISP_DATA_FANGWAIPO2_TEMP_TJW_ADDR          0X748A

#define 			DISP_DATA_FANGWAIPO3_ID_TJW_ADDR            0X7490
#define       DISP_DATA_FANGWAIPO3_ACCX_TJW_ADDR      		0x74A0
#define       DISP_DATA_FANGWAIPO3_ACCY_TJW_ADDR			 		0X74AA
#define       DISP_DATA_FANGWAIPO3_ACCZ_TJW_ADDR      		0x74B0
#define       DISP_DATA_FANGWAIPO3_TEMP_TJW_ADDR          0X74BA
#define 			DISP_DATA_FANGWAIPO4_ID_TJW_ADDR            0X74C0
#define       DISP_DATA_FANGWAIPO4_ACCX_TJW_ADDR      		0x74D0
#define       DISP_DATA_FANGWAIPO4_ACCY_TJW_ADDR			 		0X74DA
#define       DISP_DATA_FANGWAIPO4_ACCZ_TJW_ADDR      		0x74E0
#define       DISP_DATA_FANGWAIPO4_TEMP_TJW_ADDR          0X74EA

//天井洼 end
//甲烷  75.bmp
#define 			DISP_DATA_CH4_ID_ADDR           	0X7500
#define       DISP_DATA_CH4_VALUE_ADDR      		0x7510
#define 			DISP_DATA_CH4_TOTALNUM_ADDR    	  0X755A
#define 			KEY_DATA_CH4_LAST_ADDR          	0X755C
#define 			KEY_DATA_CH4_NEXT_ADDR          	0X755E




//氧气  76.bmp
#define 			DISP_DATA_O2_ID_ADDR           	0X7600
#define       DISP_DATA_O2_VALUE_ADDR      		0x7610
#define 			DISP_DATA_O2_TOTALNUM_ADDR    	0X765A
#define 			KEY_DATA_O2_LAST_ADDR          	0X765C
#define 			KEY_DATA_O2_NEXT_ADDR          	0X765E


//硫化氢  77.bmp
#define 			DISP_DATA_H2S_ID_ADDR           	0X7700
#define       DISP_DATA_H2S_VALUE_ADDR      		0x7710
#define 			DISP_DATA_H2S_TOTALNUM_ADDR    	0X775A
#define 			KEY_DATA_H2S_LAST_ADDR          	0X775C
#define 			KEY_DATA_H2S_NEXT_ADDR          	0X775E


//一氧化碳  78.bmp
#define 			DISP_DATA_CO_ID_ADDR           	0X7800
#define       DISP_DATA_CO_VALUE_ADDR      		0x7810
#define 			DISP_DATA_CO_TOTALNUM_ADDR    	0X785A
#define 			KEY_DATA_CO_LAST_ADDR          	0X785C
#define 			KEY_DATA_CO_NEXT_ADDR          	0X785E

//温湿度  79.bmp
#define 			DISP_DATA_WENSHIDU_ID_ADDR            0X7900
#define       DISP_DATA_WENSHIDU_TEMP_ADDR      		0x7910
#define       DISP_DATA_WENSHIDU_HUMI_ADDR			    0X792A
#define 			DISP_DATA_WENSHIDU_TOTALNUM_ADDR      0X795A
#define 			KEY_DATA_WENSHIDU_LAST_ADDR           0X795C
#define 			KEY_DATA_WENSHIDU_NEXT_ADDR           0X795E
//水位  80.bmp
#define 			DISP_DATA_WATER_ID_ADDR            0X8000
#define       DISP_DATA_WATER_DEPTH_ADDR      	 0x8010
#define 			DISP_DATA_WATER_TOTALNUM_ADDR      0X805A
#define 			KEY_DATA_WATER_LAST_ADDR           0X805C
#define 			KEY_DATA_WATER_NEXT_ADDR           0X805E
//裂缝仪 81.bmp
#define 			DISP_DATA_LIEFENG_ID_ADDR            0X8100
#define       DISP_DATA_LIEFENG_TEMP_ADDR      		 0x8110
#define       DISP_DATA_LIEFENG_DISTANC_ADDR			 0X812A
#define 			DISP_DATA_LIEFENG_TOTALNUM_ADDR      0X815A
#define 			KEY_DATA_LIEFENG_LAST_ADDR           0X815C
#define 			KEY_DATA_LIEFENG_NEXT_ADDR           0X815E
//井盖 82.bmp
#define 			DISP_DATA_COVER_ID_ADDR            0X8200
#define       DISP_DATA_COVER_INCLINE_ADDR       0x8210
#define       DISP_DATA_COVER_SWITCH2P_ADDR			 0X821A
#define       DISP_DATA_COVER_VIBRATION_ADDR     0x822A
#define       DISP_DATA_COVER_SWITCH1P_ADDR			 0X8230
#define 			DISP_DATA_COVER_TOTALNUM_ADDR      0X825A
#define 			KEY_DATA_COVER_LAST_ADDR           0X825C
#define 			KEY_DATA_COVER_NEXT_ADDR           0X825E





//////////////////////////////////////////////////////////////
//保存界面

#define   KEY_SAVE_ADDR             	0X1700
#define   KEY_RESET_ADDR            	0X1702
#define   KEY_SAVEOK_ADDR             0X1708
#define   KEY_RESETOK_ADDR            0X1714
#define   KEY_RETURN_ADDR             0X1706
//井盖界面
#define   TEXT_COVER_ID_ADDR         			0X1900
#define   TEXT_COVER_OPENRESP_ADDR        0X1910
#define   TEXT_COVER_CLOSERESP_ADDR       0X1918

#define   DATA_COVER_ADDR_ADDR         		0X1920
#define   DATA_COVER_TOTALPAGE_ADDR       0X1922
#define   DATA_COVER_NOWPAGE_ADDR         0X1924

#define   KEY_COVER_OPEN_ADDR         0X1926
#define   KEY_COVER_CLOSE_ADDR        0X1928
#define   KEY_COVER_LAST_ADDR         0X192A
#define   KEY_COVER_NEXT_ADDR         0X192C

#define   KEY_PROTOL_SWITCH_ADDR      0X192E

#define   SOFT_VER_ADDR        			  0X1930  
#define   SD_STATE_ADDR         			0X193A

#define   KEY_TCP_ADDR									0X193C
#define   KEY_MQTT_ADDR								  0X193E
#define   SD_TOTOAL_SIZE_ADDR             0X1940
#define   SD_REMAIN_SIZE_ADDR             0X1950
//[09:27:21.942]收←◆[LCD] fa:5a a5 15 82 19 0 47 59 4e 4a 4c 58 53 44 30 30 30 30 30 30 36 38 34 ff 
// [LCD] fa:5a a5 5 82 19 20 0 1 
// [LCD] fa:5a a5 5 82 19 26 ff ff 
// [LCD] fa:5a a5 5 82 19 28 ff ff 
// [LCDTASK]revLen:9
//5a a5 06 83 00 00 01 1a 16 
//KEY_COVER_DISPLAY_ADDR


//主界面
#define   KEY_NETERROR_ADDR         0X1A00
#define   KEY_MODBUSERR_ADDR        0X1A02
#define   KEY_INPUT_ADDR     			  0X1A0C
#define   KEY_OUTPUT_ADDR   				0X1A0E
#define   KEY_MODUBS_ADDR  					0X1A0A

#define   KEY_MODUBS_DATA_ADDR  		0X1A10
#define   KEY_COVER_DISPLAY_ADDR    0x1A16

#define   KEY_SOFT_VER_ADDR  				0X1A30
#define   KEY_SD_STATE_ADDR    		  0x1A32

#define   NET_ERR_DISPLAY_ADDR      0X1A20
#define   MODBUS_ERR_DISPLAY_ADDR   0X1A22
//密码输入界面
#define   KEY_PASSWD_ENTER_ADDR     0X1A1A
#define   TEXT_PASSWD_ADDR          0X1A24
#define   KEY_PASSWD_SURE_ADDR      0X1A2E
//#define   TEXT_ERR_PASSWD_DISP_ADDR  0X1A30

//////////////////////////////////////////////////////////////
//kenal lcd command
///#define   LCD_HEAD       0XAEA5
#define   LCD_HEAD       0X5AA5
#define   LCD_FILL_TAIL  0XFFFF
#define   LCD_WRITE      0X82
#define   LCD_READ       0X83
#define   LCD_RUN        0X4F4B



#define  KEY_ADDR  			0X0000
#define  LCD_RTC_ADDR   0X0010



extern bool pwssWdRet;

extern int LCDWtite(uint16_t addr,uint8_t *data,uint8_t dataLen);


//typedef struct{
//		char  name[25];
//		char  ID[20];
//		char  model[8];
//		uint8_t   port;
//		uint8_t   addr;
//		uint32_t  colTime;
//}LCDDispModInfoStru;
//typedef struct{
//	  uint8_t X;//modbus设备的种类
//	  uint8_t Y;//每种modbus的个数
//	  uint8_t flag;//存在标记为1
//}modbusPositStru;

//extern LCDDispModInfoStru  modbusLCDRead;
//extern modbusPositStru  modPosit[TOTOLA_485_NUM]; 
//extern modbusPositStru  modPositErr[TOTOLA_485_NUM];

typedef struct
{
		uint8_t year;
		uint8_t month;
		uint8_t day;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
}RTC_TimeTypeDef;
RTC_TimeTypeDef utc_to_beijingTime(uint32_t time);
uint32_t beijingTime_to_utc(RTC_TimeTypeDef t_location);
void  writeRTCToLcd(RTC_TimeTypeDef time);
void  readLcdRTC(void);
extern RTC_TimeTypeDef readRtc;
RTC_TimeTypeDef copyBufToRtc(char *buf);
void  correctLcdTime(uint32_t time);


uint64_t  utcTime_ms(void);
uint64_t  utcTime_s(void);
void LCDDispSameID(uint16_t addr);
void LCDRstDispSameID(uint16_t addr);

#endif

