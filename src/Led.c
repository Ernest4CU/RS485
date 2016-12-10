/*
 * Led.c
 *
 *  Created on: 2016Äê12ÔÂ7ÈÕ
 *      Author: 58255
 */
#include "Led.h"

void LED_Init()
{
	P6DIR=0xff;
	P6OUT=0xff;
}



