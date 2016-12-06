#include <msp430f149.h>
#include "Watchdog.h"

void Watchdog_init()
{
	WDTCTL = WDTPW + WDTHOLD;       //πÿ±’ø¥√≈π∑
}

