#include "stm32f10x.h"                  // Device header

/* =====================================================================
 * 光敏传感器模块 —— 具体实现
 * ---------------------------------------------------------------------
 * 把"传感器接在哪个引脚、怎么读"封在这里。
 * 其它文件只要 #include "LightSensor.h" 就能调用 LightSensor_Get()。
 *
 * 硬件接线（详见 02GPIO.md 图 27）：
 *   光敏模块的 DO（数字输出）接 PB13，VCC 接 3V3、GND 接 GND。
 *
 * 为什么配成"上拉输入"：
 *   模块内部用 LM393 电压比较器把"光照强度"和电位器设定的阈值做比较，
 *   输出端是"开集（开漏）"结构——它只能把引脚拉到低电平，不能主动输出高电平。
 *   所以必须靠芯片内部的"上拉电阻"把引脚拉高，才能得到确定的高电平。
 *   于是模式选 GPIO_Mode_IPU（IPU = Input Pull-Up，上拉输入）。
 *   这和 §4 按键用上拉输入是同一个道理。
 *
 * 读到 1 还是 0 对应"亮"还是"暗"，取决于模块上电位器设定的阈值：
 *   光敏电阻受光照后阻值变化，经比较器与阈值比较后输出 0/1。
 *   调电位器就能改变触发点（现象不对先调电位器，别急着改代码）。
 *
 * 对照来源：老师源码 3-5 光敏传感器控制蜂鸣器 / Hardware/LightSensor.c
 * ===================================================================== */

/**
  * @brief  光敏传感器初始化：把 PB13 配成"上拉输入"
  * @param  无
  * @return 无
  */
void LightSensor_Init(void)
{
	/*开启时钟：光敏传感器接在 GPIOB 上，所以开 GPIOB 的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;						//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//模式：上拉输入（IPU = Input Pull-Up）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;					//引脚：PB13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度：50MHz

	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB13初始化为上拉输入
}

/**
  * @brief  读取光敏传感器输出的高低电平
  * @param  无
  * @return 光敏传感器 DO 的电平，范围：0 / 1
  * @note   读的是"输入数据寄存器"，即引脚上当前实际的电平。
  */
uint8_t LightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);			//返回PB13输入寄存器的状态
}
