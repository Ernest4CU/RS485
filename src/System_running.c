#include "System_running.h"

#include <msp430f149.h>
#include "uart.h"
#include "uart1.h"
#include "Flash.h"
#include "lcd1602.h"
#include "Sys_Config.h"
#include "type.h"


void Sys_loop()
{
	LCD_clear();
//	LCD_write_str(LcdLine1,0,"Communication:");

////	uint8 tem[3]={0x8a,0x9b,0x7c};
//	uint8 tt=0x88;
//	Flash_WriteByte(&tt,1,0x107d);
//	/****************************
//	 * 测试代码，用lcd1602显示写入后的数据
//	 */
//	LCD_Hex8ToAscii(LcdLine2,0,*(uint8 *)0x107e);
//	LCD_Hex8ToAscii(LcdLine2,6,*(uint8 *)0x107d);
//	LCD_Hex8ToAscii(LcdLine2,12,*(uint8 *)0x107c);
	while(1){
		if(uart0_commands>0){
			uart0_get_cmd();
			displayAll();
			LCD_write_char(LcdLine2,13,uart0_commands|0x30);
			uart0_commands--;
		}
		if(uart1_commands>0){
			if(uart1_get_cmd()){
				LCD_write_str(LcdLine2,0,"ERROR");
			}
			else{

				uart1_displayAll();
				Uart1_cmd_hander();
				Uart1_SendTxBuff(Os_uart1_rsp_len);
			}
//			LCD_write_char(LcdLine2,15,uart1_commands|0x30);
			uart1_commands--;
		}
	}
}



