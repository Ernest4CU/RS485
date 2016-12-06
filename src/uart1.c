#include "uart1.h"
#include "lcd1602.h"
#include "Sys_Config.h"
#include <msp430x14x.h>

#include "const_str.h"
#include "type.h"

uint8 uart1_rx_buf[UART1_RX_BUFLEN];//串口0的读buff区

//uint8 uart0_echo=1;
uint8 uart1_rx_end=1;
signed char uart1_receTimeOut=-1;
signed char uart1_commands=0;//记录命令个数

uint8 Os_uart1_cmd_nln=1;
uint8 Os_uart1_cmd_nll=1;
uint8 Os_uart1_cmd_len=0;
uint8 OsUart1CmdBuff[UART1CMDLEN];//串口0命令buff区


void UART1_Init()
{

/*************************************************************
* 串口1初始化
*************************************************************/
  U1CTL|=SWRST + CHAR + PEV + PENA;                //复位SWRST，8位数据模式
  U1TCTL|=SSEL1;                      //SMCLK为串口时钟
  U1BR1=baud_h;                       //BRCLK=8MHZ,Baud=BRCLK/N
  U1BR0=baud_l;                       //N=UBR+(UxMCTL)/8
  U1MCTL=0x00;                        //微调寄存器为0，波特率9600bps
  ME2|=UTXE1;                         //UART1发送使能
  ME2|=URXE1;                         //UART1接收使能
  U1CTL&=~SWRST;
  IE2|=URXIE1;                        //接收中断使能位

  P3SEL|= BIT6 + BIT7;                //设置IO口为第二功能模式，启用UART功能
  P3DIR|= BIT6;                       //设置TXD1口方向为输出

  P5DIR|=BIT2;
  P5OUT&=~BIT2;
}

void UART1_CLOSE()
{
	  U1CTL=0X00;               //复位SWRST
	  U1TCTL=0X00;              //SMCLK为串口时钟
	  ME2=0X00;

	  P3SEL&= ~(BIT6+BIT7);               //设置IO口为普通I/O模式
	  P3DIR|= BIT6+BIT7;               //设置IO口方向为输出

}


void Uart1_Send_Byte(unsigned char data)
{
	P5OUT|=BIT2;
	while((IFG2&UTXIFG1)==0);          //发送寄存器空的时候发送数据
	U1TXBUF=data;
	delay_ms(5);		//切换之前先有个小延时
	P5OUT&=~BIT2;
}


void UART1_SendString(unsigned char *s)
{
	while(*s!=0){
		Uart1_Send_Byte(*s);
		s++;
	}
}

void Uart1_Send_Uint32(uint32 data)
{
	uint8 temp=0;
	temp=data/1000000000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%1000000000/100000000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%100000000/10000000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%10000000/1000000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%1000000/100000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%100000/10000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%10000/1000;
	Uart1_Send_Byte(temp|0x30);
	temp=data%1000/100;
	Uart1_Send_Byte(temp|0x30);
	temp=data%100/10;
	Uart1_Send_Byte(temp|0x30);
	temp=data%10;
	Uart1_Send_Byte(temp|0x30);
}


void uart1_displayAll()
{
	uint8 i=0;
	for(i=0;i<Os_uart1_cmd_len;i++)
	{
		LCD_write_char(LcdLine2,i,OsUart1CmdBuff[i]);
	}
}


/*串口1中断*/
//#pragma vector=UART1RX_VECTOR
#pragma vector=USART1RX_VECTOR
__interrupt void UART1_RX_ISR(void)
{
	uart1_receTimeOut=30;
	//来数据就接收到uart0_rx_buf[UART0_RX_BUFLEN]中
	uart1_rx_buf[uart1_rx_end++]=U1RXBUF;

	if(uart1_rx_end==UART1_RX_BUFLEN)
	{  //如果出现环形存储，则还需标志
		uart1_rx_end=0;
	}
//  RS485_CTR0;                         //切换到接收状态
}



void uart1_get_cmdadd()
{
	Os_uart1_cmd_nll=Os_uart1_cmd_nln;
	Os_uart1_cmd_nln=uart1_rx_end;
	if(Os_uart1_cmd_nln>Os_uart1_cmd_nll){
		Os_uart1_cmd_len=Os_uart1_cmd_nln-Os_uart1_cmd_nll;
	}else if (Os_uart1_cmd_nln<Os_uart1_cmd_nll) {
		Os_uart1_cmd_len=UART1_RX_BUFLEN-Os_uart1_cmd_nll+Os_uart1_cmd_nln;
	}
}

void uart1_get_cmd()
{
	uint8 i=0;
	uint8 j=0;
	if(Os_uart1_cmd_nln>Os_uart1_cmd_nll){
		for(i=Os_uart1_cmd_nll;i<Os_uart1_cmd_nln;i++){
				OsUart1CmdBuff[j++]=uart1_rx_buf[i];
			}
	}else if(Os_uart1_cmd_nln<Os_uart1_cmd_nll){
		for(i=Os_uart1_cmd_nll;i<UART1_RX_BUFLEN;i++){
				OsUart1CmdBuff[j++]=uart1_rx_buf[i];
			}
		for(i=0;i<Os_uart1_cmd_nln;i++){
				OsUart1CmdBuff[j++]=uart1_rx_buf[i];
			}
	}else {
		;
	}
}

