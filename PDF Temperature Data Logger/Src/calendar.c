#include <string.h>
#include "calendar.h"

struct date
{
  uint16_t year;         //定义年
 uint8_t month;         //定义月
 uint16_t day;         //定义天
};
struct date next_day(struct date in);      //声明进入下一天的函数
struct date nextN_day(struct date in, uint16_t x);     //声明进入下N天的函数
void print(struct date in);         //声明输入日期的函数
int leap( uint16_t x);           //声明闰年的函数

uint8_t temper_year=0,temper_month=0,temper_day=0;
//uint8_t main()
void Day_Canclude(uint8_t temp_y,uint8_t temp_m,uint16_t temp_d, uint16_t N_day)
{
 struct date d;
 struct date r1;
	d.year=temp_y+2000;
	d.month=temp_m;
	d.day=temp_d;

  r1 = nextN_day(d,N_day);           //计算N天后的日期
	
	temper_year=r1.year-2000;
	temper_month=r1.month;
	temper_day=r1.day;

}
int leap( uint16_t x)
{
 if(((x%4 == 0)&&(x%100 != 0)) || x%400 == 0)     //定义判断函数是否为闰年
  return 1;            //闰年返回1
 else
  return 0;            //平年返回0
}
struct date next_day(struct date in)
{
 uint16_t m_day;             //??m_day??????
 struct date r;
 switch(in.month)
 {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 4:
  case 6:
  case 9:
  case 11: m_day = 30; break;         //?????????
	case 12: m_day = 31;
						break;
  case 2: {
					 if(leap(in.year))
						 m_day = 29;
					 else
						 m_day = 28;
					}
					break;            //???????????
 }

		 if(in.day+1 <= m_day)
	 {
			r.year = in.year;
			r.month = in.month;
			r.day = in.day+1;           //????????1?????,?????1
	 }
 if(in.day+1 > m_day && in.month != 12)
	 {
			r.year = in.year;
			r.month = in.month+1;
			r.day = 1;              //??????????????,????12?,????1,???1
	 }
 if(in.month == 12 && in.day == 31)
	 {
			r.year = in.year+1;
			r.month = 1;
			r.day = 1;             //?????????????,????1,?????1
	 } 
 return r;
}
struct date nextN_day(struct date in, uint16_t x)
{
 struct date re = in;
 while(x--)
	 {
		re = next_day(re);           //????x????????,?????x??????
	 }
 return re;
}
