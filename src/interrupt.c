#include "Const_str.h"
#include "lcd1602.h"
#include "uart.h"
#include "uart1.h"
#include "type.h"
#include <msp430f149.h>
#pragma vector=PORT1_VECTOR
__interrupt void External_interrupt(void)
{
	Frequency_cnt++;
	P1IFG=0;
}


#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
	switch(TAIV)                                  //需要判断中断的类型
	{
	case 2:break;
	case 4:break;
	case 10://定时器中断
	{
		if(receTimeOut==0)
		{
			/*串口0*/
			receTimeOut--;
			//取命令
			uart0_get_cmdadd();//获取命令地址
			//进行命令标志
			uart0_commands++;
		}
		else if(receTimeOut>0)
		{
			/*串口0*/
			receTimeOut--;
		}
		else
		{

		}

		if(uart1_receTimeOut==0)
		{
			/*串口1*/
			uart1_receTimeOut--;
			uart1_get_cmdadd();
			uart1_commands++;

		}
		else if(uart1_receTimeOut>0)
		{
			/*串口1*/
			uart1_receTimeOut--;
		}
		else
		{

		}
	}break;
	}
}
