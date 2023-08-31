#include  "board.h"




extern  CIRCURStru  cirCurStru_p[CIRCULA_485_NUM];


static int dispHuanliuIndex=0;
static int dispHuanliuTotlNum=0;



//显示环流界面71.bmp的所有数据
void  dispHuanliuData()
{
	  dispHuanliuTotlNum=0;
	  uint8_t buf[MODBID_LEN+2]={0};
		for(int i=0;i<CIRCULA_485_NUM;i++){
			if(sheet.cirCula[i].workFlag==RT_TRUE){
				    dispHuanliuTotlNum++;
				}
		}
		if(dispHuanliuTotlNum==0){//没有工作的环流
				//显示总页数
				buf[0]=0;
				buf[1]=0;
				LCDWtite(DISP_DATA_HUANLIU_TOTALNUM_ADDR,buf,2);
				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_ID_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_EARTH_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_EARTH_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_EARTH_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_RUN_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_RUN_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_RUN_C_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_LOADR_A_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_LOADR_B_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU_LOADR_C_ADDR,buf,2);
		}
		else{
		
				//显示总页数
				buf[0]=0;
				buf[1]=dispHuanliuTotlNum;
				LCDWtite(DISP_DATA_HUANLIU_TOTALNUM_ADDR,buf,2);
				int j=0,k=0;
				for (int i = 0; i < CIRCULA_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.cirCula[i].workFlag==RT_TRUE){
							  if(j==dispHuanliuIndex){
									k=i;
								}
								j++;
						}
				}
				//显示idr
			  int len=0;
			  for(len=0;len<MODBID_LEN;len++){
						buf[len]=sheet.cirCula[k].ID[len];
					  if(buf[len]==0){
								break;
						}
				}
				buf[len++]	=0xff;  
				buf[len++]  =0xff; 
				LCDWtite(DISP_DATA_HUANLIU_ID_ADDR,buf,len);

					
				sprintf((char *)buf,"%0.2f",cirCurStru_p[k].circlCurA);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_EARTH_A_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",cirCurStru_p[k].circlCurB);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_EARTH_B_ADDR,buf,len);
				sprintf((char *)buf,"%0.2f",cirCurStru_p[k].circlCurC);
				len=strlen((char *)buf);
				buf[len++]=0xff;
				buf[len++]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_EARTH_C_ADDR,buf,len);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_RUN_A_ADDR,buf,3);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_RUN_B_ADDR,buf,3);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_RUN_C_ADDR,buf,3);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_LOADR_A_ADDR,buf,3);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_LOADR_B_ADDR,buf,3);
				buf[0]='0';
				buf[1]=0xff;
				buf[2]=0xff;
				LCDWtite(DISP_DATA_HUANLIU_LOADR_C_ADDR,buf,3);
		}

}

//上一组环流数据
void  nextHuanliuIndex()
{
	if(dispHuanliuTotlNum==0)
		return;
	else{
		dispHuanliuIndex++;
		if(dispHuanliuIndex>=dispHuanliuTotlNum)
				dispHuanliuIndex=0;
	}	
}
//下一组环流数据
void  lastHuanliuIndex()
{
	if(dispHuanliuTotlNum==0)
		return;
	else{
		if(dispHuanliuIndex==0)
			dispHuanliuIndex=dispHuanliuTotlNum-1;
		else
			dispHuanliuIndex--;
	}
}




void  dispHuanliuData_tjw()
{
	  uint8_t buf[MODBID_LEN+2]={0};
		dispHuanliuTotlNum=0;
		for(int i=0;i<CIRCULA_485_NUM;i++){
			if(sheet.cirCula[i].workFlag==RT_TRUE){
				    dispHuanliuTotlNum++;
				}
		}
		if(dispHuanliuTotlNum==0){//没有工作的环流

				//显示id
				buf[0]=0xff;
				buf[1]=0xff;
				LCDWtite(DISP_DATA_HUANLIU1_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU1_EARTH_A_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU1_EARTH_B_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU1_EARTH_C_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU1_CURRENT_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU2_ID_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU2_EARTH_A_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU2_EARTH_B_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU2_EARTH_C_TJW_ADDR,buf,2);
				LCDWtite(DISP_DATA_HUANLIU2_CURRENT_TJW_ADDR,buf,2);
		}
		else{

				int j=0,k1=0,k2=0;
				for (int i = 0; i < CIRCULA_485_NUM; i++)//查找真正的下标
				{		
						if(sheet.cirCula[i].workFlag==RT_TRUE){
								k1=i;
								int len=0;
								for(len=0;len<MODBID_LEN;len++){
										buf[len]=sheet.cirCula[k1].ID[len];
										if(buf[len]==0){
												break;
										}
								}
								buf[len++]	=0xff;  
								buf[len++]  =0xff; 
								LCDWtite(DISP_DATA_HUANLIU1_ID_TJW_ADDR,buf,len);
								sprintf((char *)buf,"%0.2f",cirCurStru_p[k1].circlCurA);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_HUANLIU1_EARTH_A_TJW_ADDR,buf,len);

								sprintf((char *)buf,"%0.2f",cirCurStru_p[k1].circlCurB);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_HUANLIU1_EARTH_B_TJW_ADDR,buf,len);

								sprintf((char *)buf,"%0.2f",cirCurStru_p[k1].circlCurC);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_HUANLIU1_EARTH_C_TJW_ADDR,buf,len);

								sprintf((char *)buf,"%0.2f",cirCurStru_p[k1].circlCurD);
								len=strlen((char *)buf);
								buf[len++]=0xff;
								buf[len++]=0xff;
								LCDWtite(DISP_DATA_HUANLIU1_CURRENT_TJW_ADDR,buf,len);
							  break;
						}
				}
				if(dispHuanliuTotlNum>=2){
					for (int i = k1+1; i < CIRCULA_485_NUM; i++)//查找真正的下标
					{		
							if(sheet.cirCula[i].workFlag==RT_TRUE){
									k2=i;
								  int len=0;
									for(len=0;len<MODBID_LEN;len++){
											buf[len]=sheet.cirCula[k2].ID[len];
											if(buf[len]==0){
													break;
											}
									}
									buf[len++]	=0xff;  
									buf[len++]  =0xff; 
									LCDWtite(DISP_DATA_HUANLIU2_ID_TJW_ADDR,buf,len);
									sprintf((char *)buf,"%0.2f",cirCurStru_p[k2].circlCurA);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_HUANLIU2_EARTH_A_TJW_ADDR,buf,len);

									sprintf((char *)buf,"%0.2f",cirCurStru_p[k2].circlCurB);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_HUANLIU2_EARTH_B_TJW_ADDR,buf,len);

									sprintf((char *)buf,"%0.2f",cirCurStru_p[k2].circlCurC);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_HUANLIU2_EARTH_C_TJW_ADDR,buf,len);

									sprintf((char *)buf,"%0.2f",cirCurStru_p[k2].circlCurD);
									len=strlen((char *)buf);
									buf[len++]=0xff;
									buf[len++]=0xff;
									LCDWtite(DISP_DATA_HUANLIU2_CURRENT_TJW_ADDR,buf,len);
									break;
							}
					}
				}
				//显示idr
	
				
				
			  

		}

}
