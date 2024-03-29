#include "board.h"
static const char sign[]="[SDHuanLiu]";
static const char huanliuText[]="earthCurA runCurA loadRatioA earthCurB runCurB loadRatioB earthCurC runCurC loadRatioC time(ms)\r\n";
#define  HUANLIU_DATA_LEN   200
//在 creatFolder/ID号创建的文件夹下边创建带有时间的txt文件
//创建txt文件 以utc时间戳为基准/3600*24 得到每天的时间戳作为txt文件的名称
void huanLiuTxtReadSD(char *id)
{
		char *txtName;
	  char *readData;
    int ret;
		if(gbSDExit==false){
				return;
		}
		txtName =rt_malloc(50);
		readData=rt_malloc(HUANLIU_DATA_LEN);
		strcpy(txtName,modbusName[CIRCULA]);
		strcat(txtName,"/");
		strcat(txtName,id);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_OPEN_EXISTING|FA_READ);//suqi
		int realLen=0;
		if(ret==FR_OK){
			  //printf("%ssizeof(fnew):%d\r\n",sign,fnew.fsize);
				memset(readData,0,HUANLIU_DATA_LEN);
				while(realLen<f_size(&fnew))
				{
					f_gets(readData,HUANLIU_DATA_LEN,&fnew);
					printf("%sread:%s\r\n",sign,readData);
					realLen=f_tell(&fnew);
					printf("%sreallen:%d\r\n",sign,realLen);
					f_lseek(&fnew, realLen);  
					memset(readData,0,HUANLIU_DATA_LEN);
				}
		}
		else{
				rt_kprintf("%sERR:f_open read %s,ret=%d\n",sign,txtName,ret);
		}
		f_close(&fnew);
		rt_free(txtName);
		txtName=NULL;
		rt_free(readData);
		readData=NULL;
		static int testtime=0;
		testtime++;
		while(testtime==3){
			rt_thread_delay(2000);
			rt_kprintf("%sRead end\n",sign);
		}
}
static uint32_t writetime=0;


//在 creatFolder/ID号创建的文件夹下边创建带有时间的txt文件
//创建txt文件 以utc时间戳为基准/3600*24 得到每天的时间戳作为txt文件的名称
void huanLiuTxtSaveSD(char *id,char *data)
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
		strcpy(txtName,modbusName[CIRCULA]);
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
						ret=f_write(&fnew,huanliuText,strlen(huanliuText),&fnum);//写入name列表
						//f_lseek(&fnew,f_size(&fnew));
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
void huanLiuDelEarlyTxt()
{
		for (int i = 0; i < CIRCULA_485_NUM; i++)
		{		
			  if(sheet.cirCula[i].workFlag==RT_TRUE){
						ReadAndDelEarlyDataTxt((char *)modbusName[CIRCULA],sheet.cirCula[i].ID);
				}
		}
}
