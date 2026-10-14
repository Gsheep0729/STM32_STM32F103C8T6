#include "stm32f10x.h"                  // Device header（STM32 标准外设库的总头文件）
#include "Delay.h"                      // 我们自己写的延时模块，Delay_ms() 就来自这里

/* =====================================================================
 * 【已禁用的旧代码】蜂鸣器实验（视频 3-3）
 * ---------------------------------------------------------------------
 * 这段就是刚才已经烧录验证通过、能正常发声的代码。
 * 现在整段用 // 注释掉保留在这里备查，没有删除。
 * 想恢复使用：把每行开头的 // 去掉，同时把下面新代码的 main() 注释掉即可。
 * ===================================================================== */

// /**
//  * @brief 主函数 - 用 GPIO 输出驱动蜂鸣器按节拍鸣叫
//  *
//  * 硬件接线（详见 02GPIO.md 图 23）：
//  *   蜂鸣器模块  VCC → 3V3、GND → GND、I/O → PB12
//  *   模块板载丝印是"低电平触发"：PB12 输出低电平 → 蜂鸣器响；输出高电平 → 停止
//  *
//  * 与上一个实验（LED 流水灯）的关系：
//  *   上一个是 GPIOA 的"整端口输出"；这次换成 GPIOB 的第 12 号引脚。
//  *   思路完全一样，只是"用哪个端口、哪个引脚"这三处参数要跟着改。
//  *
//  * 对照来源：老师源码 3-3 蜂鸣器（见 CLAUDE.md 第 10 节）。
//  *
//  * @return int 返回程序执行状态，正常情况下不会返回
//  */
// int main(void)
// {
// 	/* ===== 第 1 步：开启时钟 =====
// 	 * STM32 每个外设（GPIOA、GPIOB 这些端口也算）都有自己的"时钟开关"。
// 	 * 时钟没开，外设相当于断电，后面写进去的配置全部无效。
// 	 * 注意：这次用的是 GPIOB，所以这里要开 GPIOB 的时钟，不能写成 GPIOA。
// 	 */
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
// 															//使用各个外设前必须开启时钟，否则对外设的操作无效
//
// 	/* ===== 第 2 步：配置引脚 =====
// 	 * GPIO_Init 需要一个"结构体"当参数，结构体里要写清楚三件事：
// 	 * 用哪个引脚、什么模式、什么速度。填完再交给 GPIO_Init 去改寄存器。
// 	 */
// 	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量（GPIO_InitTypeDef 是库里的类型名）
//
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//模式：推挽输出（能主动输出高电平或低电平）
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//引脚：第 12 号，也就是 PB12
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度：50MHz（引脚翻转的最快速度，够用即可）
//
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);					//把填好的结构体交给 GPIO_Init
// 															//"GPIOB + GPIO_Pin_12"合起来就是 PB12
// 															//函数内部会自动根据结构体参数去配置相应寄存器
//
// 	/* ===== 第 3 步：主循环，反复执行"响两声 + 停一下" =====
// 	 * 低电平触发，所以：GPIO_ResetBits（低电平）= 响，GPIO_SetBits（高电平）= 停。
// 	 * 注意这两个 API 的作用对象必须和上面初始化的端口、引脚完全一致。
// 	 */
// 	while (1)
// 	{
// 		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//PB12 输出低电平 → 蜂鸣器鸣叫
// 		Delay_ms(100);							//响 100ms，这是第一声"滴"
//
// 		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//PB12 输出高电平 → 蜂鸣器停止
// 		Delay_ms(100);							//停 100ms，两声之间的间隔
//
// 		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//再次低电平 → 第二声"滴"
// 		Delay_ms(100);							//响 100ms
//
// 		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//PB12 输出高电平 → 蜂鸣器停止
// 		Delay_ms(700);							//停 700ms，比间隔久，形成"滴滴—滴滴—"的节奏
// 	}
// }

