#include <regx52.h>
#include "dk.h"
#include "LCD1602.h"
#include "At24C02.h"
#include "I2C.h"
#include "ds1302.h"
typedef unsigned int ui;
typedef unsigned char uc;
uc Hour = 23, Min = 59, Sec = 58;
ui Year = 2001, Mon = 3, Day = 30;
void showtime();
void main()
{
	LCD_Init();
	Timer0_Init();
	LCD_ShowString(1, 5, "-");
	LCD_ShowString(1, 8, "-");
	LCD_ShowString(2, 3, ":");
	LCD_ShowString(2, 6, ":");
	while (1)
	{
		showtime();
	}
}
void showtime()
{
	LCD_ShowNum(1, 1, Year, 4);
	LCD_ShowNum(1, 6, Mon, 2);
	LCD_ShowNum(1, 9, Day, 2);
	LCD_ShowNum(2, 1, Hour, 2);
	LCD_ShowNum(2, 4, Min, 2);
	LCD_ShowNum(2, 7, Sec, 2);
}

void Timer0_Rountine() interrupt 1 //中断函数,一般放在main.c里
{
	static unsigned int T0Count;
	unsigned int t;	   //在中断函数内是局部变量，中断函数外是全局变量，static就是说下次再运行的时候，他的值不变，如果没有static，那么函数执行后变量就改变了
	TH0 = 64535 / 256; //赋初值
	TL0 = 64535 % 256;
	T0Count++;
	//定时器过一秒 sec++
	if (T0Count >= 1000)
	{
		T0Count = 0;
		Sec++;
	}
	//大于60 min++
	if (Sec >= 60)
	{
		Min++;
		Sec = 0;
	}
	//大于60 hour++
	if (Min >= 60)
	{
		Hour++;
		Min = 0;
	}
	//大于24 day++
	if (Hour >= 24)
	{
		Hour = 0;
		Day++;
	}
	//重置闰年判断条件
	t = 0;
	//闰年判断
	if ((Year % 4 == 0 && Year % 100 != 0) || Year % 400 == 0)
	{
		t = 1;
	}
	//判断闰年月份情况
	if (t == 1)
	{
		if (Day > 29 && Mon == 2)
		{
			Mon++;
			Day = 1;
		}
		else if (Day > 30 && (Mon == 4 || Mon == 6 || Mon == 9 || Mon == 11))
		{
			Mon++;
			Day = 1;
		}
		else if (Day > 31 && (Mon == 1 || Mon == 3 || Mon == 5 || Mon == 7 || Mon == 8 || Mon == 10 || Mon == 12))
		{
			Mon++;
			Day = 1;
		}
	}
	//判断平年月份情况
	if (t != 1)
	{
		if (Day > 28 && Mon == 2)
		{
			Mon++;
			Day = 1;
		}
		else if (Day > 31 && (Mon == 1 || Mon == 3 || Mon == 5 || Mon == 7 || Mon == 8 || Mon == 10 || Mon == 12))
		{
			Mon++;
			Day = 1;
		}
		else if (Day > 30 && (Mon == 4 || Mon == 6 || Mon == 9 || Mon == 11))
		{
			Mon++;
			Day = 1;
		}
	}
	//判断月份是否进入下一年
	if (Mon > 12)
	{
		Year++;
		Mon = 1;
	}
	//重开世纪超过4位数
	if (Year > 9999)
	{
		Year = 0;
		Mon = 0;
		Day = 0;
		Hour = 0;
		Min = 0;
		Sec = 0;
	}
}
