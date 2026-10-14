#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
 * @brief 主函数 - 整端口输出，实现 8 路 LED 流水灯
 *
 * 硬件接线：8 个 LED 的阳极接 3V3，阴极各经 1kΩ 限流电阻接 PA0~PA7，
 *           引脚输出低电平点亮、高电平熄灭（灌电流驱动，见 02GPIO.md 图 09）。
 * 实现思路：把 GPIOA 全部引脚配置为推挽输出，主循环用 GPIO_Write 一次写入
 *           整个端口的 16 位数据，靠"只有一位为 0"的位模式依次点亮 8 个 LED。
 * 对照来源：老师源码 3-2 LED流水灯（见 CLAUDE.md 第 10 节）。
 *           本仓库的差异：老师原版每步延时 100ms，这里改为 200ms 便于观察。
 *
 * @return int 返回程序执行状态，正常情况下不会返回
 */
int main(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				//GPIO引脚，赋值为所有引脚（PA0~PA15）
															//注意：PA13/PA14 是 SWDIO/SWCLK，被一并配成推挽输出后，
															//程序运行期间 SWD 调试口不可用。再次下载若报"无法连接目标"，
															//把 Keil 的 Debug → Settings → Connect 改成 under Reset，
															//或下载前按住复位键（详见 02GPIO.md 第 2.7 节）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz

	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将赋值后的结构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOA的初始化

	/*主循环，循环体内的代码会一直循环执行*/
	while (1)
	{
		/*使用GPIO_Write同时设置GPIOA所有引脚的高低电平，实现LED流水灯*/
		/*位模式中只有一位是 0（低电平点亮），其余都是 1；0 的位置依次左移一位*/
		GPIO_Write(GPIOA, ~0x0001);		//0000 0000 0000 0001 取反后 PA0 为低电平，点亮第 1 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0002);		//0000 0000 0000 0010 取反后 PA1 为低电平，点亮第 2 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0004);		//0000 0000 0000 0100 取反后 PA2 为低电平，点亮第 3 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0008);		//0000 0000 0000 1000 取反后 PA3 为低电平，点亮第 4 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0010);		//0000 0000 0001 0000 取反后 PA4 为低电平，点亮第 5 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0020);		//0000 0000 0010 0000 取反后 PA5 为低电平，点亮第 6 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0040);		//0000 0000 0100 0000 取反后 PA6 为低电平，点亮第 7 个 LED
		Delay_ms(200);					//延时200ms
		GPIO_Write(GPIOA, ~0x0080);		//0000 0000 1000 0000 取反后 PA7 为低电平，点亮第 8 个 LED
		Delay_ms(200);					//延时200ms
	}
}
