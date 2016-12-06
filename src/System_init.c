#include "System_init.h"
#include <in430.h>
#include <msp430f149.h>
#include "Flash.h"
#include "lcd1602.h"
#include "system.h"
#include "Sys_Config.h"
#include "Watchdog.h"
#include "uart.h"
#include "uart1.h"
#include "TimerA.h"

void Sys_init()
{
	Watchdog_init();
	Clock_Init();
	LCD_init();
	TIMERA_Init();
	UART0_Init();
	UART1_Init();
	FlashInit();
	P6DIR=0xff;
	P6OUT=0xff;
	LCD_write_str(LcdLine1,0,"System Starting");
	LCD_write_str(LcdLine2,8,"...");
	delay_ms(1000);

	_EINT();
}

//IO初始化//
/*********************************************************************
 * 例如将P6口全部设置为输出模式并输出为高，如下
 * P6DIR=0xff;
 * P6OUT=0xff;
*********************************************************************/
void IO_init()
{

}




