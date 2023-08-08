#include "board.h"
static const char sign[]="[SDch4]";
//在 creatFolder/ID号创建的文件夹下边创建带有时间的txt文件
//创建txt文件 以utc时间戳为基准/3600*24 得到每天的时间戳作为txt文件的名称

static const char jiaWanText[]="methane time(ms)\r\n";


static uint32_t writetime=0;
void ch4SaveSD(char *id,char *data)
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
		strcpy(txtName,modbusName[CH4]);
		strcat(txtName,"/");
		strcat(txtName,id);
		strcat(txtName,"/");
		sprintf(timeSign,"%llu",(utcTime_s()/TXT_DATA_TIME)*TXT_DATA_TIME);//1小时创建一个新目录 为了清除1小时内的数字为0
		strcat(txtName,timeSign);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_WRITE);
		if(ret!=FR_OK){
		    ret=f_open(&fnew,txtName, FA_CREATE_NEW |FA_WRITE);//suqi
			  if(ret==FR_OK){
					ret=f_write(&fnew,jiaWanText,strlen(jiaWanText),&fnum);//写入name列表
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
void ch4DelEarlyTxt()
{
		for (int i = 0; i < CH4_485_NUM; i++)
		{		
			  if(sheet.ch4[i].workFlag==RT_TRUE){
						ReadAndDelEarlyDataTxt((char *)modbusName[CH4],sheet.ch4[i].ID);
				}
		}
}
