#include "board.h"
static const char sign[]="[SDh2s]";
//�� creatFolder/ID�Ŵ������ļ����±ߴ�������ʱ���txt�ļ�
//����txt�ļ� ��utcʱ���Ϊ��׼/3600*24 �õ�ÿ���ʱ�����Ϊtxt�ļ�������

static const char h2sText[]="temp distance time(ms)\r\n";


static uint32_t writetime=0;
void h2sSaveSD(char *id,char *data)
{
		if(gbSDExit==false){
				return;
		}
	  extern rt_mutex_t sdWrite_mutex;
		rt_mutex_take(sdWrite_mutex,RT_WAITING_FOREVER);
		char *txtName;
		uint32_t fnum;
    int ret;
		char timeSign[12];
		txtName =rt_malloc(50);
		strcpy(txtName,modbusName[H2S]);
		strcat(txtName,"/");
		strcat(txtName,id);
		strcat(txtName,"/");
		sprintf(timeSign,"%llu",(utcTime_s()/TXT_DATA_TIME)*TXT_DATA_TIME);//1Сʱ����һ����Ŀ¼ Ϊ�����1Сʱ�ڵ�����Ϊ0
		strcat(txtName,timeSign);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_WRITE);
		if(ret!=FR_OK){
		    ret=f_open(&fnew,txtName, FA_CREATE_NEW |FA_WRITE);//suqi
			  if(ret==FR_OK){
					ret=f_write(&fnew,h2sText,strlen(h2sText),&fnum);//д��name�б�
//					f_lseek(&fnew,f_size(&fnew));
				}
		}
		if((ret==FR_OK)||(ret==FR_EXIST)){
				f_lseek(&fnew,f_size(&fnew));
				res_sd=f_write(&fnew,data,strlen(data),&fnum);
				if(res_sd!=FR_OK){
						rt_kprintf("%sERR:f_write %s,res_sd=%d\n",sign,txtName,res_sd);
						rt_thread_delay(3000);
				}
				else {
						writetime++;
						rt_kprintf("%sOK:f_write %s total times[%d] realy times[%d]\n",sign,txtName,writetime,(((writetime-1)/3)+1));
						rt_kprintf("%s %s\n",sign,txtName);
				}
		}
		else{
				rt_kprintf("%sERR:f_open  write %s,ret=%d\n",sign,txtName,ret);
		}
		f_close(&fnew);
		rt_free(txtName);
		txtName=NULL;
    rt_mutex_release(sdWrite_mutex);
}


//
void h2sDelEarlyTxt()
{
		for (int i = 0; i < H2S_485_NUM; i++)
		{		
			  if(sheet.h2s[i].workFlag==RT_TRUE){
						ReadAndDelEarlyDataTxt((char *)modbusName[H2S],sheet.h2s[i].ID);
				}
		}
}