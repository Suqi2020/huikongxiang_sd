#include <board.h>
//����8·ttl��ƽ�����2·3V3��ƽ���  2·5V��ƽ��� 4·12V���
//const static char sign[]="[output]";
extern int mod_printf(const char *fmt, ...);

//��ӡdigital output����б�
void printfDOList()
{
		for(int j=0;j<DO_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
				if(packFlash.digoutput[j].workFlag==RT_TRUE){//��
						mod_printf("digOutput ");
						mod_printf("%s ",packFlash.digoutput[j].name);
						mod_printf("%s ",packFlash.digoutput[j].devID);
						mod_printf("%s ",packFlash.digoutput[j].model);
						mod_printf("%d \n",packFlash.digoutput[j].port);
				}
		}
}

//����port����� 
//digital output ��������
//digOutput ˮ�� GYNJLXSD000000164 GY283 1
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
				mod_printf("ERR input the same ID\n");
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=DO_NUM)&&(port>0)){//���
				packFlash.digoutput[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.digoutput[port-1].name, argv[1]);
				rt_strcpy(packFlash.digoutput[port-1].devID,argv[2]);
				rt_strcpy(packFlash.digoutput[port-1].model,argv[3]);
				packFlash.digoutput[port-1].port=port;
				mod_printf("add digOutput chanl %d\n",port);
			  mod_printf("digOutput OK\n");
		}
		else{//ɾ��
			 for(int j=0;j<DO_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
					if(rt_strcmp(packFlash.digoutput[j].devID,argv[2])==0){
							packFlash.digoutput[j].workFlag=RT_FALSE;
							mod_printf("%s delete digOutput channel %d\n",j+1);
					}
			 }
		}
		return;
		ERR:
		mod_printf("for example\n");
		mod_printf("[digOutput ˮ�� GYNJLXSD000000162 GY281 1]\n");
		mod_printf("[port1-8 ֮�������ӦID�����в���]\n");

}
MSH_CMD_EXPORT(digOutput,digOutput config);//FINSH_FUNCTION_EXPORT_CMD

#if 0

//��ӡ3V3����������б�
void printfPower3V3List()
{
		for(int j=0;j<V33O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
				if(packFlash.v33output[j].workFlag==RT_TRUE){//��
						mod_printf("%s power3V3 ",sign);
						mod_printf("%s ",packFlash.v33output[j].name);
						mod_printf("%s ",packFlash.v33output[j].devID);
						mod_printf("%s ",packFlash.v33output[j].model);
						mod_printf("%d \n",packFlash.v33output[j].port);
				}
		}
}

