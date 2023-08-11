#include <board.h>


//const static char sign[]="[digInput]";

extern int mod_printf(const char *fmt, ...);
//digital input ��ȡ��ƽֵ �ߵ�ƽ����1   ��������0
bool  diReadFun(char num)
{
	  int ret=0;
		switch(num)
		{
			case 0: ret=IOIN1_READ;break;
			case 1: ret=IOIN2_READ;break;
			case 2: ret=IOIN3_READ;break;
			case 3: ret=IOIN4_READ;break;
			case 4: ret=IOIN5_READ;break;
			case 5: ret=IOIN6_READ;break;
			case 6: ret=IOIN7_READ;break;
			case 7: ret=IOIN8_READ;break;
		}
		return ret;
}
//digit input��⵽�����߼��ĵ�ƽ ��λ��Ӧ��flag
void diIOSetFlag()
{
		for(int i=0;i<DI_NUM;i++){
				if(packFlash.diginput[i].workFlag==true){
						if(diReadFun(i)==true){
								inpoutpFlag.digInput[i].upFlag =true;
							  inpoutpFlag.digInput[i].lowFlag =false; 
							  //rt_kprintf("%s diIOSet upFlag %d\n",sign,i);
						}
						else {
							  inpoutpFlag.digInput[i].upFlag =false;
								inpoutpFlag.digInput[i].lowFlag=true;
							  //rt_kprintf("%s diIOSet lowFlag %d\n",sign,i);
						}
				}
		}
}

//��ӡdigit input �б�
void printfDIList()
{
		for(int j=0;j<DI_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
				if(packFlash.diginput[j].workFlag==RT_TRUE){//��
						mod_printf("digInput ");

						mod_printf("%s ",packFlash.diginput[j].name);
						mod_printf("%s ",packFlash.diginput[j].devID);
						
						mod_printf("%s ",packFlash.diginput[j].model);
						mod_printf("%d \n",packFlash.diginput[j].port);



				}
		}
}

//digInput ˮ�� GYNJLXSD000000162 GY281 1
//digital input ��������
static void digInput(int argc, char *argv[])
{
	 int port;
	 if(0==rt_strcmp((char *)"list", argv[1])){
				printfDIList();
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

		if((port<=DI_NUM)&&(port>0)){//���
				packFlash.diginput[port-1].workFlag=RT_TRUE;
				rt_strcpy(packFlash.diginput[port-1].name, argv[1]);
				rt_strcpy(packFlash.diginput[port-1].devID,argv[2]);
				rt_strcpy(packFlash.diginput[port-1].model,argv[3]);
				packFlash.diginput[port-1].port=port;
				mod_printf("add diginput chanl %d\n",port);
        mod_printf("digInput OK\n");
		}
		else{//ɾ��
			 for(int j=0;j<DI_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
					if(rt_strcmp(packFlash.diginput[j].devID,argv[2])==0){
							packFlash.diginput[j].workFlag=RT_FALSE;
							mod_printf("delete diginput channel %d\n",j+1);
					}
			 }
		}
		return;
		ERR:
		mod_printf("for example\n");
		mod_printf("[digInput ˮ�� GYNJLXSD000000162 GY281 1]\n");
		mod_printf("[port1-8 ֮�������ӦID�����в���]\n");

}
MSH_CMD_EXPORT(digInput,digInput config);//FINSH_FUNCTION_EXPORT_CMD


