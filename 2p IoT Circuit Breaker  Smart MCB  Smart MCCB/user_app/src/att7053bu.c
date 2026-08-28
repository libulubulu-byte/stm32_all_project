#include "main.h"
#include "stm32f0xx_it.h"
#include "att7053bu.h"
#include "bord.h"
#include "proc.h"
#include "prjlib.h"
#include "user_comm.h"
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;

void init_att7053bu(void)
{	
	write_att7053bu(ATT7053_REG_RST,0x0055);  	//软件复位寄存器
	TS_DELAY(200);
	write_att7053bu(ATT7053_REG_WP, 0xBC);		//写保护寄存器
	write_att7053bu(ATT7053_REG_EMU_CFG,(uint32_t)0x2004); //脉冲尾数采集一次清一次

	write_att7053bu(ATT7053_REG_FREQ_CFG,(uint32_t)0x0088);   //1MHz, 3.8Hz有效值更新，0.976KHz波形采样，CFP=90ms  
	write_att7053bu(ATT7053_REG_ADC_EN,0x0007);     //U,I1,I2的AD全打开   
       
    write_att7053bu(ATT7053_REG_WP, 0xA6);
    write_att7053bu(ATT7053_REG_ADC_G,0x0000);	//ADC通道增益
    write_att7053bu(ATT7053_REG_GP2,0XFE33);	//通道2的有功功率校正
    write_att7053bu(ATT7053_REG_GQ2,0XFE33);
    write_att7053bu(ATT7053_REG_GS2,0XFE33);
    write_att7053bu(ATT7053_REG_HFCONST,0x5E);	//输出脉冲频率设置
    write_att7053bu(ATT7053_REG_GPHASE2,0X37B);
}



uint8_t read_att7053buid(uint8_t addr,uint32_t *data){
	uint8_t wr_buf[4];
	wr_buf[0]= addr | ATT7053_CMD_READ;
	wr_buf[1]=ATT7053_REG_DUMMY;
	wr_buf[2]=ATT7053_REG_DUMMY;
	wr_buf[3]=ATT7053_REG_DUMMY;	
	SPI_CS_CLEAR();
        HAL_SPI_Transmit(&hspi1,wr_buf,1,10);
        HAL_SPI_Receive(&hspi1, (uint8_t *)data, 3,20);
	SPI_CS_SET();
        return 0;
}

