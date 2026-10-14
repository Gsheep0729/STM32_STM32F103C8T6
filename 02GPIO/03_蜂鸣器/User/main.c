#include "stm32f10x.h"                  // Device header（STM32 标准外设库的总头文件）
#include "Delay.h"                      // 我们自己写的延时模块，Delay_ms() 就来自这里

/**
 * @brief 主函数 - 用 GPIO 输出驱动蜂鸣器按节拍鸣叫
 *
 * 硬件接线（详见 02GPIO.md 图 23）：
 *   蜂鸣器模块  VCC → 3V3、GND → GND、I/O → PB12
 *   模块板载丝印是"低电平触发"：PB12 输出低电平 → 蜂鸣器响；输出高电平 → 停止
 *
 * 与上一个实验（LED 流水灯）的关系：
 *   上一个是 GPIOA 的"整端口输出"；这次换成 GPIOB 的第 12 号引脚。
 *   思路完全一样，只是"用哪个端口、哪个引脚"这三处参数要跟着改。
 *
 * 对照来源：老师源码 3-3 蜂鸣器（见 CLAUDE.md 第 10 节）。
 *
 * @return int 返回程序执行状态，正常情况下不会返回
 */
int main(void)
{
	/* ===== 第 1 步：开启时钟 =====
	 * STM32 每个外设（GPIOA、GPIOB 这些端口也算）都有自己的"时钟开关"。
	 * 时钟没开，外设相当于断电，后面写进去的配置全部无效。
	 * 注意：这次用的是 GPIOB，所以这里要开 GPIOB 的时钟，不能写成 GPIOA。
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效

	/* ===== 第 2 步：配置引脚 =====
	 * GPIO_Init 需要一个"结构体"当参数，结构体里要写清楚三件事：
	 * 用哪个引脚、什么模式、什么速度。填完再交给 GPIO_Init 去改寄存器。
	 */
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量（GPIO_InitTypeDef 是库里的类型名）

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//模式：推挽输出（能主动输出高电平或低电平）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//引脚：第 12 号，也就是 PB12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度：50MHz（引脚翻转的最快速度，够用即可）

	GPIO_Init(GPIOB, &GPIO_InitStructure);					//把填好的结构体交给 GPIO_Init
															//"GPIOB + GPIO_Pin_12"合起来就是 PB12
															//函数内部会自动根据结构体参数去配置相应寄存器

	/* ===== 第 3 步：主循环，反复执行"响两声 + 停一下" =====
	 * 低电平触发，所以：GPIO_ResetBits（低电平）= 响，GPIO_SetBits（高电平）= 停。
	 * 注意这两个 API 的作用对象必须和上面初始化的端口、引脚完全一致。
	 */
	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//PB12 输出低电平 → 蜂鸣器鸣叫
		Delay_ms(100);							//响 100ms，这是第一声"滴"

		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//PB12 输出高电平 → 蜂鸣器停止
		Delay_ms(100);							//停 100ms，两声之间的间隔

		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//再次低电平 → 第二声"滴"
		Delay_ms(100);							//响 100ms

		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//PB12 输出高电平 → 蜂鸣器停止
		Delay_ms(700);							//停 700ms，比间隔久，形成"滴滴—滴滴—"的节奏
	}
}
