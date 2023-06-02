#include "board.h"
static char  *sign="[LCDCHOOSE]";
int modbusChosIndex=0;
//显示选择的modbus名字
void dispChooseModbusName()
{

	 uint8_t buf[NAME_LEN]="";
		for(int i=0;i<NAME_LEN;i++){
			  if(modbusName[modbusChosIndex][i]==0){
					buf[i]=0xff;
					buf[i+1]=0xff;
					break;
				}
				buf[i]=modbusName[modbusChosIndex][i];
			  
		}
	  LCDWtite(TEST_CHOOSE_MODBUS_ADDR,buf,strlen((char *)buf));

}
//下一个modbus名字
void  nextModName()
{
	modbusChosIndex++;
	if(modbusChosIndex==MODBUS_NUM)
		modbusChosIndex=0;
}
//上一个modbusname
void  lastModName()
{
		if(modbusChosIndex==0)
			modbusChosIndex=MODBUS_NUM-1;
		else
			modbusChosIndex--;
}

void dispHuanliuData(void);
void  dispJufangData(void);
void  dispChenjiangData(void);
void  dispWaipoData(void);
void  dispCH4Data(void);
void  dispO2Data(void);
void  dispH2SData(void);
void  dispCOData(void);
void  dispWenshiduData(void);
void  dispLiefengData(void);
void  dispJinggaiData(void);
void  dispWaterData(void);
//切换传感器目录后 按确定键盘显示当前modbus的数据
void  dispCurrentModData()
{
		//"HuanLiu","JuFang","FangChenJiang","FangWaiPo","JiaWan","YangQi","LiuHuaQing","YiYangHuaTan","WenShiDu","ShuiWei","LieFengYi","JingGai"};

		switch(modbusChosIndex){ 
			case 0://环流
				dispHuanliuData();
				break;
			case 1://"JuFang",
				dispJufangData();
				break;
			case 2://"FangChenJiang",
				dispChenjiangData();
				break;
			case 3://"FangWaiPo"
				dispWaipoData();
				break;
			case 4://"JiaWan"();
				dispCH4Data();
				break;
			case 5://"YangQi",
				dispO2Data();
				break;
			case 6://"LiuHuaQing",
				dispH2SData();
				break;
			case 7://"YiYangHuaTan",
				dispCOData();
				break;
			case 8://"WenShiDu"
				dispWenshiduData();
				break;
			case 9://"ShuiWei"
				dispWaterData();
				break;
			case 10://"LieFengYi"
				dispLiefengData();
				break;
			case 11://"JingGai"
				dispJinggaiData();
				break;
			default:
        rt_kprintf("%s ERR:dispCurrentModData \n",sign);
				break;
		}
}


