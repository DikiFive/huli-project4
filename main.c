#include <REGX52.H>
#include "LCD1602.h"
// #include "ds1302.h"
#include "dk.h"
// #include "timeclock.h"
#include "AT24C02.h"
#include <string.h>
#include <stdio.h>

typedef unsigned char uc;
typedef unsigned int ui;

// uc time[]={2000,2,28,,23,59,58};
struct date
{
	ui year;
	uc mon;
	uc day;
	uc hour;
	uc min;
	uc sec;
};

// todo
int main()
{
	LCD_Init();

	struct date now;
	now.year = 2020;
	while (1)
	{
		LCD_ShowNum(1, 1, now.year, 4);
	}
}

/**
 * @brief  中断函数
 * @retval 每500ms进入一次
 */
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18; //设置定时初值
	TH0 = 0xFC; //设置定时初值
	T0Count++;
	if (T0Count >= 500) //每500ms进入一次
	{
		T0Count = 0;
	}
}
