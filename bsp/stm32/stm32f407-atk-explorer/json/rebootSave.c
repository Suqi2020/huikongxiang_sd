#include  "board.h"
//������������ý���
const static char sign[]="[reboot]";







//��λmcu �Ȼظ���λ
uint16_t resetMcuResp(cJSON *Json)
{
		char* out = NULL;
		//��������
//		cJSON* Array = NULL;
		// ����JSON Object  
		cJSON* root = NULL;
		//cJSON* nodeobj = NULL;
		//cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// ����ڵ㣨��ֵ�ԣ�
//		bool rspFlag=false;
		cJSON_AddNumberToObject(root, "mid",respMid);
		cJSON_AddStringToObject(root, "packetType","SERVICES_ACU_REBOOT_RESP");
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		cJSON_AddStringToObject(root, "identifier","digital_output");
		cJSON_AddNumberToObject(root, "code",0);
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
		//���
	  int len=jsonPackMqttTcp(&out,&root,&sprinBuf,true);
	  return len;
}


//���������źŶ�ȡ��Ӧ
uint16_t resetDeviceResp(cJSON *Json,char *identify)
{
	
	  int devRstNum=0;
		char* out = NULL;
		//��������
//		cJSON* Array = NULL;
		// ����JSON Object  
		cJSON* root = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// ����ڵ㣨��ֵ�ԣ�
		bool rspFlag=false;
		cJSON_AddNumberToObject(root, "mid",respMid);
		cJSON_AddStringToObject(root, "packetType","SERVICES_DEV_REBOOT_RESP");
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		
		{
		cJSON   *arrayGet=cJSON_GetObjectItem(Json,"params");
		int arrayGet_size = cJSON_GetArraySize(arrayGet);
		for(int i=0;i<arrayGet_size;i++){
				cJSON *item=cJSON_GetArrayItem(arrayGet,i);
				cJSON  *devID =cJSON_GetObjectItem(item,"deviceId");
			#if 0
					if(rt_strcmp(identify,"3v3_output")==0){
						for(int j=0;j<V33O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
							if(0==rt_strcmp(packFlash.v33output[j].devID ,devID->valuestring)){//��
								rt_kprintf("%s3v3rst ok id[%s],port[%d]\n",sign,packFlash.v33output[j].devID,packFlash.v33output[j].port);
								if((++devRstNum)==arrayGet_size)
									break;
							}
						}
					}
					else if(rt_strcmp(identify,"5v_output")==0){
						for(int j=0;j<V5O_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
							if(0==rt_strcmp(packFlash.v5output[j].devID ,devID->valuestring)){//��
								rt_kprintf("%s5vrst ok id[%s],port[%d]\n",sign,packFlash.v5output[j].devID,packFlash.v5output[j].port);
								if((++devRstNum)==arrayGet_size)
									break;
							}
						}
					}
					else
#endif
					if(rt_strcmp(identify,"switch_output")==0){
						for(int j=0;j<SWITCH_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
							if(0==rt_strcmp(packFlash.switchoutput[j].devID ,devID->valuestring)){//��
								rt_kprintf("%sswitch out rst ok id[%s],port[%d]\n",sign,packFlash.switchoutput[j].devID,packFlash.switchoutput[j].port);
								if((++devRstNum)==arrayGet_size)
									break;
							}
						}
					}
					else 	if(rt_strcmp(identify,"digital_output")==0){
						for(int j=0;j<DO_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
							if(0==rt_strcmp(packFlash.digoutput[j].devID ,devID->valuestring)){//��
								rt_kprintf("%sdo rst ok id[%s],port[%d]\n",sign,packFlash.digoutput[j].devID,packFlash.digoutput[j].port);
								if((++devRstNum)==arrayGet_size)
									break;
							}
						}
					}
					else if(rt_strcmp(identify,"digital_input")==0){
						for(int j=0;j<DI_NUM;j++){//��һ�� �ҵ� GYNJLXSD000000499  ���
							if(0==rt_strcmp(packFlash.diginput[j].devID ,devID->valuestring)){//��
								rt_kprintf("%sdi rst ok id[%s],port[%d]\n",sign,packFlash.diginput[j].devID,packFlash.diginput[j].port);
								if((++devRstNum)==arrayGet_size)
									break;
							}
						}
					}
				
					else if(rt_strcmp(identify,"partial_discharge_monitor")==0){
							for(int j=0;j<PARTDISCHAG_485_NUM;j++){//�˶���û�����ù�
									if(0==rt_strcmp(sheet.partDischag[j].ID,devID->valuestring)){
										rt_kprintf("%spart dischag rst ok id[%s],port[%d]\n",sign,sheet.partDischag[j].ID,sheet.partDischag[j].useUartNum);
										if((++devRstNum)==arrayGet_size)
											break;
									}
							}
					}
					else if(rt_strcmp(identify,"grounding_current_monitor")==0){
							for(int j=0;j<CIRCULA_485_NUM;j++){//�˶���û�����ù�
									if(0==rt_strcmp(sheet.cirCula[j].ID,devID->valuestring)){
										rt_kprintf("%scirclua rst ok id[%s],port[%d]\n",sign,sheet.cirCula[j].ID,sheet.cirCula[j].useUartNum);
										if((++devRstNum)==arrayGet_size)
											break;
										}
								}
					}
					else if(rt_strcmp(identify,"water_level_monitor")==0){
							for(int j=0;j<WATERDEPTH_485_NUM;j++){//�˶���û�����ù�
									if(0==rt_strcmp(sheet.waterDepth[j].ID,devID->valuestring)){
										 rt_kprintf("%swaterDepth rst ok id[%s],port[%d]\n",sign,sheet.waterDepth[j].ID,sheet.waterDepth[j].useUartNum);
										 if((++devRstNum)==arrayGet_size)
											break;
									}
							}
					}
					else if(rt_strcmp(identify,"crackmeter_monitor")==0){
							for(int j=0;j<CRACKMETER_485_NUM;j++){//�˶���û�����ù�
									if(0==rt_strcmp(sheet.crackMeter[j].ID,devID->valuestring)){
										 rt_kprintf("%scrackMeter rst ok id[%s],port[%d]\n",sign,sheet.crackMeter[j].ID,sheet.crackMeter[j].useUartNum);
										 if((++devRstNum)==arrayGet_size)
											break;
									}
							}
					}
					else if(rt_strcmp(identify,"environment_mointor")==0){
								for(int j=0;j<CO_485_NUM;j++){//�˶���û�����ù�
										if(0==rt_strcmp(sheet.co[j].ID,devID->valuestring)){
												rt_kprintf("%sgas rst ok id[%s],port[%d]\n",sign,sheet.co[j].ID,sheet.co[j].useUartNum);
												 if((++devRstNum)==arrayGet_size)
															break;
										}
								}
					}
					else if(rt_strcmp(identify,"vibration_monitor")==0){
								for(int j=0;j<THREEAXIS_485_NUM;j++){//�˶���û�����ù�
										if(0==rt_strcmp(sheet.threeAxiss[j].ID,devID->valuestring)){
											 rt_kprintf("%sthreeAxiss rst ok id[%s],port[%d]\n",sign,sheet.threeAxiss[j].ID,sheet.threeAxiss[j].useUartNum);
											 if((++devRstNum)==arrayGet_size)
														break;
										}
								}
					}
					else if(rt_strcmp(identify,"settlement_monitor")==0){
								for(int j=0;j<PRESSSETTL_485_NUM;j++){//�˶���û�����ù�
										if(0==rt_strcmp(sheet.pressSetl[j].ID,devID->valuestring)){
													rt_kprintf("%spressSetl rst ok id[%s],port[%d]\n",sign,sheet.pressSetl[j].ID,sheet.pressSetl[j].useUartNum);
												 if((++devRstNum)==arrayGet_size)
															break;
										}
								}
					}
					else if(rt_strcmp(identify,"temperature_and_humidity_monitor")==0){
	#ifndef     ANA_MASK
								for(int j=0;j<ANALOG_NUM;j++){//�˶���û�����ù�
										if(0==rt_strcmp(sheet.analog[j].ID,devID->valuestring)){
												rt_kprintf("%sanalog rst ok id[%s],port[%d]\n",sign,sheet.analog[j].ID,sheet.analog[j].port);
												++devRstNum;
										}
							 }
	#endif
								for(int j=0;j<TEMPHUM_485_NUM;j++){//�˶���û�����ù�
										if(0==rt_strcmp(sheet.tempHum[j].ID,devID->valuestring)){
												rt_kprintf("%sanalog rst ok id[%s],port[%d]\n",sign,sheet.tempHum[j].ID,sheet.tempHum[j].useUartNum);
												 if((++devRstNum)==arrayGet_size)
															break;
										}
								}
					}
					if((devRstNum)>=arrayGet_size){
						rspFlag=true;
						break;
					}
			}
		}
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		cJSON_AddStringToObject(root, "identifier","digital_output");
	  if(rspFlag==false)
			cJSON_AddNumberToObject(root, "code",1);
		else
			cJSON_AddNumberToObject(root, "code",0);
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
		//���
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,true);
	  return len;
}






//��λmcu �Ȼظ���λ
uint16_t saveMcuResp()
{
	
		char* out = NULL;
		//��������
//		cJSON* Array = NULL;
		// ����JSON Object  
		cJSON* root = NULL;
		//cJSON* nodeobj = NULL;
		//cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// ����ڵ㣨��ֵ�ԣ�
//		bool rspFlag=false;

		cJSON_AddNumberToObject(root, "mid",respMid);
		cJSON_AddStringToObject(root, "packetType","SERVICES_SAVE_RESP");
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		cJSON_AddNumberToObject(root, "code",0);

		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  

		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,true);
	  return len;

}


