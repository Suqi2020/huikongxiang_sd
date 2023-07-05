#include <board.h>
//包括8路ttl电平输出和2路3V3电平输出  2路5V电平输出 4路12V输出
const static char sign[]="[output]";


//打印digital output输出列表
void printfDOList()
{
		for(int j=0;j<DO_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
				if(packFlash.digoutput[j].workFlag==RT_TRUE){//打开
						rt_kprintf("%s digOutput ",sign);
						rt_kprintf("%s ",packFlash.digoutput[j].name);
						rt_kprintf("%s ",packFlash.digoutput[j].devID);
						rt_kprintf("%s ",packFlash.digoutput[j].model);
						rt_kprintf("%d \n",packFlash.digoutput[j].port);
				}
		}
}

//按照port来添加 
//digital output 输入配置
//digOutput 水泵 GYNJLXSD000000164 GY283 1
static void digOutput(int argc, char *argv[])
{
	  int port;
	  if(0==rt_strcmp((char *)"list", argv[1])){
				printfDOList();
				return;
		}
		if(argc!=5){
				goto ERR;
		}	
		if(devIDOKCheck(argv[2])!=true){
			if((strcmp(argv[5],"0")!=0)&&(strcmp(argv[5],"255")!=0)){
				rt_kprintf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=DO_NUM)&&(port>0)){//添加
				packFlash.digoutput[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.digoutput[port-1].name, argv[1]);
				rt_strcpy(packFlash.digoutput[port-1].devID,argv[2]);
				rt_strcpy(packFlash.digoutput[port-1].model,argv[3]);
				packFlash.digoutput[port-1].port=port;
				rt_kprintf("%s add digOutput chanl %d\n",sign,port);
			  rt_kprintf("%s digOutput OK\n",sign);
		}
		else{//删除
			 for(int j=0;j<DO_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
					if(rt_strcmp(packFlash.digoutput[j].devID,argv[2])==0){
							packFlash.digoutput[j].workFlag=RT_FALSE;
							rt_kprintf("%s delete digOutput channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		rt_kprintf("%sfor example\n",sign);
		rt_kprintf("%s[digOutput 水泵 GYNJLXSD000000162 GY281 1]\n",sign);
		rt_kprintf("%s[port1-8 之外清除对应ID的所有参数]\n",sign);

}
MSH_CMD_EXPORT(digOutput,digOutput config);//FINSH_FUNCTION_EXPORT_CMD

#if 0

//打印3V3的输出配置列表
void printfPower3V3List()
{
		for(int j=0;j<V33O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
				if(packFlash.v33output[j].workFlag==RT_TRUE){//打开
						rt_kprintf("%s power3V3 ",sign);
						rt_kprintf("%s ",packFlash.v33output[j].name);
						rt_kprintf("%s ",packFlash.v33output[j].devID);
						rt_kprintf("%s ",packFlash.v33output[j].model);
						rt_kprintf("%d \n",packFlash.v33output[j].port);
				}
		}
}

//按照port来添加 
//power3V3输出控制 输入配置
static void power3V3(int argc, char *argv[])
{
	  int port;
	  if(0==rt_strcmp((char *)"list", argv[1])){
				printfPower3V3List();
				return;
		}
		if(argc!=5){
				goto ERR;
		}
		if(devIDOKCheck(argv[2])!=true){
			if((strcmp(argv[5],"0")!=0)&&(strcmp(argv[5],"255")!=0)){
				rt_kprintf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=V33O_NUM)&&(port>0)){//添加
				packFlash.v33output[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.v33output[port-1].name, argv[1]);
				rt_strcpy(packFlash.v33output[port-1].devID,argv[2]);
				rt_strcpy(packFlash.v33output[port-1].model,argv[3]);
				packFlash.v33output[port-1].port=port;
				rt_kprintf("%s add 3V3Output chanl %d\n",sign,port);
			  rt_kprintf("%s 3V3Output OK\n",sign);
		}
		else{//删除
			 for(int j=0;j<V33O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
					if(rt_strcmp(packFlash.v33output[j].devID,argv[2])==0){
							packFlash.v33output[j].workFlag=RT_FALSE;
							rt_kprintf("%s delete 3V3Output channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		rt_kprintf("%sfor example\n",sign);
		rt_kprintf("%s[power3V3 水泵 GYNJLXSD000000162 GY281 1]\n",sign);
		rt_kprintf("%s[port1-8 之外清除对应ID的所有参数]\n",sign);

}
MSH_CMD_EXPORT(power3V3,power3V3 config);//FINSH_FUNCTION_EXPORT_CMD




//打印5V的输出配置列表
void printfPower5VList()
{
		for(int j=0;j<V5O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
				if(packFlash.v5output[j].workFlag==RT_TRUE){//打开
						rt_kprintf("%s power5V ",sign);
						rt_kprintf("%s ",packFlash.v5output[j].name);
						rt_kprintf("%s ",packFlash.v5output[j].devID);
						rt_kprintf("%s ",packFlash.v5output[j].model);
						rt_kprintf("%d \n",packFlash.v5output[j].port);
				}
		}
}

//按照port来添加 
//power5V输出控制 输入配置
static void power5V(int argc, char *argv[])
{
    int port;
	 
	  if(0==rt_strcmp((char *)"list", argv[1])){
				printfDOList();
				return;
		}
		if(argc!=5){
				goto ERR;
		}
		if(devIDOKCheck(argv[2])!=true){
			if((strcmp(argv[5],"0")!=0)&&(strcmp(argv[5],"255")!=0)){
				rt_kprintf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=V5O_NUM)&&(port>0)){//添加
				packFlash.v5output[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.v5output[port-1].name, argv[1]);
				rt_strcpy(packFlash.v5output[port-1].devID,argv[2]);
				rt_strcpy(packFlash.v5output[port-1].model,argv[3]);
				packFlash.v5output[port-1].port=port;
				rt_kprintf("%s add 5VOutput chanl %d\n",sign,port);
			  rt_kprintf("%s 5VOutput OK\n",sign);
		}
		else{//删除
			 for(int j=0;j<V5O_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
					if(rt_strcmp(packFlash.v5output[j].devID,argv[2])==0){
							packFlash.v5output[j].workFlag=RT_FALSE;
							rt_kprintf("%s delete 5VOutput channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		rt_kprintf("%sfor example\n",sign);
		rt_kprintf("%s[power5V 水泵 GYNJLXSD000000162 GY281 1]\n",sign);
		rt_kprintf("%s[port1-8 之外清除对应ID的所有参数]\n",sign);

}
MSH_CMD_EXPORT(power5V,power5V config);//FINSH_FUNCTION_EXPORT_CMD
#endif

//打印12V的输出列表
void printfSwitchList()
{
		for(int j=0;j<SWITCH_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
				if(packFlash.switchoutput[j].workFlag==RT_TRUE){//打开
						rt_kprintf("%s switch ",sign);
						rt_kprintf("%s ",packFlash.switchoutput[j].name);
						rt_kprintf("%s ",packFlash.switchoutput[j].devID);
						rt_kprintf("%s ",packFlash.switchoutput[j].model);
						rt_kprintf("%d \n",packFlash.switchoutput[j].port);
				}
		}
}

//按照port来添加 
//power12V输出控制 输入配置
static void sswitch(int argc, char *argv[])
{
		int port;
	  if(0==rt_strcmp((char *)"list", argv[1])){
				printfSwitchList();
				return;
		}
		if(argc!=5){
			
				goto ERR;
		}
		if(devIDOKCheck(argv[2])!=true){
			if((strcmp(argv[5],"0")!=0)&&(strcmp(argv[5],"255")!=0)){
				rt_kprintf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=SWITCH_NUM)&&(port>0)){//添加
				packFlash.switchoutput[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.switchoutput[port-1].name, argv[1]);
				rt_strcpy(packFlash.switchoutput[port-1].devID,argv[2]);
				rt_strcpy(packFlash.switchoutput[port-1].model,argv[3]);
				packFlash.switchoutput[port-1].port=port;
				rt_kprintf("%s add sswitch output chanl %d\n",sign,port);
			  rt_kprintf("%s switchOutput OK\n",sign);
		}
		else{//删除
			 for(int j=0;j<SWITCH_NUM;j++){//查一遍 找到 GYNJLXSD000000499  如果
					if(rt_strcmp(packFlash.switchoutput[j].devID,argv[2])==0){
							packFlash.switchoutput[j].workFlag=RT_FALSE;
							rt_kprintf("%s delete switchOutput channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		rt_kprintf("%sfor example\n",sign);
		rt_kprintf("%s[sswitch 水泵 GYNJLXSD000000162 GY281 1]\n",sign);
		rt_kprintf("%s[port1-4 之外清除对应ID的所有参数]\n",sign);

}
MSH_CMD_EXPORT(sswitch,sswitch config);//FINSH_FUNCTION_EXPORT_CMD


//打印控制输出的列表
//包括 打印digital output 8路
//打印3V3输出 2路
//打印5V输出 2路
//打印12V输出 4路
void printfOutputList()
{
		printfDOList();
//	  printfPower3V3List();
//	  printfPower5VList();
	  printfSwitchList();
}

