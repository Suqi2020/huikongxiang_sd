#include  "board.h"




extern  CIRCURStru  cirCurStru_p[CIRCULA_485_NUM];


static int dispJufangIndex=0;
static int dispJufangTotlNum=0;

extern partDischargeStru partDiscStru_p[PARTDISCHAG_485_NUM];

//��ʾ��������71.bmp����������
void  dispJufangData()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispJufangTotlNum=0;
		for(int i=0;i<PARTDISCHAG_485_NUM;i++){
			 if(sheet.partDischag[i].workFlag==RT_TRUE){
				    dispJufangTotlNum++;
				}
		}
		if(dispJufangTotlNum==0){//û�й����Ļ���
				//��ʾ��ҳ��
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				//��ʾid
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,2);
			


		}
		else{
				//��ʾ��ҳ��
				buf[0]=0;
				buf[1]=dispJufangTotlNum;
				LCDWtite(DISP_DATA_JUFANG_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < PARTDISCHAG_485_NUM; i++)//�����������±�
				{		
						if(sheet.partDischag[i].workFlag==RT_TRUE){
							  if(j==dispJufangIndex){
									k=i;
								}
								j++;
						}
				}
				//��ʾidr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.partDischag[k].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_JUFANG_ID_ADDR,buf,len);
				
				sprintf((char *)buf,"%u",partDiscStru_p[k].amplitudeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].amplitudeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].amplitudeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_PD_C_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].freqA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].freqB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].freqC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_FREQ_C_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].dischargeA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_A_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].dischargeB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_B_ADDR,buf,len);
				sprintf((char *)buf,"%u",partDiscStru_p[k].dischargeC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_JUFANG_DISCHG_C_ADDR,buf,len);
		}

}




//��һ�黷������
void  nextJufangIndex()
{
	if(dispJufangTotlNum==0)
		return;
	else{
		dispJufangIndex++;
		if(dispJufangIndex>=dispJufangTotlNum)
				dispJufangIndex=0;
	}	
}
//��һ�黷������
void  lastJufangIndex()
{
	if(dispJufangTotlNum==0)
		return;
	else{
		if(dispJufangIndex==0)
			dispJufangIndex=dispJufangTotlNum-1;
		else
			dispJufangIndex--;
	}
}
