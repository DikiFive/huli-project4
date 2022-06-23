#include <regx52.h>
#include "dk.h"
#include "LCD1602.h"
#include "AT24C02.h"
#include "MatrixKey.h"
//定义数据类型
typedef unsigned int ui;
typedef unsigned char uc;
//定义时间变量
uc MODE = 0, TimeSetSelect, KeyNum, Mxk, TimeSetFlashFlag;
uc Year[4], Mon[2], Day[2], Hour[2], Min[2], Sec[2];
//闰年判断条件
unsigned int t;
//声明子函数
void showtime();
void tis();
void TimeSet(void);
void LCD_ShowNumArray(unsigned char Line, unsigned char Column, unsigned int *Number, unsigned char Length);
void ReadTime();
void WriteTime();

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
	while (1)
	{
		KeyNum = Key();
		Mxk = MatrixKey();
		if (KeyNum == 1) //按键1按下
		{
			if (MODE == 0)
			{
				MODE = 1;
				TimeSetSelect = 0;
			} //功能切换
			else if (MODE == 1)
			{
				MODE = 0;
				break;
			}
		}
		switch (MODE) //根据不同的功能执行不同的函数
		{
		case 0:
			showtime();
			break;
		case 1:
			TimeSet();
			break;
		}
		// if (KeyNum == 3)
		// {
		// 	ReadTime();
		// }
		// if (KeyNum == 4)
		// {
		// 	WriteTime();
		// }
	}
}

/**
 * @brief  时间显示
 */
void showtime()
{
}

/**
 * @brief  读取at24c02数据
 */
void ReadTime()
{
	Mon = AT24C02_ReadByte(111);
	Day = AT24C02_ReadByte(112);
	Hour = AT24C02_ReadByte(113);
	Min = AT24C02_ReadByte(114);
	Sec = AT24C02_ReadByte(115);
	y1 = AT24C02_ReadByte(116);
	y2 = AT24C02_ReadByte(110);
	Year = (y1 * 100) + y2;
}

/**
 * @brief  写入at24c02数据
 */
void WriteTime()
{
	AT24C02_WriteByte(111, Mon);
	AT24C02_WriteByte(112, Day);
	AT24C02_WriteByte(113, Hour);
	AT24C02_WriteByte(114, Min);
	AT24C02_WriteByte(115, Sec);
	AT24C02_WriteByte(116, y1);
	AT24C02_WriteByte(110, y2);
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
	{
		Year = 0;
		Mon = 0;
		Day = 0;
		Hour = 0;
		Min = 0;
		Sec = 0;
	}
}
/**
 * @brief  时间设置
 */
void TimeSet(void) //时间设置功能
{
	uc i;
	if (KeyNum == 2) //按键2按下
	{

		TimeSetSelect++;	//设置选择位加1
		TimeSetSelect %= 6; //越界清零
	}
	switch (TimeSetSelect)
	{
	case 0:
		while (i = 0; i < 4; i++)
		{
			/* code */
		}

		break;
	case 1:
		/* code */
		break;
	case 2:
		/* code */
		break;
	case 3:
		/* code */
		break;
	case 0:
		/* code */
		break;
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
}

/**
 * @brief  在LCD1602指定位置开始显示所给数字数组
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字数组，范围：0~65535
 * @param  Length 要显示数组的长度，范围：1~5
 * @retval 无
 */
void LCD_ShowNumArray(unsigned char Line, unsigned char Column, unsigned int *Number, unsigned char Length)
{
	unsigned char i;
	while (i = 0; i < Length; i++)
	{
		LCD_ShowNum(Line, Column, Number[i], Length);
	}
}

/**
 * @brief  中断函数
 */
void Timer0_Routine() interrupt 1 //中断函数,一般放在main.c里
{
	static unsigned int T0Count, tf; //在中断函数内是局部变量，中断函数外是全局变量

	TH0 = 64535 / 256; //赋初值
	TL0 = 64535 % 256;
	T0Count++;
	//定时器过一秒 sec++
	if (T0Count >= 1000)
	{
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag;
		Sec++;
		tis();
	}
}
