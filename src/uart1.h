#ifndef _UART_H_
#define _UARTH_

#include "type.h"
#define baud_h    0x03           //提取高位
#define baud_l        0x41              //低位

#define UART1_RX_BUFLEN 30
#define UART1_TX_BUFLEN 30
//#define UART0_TX_BUFLEN 40
#define UART1CMDLEN 10


extern unsigned char work_mode;
extern signed char uart1_commands;
extern signed char uart1_receTimeOut;
extern uint8 Os_uart1_rsp_len;//记录相应数据有效长度
//const unsigned char root_code[]="fdsa";

void UART1_Init();
void UART1_CLOSE();
void Uart1_Send_Byte(unsigned char data);
void UART1_SendString(unsigned char *s);
void Uart1_Send_Uint32(unsigned long data);
void Uart1_SendTxBuff(uint8 len);
uint8 uart1_get_cmd();
void uart1_get_cmdadd();
void uart1_displayAll();

void Uart1_cmd_hander();
void Uart1_cmd_parser(uint8 *cmd);
void Uart1_cmd_parser_1(uint8 *cmd);
void Uart1_cmd_parser_5(uint8 *cmd);

void set_uart1_tx_buff(uint8 temp,uint8 add);
void set_uart1_tx_crc(uint8 add,uint8 len);
#endif
