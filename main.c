#include <regx52.h>	 //stc89c52头文件
#include "dk.h"		 //自己集成常用函数头文件
#include "LCD1602.h" //lcd头文件
#include "AT24C02.h" //掉电记忆储存头文件
//定义数据类型
typedef unsigned char uc;
typedef unsigned int ui;
//定义时间变量
uc Mon = 6, Day = 22, Hour = 21, Min = 4, Sec = 30, MODE = 0, TimeSetSelect, KeyNum, mxk, TimeSetFlashFlag, y1, y2;
ui Year = 2022;
//闰年判断条件
unsigned int t;
//声明子函数
void showtime();
void tis();
void TimeSet(void);
void WriteTime();
void ReadTime();
//主函数入口
void main()
{
	//初始化和静态显示
	LCD_Init();
	Timer0_Init();
	LCD_ShowString(1, 5, "-");
	LCD_ShowString(1, 8, "-");
	LCD_ShowString(2, 3, ":");
	LCD_ShowString(2, 6, ":");
	ReadTime(); //读取数据
	while (1)
	{
		KeyNum = Key();	 //获取独立按键键位数据
		if (KeyNum == 1) //按键1按下
		{
			if (MODE == 0) //模式0到模式1
			{
				MODE = 1;
				TimeSetSelect = 0;
			}					//功能切换
			else if (MODE == 1) //模式1到模式0
			{
				MODE = 0;
				showtime(); //展示时间数据
			}
		}
		switch (MODE) //根据不同的功能执行不同的函数
		{
		case 0:
			showtime();
			tis(); //计算时间进制
			break;
		case 1:
			TimeSet(); //时间设置函数
			break;
		}
	}
}
/**
 * @brief  时间显示与模式显示
 */
void showtime()
{
	LCD_ShowNum(1, 1, Year, 4);
	LCD_ShowNum(1, 6, Mon, 2);
	LCD_ShowNum(1, 9, Day, 2);
	LCD_ShowNum(2, 1, Hour, 2);
	LCD_ShowNum(2, 4, Min, 2);
	LCD_ShowNum(2, 7, Sec, 2);
	LCD_ShowString(2, 11, "normal");
	if ((Year % 4 == 0 && Year % 100 != 0) || Year % 400 == 0) //判断闰平年
	{
		LCD_ShowString(1, 16, "R");
	}
	else
	{
		LCD_ShowString(1, 16, "P");
	}
}
/**
 * @brief  时间设置
 */
void TimeSet(void) //时间设置功能
{
	LCD_ShowString(2, 11, "change");
	if (KeyNum == 2) //按键2按下
	{

		TimeSetSelect++;	//设置选择位加1
		TimeSetSelect %= 6; //越界清零
	}
	//所在位置时间增加
	if (KeyNum == 3) //按键3按下
	{

		//时间设置位数值加1
		switch (TimeSetSelect)
		{
		case 0:
			Year++;
			break;
		case 1:
			Mon++;
			if (Mon == 13)
			{
				Mon = 1;
			}
			break;
		case 2:
			Day++;
			break;
		case 3:
			Hour++;
			if (Hour == 24)
			{
				Hour = 0;
			}
			break;
		case 4:
			Min++;
			if (Min > 59)
			{
				Min = 0;
			}
			break;
		case 5:
			Sec++;
			break;
		}
	}
	//所在位置时间减少
	if (KeyNum == 4) //按键4按下
	{

		switch (TimeSetSelect)
		{
		case 0:
			Year--;
			break;
		case 1:
			Mon--;
			if (Mon == 0)
			{
				Mon = 12;
			}
			break;
		case 2:
			Day--;
			if (Day == 0)
			{
				Day = 28;
			}
			break;
		case 3:
			Hour--;
			if (Hour > 24)
			{
				Hour = 23;
			}
			break;
		case 4:
			Min--;
			if (Min > 60)
			{
				Min = 59;
			}
			break;
		case 5:
			Sec--;
			break;
		}
	}
	//更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 1, "    ");
	}
	else
	{
		LCD_ShowNum(1, 1, Year, 4);
	}
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 6, "  ");
	}
	else
	{
		LCD_ShowNum(1, 6, Mon, 2);
	}
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 9, "  ");
	}
	else
	{
		LCD_ShowNum(1, 9, Day, 2);
	}
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, Hour, 2);
	}
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 4, "  ");
	}
	else
	{
		LCD_ShowNum(2, 4, Min, 2);
	}
	if (TimeSetSelect == 5 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 7, "  ");
	}
	else
	{
		LCD_ShowNum(2, 7, Sec, 2);
	}
	WriteTime(); //写入时间数据
}
/**
 * @brief  读取at24c02时间数据
 */
void ReadTime()
{
	y1 = AT24C02_ReadByte(0);
	Delay(5);
	y2 = AT24C02_ReadByte(1);
	Delay(5);
	Mon = AT24C02_ReadByte(2);
	Delay(5);
	Day = AT24C02_ReadByte(3);
	Delay(5);
	Hour = AT24C02_ReadByte(4);
	Delay(5);
	Min = AT24C02_ReadByte(5);
	Delay(5);
	Sec = AT24C02_ReadByte(6);
	Delay(5);
	Year = (y1 * 100) + y2; //计算year
}
/**
 * @brief  写入at24c02时间数据
 */
void WriteTime()
{
	y2 = (Year % 100); //拆分year到y1，y2方便写入
	y1 = (Year / 100);
	AT24C02_WriteByte(0, y1);
	Delay(5);
	AT24C02_WriteByte(1, y2);
	Delay(5);
	AT24C02_WriteByte(2, Mon);
	Delay(5);
	AT24C02_WriteByte(3, Day);
	Delay(5);
	AT24C02_WriteByte(4, Hour);
	Delay(5);
	AT24C02_WriteByte(5, Min);
	Delay(5);
	AT24C02_WriteByte(6, Sec);
	Delay(5);
}
/**
 * @brief  中断函数
 */
void Timer0_Routine() interrupt 1 //中断函数,一般放在main.c里
{
	static unsigned int T0Count; //在中断函数内是局部变量，中断函数外是全局变量

	TH0 = 64535 / 256; //赋初值
	TL0 = 64535 % 256;
	T0Count++;
	//定时器过一秒 sec++
	if (T0Count >= 1000)
	{
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag; //取反
		Sec++;
		tis();
	}
}
/**
 * @brief  时间计算
 */
void tis()
{
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
	if (Mon > 12 && Mon > 0)
	{
		Year++;
		Mon = 1;
	}
	//重开世纪超过4位数
	if (Year > 9999)
		Year = 0;
	Mon = 0;
	Day = 0;
	Hour = 0;
	Min = 0;
	Sec = 0;
}

