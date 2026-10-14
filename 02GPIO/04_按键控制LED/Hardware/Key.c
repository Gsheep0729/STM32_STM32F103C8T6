#include "stm32f10x.h"                  // Device header
#include "Delay.h"                      // 消抖要用到 Delay_ms()

/* =====================================================================
 * 按键模块 —— 具体实现
 * ---------------------------------------------------------------------
 * 把"按键接在哪个引脚、怎么读、怎么消抖"全部封在这里。
 * 其它文件只要 #include "Key.h" 就能调用 Key_GetNum()。
 *
 * 硬件接线（详见 02GPIO.md 图 24）：
 *   按键1 一端接 PB1、另一端接 GND；按键2 一端接 PB11、另一端接 GND。
 *   引脚配成"上拉输入"：松开时内部上拉电阻把电平拉高（读到 1），
 *   按下时引脚被接到 GND（读到 0）。所以"读到 0"才算按下。
 *
 * 对照来源：老师源码 3-4 按键控制LED / Hardware/Key.c
 * ===================================================================== */

/**
  * @brief  按键初始化：把 PB1、PB11 配成"上拉输入"
  * @param  无
  * @return 无
  */
void Key_Init(void)
{
	/*开启时钟：按键接在 GPIOB 上，所以开 GPIOB 的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;						//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//模式：上拉输入（IPU = Input Pull-Up）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;		//引脚：PB1 和 PB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度：50MHz

	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB1和PB11初始化为上拉输入
}

/**
  * @brief  读取键码
  * @param  无
  * @return 1 = 按键1被按下；2 = 按键2被按下；0 = 没有按键按下
  * @note   本函数是"阻塞式"的：如果一直按住不放，会卡在 while 里等松手，
  *         这期间主循环后面的代码都不会执行。这是老师这一版的写法。
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码为 0（表示没有按键按下）

	/*先判断按键1（PB1）*/
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			//上拉输入，读到 0 说明被按下
	{
		Delay_ms(20);											//延时 20ms，躲开按下瞬间的抖动
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);	//一直等到松手为止
		Delay_ms(20);											//松手后再躲一次抖动
		KeyNum = 1;												//记录键码为 1
	}

	/*再判断按键2（PB11），逻辑与上面完全一样，只是引脚和键码不同*/
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)			//读到 0 说明按键2被按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);	//等待松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												//记录键码为 2
	}

	return KeyNum;			//返回键码；两个 if 都没进时返回默认值 0
}
