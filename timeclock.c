#include <regx52.h>
#include "ds1302.h"
#include "dk.h"
#include "LCD1602.h"
unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;
/**
 * @brief  时间显示功能
 * @retval 无
 */
void TimeShow(void) //时间显示功能
{
    DS1302_ReadTime();                    //读取时间
    LCD_ShowNum(1, 1, DS1302_Time[0], 2); //显示年
    LCD_ShowNum(1, 4, DS1302_Time[1], 2); //显示月
    LCD_ShowNum(1, 7, DS1302_Time[2], 2); //显示日
    LCD_ShowNum(2, 1, DS1302_Time[3], 2); //显示时
    LCD_ShowNum(2, 4, DS1302_Time[4], 2); //显示分
    LCD_ShowNum(2, 7, DS1302_Time[5], 2); //显示秒
}
/**
 * @brief  时间设置功能
 * @retval 按键2按下设置选择位加1按键3按下时间设置位数值加1
 */
void TimeSet(void) //时间设置功能
{
    if (KeyNum == 2) //按键2按下
    {
        TimeSetSelect++;    //设置选择位加1
        TimeSetSelect %= 6; //越界清零
    }
    if (KeyNum == 3) //按键3按下
    {
        DS1302_Time[TimeSetSelect]++; //时间设置位数值加1
        if (DS1302_Time[0] > 99)
        {
            DS1302_Time[0] = 0;
        } //年越界判断
        if (DS1302_Time[1] > 12)
        {
            DS1302_Time[1] = 1;
        } //月越界判断
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //日越界判断
        {
            if (DS1302_Time[2] > 31)
            {
                DS1302_Time[2] = 1;
            } //大月
        }
        else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if (DS1302_Time[2] > 30)
            {
                DS1302_Time[2] = 1;
            } //小月
        }
        else if (DS1302_Time[1] == 2)
        {
            if (DS1302_Time[0] % 4 == 0)
            {
                if (DS1302_Time[2] > 29)
                {
                    DS1302_Time[2] = 1;
                } //闰年2月
            }
            else
            {
                if (DS1302_Time[2] > 28)
                {
                    DS1302_Time[2] = 1;
                } //平年2月
            }
        }
        if (DS1302_Time[3] > 23)
        {
            DS1302_Time[3] = 0;
        } //时越界判断
        if (DS1302_Time[4] > 59)
        {
            DS1302_Time[4] = 0;
        } //分越界判断
        if (DS1302_Time[5] > 59)
        {
            DS1302_Time[5] = 0;
        } //秒越界判断
    }
    if (KeyNum == 4) //按键3按下
    {
        DS1302_Time[TimeSetSelect]--; //时间设置位数值减1
        if (DS1302_Time[0] < 0)
        {
            DS1302_Time[0] = 99;
        } //年越界判断
        if (DS1302_Time[1] < 1)
        {
            DS1302_Time[1] = 12;
        } //月越界判断
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //日越界判断
        {
            if (DS1302_Time[2] < 1)
            {
                DS1302_Time[2] = 31;
            } //大月
            if (DS1302_Time[2] > 31)
            {
                DS1302_Time[2] = 1;
            }
        }
        else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if (DS1302_Time[2] < 1)
            {
                DS1302_Time[2] = 30;
            } //小月
            if (DS1302_Time[2] > 30)
            {
                DS1302_Time[2] = 1;
            }
        }
        else if (DS1302_Time[1] == 2)
        {
            if (DS1302_Time[0] % 4 == 0)
            {
                if (DS1302_Time[2] < 1)
                {
                    DS1302_Time[2] = 29;
                } //闰年2月
                if (DS1302_Time[2] > 29)
                {
                    DS1302_Time[2] = 1;
                }
            }
            else
            {
                if (DS1302_Time[2] < 1)
                {
                    DS1302_Time[2] = 28;
                } //平年2月
                if (DS1302_Time[2] > 28)
                {
                    DS1302_Time[2] = 1;
                }
            }
        }
        if (DS1302_Time[3] < 0)
        {
            DS1302_Time[3] = 23;
        } //时越界判断
        if (DS1302_Time[4] < 0)
        {
            DS1302_Time[4] = 59;
        } //分越界判断
        if (DS1302_Time[5] < 0)
        {
            DS1302_Time[5] = 59;
        } //秒越界判断
    }
    //更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
    if (TimeSetSelect == 0 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(1, 1, "  ");
    }
    else
    {
        LCD_ShowNum(1, 1, DS1302_Time[0], 2);
    }
    if (TimeSetSelect == 1 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(1, 4, "  ");
    }
    else
    {
        LCD_ShowNum(1, 4, DS1302_Time[1], 2);
    }
    if (TimeSetSelect == 2 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(1, 7, "  ");
    }
    else
    {
        LCD_ShowNum(1, 7, DS1302_Time[2], 2);
    }
    if (TimeSetSelect == 3 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(2, 1, "  ");
    }
    else
    {
        LCD_ShowNum(2, 1, DS1302_Time[3], 2);
    }
    if (TimeSetSelect == 4 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(2, 4, "  ");
    }
    else
    {
        LCD_ShowNum(2, 4, DS1302_Time[4], 2);
    }
    if (TimeSetSelect == 5 && TimeSetFlashFlag == 1)
    {
        LCD_ShowString(2, 7, "  ");
    }
    else
    {
        LCD_ShowNum(2, 7, DS1302_Time[5], 2);
    }
}