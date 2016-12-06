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
	switch(TAIV)                                  //��Ҫ�ж��жϵ�����
	{
	case 2:break;
	case 4:break;
	case 10://��ʱ���ж�
	{
		if(receTimeOut==0)
		{
			/*����0*/
			receTimeOut--;
			//ȡ����
			uart0_get_cmdadd();//��ȡ�����ַ
			//���������־
			uart0_commands++;
		}
		else if(receTimeOut>0)
		{
			/*����0*/
			receTimeOut--;
		}
		else
		{

		}

		if(uart1_receTimeOut==0)
		{
			/*����1*/
			uart1_receTimeOut--;
			uart1_get_cmdadd();
			uart1_commands++;

		}
		else if(uart1_receTimeOut>0)
		{
			/*����1*/
			uart1_receTimeOut--;
		}
		else
		{

		}
	}break;
	}
}
