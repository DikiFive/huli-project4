#include <regx52.h>
#include "dk.h"
#include "LCD1602.h"
#include "AT24C02.h"
//定义数据类型
typedef unsigned int ui;
typedef unsigned char uc;
//定义时间变量
uc Hour = 23, Min = 59, Sec = 58;
ui Year = 2001, Mon = 3, Day = 30;
uc MODE = 0, TimeSetSelect, KeyNum, TimeSetFlashFlag;
//闰年判断条件
unsigned int t;
//声明子函数
void showtime();
void tis();
void TimeSet(void);

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
	}
}

/**
 * @brief  时间显示
 */
void showtime()
{
	LCD_ShowNum(1, 1, Year, 4);
	LCD_ShowNum(1, 6, Mon, 2);
	LCD_ShowNum(1, 9, Day, 2);
	LCD_ShowNum(2, 1, Hour, 2);
	LCD_ShowNum(2, 4, Min, 2);
	LCD_ShowNum(2, 7, Sec, 2);
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

/**
 * @brief  时间设置
 */
void TimeSet(void) //时间设置功能
{
	if (KeyNum == 2) //按键2按下
	{
		TimeSetSelect++;	//设置选择位加1
		TimeSetSelect %= 6; //越界清零
	}
	if (KeyNum == 3) //按键3按下
	{
		TimeSetSelect++; //时间设置位数值加1
		if (Year > 99)
		{
			Year = 0;
		} //年越界判断
		if (Mon > 12)
		{
			Mon = 1;
		} //月越界判断
		if (Mon == 1 || Mon == 3 || Mon == 5 || Mon == 7 ||
			Mon == 8 || Mon == 10 || Mon == 12) //日越界判断
		{
			if (Day > 31)
			{
				Day = 1;
			} //大月
		}
		else if (Mon == 4 || Mon == 6 || Mon == 9 || Mon == 11)
		{
			if (Day > 30)
			{
				Day = 1;
			} //小月
		}
		else if (Mon == 2)
		{
			if (Year % 4 == 0)
			{
				if (Day > 29)
				{
					Day = 1;
				} //闰年2月
			}
			else
			{
				if (Day > 28)
				{
					Day = 1;
				} //平年2月
			}
		}
		if (Hour > 23)
		{
			Hour = 0;
		} //时越界判断
		if (Min > 59)
		{
			Min = 0;
		} //分越界判断
		if (Sec > 59)
		{
			Sec = 0;
		} //秒越界判断
	}
	if (KeyNum == 4) //按键3按下
	{
		// todo
		TimeSetSelect--; //时间设置位数值减1
		if (Year < 0)
		{
			Year = 99;
		} //年越界判断
		if (Mon < 1)
		{
			Mon = 12;
		} //月越界判断
		if (Mon == 1 || Mon == 3 || Mon == 5 || Mon == 7 ||
			Mon == 8 || Mon == 10 || Mon == 12) //日越界判断
		{
			if (Day < 1)
			{
				Day = 31;
			} //大月
			if (Day > 31)
			{
				Day = 1;
			}
		}
		else if (Mon == 4 || Mon == 6 || Mon == 9 || Mon == 11)
		{
			if (Day < 1)
			{
				Day = 30;
			} //小月
			if (Day > 30)
			{
				Day = 1;
			}
		}
		else if (Mon == 2)
		{
			if (Year % 4 == 0)
			{
				if (Day < 1)
				{
					Day = 29;
				} //闰年2月
				if (Day > 29)
				{
					Day = 1;
				}
			}
			else
			{
				if (Day < 1)
				{
					Day = 28;
				} //平年2月
				if (Day > 28)
				{
					Day = 1;
				}
			}
		}
		if (Hour < 0)
		{
			Hour = 23;
		} //时越界判断
		if (Min < 0)
		{
			Min = 59;
		} //分越界判断
		if (Sec < 0)
		{
			Sec = 59;
		} //秒越界判断
	}
	//更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 1, "  ");
	}
	else
	{
		LCD_ShowNum(1, 1, Year, 2);
	}
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 4, "  ");
	}
	else
	{
		LCD_ShowNum(1, 4, Mon, 2);
	}
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 7, "  ");
	}
	else
	{
		LCD_ShowNum(1, 7, Day, 2);
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
		Sec++;
	}
	tis();
}
