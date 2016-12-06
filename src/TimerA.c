#include <in430.h>
#include <msp430f149.h>

void TIMERA_Init(void)                                   //UP模式计数，计数周期为CCR0+1
{

//	CCTL0 = CCIE;                         // CCR0 interrupt enabled
								//
	TACTL = TASSEL_2 + MC_1 +ID_3+TAIE+TACLR;              // ACLK, upmode
	TACCR0 = 999;

}
