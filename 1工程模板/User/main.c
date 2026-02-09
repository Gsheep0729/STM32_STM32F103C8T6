#include "stm32f10x.h"                  // Device header

/**
 * @brief 主函数 - 初始化GPIO并控制LED闪烁
 * 
 * 该函数初始化STM32的GPIOC端口，配置PC13引脚为推挽输出模式，
 * 用于控制连接到该引脚的LED灯。
 * 
 * @return int 返回程序执行状态，正常情况下不会返回
 */
int  main ()
{
	// 使能GPIOC端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// 配置GPIO初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 初始化GPIOC的第13号引脚
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	// 设置PC13引脚为高电平（熄灭LED）
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	// 设置PC13引脚为低电平（点亮LED）
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	// 主循环
	while (1)
	{
		/* code */
	}
	
}
