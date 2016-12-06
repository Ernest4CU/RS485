#ifndef _UART_H_
#define _UARTH_


#define baud_h    0x03           //提取高位
#define baud_l        0x41              //低位

#define UART0_RX_BUFLEN 30
//#define UART0_TX_BUFLEN 40
#define UART0CMDLEN 10

extern unsigned char work_mode;
extern signed char uart0_commands;
extern signed char receTimeOut;
//const unsigned char root_code[]="fdsa";

void UART0_Init();
void UART0_CLOSE();
void Uart0_Send_Byte(unsigned char data);
void UART0_SendString(unsigned char *s);
void Uart0_Send_Uint32(unsigned long data);
void Uart0_HexToASCII(unsigned char tt);
void Uart0_HexToASCII2(unsigned char tt);
unsigned char ASCIIToHex(unsigned char t1,unsigned char t2); //将ascii码转换为hex值并返回
void uart0_get_cmd();
void uart0_get_cmdadd();
void displayAll();
#endif
