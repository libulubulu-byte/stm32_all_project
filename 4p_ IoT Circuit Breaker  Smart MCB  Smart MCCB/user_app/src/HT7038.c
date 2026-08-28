#include "main.h"
#include "stm32f0xx_it.h"
#include "HT7038bu.h"
#include "bord.h"
#include "proc.h"
#include "prjlib.h"
#include "user_comm.h"
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>
#include <TypeDef.h>
#include<math.h>
extern SPI_HandleTypeDef hspi1;
#define	LEN_CaliPara	50u				//校表参数
static	INT16U	g_CaliPara[LEN_CaliPara/2]		__no_init;					//计量芯片参数
/*******************************************************************************
功能描述：	开启电流采样通道
输入参数：
返回参数：
函数说明：	用于掉电时作全失压,掉电纪录判断
*******************************************************************************/
void Open_Current(void)
{
	uint32_t ADC_I_A=0, ADC_I_B=0, ADC_I_C=0,ADC_U_ABC=0;uint16_t i=0;
	HAL_GPIO_WritePin(HT7038_RET_GPIO_Port, HT7038_RET_Pin, GPIO_PIN_RESET);
//	Open_RelayLed();
//	Reset_Meter();															//硬复位ATT702x
//	Read_CaliPara();														//读取校表参数
//	HAL_Delay(5);
	TS_DELAY(200);
	HAL_GPIO_WritePin(HT7038_RET_GPIO_Port, HT7038_RET_Pin, GPIO_PIN_SET);
	
	write_HT7038bu(HT7038_REG_RST,0x00D3); 
	TS_DELAY(10);
//	write_HT7038bu(w_ClrCali,0x0000); 

	write_HT7038bu(w_EnWrCali,	0x00005A);										//开启写校表参数
		for(i=0;i<3;i++)
	{
		write_HT7038bu(TAB_CaliPara[i].addr,	TAB_CaliPara[i].set);	
	}
	write_HT7038bu(w_EMCfg,0x0000);
//	write_HT7038bu(w_ModeCfg,0xB8FF);
//	write_HT7038bu(w_EMUCfg,	TAB_CaliPara[2].set);	
//		write_HT7038bu(w_ModuleCfg,	0x3427);	
	//0x00 表示1倍增益 0x01 表示2倍增益	 0x02 表示8倍增益	 0x03 表示16倍增益	
	ADC_I_A=0x00;
	ADC_I_B=0x00;
	ADC_I_C=0x00;
	ADC_U_ABC=0x00;
	write_HT7038bu(2,(ADC_I_A<<2)|(ADC_I_B<<4)|(ADC_I_C<<6)|(ADC_U_ABC<<8)); 
		write_HT7038bu(w_UgainA,0xdb0b);
			write_HT7038bu(w_UgainB,0xdb0f);
				write_HT7038bu(w_UgainC,0xdb25);
	write_HT7038bu(w_IgainA,0x63EF);
		write_HT7038bu(w_IgainB,0x639A);
			write_HT7038bu(w_IgainC,0x6345);
			write_HT7038bu(w_HFConst,0x003d);
					write_HT7038bu(w_EMCfg,	0x0022);
				write_HT7038bu(0x0d,0xf05);
				write_HT7038bu(0x0E,0xf05);
				write_HT7038bu(0x0F,0xf05);
				write_HT7038bu(0x10,0xf05);
				write_HT7038bu(0x11,0xf05);
				write_HT7038bu(0x12,0xf05);
					write_HT7038bu(w_PoffsetAL,0);
					write_HT7038bu(w_PoffsetBL,0);
					write_HT7038bu(w_PoffsetCL,0);
					
//					write_HT7038bu(w_PgainA,0xa0b);
//					write_HT7038bu(w_PgainB,0xa0b);
//					write_HT7038bu(w_PgainC,0xa0b);
//					write_HT7038bu(w_QgainA,0xa0b);
//					write_HT7038bu(w_QgainB,0xa0b);
//					write_HT7038bu(w_QgainC,0xa0b);
//					write_HT7038bu(w_SgainA,0xa0b);
//					write_HT7038bu(w_SgainB,0xa0b);
//					write_HT7038bu(w_SgainC,0xa0b);
					write_HT7038bu(w_PgainA,0xffd3);
					write_HT7038bu(w_PgainB,0xffd3);
					write_HT7038bu(w_PgainC,0xffd3);
					write_HT7038bu(w_QgainA,0xffd3);
					write_HT7038bu(w_QgainB,0xffd3);
					write_HT7038bu(w_QgainC,0xffd3);
					write_HT7038bu(w_SgainA,0xffd3);
					write_HT7038bu(w_SgainB,0xffd3);
					write_HT7038bu(w_SgainC,0xffd3);

	write_HT7038bu(w_EnWrCali,	~0x00005A);										//禁止写校表参数
	HAL_Delay(250);														//3.6个采样周器,(1s/14.4Hz=69.4ms)
}
void init_HT7038bu(void)
{	
	Open_Current();
//	write_HT7038bu(HT7038_REG_RST,0x00D3);  	//软件复位寄存器
//	TS_DELAY(200);
//	write_HT7038bu(HT7038_REG_WP, 0xBC);		//写保护寄存器
//	write_HT7038bu(HT7038_REG_EMU_CFG,(uint32_t)0x2004); //脉冲尾数采集一次清一次

//	write_HT7038bu(HT7038_REG_FREQ_CFG,(uint32_t)0x0088);   //1MHz, 3.8Hz有效值更新，0.976KHz波形采样，CFP=90ms  
//	write_HT7038bu(HT7038_REG_ADC_EN,0x0007);     //U,I1,I2的AD全打开   
//       
//    write_HT7038bu(HT7038_REG_WP, 0xA6);
//    write_HT7038bu(HT7038_REG_ADC_G,0x0000);	//ADC通道增益
//    write_HT7038bu(HT7038_REG_GP2,0XFE33);	//通道2的有功功率校正
//    write_HT7038bu(HT7038_REG_GQ2,0XFE33);
//    write_HT7038bu(HT7038_REG_GS2,0XFE33);
//    write_HT7038bu(HT7038_REG_HFCONST,0x5E);	//输出脉冲频率设置
//    write_HT7038bu(HT7038_REG_GPHASE2,0X37B);
}



