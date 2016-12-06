#include "System_init.h"
#include "System_running.h"

/*
 * main.c
 */
int main(void) {
	Sys_init();
	Sys_loop();
	return 0;
}
