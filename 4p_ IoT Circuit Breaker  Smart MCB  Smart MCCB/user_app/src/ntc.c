/*****************************************************************************
 *
 * �ļ�����: NTC�¶�
 * �������: void
 * �޸ļ�¼:
 *
 *           1. 2016-8-19 chenyue Create
 *****************************************************************************/

/*****************************************************************************
**                          ͷ�ļ������ͺ궨��
******************************************************************************/
#include "main.h"
#include "ntc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t adclist[] = {
1128 ,1149 ,1170 ,1191 ,1211 ,1222 ,1232 ,1310 ,1347 ,1365 ,1374 ,1392 ,1419 ,1470 , 
1504 ,1520 ,1536 ,1560 ,1583 ,1614 ,1672 ,1687 ,1701 ,1715 ,1728 ,1795 ,1808 ,1820 ,
1858 ,1917 ,1940 ,1974 ,2027 ,2048 ,2098 ,2146 ,2173 ,2191 ,2208 ,2226 ,2234 ,2251 ,
2276 ,2292 ,2315 ,2353 ,2389 ,2424 ,2458 ,2521 ,2579 ,2607 ,2633 ,2684 ,2731 ,2775 ,
2796 ,2816 ,2855 ,2891 ,2926 ,2958 ,2989 ,3018 ,3046 ,3072 ,3085 ,3097 ,3121 ,3186 ,
3225 ,3243 ,3260 ,3277 ,3285 ,3293 ,3316 ,3390 ,3402 ,3425 ,3435 ,3446 ,3502 ,3511 ,
3527 ,3550 ,3571 ,3584 ,3603 ,3625 ,3641 ,3651 ,3665 ,3686 ,3710 ,3724 ,3740 ,3749 ,
3755 ,3768 ,3781 ,3793 ,3799 ,3814 ,3840 ,3855 ,3868 ,3880 ,3891 ,3901 ,3905 ,3910 ,
3916 ,3922 ,3925 ,3950 ,3955 ,3959 ,3964 ,3968 ,3972 ,3976 ,3982 ,3996 ,3998 ,4002 ,
4005 ,4024 ,4016 ,4020 ,4024 ,4028 ,4030 ,4033 ,4039 ,4041 ,4045 ,4048 ,4051 ,4055 ,
4057 ,4059 ,4060 ,4061 ,4062 ,4063 ,4065 ,4067 ,4069 ,4071 ,4075 ,4076 ,4077 ,4077 ,
4080 ,4081 ,4082 ,4082, 4083 ,4084 ,4085  
};

static float _GetTemperature(const uint16_t *adclist,uint16_t listlen,uint16_t adcdata,int tempmin);
/** 
 * Get Temperature of list.
 * 
 * @param adclist   温度对应的ADC值列表
 * @param listlen   列表长度
 * @param adcdata   采集到的ADC值  
 * @param tempmin   温度下限 必须为负值
 * @return  温度 0xffff表示超出范围
 */
static float _GetTemperature(const uint16_t *adclist,uint16_t listlen,uint16_t adcdata,int tempmin) {
	float T = 0xffff;	
	int Subscript = 0xffff;
	int Monotonicity = 0xffff;
	int Max = 0;
	int i;
	/* 判断电路接法 */
	if(adclist[0] > adclist[listlen-1]) {
		Monotonicity = 1;//降序 
	}
	else if(adclist[listlen-1] > adclist[0]) {
		Monotonicity = 0;//升序 
	}
	else {
		return T;
	}
	/* 查表 */
	if(!Monotonicity) {//升序 
		for(i=0;i<listlen;i++) {
			if(adcdata >= adclist[i] && Max != adclist[i]) {
				Max = adclist[i];
				Subscript = i;
			}
		}	
	}
	else {
		for(i=1;i<=listlen;i++) {
			if(adcdata >= adclist[listlen-i]) {
				Subscript = listlen-i;
			}
		}
	}
	/* 判断是否在表中 */
	if((Monotonicity == 0 && (adcdata < adclist[0] || adcdata > adclist[listlen-1])) ||\
	   (Monotonicity == 1 && (adcdata > adclist[0] || adcdata < adclist[listlen-1]))) {
		//printf("out of range\r\n");
	}
	else {
	    /* 判断是否为列表最小边界 */
	    if(Subscript > 0) {
	    	/* x = i = T */
	    	/* y= adc */
	    	/* 查表法计算出温度 */
	    	T = ((tempmin>0) ? (listlen-1-Subscript-tempmin) : (listlen-1-Subscript+tempmin));
	    	/* 判断是否为最大边界 */
    		if(Subscript != (listlen-1)) {
		    	/* 插值法提高精度 */
				if(adclist[Subscript] != adclist[(Subscript+1)]) {
					T = adcdata-adclist[Subscript];
					T = T/(float)(adclist[(Subscript+1)]-adclist[Subscript]);
					T += ((tempmin>0) ? (listlen-1-Subscript-tempmin) : (listlen-1-Subscript+tempmin));	
				}    			
    		}	
	    }
	    else {
	    	T = tempmin+listlen-1;
	    }		
	}
    return T;
}

/** 
 * Get Temperature of list.
 * 
 * @param adclist   温度对应的ADC值列表
 * @param listlen   列表长度
 * @param adcdata   采集到的ADC值  
 * @param tempmin   温度下限 必须为负值
 * @return  温度 0xffff表示超出范围
 */
int8_t GetTemperature(uint16_t adcdata) {
	return (int8_t)_GetTemperature(adclist,sizeof(adclist)/sizeof(adclist[0]),adcdata,(-50));
}

/** 
 * 卡尔曼滤波器.
 * 
 * @param  inData   测量值
 * @return  最优值
 */
float kalmanFilter_A(float inData) 
{
  static float prevData=0; 
  static float p=10, q=0.0001, r=0.050, kGain=0;
	//过程噪声的方差Q和测量噪声的方差R、卡尔曼增益kGain
	//prevData上一次的值
    p += q; 
    kGain = p/(p+r);

    inData = prevData+(kGain*(inData-prevData)); 
	
    p = (1-kGain)*p;
    prevData = inData;
    return inData; 
}
/*****************************************************************************
**                            End of File
******************************************************************************/
