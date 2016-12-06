#ifndef _UART_H_
#define _UARTH_


#define baud_h    0x03           //提取高位
#define baud_l        0x41              //低位

#define UART1_RX_BUFLEN 30
//#define UART0_TX_BUFLEN 40
#define UART1CMDLEN 10

extern unsigned char work_mode;
extern signed char uart1_commands;
extern signed char uart1_receTimeOut;
//const unsigned char root_code[]="fdsa";

void UART1_Init();
void UART1_CLOSE();
void Uart1_Send_Byte(unsigned char data);
void UART1_SendString(unsigned char *s);
void Uart1_Send_Uint32(unsigned long data);

void uart1_get_cmd();
void uart1_get_cmdadd();
void uart1_displayAll();
#endif
