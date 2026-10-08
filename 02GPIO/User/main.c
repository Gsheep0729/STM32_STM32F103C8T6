#include "stm32f10x.h"                  // Device header
#include "Delay.h"

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
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//GPIO引脚，赋值为第0号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOA的初始化
	
	
	// 主循环
	while (1)
	{
		// 设置PC13引脚为高电平（熄灭LED）
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(500);
		
		// 设置PC13引脚为低电平（点亮LED）
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(500);
	}
	
}