/*---------------------------------------------------------------------------------
* Name:   write_att7053bu
*----------------------------------------------------------------------------------
* info:  向att7053指定的地址写三字节内容
*
* In:    addr, att7053寄存器地址;  ATT7053_REG_CHIPID ATT7053_REG_DEVID
* Out:
* Ret:   0, 成功 0xff,超时
-----------------------------------------------------------------------------------*/
uint8_t write_att7053bu(uint8_t addr, uint32_t dt)
{
        uint8_t wr_buf[4];
        uint32_t dt_temp = dt;
	wr_buf[0] = addr | ATT7053_CMD_WRITE;
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
	
	wr_dt.dt = (dt&0x00ffffffu) | (((uint32_t)(addr|ATT7053_CMD_WRITE))<<24u);
	//SPI_MasterTransferBlocking(ATT7053_SPI, &pxdata);
	for(i=0;i<4;i++)
	{
		do
		{
			wait_cnt++;			
		}
		while ((!SPI_IsSPTEF(ATT7053_SPI)) && wait_cnt <= TIMEOUT_DEAD_LINE);
		
		if(wait_cnt < TIMEOUT_DEAD_LINE)
		{
			SPI_WriteData(ATT7053_SPI, wr_dt.buf[3 - i]);
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

uint8_t read_att7053bu(uint8_t addr,uint8_t *rd_buff)
{
	uint8_t wr_buf[4];
	wr_buf[0]= addr | ATT7053_CMD_READ;
	wr_buf[1]=ATT7053_REG_DUMMY;
	wr_buf[2]=ATT7053_REG_DUMMY;
	wr_buf[3]=ATT7053_REG_DUMMY;	
	SPI_CS_CLEAR();
    TS_DELAY(1);
    HAL_SPI_Transmit(&hspi1,wr_buf,1,10);
    HAL_SPI_Receive(&hspi1, rd_buff, 3,20);
	SPI_CS_SET();
	return 0;
}

uint8_t read_att7053bu32(uint8_t addr,uint8_t *rd_buff)
{
	uint8_t wr_buf[3];
	wr_buf[0]= addr | ATT7053_CMD_READ;
	wr_buf[1]=ATT7053_REG_DUMMY;
	wr_buf[2]=ATT7053_REG_DUMMY;	
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
		while ((!SPI_IsSPTEF(ATT7053_SPI))); // && wait_cnt <= TIMEOUT_DEAD_LINE
		
		if(wait_cnt < TIMEOUT_DEAD_LINE)
		{
			SPI_WriteData(ATT7053_SPI, wr_buf[i]);
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
		while ((!SPI_IsSPRF(ATT7053_SPI))); // && wait_cnt <= TIMEOUT_DEAD_LINE

		if(wait_cnt<TIMEOUT_DEAD_LINE)
		{
			rd_buff[i] = (uint8_t)SPI_ReadData(ATT7053_SPI);
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

uint8_t read_rms(uint8_t addr,uint32_t *vi, float k)
{
	static uint8_t ret=0;
   uint8_t rd_buf[20];
	static uint32_t tmp_dt=0;
	static double V_tmp=0;
	uint8_t i=0,rd_cnt=0;	
	tmp_dt=0;
	for(i=0;i<20;i++)
	{
		rd_buf[i]=0;
	}
	switch(addr)
	{
		case ATT7053_REG_V_RMS:
		case ATT7053_REG_I1_RMS:
		case ATT7053_REG_I2_RMS:
		do
		{
			for(i=0;i<20;i++);
			ret =read_att7053bu(addr,rd_buf);
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
			
			if(addr==ATT7053_REG_V_RMS)
			{
				V_tmp=((float)tmp_dt * k/100);
//				if(V_tmp>=238)
//				{
//					*vi =(uint32_t)(100*(1949.4*pow(V_tmp,2)/pow(10,4) -247.55*pow(V_tmp,3)/pow(10,6)- 4983.1*V_tmp/pow(10,2) + 4394.9));
//				}else{
                          *vi =(uint32_t)(V_tmp*1.055-7.4137); //算实际电压，k值校表获得，详见7053数据手册
													if(V_tmp<=8)
													{
														 *vi =0;
													}
//				}
				
			} 
			else if(addr==ATT7053_REG_I1_RMS)
			{      
                          if((float)tmp_dt* k >=1.1){
                             *vi = (uint32_t)((float)tmp_dt* k ) -1;	//算实际人第一通道电流，k值校表获得，详见7053数据手册
														if(tmp_dt==0)
														{
															*vi=0;
														}
                          }else{
                             *vi = (uint32_t)((float)tmp_dt* k );
                          }				
			}
			else if(addr==ATT7053_REG_I2_RMS)
			{
				if(tmp_dt<490)
				{
					tmp_dt=0;
				}
				*vi = (uint32_t)((float)tmp_dt * k) ;
//				if(*vi>=2000&&*vi<=5000)
//				{
//				*vi = (uint32_t)((float)1.62*tmp_dt * k) ;	//算实际人第二通道电流，k值校表获得，详见7053数据手册
//				}
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
		ret = read_att7053bu(ATT7053_REG_FREQ,rd_buf);
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

/*---------------------------------------------------------------------------------
* Name:   read_power
*----------------------------------------------------------------------------------
* info:  从att7053读取P,Q,S
*
* In:    addr, att7053寄存器地址,仅限通道1的功率寄存器 ;
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
		case ATT7053_REG_POW_P2:
		case ATT7053_REG_POW_Q2:
		case ATT7053_REG_POW_P1:
		case ATT7053_REG_POW_Q1:
		case ATT7053_REG_POW_S:
		do
		{
			for(i=0;i<20;i++);
			ret = read_att7053bu(addr,rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<3));
		
		if(ret==0)
		{
			tmp_dt=(((uint32_t)rd_buf[0])<<16)|(((uint32_t)rd_buf[1])<<8)|((uint32_t)rd_buf[2]);
            //PRINTF("power:%u\r",tmp_dt);
			if(tmp_dt>0x800000)
			{
				real_dt =0x1000000-tmp_dt;
			}
			else
			{
				real_dt=(int32_t)tmp_dt;
			}
//            double tempd = (double)real_dt;
			//*vi = (tempd / 100000 * k); 
			//*power = (uint16_t)(tempd / 10000 * k);
			*power = (uint32_t)(real_dt  * k );
		}
		break;
		
		default:
		ret=0xff;
	}
	return ret;	
}

/*---------------------------------------------------------------------------------
* Name:   read_energy
*----------------------------------------------------------------------------------
* info:  从att7053读取P,Q,S
*
* In:    addr, att7053寄存器地址,仅限电能寄存器 ;
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
		case ATT7053_REG_ENE_P:
		case ATT7053_REG_ENE_Q:
		case ATT7053_REG_ENE_S:
		do
		{
			for(i=0;i<20;i++);
			ret = read_att7053bu(addr, rd_buf);
			rd_cnt++;
		}
		while((ret==0xff) && (rd_cnt<3));
		
		if(ret==0)
		{
			tmp_dt = ((uint32_t)rd_buf[0]<<16)|((uint32_t)rd_buf[1]<<8)|((uint32_t)rd_buf[2]);
			//PRINTF("energytmp_dt:%u\r",tmp_dt);
			*energy = tmp_dt; 
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
		case ATT7053_REG_GP1:
		case ATT7053_REG_GQ1:
		case ATT7053_REG_GS1:
		case ATT7053_REG_P1_OFT_H:
		case ATT7053_REG_P1_OFT_L:
		case ATT7053_REG_HFCONST:
		case ATT7053_REG_GPHASE1:
			write_att7053bu(ATT7053_REG_WP, 0xA6);
			if((addr==ATT7053_REG_GP1)||(addr==ATT7053_REG_GQ1)||(addr==ATT7053_REG_GS1))
			{
				write_att7053bu(ATT7053_REG_GP1,dt);
				write_att7053bu(ATT7053_REG_GQ1,dt);
				write_att7053bu(ATT7053_REG_GS1,dt);
			}
			else if((addr==ATT7053_REG_P1_OFT_H)||(addr==ATT7053_REG_P1_OFT_L))
			{
				tmp = (uint8_t)((dt>>8)&0xff);
				write_att7053bu(ATT7053_REG_P1_OFT_H,tmp);
				tmp = (uint8_t)(dt&0xff);
				write_att7053bu(ATT7053_REG_P1_OFT_L,tmp);
			}
			else
			{
				write_att7053bu(addr,dt);
			}
			write_att7053bu(ATT7053_REG_WP, 0);
			break;
		default:;
	}
}