uint8_t read_HT7038buid(uint8_t addr,uint32_t *data){
	uint8_t wr_buf[4];
	wr_buf[0]= addr | HT7038_CMD_READ;
	wr_buf[1]=HT7038_REG_DUMMY;
	wr_buf[2]=HT7038_REG_DUMMY;
	wr_buf[3]=HT7038_REG_DUMMY;	
	SPI_CS_CLEAR();
        HAL_SPI_Transmit(&hspi1,wr_buf,1,10);
        HAL_SPI_Receive(&hspi1, (uint8_t *)data, 3,20);
	SPI_CS_SET();
        return 0;
}

/*---------------------------------------------------------------------------------
* Name:   write_HT7038bu
*----------------------------------------------------------------------------------
* info:  向HT7038指定的地址写三字节内容
*
* In:    addr, HT7038寄存器地址;  HT7038_REG_CHIPID HT7038_REG_DEVID
* Out:
* Ret:   0, 成功 0xff,超时
-----------------------------------------------------------------------------------*/
uint8_t write_HT7038bu(uint8_t addr, uint32_t dt)
{
        uint8_t wr_buf[4];
        uint32_t dt_temp = dt;
	wr_buf[0] = addr | HT7038_CMD_WRITE;
	wr_buf[1] = (uint8_t)(dt_temp >> 16);
	wr_buf[2] = (uint8_t)(dt_temp >> 8);
	wr_buf[3] = (uint8_t)dt_temp;	
	SPI_CS_CLEAR();
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&wr_buf,4,10);
	SPI_CS_SET();

	#if 0
	static UINT_32_8_T wr_dt;
	uint8_t i; 
	uint32_t wait_cnt = 0;
	
	wr_dt.dt = (dt&0x00ffffffu) | (((uint32_t)(addr|HT7038_CMD_WRITE))<<24u);
	//SPI_MasterTransferBlocking(HT7038_SPI, &pxdata);
	for(i=0;i<4;i++)
	{
		do
		{
			wait_cnt++;			
		}
		while ((!SPI_IsSPTEF(HT7038_SPI)) && wait_cnt <= TIMEOUT_DEAD_LINE);
		
		if(wait_cnt < TIMEOUT_DEAD_LINE)
		{
			SPI_WriteData(HT7038_SPI, wr_dt.buf[3 - i]);
			wait_cnt=0;
		}
		else
		{
			return 0xff;
		}
	}
	#endif
	return 0;
}