//����port����� 
//power3V3������� ��������
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
				mod_printf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=V33O_NUM)&&(port>0)){//���
				packFlash.v33output[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.v33output[port-1].name, argv[1]);
				rt_strcpy(packFlash.v33output[port-1].devID,argv[2]);
				rt_strcpy(packFlash.v33output[port-1].model,argv[3]);
				packFlash.v33output[port-1].port=port;
				mod_printf("%s add 3V3Output chanl %d\n",sign,port);
			  mod_printf("%s 3V3Output OK\n",sign);
		}
		else{//ɾ��
			 for(int j=0;j<V33O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
					if(rt_strcmp(packFlash.v33output[j].devID,argv[2])==0){
							packFlash.v33output[j].workFlag=RT_FALSE;
							mod_printf("%s delete 3V3Output channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		mod_printf("%sfor example\n",sign);
		mod_printf("%s[power3V3 ˮ�� GYNJLXSD000000162 GY281 1]\n",sign);
		mod_printf("%s[port1-8 ֮�������ӦID�����в���]\n",sign);

}
MSH_CMD_EXPORT(power3V3,power3V3 config);//FINSH_FUNCTION_EXPORT_CMD




//��ӡ5V����������б�
void printfPower5VList()
{
		for(int j=0;j<V5O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
				if(packFlash.v5output[j].workFlag==RT_TRUE){//��
						mod_printf("%s power5V ",sign);
						mod_printf("%s ",packFlash.v5output[j].name);
						mod_printf("%s ",packFlash.v5output[j].devID);
						mod_printf("%s ",packFlash.v5output[j].model);
						mod_printf("%d \n",packFlash.v5output[j].port);
				}
		}
}

//����port����� 
//power5V������� ��������
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
				mod_printf("%sERR input the same ID\n",sign);
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=V5O_NUM)&&(port>0)){//���
				packFlash.v5output[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.v5output[port-1].name, argv[1]);
				rt_strcpy(packFlash.v5output[port-1].devID,argv[2]);
				rt_strcpy(packFlash.v5output[port-1].model,argv[3]);
				packFlash.v5output[port-1].port=port;
				mod_printf("%s add 5VOutput chanl %d\n",sign,port);
			  mod_printf("%s 5VOutput OK\n",sign);
		}
		else{//ɾ��
			 for(int j=0;j<V5O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
					if(rt_strcmp(packFlash.v5output[j].devID,argv[2])==0){
							packFlash.v5output[j].workFlag=RT_FALSE;
							mod_printf("%s delete 5VOutput channel %d\n",sign,j+1);
					}
			 }
		}
		return;
		ERR:
		mod_printf("%sfor example\n",sign);
		mod_printf("%s[power5V ˮ�� GYNJLXSD000000162 GY281 1]\n",sign);
		mod_printf("%s[port1-8 ֮�������ӦID�����в���]\n",sign);

}
MSH_CMD_EXPORT(power5V,power5V config);//FINSH_FUNCTION_EXPORT_CMD
#endif

//��ӡ12V������б�
void printfSwitchList()
{
		for(int j=0;j<SWITCH_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
				if(packFlash.switchoutput[j].workFlag==RT_TRUE){//��
						mod_printf("switch ");
						mod_printf("%s ",packFlash.switchoutput[j].name);
						mod_printf("%s ",packFlash.switchoutput[j].devID);
						mod_printf("%s ",packFlash.switchoutput[j].model);
						mod_printf("%d \n",packFlash.switchoutput[j].port);
				}
		}
}

//����port����� 
//power12V������� ��������
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
				mod_printf("ERR input the same ID\n");
			  return;
			}
		}
    port = atoi16(argv[4],10);
		if((port<=SWITCH_NUM)&&(port>0)){//���
				packFlash.switchoutput[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.switchoutput[port-1].name, argv[1]);
				rt_strcpy(packFlash.switchoutput[port-1].devID,argv[2]);
				rt_strcpy(packFlash.switchoutput[port-1].model,argv[3]);
				packFlash.switchoutput[port-1].port=port;
				mod_printf("add sswitch output chanl %d\n",port);
			  mod_printf("switchOutput OK\n");
		}
		else{//ɾ��
			 for(int j=0;j<SWITCH_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
					if(rt_strcmp(packFlash.switchoutput[j].devID,argv[2])==0){
							packFlash.switchoutput[j].workFlag=RT_FALSE;
							mod_printf("delete switchOutput channel %d\n",j+1);
					}
			 }
		}
		return;
		ERR:
		mod_printf("for example\n");
		mod_printf("[sswitch ˮ�� GYNJLXSD000000162 GY281 1]\n");
		mod_printf("[port1-4 ֮�������ӦID�����в���]\n");

}
MSH_CMD_EXPORT(sswitch,sswitch config);//FINSH_FUNCTION_EXPORT_CMD


//��ӡ����������б�
//���� ��ӡdigital output 8·
//��ӡ3V3��� 2·
//��ӡ5V��� 2·
//��ӡ12V��� 4·
void printfOutputList()
{
		printfDOList();
//	  printfPower3V3List();
//	  printfPower5VList();
	  printfSwitchList();
}

