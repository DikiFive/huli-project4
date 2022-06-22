#include <regx52.h>

unsigned char ntb[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; //数码管数组初始化

void Delay(unsigned int xms); //延时函数声明

/**
 * @brief  数码管显示
 * @param  location 显示的地址
 * @param  num 要显示的数据
 * @retval 无
 */
void Nt(unsigned char location, num) //数码管显示： 位置 数据
{
	switch (location)
	{
	case 1:
		P2_4 = 1;
		P2_3 = 1;
		P2_2 = 1;
		break;
	case 2:
		P2_4 = 1;
		P2_3 = 1;
		P2_2 = 0;
		break;
	case 3:
		P2_4 = 1;
		P2_3 = 0;
		P2_2 = 1;
		break;
	case 4:
		P2_4 = 1;
		P2_3 = 0;
		P2_2 = 0;
		break;
	case 5:
		P2_4 = 0;
		P2_3 = 1;
		P2_2 = 1;
		break;
	case 6:
		P2_4 = 0;
		P2_3 = 1;
		P2_2 = 0;
		break;
	case 7:
		P2_4 = 0;
		P2_3 = 0;
		P2_2 = 1;
		break;
	case 8:
		P2_4 = 0;
		P2_3 = 0;
		P2_2 = 0;
		break;
	}
	P0 = ntb[num];
	Delay(1);
	P0 = 0x00;
}

/**
 * @brief  延时函数
 * @param  xms 延时时间
 * @retval 无
 */
void Delay(unsigned int xms) //延时函数： 时间
{
	unsigned char i, j;
	while (xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j)
				;
		} while (--i);
	}
}
/**
 * @brief  获取独立按键键码
 * @param  无
 * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
 */
unsigned char Key()
{
	unsigned char KeyNumber = 0;

	if (P3_1 == 0)
	{
		Delay(20);
		while (P3_1 == 0)
			;
		Delay(20);
		KeyNumber = 1;
	}
	if (P3_0 == 0)
	{
		Delay(20);
		while (P3_0 == 0)
			;
		Delay(20);
		KeyNumber = 2;
	}
	if (P3_2 == 0)
	{
		Delay(20);
		while (P3_2 == 0)
			;
		Delay(20);
		KeyNumber = 3;
	}
	if (P3_3 == 0)
	{
		Delay(20);
		while (P3_3 == 0)
			;
		Delay(20);
		KeyNumber = 4;
	}

	return KeyNumber;
}

/**
 * @brief  定时器0初始化，1毫秒@12.000MHz
 * @param  无
 * @retval 无
 */
void Timer0_Init() //定时器： 初始化
{
	TMOD = 0x01; // 0000 0001
	TF0 = 0;
	TR0 = 1;
	TH0 = 64535 / 256;
	TL0 = 64535 % 256;
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;

	}
}
*/
