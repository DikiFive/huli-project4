#include <REGX52.H>
#include "dk.h"

void TimeSet(void) //时间设置功能
{
    if (KeyNum == 2) //按键2按下
    {
        TimeSetSelect++;    //设置选择位加1
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
        // todo; //时间设置位数值减1
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