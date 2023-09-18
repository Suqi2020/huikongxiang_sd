#include <board.h>
//��ֵ���òο� �������modbus��������V0.4.doc��
/*
a�����������豸����ʱ���豸��Ĺ�ϵΪ�߼��루����������ȫ�����㣩��
b���������豸����ʱ���豸��Ĺ�ϵΪ�߼��루��������豸����Ҫִ�У�
c�����������豸�����������豸��Ĺ�ϵ���߼����ʱ����Ҫ��ε������ü���
d����֧��ɾ�����������еĵ����豸����Ҫ����cancel�����������豸
e��sure cancelȷ����ȡ������(a/b/c/d)����
f��list��ʾ���10��������Ч���߼��б�
g��delete(1-10)ɾ�����ù����߼��б�
h��10���߼�������ָ����ʽָ����ִֵ�е�flag
{ char *inputFlag[10];
	Char *outputFlag[10];
	Char workFlag;
}total[10];
//////////////////////////////////////////////////
autoctrl AI ��ʪ��  GYNJLXSD000000499 1 
autoctrl AI ��ʪ��  GYNJLXSD000000499 2 
autoctrl DI ��ʪ��  GYNJLXSD000000498 1
autoctrl DI ��ʪ��  GYNJLXSD000000498 2 
autoctrl MB ����  GYNJLXSD000000491 1 
autoctrl MB ����  GYNJLXSD000000491 1 
autoctrl MB ����  GYNJLXSD000000492 1 
autoctrl MB ����  GYNJLXSD000000492 2 
autoctrl MB ����  GYNJLXSD000000492 3 
autoctrl DO ˮ��  GYNJLXSD000000499 1 
autoctrl DO ˮ��  GYNJLXSD000000499 2
autoctrl V3O ˮ��  GYNJLXSD000000419 1
autoctrl V5O ˮ��  GYNJLXSD000000429 1
autoctrl SWITCH ˮ��  GYNJLXSD000000439 1
//////////////////////////////////////////////////
*/
extern int mod_printf(const char *fmt, ...);
inoutDevStru inpoutpFlag __attribute__((at(0x2000D000)))={0};//���������ֵ�ı��λ  ָ���ڴ��ַ  ��ֹ���Ĵ�����ڴ��ַ��λ
////sheet.autoctrl[pindex].output[outputIndex].flag��ָ��ָ����inpoutpFlag
//const static char sign[]="[autoCtrl]";
const char inName[INNAME_NUM][INOUTNAME_LEN]={"AI","DI","MB",};
//const char outName[OUTNAME_NUM][INOUTNAME_LEN]={"DO","V3O","V5O","SWITCH"};
//char outSwitchNum[OUTNAME_NUM]={DO_NUM,V33O_NUM,V5O_NUM,SWITCH_NUM};
const char outName[OUTNAME_NUM][INOUTNAME_LEN]={"DO","SSWITCH"};


const uint8_t outChinaName[OUTNAME_NUM][INOUTNAME_LEN]={{0x54,0x54,0x4c,0xb5,0xe7,0xc6,0xbd,0x00},\
																												{0xBC,0xCC,0xB5,0xE7,0xC6,0xF7,0x00,0x00}};

																										

char outSwitchNum[OUTNAME_NUM]={DO_NUM,SWITCH_NUM};
uint8_t inputIndex=0; //�������ÿ����߼���������������   ��inputFlag ָ�����
uint8_t outputIndex=0;//�������ÿ����߼���������������� ��outputFlagָ�����

uint8_t pindex=0;//�ܹ����ù���ô��������� ÿ���û���ɾ����һ���Լ��ϵ�ʱ�� ��Ҫ���¶�ȡpindex 
//������������ý��� ��û���������  ���ɾ�� ��ʱ���ܸ���pindex
bool configFlag=false;//����������ж��ǲ�����������
//���һ��û�����ù���Index 
int findCtrlIndex()
{
		for(int i=0;i<CRTL_TOTAL_NUM;i++){
				if(sheet.autoctrl[i].workFlag!=1){
					  
						return i;//�ҵ�û���ù��� 
				}
		}
		return 255;
}
bool firstReadFlag=false;



