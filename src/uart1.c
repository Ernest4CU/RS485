#include "uart1.h"
#include "lcd1602.h"
#include "Sys_Config.h"
#include <msp430x14x.h>

#include "const_str.h"
#include "type.h"

uint8 uart1_rx_buf[UART1_RX_BUFLEN];//����0�Ķ�buff��

//uint8 uart0_echo=1;
uint8 uart1_rx_end=1;
signed char uart1_receTimeOut=-1;
signed char uart1_commands=0;//��¼�������

uint8 Os_uart1_cmd_nln=1;
uint8 Os_uart1_cmd_nll=1;
uint8 Os_uart1_cmd_len=0;
uint8 OsUart1CmdBuff[UART1CMDLEN];//����0����buff��


void UART1_Init()
{

/*************************************************************
* ����1��ʼ��
*************************************************************/
  U1CTL|=SWRST + CHAR + PEV + PENA;                //��λSWRST��8λ����ģʽ
  U1TCTL|=SSEL1;                      //SMCLKΪ����ʱ��
  U1BR1=baud_h;                       //BRCLK=8MHZ,Baud=BRCLK/N
  U1BR0=baud_l;                       //N=UBR+(UxMCTL)/8
  U1MCTL=0x00;                        //΢���Ĵ���Ϊ0��������9600bps
  ME2|=UTXE1;                         //UART1����ʹ��
  ME2|=URXE1;                         //UART1����ʹ��
  U1CTL&=~SWRST;
  IE2|=URXIE1;                        //�����ж�ʹ��λ

  P3SEL|= BIT6 + BIT7;                //����IO��Ϊ�ڶ�����ģʽ������UART����
  P3DIR|= BIT6;                       //����TXD1�ڷ���Ϊ���

  P5DIR|=BIT2;
  P5OUT&=~BIT2;
}

void UART1_CLOSE()
{
	  U1CTL=0X00;               //��λSWRST
	  U1TCTL=0X00;              //SMCLKΪ����ʱ��
	  ME2=0X00;

	  P3SEL&= ~(BIT6+BIT7);               //����IO��Ϊ��ͨI/Oģʽ
	  P3DIR|= BIT6+BIT7;               //����IO�ڷ���Ϊ���

}


void Uart1_Send_Byte(unsigned char data)
{
	P5OUT|=BIT2;
	while((IFG2&UTXIFG1)==0);          //���ͼĴ����յ�ʱ��������
	U1TXBUF=data;
	delay_ms(5);		//�л�֮ǰ���и�С��ʱ
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


/*����1�ж�*/
//#pragma vector=UART1RX_VECTOR
#pragma vector=USART1RX_VECTOR
__interrupt void UART1_RX_ISR(void)
{
	uart1_receTimeOut=30;
	//�����ݾͽ��յ�uart0_rx_buf[UART0_RX_BUFLEN]��
	uart1_rx_buf[uart1_rx_end++]=U1RXBUF;

	if(uart1_rx_end==UART1_RX_BUFLEN)
	{  //������ֻ��δ洢�������־
		uart1_rx_end=0;
	}
//  RS485_CTR0;                         //�л�������״̬
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

