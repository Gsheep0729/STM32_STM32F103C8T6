#include "stm32f10x.h"                  // Device header

/* =====================================================================
 * LED 模块 —— 具体实现
 * ---------------------------------------------------------------------
 * 把"LED 接在哪个引脚、怎么点亮、怎么熄灭、怎么翻转"这些细节全部封在这里。
 * 其它文件只要 #include "LED.h" 就能调用 LED1_ON() 这类函数。
 *
 * 硬件接线（详见 02GPIO.md 图 24）：
 *   LED1 阳极接 3V3、阴极经 1kΩ 接 PA1；LED2 同理接 PA2。
 *   所以引脚输出低电平点亮、高电平熄灭（灌电流驱动）。
 *
 * 对照来源：老师源码 3-4 按键控制LED / Hardware/LED.c
 * ===================================================================== */

/**
  * @brief  LED 初始化：把 PA1、PA2 配成推挽输出，并默认输出高电平（LED 灭）
  * @param  无
  * @return 无
  */
void LED_Init(void)
{
	/*开启时钟：LED 接在 GPIOA 上，所以开 GPIOA 的时钟
	 *（时钟没开，外设相当于断电，后面配了也没用）*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;						//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//模式：推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;		//引脚：PA1 和 PA2
																//用"按位或 |"把两个引脚的宏合成一个值，
																//这样一次 GPIO_Init 就能配好两个引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度：50MHz

	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA1和PA2初始化为推挽输出

	/*设置初始化后的默认电平：输出高电平 = LED 灭
	 * 这样一上电灯是暗的，而不是莫名其妙一直亮着*/
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/**
  * @brief  点亮 LED1（PA1 输出低电平）
  * @param  无
  * @return 无
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);		//设置PA1引脚为低电平 → 灯亮
}

/**
  * @brief  熄灭 LED1（PA1 输出高电平）
  * @param  无
  * @return 无
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);		//设置PA1引脚为高电平 → 灯灭
}

/**
  * @brief  翻转 LED1：亮的变灭、灭的变亮
  * @param  无
  * @return 无
  * @note   先读"输出数据寄存器"里 PA1 当前的值，再反过来设置。
  *         注意读的是"我上次让它输出什么"，不是引脚上的实际电平。
  */
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)		//当前输出低电平（灯亮）
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);					//则改为高电平（灯灭）
	}
	else													//否则当前输出高电平（灯灭）
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);					//则改为低电平（灯亮）
	}
}

/**
  * @brief  点亮 LED2（PA2 输出低电平）
  * @param  无
  * @return 无
  */
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);		//设置PA2引脚为低电平 → 灯亮
}

/**
  * @brief  熄灭 LED2（PA2 输出高电平）
  * @param  无
  * @return 无
  */
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);		//设置PA2引脚为高电平 → 灯灭
}

/**
  * @brief  翻转 LED2：亮的变灭、灭的变亮
  * @param  无
  * @return 无
  */
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)		//当前输出低电平（灯亮）
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);					//则改为高电平（灯灭）
	}
	else													//否则当前输出高电平（灯灭）
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);					//则改为低电平（灯亮）
	}
}