//ָ��ָ��ַŵ���ֵ
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
					if(rt_strcmp(sheet.cirCula[i].ID,argv[3])==0){//�ҵ���ͬID��

							if(strcmp(argv[4],"earthCurA")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurALowFlag;
						
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurAUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"earthCurB")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurBLowFlag;
											
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCircul[i].cirCurBUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"earthCurC")==0){//A
									if(limit==0){//����
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


//ָ��ָ��ַŵ���ֵ
static bool crtlPartDischagConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:partDischagConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<PARTDISCHAG_485_NUM;i++){
					if(rt_strcmp(sheet.partDischag[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"pdA")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"pdB")==0){//B
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"pdC")==0){//C
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeCLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].amplitudeCUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqA")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqB")==0){//B
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"freqC")==0){//C
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqCLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].freqCUpFlag;
											return true;
									}
							}	
							else if(strcmp(argv[4],"dischargeDateA")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeALowFlag;
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeAUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"dischargeDateB")==0){//B
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeBLowFlag;
											return true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPartDisChg[i].dischargeBUpFlag;
											return true;
									}
							}
							else if(strcmp(argv[4],"dischargeDateC")==0){//C
									if(limit==0){//����
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
//ָ��ָ�����������ֵ
static bool ctrlPressSettlConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlPressSettlConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<PRESSSETTL_485_NUM;i++){
						if(rt_strcmp(sheet.pressSetl[i].ID,argv[3])==0){//�ҵ���ͬID��
								if(strcmp(argv[4],"temperature")==0){//A
									  if(limit==0){//����
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].tempLowFlag;
											  
											  return true;// true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusPreSettl[i].tempUpFlag;
											  return true;
										}
								}
								else if(strcmp(argv[4],"height")==0){//A
									  if(limit==0){//����
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

//ָ��ָ��������ٶȵĲ���
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
						if(rt_strcmp(sheet.threeAxiss[i].ID,argv[3])==0){//�ҵ���ͬID��
								if(strcmp(argv[4],"temperature")==0){//A
									  if(limit==0){//����
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].tempLowFlag;
											  
											  return true;// true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].tempUpFlag;
												
											  return true; 
										}
								}
								else if(strcmp(argv[4],"accelerationX")==0){//A
									  if(limit==0){//����
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accXLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accXUpFlag;
												
											  return true;
										}
								}
								else if(strcmp(argv[4],"accelerationY")==0){//B
									  if(limit==0){//����
												sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accYLowFlag;
											  
											  return true;
										}
										else{
											  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusThreAxis[i].accYUpFlag;
												
											  return true;
										}
								}							
								else if(strcmp(argv[4],"accelerationZ")==0){//C
									  if(limit==0){//����
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
//ָ��ָ��������ֵ
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
						if(rt_strcmp(sheet.ch4[i].ID,argv[3])==0){//�ҵ���ͬID��
								if(strcmp(argv[4],"methane")==0){//A
									  if(limit==0){//����
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
//ָ��ָ����������ֵ
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
					if(rt_strcmp(sheet.o2[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"oxy")==0){//A
									if(limit==0){//����
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
//ָ��ָ���������ֵ
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
					if(rt_strcmp(sheet.h2s[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"hydrogenSulfide")==0){//A
									if(limit==0){//����
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
//ָ��ָ��CO����ֵ
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
					if(rt_strcmp(sheet.co[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"monoxide")==0){//A
									if(limit==0){//����
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

//ָ��ָ����ʪ�ȵ���ֵ
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
					if(rt_strcmp(sheet.tempHum[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"temperature")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].tempLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusTempHum[i].tempUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"humidity")==0){//A
									if(limit==0){//����
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
//ָ��ָ��ˮλ����������ֵ
static bool  ctrlWaterConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrlWaterConf argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<WATERDEPTH_485_NUM;i++){
					if(rt_strcmp(sheet.waterDepth[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"depth")==0){//A
									if(limit==0){//����
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
//ָ��ָ��ˮλ����������ֵ
static bool  ctrlCrackMeterConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrl crackMeter argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<CRACKMETER_485_NUM;i++){
					if(rt_strcmp(sheet.crackMeter[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"temperature")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].tempLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.modbusCrackMeter[i].tempUpFlag;
											
											return true;
									}
							}
							else 	if(strcmp(argv[4],"distance")==0){//A
									if(limit==0){//����
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

//ָ��ָ�򴫸�������ֵ
static bool  ctrlCoverConf(char **argv)
{
			uint8_t limit   = atoi16(argv[5],10);
	    int i;
			if(!((limit==0)||(limit==1))){
					mod_printf("ERR:ctrl cover argv[5] limit %s should be 0 1\n");
				  return false;
			}
			for(i=0;i<COVER_485_NUM;i++){
					if(rt_strcmp(sheet.cover[i].ID,argv[3])==0){//�ҵ���ͬID��
							if(strcmp(argv[4],"incline")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].inclineLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].inclineUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"switch2")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch2LowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].switch2UpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"vibration")==0){//A
									if(limit==0){//����
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].vibrationLowFlag;
											
											return true;// true;
									}
									else{
											sheet.autoctrl[pindex].input[inputIndex].flag=(uint32_t*)&inpoutpFlag.modbusCover[i].vibrationUpFlag;
											
											return true;
									}
							}
							else if(strcmp(argv[4],"switch1")==0){//A
									if(limit==0){//����
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
//�����ָ������
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
														if(sheet.analog[j].subName==1){//find subname �������������
															  if(rt_strcmp(argv[4],"temperature")==0){//�¶�
																		if(limit==0){//����
																				sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.tempLowFlag;
																		}
																		else{//����
																			  sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.tempUpFlag;
																		}
																		inputIndex++;
																}
																if(rt_strcmp(argv[4],"humidity")==0){//ʪ��
																		if(limit==0){//����
																				sheet.autoctrl[pindex].input[inputIndex].flag=&inpoutpFlag.analogTempHum.humLowFlag;
																		}
																		else{//����
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
												//packFlash.input[i].levelFlag=limit;//ͨ��levelFlag����λ lowFlag upFlag
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

//�����ָ������
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
												//packFlash.digoutput[i].levelFlag=limit;//ͨ��levelFlag����λ lowFlag upFlag
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
//												//packFlash.digoutput[i].levelFlag=limit;//ͨ��levelFlag����λ lowFlag upFlag
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
												//packFlash.digoutput[i].levelFlag=limit;//ͨ��levelFlag����λ lowFlag upFlag
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
//��ӡ�Զ����Ƶ������б�
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
//ÿ���ϵ���߱�����ʱ���ȡ
void checkIndex()
{
		  if(firstReadFlag==false){
				firstReadFlag=true;
				pindex=findCtrlIndex();//ÿ���ϵ���ȡһ�μ���
		}
}
//ȷ������
void  autoCfgSure()
{
		sheet.autoctrl[pindex].workFlag=true;//����������Ч
		pindex =findCtrlIndex();//ȷ����ˢ������
		configFlag=false;
		inputIndex=0; 
		outputIndex=0;
		mod_printf("aotuctrl sure OK\n");
}
void delAutoCfg(int num)
{
		sheet.autoctrl[num-1].workFlag=false;//��������ʧЧ
		for(int j=0;j<CRTL_IN_NUM;j++)
			 sheet.autoctrl[num-1].input[j].flag=NULL;
		for(int j=0;j<CRTL_OUT_NUM;j++)
			 sheet.autoctrl[num-1].output[j].flag=NULL;
		if(configFlag==false)
				pindex =findCtrlIndex();//ɾ����ˢ������
		mod_printf("aotuctrl delete OK\n");
}
//�Զ��������뺯��  ��������������ȵ��ô˺�������
//�����߼�����ʱ������Ҫɾ�����������������豸��ѡ��Ϊ0���ɣ�
//����Ӻ���ǰ��ͼ�ν�����ʾ��һ��һ��ɾ�����߼������б��ָ��Ϊ�գ����������豸��cancel����Ҳ����ȡ��������������
static void autoctrl(char argc,char*argv[])
{

    checkIndex();
		if(rt_strcmp("list",argv[1])==0){
				printfCtrl();
				return;
		}//��ӡ
		if(rt_strcmp("delete",argv[1])==0){
			  
			  int num=atoi16(argv[2],10);
			  if( num==0){
						mod_printf("argv[2] should not 0\n");
				}
			  mod_printf("%d %d\n",num,pindex);
			  delAutoCfg(num);
				return;
		}//��ӡɾ����ť

	  if(pindex==255){
				mod_printf("ERR:aotuctrl is full,the total number is %d\n",CRTL_TOTAL_NUM);
			  return;
		}
		if(rt_strcmp("sure",argv[1])==0){//ȷ���������ð�ť  ȷ������ֻ��ɾ�� ����ȡ��
			  autoCfgSure();
				return;
		}
		int k;
		if(rt_strcmp("cancel",argv[1])==0){//ȡ���������ð�ť
			  for( k=0;k<CRTL_IN_NUM;k++){
						sheet.autoctrl[pindex].input[inputIndex].flag=NULL;
				}
				for( k=0;k<CRTL_OUT_NUM;k++){
						sheet.autoctrl[pindex].output[outputIndex].flag=NULL;
				}
				pindex =findCtrlIndex();//ȷ����ˢ������
			  configFlag=false;
				inputIndex=0; 
				outputIndex=0;
				mod_printf("aotuctrl cancel OK\n");
				return;
		}

		if(pindex>=CRTL_TOTAL_NUM){
				mod_printf("err:autoctrl totoal num is %d\n",CRTL_TOTAL_NUM);
		}
		autoctrlInputcfg(argv); //����ǲ�����������
		autoctrlOutputcfg(argv);//����ǲ����������
}
MSH_CMD_EXPORT(autoctrl,autoctrl config);

