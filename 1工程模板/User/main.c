#include "stm32f10x.h"

int  main ()
{
	RCC->APB2ENR = 0x00000010; // 使能 GPIOC 时钟
	GPIOC->CRH = 0x00300000; // 配置 PC13 为推挽输出模式
	//GPIOC->ODR |= 0x00002000; // 配置 PC13 高电平，熄灭 LED
	GPIOC->ODR |= 0x00000000; // 配置 PC13 高电平，熄灭 LED



	// 主循环
	while (1)
	{
		/* code */
	}
	
}
//注意！最后一行为空，否则会出现报错
