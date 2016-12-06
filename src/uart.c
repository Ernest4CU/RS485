#include "uart.h"
#include "lcd1602.h"
#include "Sys_Config.h"
#include <msp430x14x.h>

#include "const_str.h"
#include "type.h"

uint8 uart0_rx_buf[UART0_RX_BUFLEN];//串口0的读buff区

uint8 uart0_echo=1;
uint8 uart0_rx_end=1;
signed char receTimeOut=-1;
signed char uart0_commands=0;//记录命令个数

uint8 Os_uart0_cmd_nln=1;
uint8 Os_uart0_cmd_nll=1;
uint8 Os_uart0_cmd_len=0;
uint8 OsUart0CmdBuff[UART0CMDLEN];//串口0命令buff区


void UART0_Init()
{

/*************************************************************
 * 串口0初始化 9600 偶校验 1停止位
 *************************************************************/
  U0CTL|=SWRST+PEV+PENA;               //复位SWRST
  U0CTL|=CHAR+PEV+PENA;                //8位数据模式
  U0TCTL|=SSEL1;              //SMCLK为串口时钟
  U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //微调寄存器为0，波特率9600bps
  ME1|=UTXE0;                 //UART0发送使能
  ME1|=URXE0;                 //UART0接收使能
  U0CTL&=~SWRST;
  IE1|=URXIE0;                //接收中断使能位
  
  P3SEL|= BIT4;               //设置IO口为普通I/O模式
  P3DIR|= BIT4;               //设置IO口方向为输出
  P3SEL|= BIT5;
}

void UART0_CLOSE()
{
	  U0CTL=0X00;               //复位SWRST
	  U0TCTL=0X00;              //SMCLK为串口时钟
	  ME1=0X00;

	  P3SEL&= ~(BIT4+BIT5);               //设置IO口为普通I/O模式
	  P3DIR|= BIT4+BIT5;               //设置IO口方向为输出

}

void Uart0_Send_Byte(unsigned char data)
{
	while((IFG1&UTXIFG0)==0);          //发送寄存器空的时候发送数据
	U0TXBUF=data;
}


void UART0_SendString(unsigned char *s)
{
	while(*s!=0){
		Uart0_Send_Byte(*s);
		s++;
	}
}


void Uart0_Send_Uint32(uint32 data)
{
	uint8 temp=0;
	temp=data/1000000000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%1000000000/100000000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%100000000/10000000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%10000000/1000000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%1000000/100000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%100000/10000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%10000/1000;
	Uart0_Send_Byte(temp|0x30);
	temp=data%1000/100;
	Uart0_Send_Byte(temp|0x30);
	temp=data%100/10;
	Uart0_Send_Byte(temp|0x30);
	temp=data%10;
	Uart0_Send_Byte(temp|0x30);
}


void displayAll()
{
	uint8 i=0;
	for(i=0;i<Os_uart0_cmd_len;i++)
	{
		LCD_write_char(LcdLine2,i,OsUart0CmdBuff[i]);
	}
}

/*串口0中断*/
#pragma vector=USART0RX_VECTOR
interrupt void UART0_RX_ISR(void)
{
//	uint8 i=0;
	receTimeOut=30;
	//来数据就接收到uart0_rx_buf[UART0_RX_BUFLEN]中
	uart0_rx_buf[uart0_rx_end++]=U0RXBUF;

	if(uart0_rx_end==UART0_RX_BUFLEN)
	{  //如果出现环形存储，则还需标志
		uart0_rx_end=0;
	}

//	uint8 temp=U0RXBUF;
//	LCD_write_char(LcdLine2,1,temp);
//	Uart0_Send_Byte(temp);
}


void HexToASCII(unsigned char tt)
{
	unsigned char temp = 0x00;
	Uart0_Send_Byte(0x30);
	Uart0_Send_Byte('x');
	temp = (tt >> 4) & 0x0f;
	if (temp < 10) {
		Uart0_Send_Byte(temp | 0x30);
	} else {
		Uart0_Send_Byte(temp + 55);
	}
	temp = tt & 0x0f;
	if (temp < 10) {
		Uart0_Send_Byte(temp | 0x30);
	} else {
		Uart0_Send_Byte(temp + 55);
	}
}

uint8 ASCIIToHex(uint8 t1,uint8 t2) //将ascii码转换为hex值并返回
{
	uint8 temp=0x00;
	if((t1>=0x30)&&(t1<=0x39)){
		temp=(t1&0x0f)<<4;
	}else if ((t1>0x40)&&(t1<=0x47)) {
		temp=(t1-55)<<4;
	}else{
		//可以输入报错信息，如请输入0-f之间的数
	}

	if((t2>=0x30)&&(t2<=0x39)){
		temp|=t2&0x0f;
	}else if ((t2>0x40)&&(t2<=0x47)) {
		temp|=t2-55;
	}else{
		//可以输入报错信息，如请输入0-f之间的数
	}
	return temp;
}

void Uart0_HexToASCII2(uint8 tt)
{
	unsigned char temp = 0x00;
	temp = (tt >> 4) & 0x0f;
	if (temp < 10) {
		Uart0_Send_Byte(temp | 0x30);
	} else {
		Uart0_Send_Byte(temp + 55);
	}
	temp = tt & 0x0f;
	if (temp < 10) {
		Uart0_Send_Byte(temp | 0x30);
	} else {
		Uart0_Send_Byte(temp + 55);
	}
}

void uart0_get_cmdadd()
{
	Os_uart0_cmd_nll=Os_uart0_cmd_nln;
	Os_uart0_cmd_nln=uart0_rx_end;
	if(Os_uart0_cmd_nln>Os_uart0_cmd_nll){
		Os_uart0_cmd_len=Os_uart0_cmd_nln-Os_uart0_cmd_nll;
	}else if (Os_uart0_cmd_nln<Os_uart0_cmd_nll) {
		Os_uart0_cmd_len=UART0_RX_BUFLEN-Os_uart0_cmd_nll+Os_uart0_cmd_nln;
	}
}

void uart0_get_cmd()
{
	uint8 i=0;
	uint8 j=0;
	if(Os_uart0_cmd_nln>Os_uart0_cmd_nll){
		for(i=Os_uart0_cmd_nll;i<Os_uart0_cmd_nln;i++){
				OsUart0CmdBuff[j++]=uart0_rx_buf[i];
			}
	}else if(Os_uart0_cmd_nln<Os_uart0_cmd_nll){
		for(i=Os_uart0_cmd_nll;i<UART0_RX_BUFLEN;i++){
				OsUart0CmdBuff[j++]=uart0_rx_buf[i];
			}
		for(i=0;i<Os_uart0_cmd_nln;i++){
				OsUart0CmdBuff[j++]=uart0_rx_buf[i];
			}
	}else {
		;
	}
}