uint8_t read_HT7038bu(uint8_t addr,uint8_t *rd_buff)
{
	uint8_t wr_buf[4];
	wr_buf[0]= addr | HT7038_CMD_READ;
	wr_buf[1]=HT7038_REG_DUMMY;
	wr_buf[2]=HT7038_REG_DUMMY;
	wr_buf[3]=HT7038_REG_DUMMY;	
	SPI_CS_CLEAR();
    TS_DELAY(5);
    HAL_SPI_Transmit(&hspi1,wr_buf,1,10);
    HAL_SPI_Receive(&hspi1, rd_buff, 3,20);
	SPI_CS_SET();
	return 0;
}

uint8_t read_HT7038bu32(uint8_t addr,uint8_t *rd_buff)
{
	uint8_t wr_buf[3];
	wr_buf[0]= addr | HT7038_CMD_READ;
	wr_buf[1]=HT7038_REG_DUMMY;
	wr_buf[2]=HT7038_REG_DUMMY;	
	SPI_CS_CLEAR();
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&wr_buf,rd_buff,3,10);
	SPI_CS_SET();
    //delayms(5);
	#if 0
	for(i=0;i<4;i++)
	{
		do
		{
			//wait_cnt++;			
		}
		while ((!SPI_IsSPTEF(HT7038_SPI))); // && wait_cnt <= TIMEOUT_DEAD_LINE
		
		if(wait_cnt < TIMEOUT_DEAD_LINE)
		{
			SPI_WriteData(HT7038_SPI, wr_buf[i]);
			wait_cnt=0;
            
		}
		else
		{
			return 0xff;
		}
        
		do
		{
			//wait_cnt++;			
		}
		while ((!SPI_IsSPRF(HT7038_SPI))); // && wait_cnt <= TIMEOUT_DEAD_LINE

		if(wait_cnt<TIMEOUT_DEAD_LINE)
		{
			rd_buff[i] = (uint8_t)SPI_ReadData(HT7038_SPI);
			wait_cnt=0;
		}
		else
		{
			return 0xff;
		}
	}
	#endif
    //memcpy(rd_buff, rd_bufftemp, 4);
	return 0;
}
uint32_t tmp_dyA=0,tmp_dyB=0,tmp_dyC=0;
uint32_t tmp_dIA=0,tmp_dIB=0,tmp_dIC=0;
uint8_t read_rms(uint8_t addr,uint32_t *vi, float k)
{
	static uint8_t ret=0;
   uint8_t rd_buf[20];
	static uint32_t tmp_dt=0,dyy=0;
	static double V_tmp=0;
	uint8_t i=0,rd_cnt=0;		double	tmp_df=0.0;
	tmp_dt=0;
	for(i=0;i<20;i++)
	{
		rd_buf[i]=0;
	}
	switch(addr)
	{
		case HT7038_REGA_V_RMS:
		case HT7038_REGB_V_RMS:
		case HT7038_REGC_V_RMS:
		case HT7038_REGsum_V_RMS:
//		case HT7038_REG_I1_RMS:
		case HT7038_REG_IA_RMS:
		case HT7038_REG_IB_RMS:
		case HT7038_REG_IC_RMS:
		case HT7038_REG_IABC_RMS:
		do
		{
			for(i=0;i<20;i++);
			ret =read_HT7038bu(addr,rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<3));
		
		if(ret==0)
		{
			tmp_dt = (((uint32_t)rd_buf[0])<<16)|(((uint32_t)rd_buf[1])<<8)|((uint32_t)rd_buf[2]);
            
      //       double tempd = (double)tmp_dt - 400;
      //       //PRINTF("rms:%u\r",tmp_dt);
			// *vi = (uint32_t)(tempd / 100000 * k);  
       if(tmp_dt==0xFFFFFF)
				{
					tmp_dt=0;
				}
			if(addr==HT7038_REGsum_V_RMS)
			{
				
				V_tmp=((float)tmp_dt*k/pow(2,12));
				*vi=V_tmp;
				
			}
			if(addr==HT7038_REGA_V_RMS||addr==HT7038_REGB_V_RMS||addr==HT7038_REGC_V_RMS)
			{
				
				V_tmp=((float)tmp_dt*k/pow(2,13));
//				if(addr==HT7038_REGA_V_RMS)
//				{
//					tmp_dyA=tmp_dt;
//				}
//				if(addr==HT7038_REGB_V_RMS)
//				{
//					tmp_dyB=tmp_dt;
//				}
//				if(addr==HT7038_REGC_V_RMS)
//				{
//					tmp_dyC=tmp_dt;
//				}
//				
//				if(V_tmp>=226&&V_tmp<255)
//				{
//					*vi=1.7633*V_tmp-170.46;
//					
////					*vi =(uint32_t)((1949.4*pow(V_tmp,2)/pow(10,4) -247.55*pow(V_tmp,3)/pow(10,6)- 4983.1*V_tmp/pow(10,2) + 4394.9));
//				}else if(V_tmp>=255)
//				{
//				
//						*vi=(uint32_t)(-0.0415*V_tmp*V_tmp+22.915*V_tmp-2866.5);
//				}
//				else{
//                        
//														
													*vi=V_tmp;
//													if(*vi<=8)
//													{
//														 *vi =0;
//													}
//				}
				
			} 

//			else if(addr==HT7038_REG_I1_RMS)
//			{      
//                          if((float)tmp_dt* k >=1.1){
//                             *vi = (uint32_t)((float)tmp_dt* k ) -1;	//算实际人第一通道电流，k值校表获得，详见7053数据手册
//														if(tmp_dt==0)
//														{
//															*vi=0;
//														}
//                          }else{
//                             *vi = (uint32_t)((float)tmp_dt* k );
//                          }				
//			}
			else if(addr==HT7038_REG_IA_RMS||addr==HT7038_REG_IB_RMS||addr==HT7038_REG_IC_RMS||addr==HT7038_REG_IABC_RMS)
			{
//				if(tmp_dt<490)
//				{
//					tmp_dt=0;
//				}
				*vi = (uint32_t)((float)tmp_dt*k*100 /pow(2,13)) ;	//算实际人第二通道电流，k值校表获得，详见7053数据手册
				
//				if(addr==HT7038_REG_IA_RMS)
//				{
//					tmp_dIA=tmp_dt;
//				}
//				if(addr==HT7038_REG_IB_RMS)
//				{
//					tmp_dIB=tmp_dt;
//				}
//				if(addr==HT7038_REG_IC_RMS)
//				{
//					tmp_dIC=tmp_dt;
//				}
//				if(addr==HT7038_REG_IABC_RMS)
//				{
//					
//					*vi = (uint32_t)((float)tmp_dt*k*10 /pow(2,8)) ;
//					
//				}
//				if(addr==HT7038_REG_IABC_RMS)
//				{
//					*vi = (uint32_t)((float)tmp_dt*k*10);
//				}
				if(*vi<=12)
				{
					*vi=0;
				
				}
			}
			
		}
		break;
		
		default:
		ret=0xff;
	}
	return ret;
}

