#include "stm32f10x.h"                  // Device header

/* =====================================================================
 * 蜂鸣器模块 —— 具体实现
 * ---------------------------------------------------------------------
 * 把"蜂鸣器接在哪个引脚、怎么让它响、怎么让它停"这些细节全部封在这里。
 * 其它文件只要 #include "Buzzer.h" 就能调用 Buzzer_ON() 这类函数。
 *
 * 硬件接线（详见 02GPIO.md 图 27，与 §3 蜂鸣器是同一个引脚，不用重新接线）：
 *   蜂鸣器模块的 I/O 接 PB12，VCC 接 3V3、GND 接 GND。
 *   本模块是"低电平触发"：引脚输出低电平就响、高电平就停。
 *
 * 对照来源：老师源码 3-5 光敏传感器控制蜂鸣器 / Hardware/Buzzer.c
 * ===================================================================== */

/**
  * @brief  蜂鸣器初始化：把 PB12 配成推挽输出，并默认输出高电平（不响）
  * @param  无
  * @return 无
  */
void Buzzer_Init(void)
{
	/*开启时钟：蜂鸣器接在 GPIOB 上，所以开 GPIOB 的时钟
	 *（时钟没开，外设相当于断电，后面配了也没用）*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;						//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//模式：推挽输出（Out_PP = Output Push-Pull）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//引脚：PB12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度：50MHz

	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB12初始化为推挽输出

	/*设置初始化后的默认电平：输出高电平 = 蜂鸣器不响
	 * 这样一上电是安静的，而不是莫名其妙一直响*/
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

/**
  * @brief  蜂鸣器开启（PB12 输出低电平 → 响）
  * @param  无
  * @return 无
  */
void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//设置PB12引脚为低电平 → 响
}

/**
  * @brief  蜂鸣器关闭（PB12 输出高电平 → 停）
  * @param  无
  * @return 无
  */
void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);		//设置PB12引脚为高电平 → 停
}

/**
  * @brief  蜂鸣器状态翻转：响的变停、停的变响
  * @param  无
  * @return 无
  * @note   先读"输出数据寄存器"里 PB12 当前的值，再反过来设置。
  *         注意读的是"我上次让它输出什么"，不是引脚上的实际电平。
  */
void Buzzer_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)		//当前输出低电平（响）
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);						//则改为高电平（停）
	}
	else														//否则当前输出高电平（停）
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);						//则改为低电平（响）
	}
}
