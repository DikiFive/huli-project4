void Timer0_Routine() interrupt 1 //中断函数,一般放在main.c里
{
    static unsigned int T0Count; //在中断函数内是局部变量，中断函数外是全局变量，static就是说下次再运行的时候，他的值不变，如果没有static，那么函数执行后变量就改变了
    TH0 = 64535 / 256;           //赋初值
    TL0 = 64535 % 256;
    T0Count++;
    //定时器过一秒 sec++
    if (T0Count >= 1000)
    {
        T0Count = 0;
        time[5]++;
    }
    // tis();
    if (time[5] >= 60)
    {
        time[5] = 0;
        time[4]++;
    }
    if (time[4] >= 60)
    {
        time[4] = 0;
        time[3]++;
    }
    if (time[3] >= 24)
    {
        time[3] = 0;
        time[2]++;
    }
    if (time[1] == 1 || time[1] == 3 || time[1] == 5 || time[1] == 7 ||
        time[1] == 8 || time[1] == 10 || time[1] == 12)
    {
        if (time[2] > 31)
        {
            time[2] = 1;
        } //大月
    }
    else if (time[1] == 4 || time[1] == 6 || time[1] == 9 || time[1] == 11)
    {
        if (time[2] > 30)
        {
            time[2] = 1;
        } //小月
    }
    else if (time[1] == 2)
    {
        if (time[0] % 4 == 0)
        {
            if (time[2] > 29)
            {
                time[2] = 1;
            } //闰年二月
        }
        else
        {
            if (time[2] > 28)
            {
                time[2] = 1;
            } //平年二月
        }
    }
}