uint8_t read_freq(uint16_t *freq)
{
	uint8_t ret=0;
	static  uint8_t rd_buf[4];
	uint16_t tmp_dt=0;
	uint8_t i=0,rd_cnt=0;		
	do
	{
		for(i=0;i<20;i++);
		ret = read_HT7038bu(HT7038_REG_FREQ,rd_buf);
		rd_cnt++;
	}
	while((ret==0xff) && (rd_cnt<3));
	
	if(ret==0)
	{
		tmp_dt=(((uint16_t)rd_buf[2])<<8)|((uint16_t)rd_buf[3]);
		*freq = 100000000 / (tmp_dt*2) ; //扩大10，去掉小数点
	}
	return ret;
}
uint32_t tmp_dpA=0,tmp_dpB=0,tmp_dpC=0,tmp_dpO=0;
/*---------------------------------------------------------------------------------
* Name:   read_power
*----------------------------------------------------------------------------------
* info:  从HT7038读取P,Q,S
*
* In:    addr, HT7038寄存器地址,仅限通道1的功率寄存器 ;
* Out:   power,返回计算出的参数值 单位W
* Ret:   0, 成功 0xff,错误
-----------------------------------------------------------------------------------*/
uint8_t read_power(uint8_t addr,int16_t *power, float k)
{
	uint8_t ret=0;
	uint8_t rd_buf[4];
	static uint32_t tmp_dt=0;
	uint32_t real_dt=0;
	uint8_t i=0,rd_cnt=0;

	
	switch(addr)
	{
//		case HT7038_REG_POW_P2:
//		case HT7038_REG_POW_Q2:
		case HT7038_REG_POW_AP:
		case HT7038_REG_POW_AQ:
		case HT7038_REG_POW_AS:
		case HT7038_REG_POW_BP:
		case HT7038_REG_POW_BQ:
		case HT7038_REG_POW_BS:
		case HT7038_REG_POW_CP:
		case HT7038_REG_POW_CQ:
		case HT7038_REG_POW_CS:
		case HT7038_REG_POW_ABCP:
		case HT7038_REG_POW_ABCS:
		case HT7038_REG_POW_ABCQ:
		do
		{
			for(i=0;i<20;i++);
			ret = read_HT7038bu(addr,rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<3));
		
		if(ret==0)
		{
			tmp_dt=(((uint32_t)rd_buf[0])<<16)|(((uint32_t)rd_buf[1])<<8)|((uint32_t)rd_buf[2]);
            //PRINTF("power:%u\r",tmp_dt);
			if(tmp_dt>pow(2,23))
			{
				real_dt =tmp_dt-pow(2,24);
			}
			else
			{
				real_dt=(int32_t)tmp_dt;
			}
			*power = (uint32_t)(real_dt  * k );
//			if(addr==HT7038_REG_POW_AP)
//				{
//					tmp_dpA=tmp_dt;
//				}
//				if(addr==HT7038_REG_POW_BP)
//				{
//					tmp_dpB=tmp_dt;
//				}
//				if(addr==HT7038_REG_POW_CP)
//				{
//					tmp_dpC=tmp_dt;
//				}
			if (addr==HT7038_REG_POW_ABCP||addr==HT7038_REG_POW_ABCS||addr==HT7038_REG_POW_ABCQ)
			{
				*power = (uint32_t)(real_dt  *2* k );
			}
//			if (addr==HT7038_REG_POW_ABCP)
//			{
//			tmp_dpO=tmp_dt;
//			}
//			if(addr==HT7038_REG_POW_ABCP||addr==HT7038_REG_POW_ABCS)
//			{
//				*power = (uint32_t)(real_dt  *2* k );
//			}
		}
		break;
		
		default:
		ret=0xff;
	}
	return ret;	
}
double jiaodu=0;
/*---------------------------------------------------------------------------------
* Name:   read_power
*----------------------------------------------------------------------------------
* info:  从HT7038读取P,Q,S
*
* In:    addr, HT7038寄存器地址,仅限通道1的功率寄存器 ;
* Out:   power,返回计算出的参数值 单位W
* Ret:   0, 成功 0xff,错误
-----------------------------------------------------------------------------------*/
uint8_t read_powerfact(uint8_t addr,uint16_t *power, float k)
{
	uint8_t ret=0;
	uint8_t rd_buf[4];
	static uint32_t tmp_dt=0;
	uint32_t real_dt=0;
	uint8_t i=0,rd_cnt=0;

	
	switch(addr)
	{
//		case HT7038_REG_POW_P2:
//		case HT7038_REG_POW_Q2:
		case r_Pfa:
		case r_Pfb:
		case r_Pfc:
		case r_Pft:
		case r_Pga:
		case r_Pgb:
		case r_Pgc:
		case r_YUaUb:
		case r_YUaUc:
		case r_YUbUc:
		do
		{
			for(i=0;i<20;i++);
			ret = read_HT7038bu(addr,rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<3));
		
		if(ret==0)
		{
			tmp_dt=(((uint32_t)rd_buf[0])<<16)|(((uint32_t)rd_buf[1])<<8)|((uint32_t)rd_buf[2]);
			if(tmp_dt>pow(2,23))
			{
				tmp_dt=tmp_dt-pow(2,23);
			
			}
			
			if(addr==r_YUaUb||addr==r_YUaUc||addr==r_YUbUc)
			{
				jiaodu=(((double)((tmp_dt)*18000/pow(2,20))/10)/10);
				//*power = (uint32_t)(cos(37)*100);//((((double)((tmp_dt)*18000/pow(2,20)))/100)); 
				if((uint32_t)((cos(jiaodu*3.1415926/180))*1000)>750&&(uint32_t)((cos(jiaodu*3.1415926/180))*1000)<850)
				{
						*power = 1762;
				
				}else
				{
					*power=1000;
				
				};
			}else
			{
			*power = (uint32_t)(tmp_dt*1000  * k /pow(2,23));
			
			}
		}
		break;
		
		default:
		ret=0xff;
	}
	return ret;	
}
//uint16_t energy_SUM=0;
/*---------------------------------------------------------------------------------
* Name:   read_energy
*----------------------------------------------------------------------------------
* info:  从HT7038读取P,Q,S
*
* In:    addr, HT7038寄存器地址,仅限电能寄存器 ;
* Out:	  energy,返回计算出的参数值
* Ret:   0, 成功 0xff,错误
-----------------------------------------------------------------------------------*/
uint8_t read_energy(uint8_t addr,uint16_t *energy, uint16_t ec)
{
	uint8_t ret=0;
	uint8_t rd_buf[4];
	uint32_t tmp_dt=0;
	uint8_t i=0,rd_cnt=0;
	
	switch(addr)
	{
		case HT7038_SUM_ENE_P:
		case HT7038_SUM_ENE_Q:
		case HT7038_SUM_ENE_S:
		case r_Epa:
		case r_Epb:
		case r_Epc:
		do
		{
			for(i=0;i<30;i++);
			ret = read_HT7038bu(addr, rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<1));
		
		if(ret==0)
		{
			tmp_dt = ((uint32_t)rd_buf[0]<<16)|((uint32_t)rd_buf[1]<<8)|((uint32_t)rd_buf[2]);
//			energy_SUM=tmp_dt;
			//PRINTF("energytmp_dt:%u\r",tmp_dt);
			*energy =tmp_dt; 
            //PRINTF("energy:%u\r",*energy);
		}
		break;
		
		default:
		ret=0xff;
	}
	return ret;	
}