/* =====================================================================
 * 【新代码】模拟"炸弹倒计时"
 * ---------------------------------------------------------------------
 * 硬件：还是用刚接好的蜂鸣器（PB12，低电平触发），不需要改接线。
 *
 * 一次"引爆"分三段：
 *   ① 倒计时阶段  从 COUNTDOWN_SECONDS 秒开始，一秒一声短促的"滴"
 *   ② 预警阶段    只剩 WARN_SECONDS 秒时，节奏明显加快（"滴滴滴滴"）
 *   ③ 爆炸阶段    归零后先长鸣一声，再快速断续若干下，模拟爆炸的轰鸣与碎裂声
 * 之后静默一会儿，自动开始下一轮倒计时（可以反复演示）。
 *
 * 想改变倒计时的表现，只改下面那几个宏即可，逻辑代码不用动。
 *
 * 说明：有源蜂鸣器内部自带振荡源，只能发出一个固定音调，做不出真正的
 *       "爆炸声"；这里的爆炸效果是用"长鸣 + 快速断续"模拟出来的。
 *       想要真正的音效需要无源蜂鸣器 + PWM（后续定时器章节会讲）。
 * ===================================================================== */

/* ---------- 可调参数（改这里就能改变倒计时的表现） ---------- */
#define COUNTDOWN_SECONDS	10		//倒计时总秒数
#define WARN_SECONDS		3		//剩余多少秒时进入"加快节奏"的预警阶段
#define TICK_ON_MS			50		//倒计时阶段：每声"滴"响多久
#define TICK_OFF_MS			950		//倒计时阶段：两声之间的间隔（50+950=1000ms，正好一秒一声）
#define WARN_ON_MS			50		//预警阶段：每声"滴"响多久
#define WARN_OFF_MS			200		//预警阶段：两声之间的间隔（50+200=250ms，一秒四声）
#define BOOM_ON_MS			2000	//爆炸阶段：先长鸣多久
#define BOOM_BURST_TIMES	20		//爆炸阶段：长鸣之后再快速断续多少次
#define BOOM_BURST_MS		20		//爆炸阶段：断续时每段通/断各持续多久
#define RESTART_PAUSE_MS	1500	//一轮结束后静默多久，然后重新开始

/* ---------- 蜂鸣器开关的小封装 ----------
 * 把"低电平响、高电平停"这件事包成两个名字直白的函数。
 * 好处：后面写逻辑时只写 Buzzer_ON() / Buzzer_OFF()，
 *       不用每次都去记 ResetBits 和 SetBits 哪个是响、哪个是停。
 */
static void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//PB12 输出低电平 → 蜂鸣器鸣叫
}

static void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);		//PB12 输出高电平 → 蜂鸣器停止
}

int main(void)
{
	/* ===== 初始化：和上面的蜂鸣器实验完全一样 =====
	 * 只配置 PB12 一个引脚，不碰 SWD 引脚，可以放心反复下载。
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟

	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//第 12 号引脚，即 PB12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz

	GPIO_Init(GPIOB, &GPIO_InitStructure);					//实现 GPIOB 的初始化

	Buzzer_OFF();											//先确保上电时蜂鸣器是停的

	/* ===== 主循环：每循环一次 = 引爆一颗炸弹 ===== */
	while (1)
	{
		uint8_t sec;		//记录"还剩几秒"
		uint8_t i;			//爆炸阶段快速断续用的计数变量

		/* --- ① 倒计时阶段：一秒一声"滴" --- */
		for (sec = COUNTDOWN_SECONDS; sec > WARN_SECONDS; sec--)
		{
			Buzzer_ON();					//响
			Delay_ms(TICK_ON_MS);			//持续 50ms
			Buzzer_OFF();					//停
			Delay_ms(TICK_OFF_MS);			//停 950ms，合起来正好一秒一声
		}

		/* --- ② 预警阶段：剩余秒数不多了，节奏加快 --- */
		for (; sec > 0; sec--)
		{
			Buzzer_ON();					//响
			Delay_ms(WARN_ON_MS);			//持续 50ms
			Buzzer_OFF();					//停
			Delay_ms(WARN_OFF_MS);			//只停 200ms，节奏变急促
		}

		/* --- ③ 爆炸阶段 --- */
		Buzzer_ON();						//先来一声长鸣（爆炸的轰鸣）
		Delay_ms(BOOM_ON_MS);

		for (i = 0; i < BOOM_BURST_TIMES; i++)	//再快速断续若干下（碎裂的"噼啪"感）
		{
			Buzzer_OFF();
			Delay_ms(BOOM_BURST_MS);
			Buzzer_ON();
			Delay_ms(BOOM_BURST_MS);
		}

		Buzzer_OFF();						//彻底停下
		Delay_ms(RESTART_PAUSE_MS);			//静默一会儿，回到循环开头重新倒计时
	}
}