//校表其实就是写如下的寄存器，校表过程需人工干预
//各寄存器值的计算方法参见数据手册P58页
void adjust_cali_para(uint8_t addr,uint16_t dt)
{
	uint8_t tmp=0;	
	switch(addr){
		case HT7038_REG_GP1:
		case HT7038_REG_GQ1:
		case HT7038_REG_GS1:
//		case HT7038_REG_P1_OFT_H:
//		case HT7038_REG_P1_OFT_L:
		case HT7038_REG_HFCONST:
//		case HT7038_REG_GPHASE1:
			write_HT7038bu(HT7038_REG_WP, 0xA6);
//			if((addr==HT7038_REG_GP1)||(addr==HT7038_REG_GQ1)||(addr==HT7038_REG_GS1))
//			{
//				write_HT7038bu(HT7038_REG_GP1,dt);
//				write_HT7038bu(HT7038_REG_GQ1,dt);
//				write_HT7038bu(HT7038_REG_GS1,dt);
//			}
//			else if((addr==HT7038_REG_P1_OFT_H)||(addr==HT7038_REG_P1_OFT_L))
//			{
//				tmp = (uint8_t)((dt>>8)&0xff);
//				write_HT7038bu(HT7038_REG_P1_OFT_H,tmp);
//				tmp = (uint8_t)(dt&0xff);
//				write_HT7038bu(HT7038_REG_P1_OFT_L,tmp);
//			}
//			else
//			{
//				write_HT7038bu(addr,dt);
//			}
//			write_HT7038bu(HT7038_REG_WP, 0);
			break;
		default:;
	}
}
