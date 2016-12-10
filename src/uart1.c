#include "uart1.h"

#include <msp430x14x.h>

#include "Const_str.h"
#include "lcd1602.h"
#include "Modbus.h"
#include "Sys_Config.h"

uint8 uart1_rx_buf[UART1_RX_BUFLEN];//����0�Ķ�buff��
uint8 uart1_tx_buf[UART1_TX_BUFLEN];//����0�Ķ�buff��
//uint8 uart0_echo=1;
uint8 uart1_rx_end=1;
signed char uart1_receTimeOut=-1;
signed char uart1_commands=0;//��¼�������

uint8 Os_uart1_cmd_nln=1;
uint8 Os_uart1_cmd_nll=1;
uint8 Os_uart1_cmd_len=0;
uint8 Os_uart1_rsp_len=0;//��¼��Ӧ������Ч����

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

void Uart1_SendTxBuff(uint8 len)
{
	uint8 i=0,templen=0;
	if(len>UART1_TX_BUFLEN){
		templen=UART1_TX_BUFLEN;
	}else{
		templen=len;
	}

	for(i=0;i<templen;i++){
		Uart1_Send_Byte(uart1_tx_buf[i]);
	}

	delay_ms(5);		//�л�֮ǰ���и�С��ʱ
	P5OUT&=~BIT2;
}

void Uart1_display_Uint32(uint32 data)
{
	uint8 temp=0;
	temp=data/1000000000;
	LCD_write_char(LcdLine2,0,temp|0x30);
	temp=data%1000000000/100000000;
	LCD_write_data(temp|0x30);
	temp=data%100000000/10000000;
	LCD_write_data(temp|0x30);
	temp=data%10000000/1000000;
	LCD_write_data(temp|0x30);
	temp=data%1000000/100000;
	LCD_write_data(temp|0x30);
	temp=data%100000/10000;
	LCD_write_data(temp|0x30);
	temp=data%10000/1000;
	LCD_write_data(temp|0x30);
	temp=data%1000/100;
	LCD_write_data(temp|0x30);
	temp=data%100/10;
	LCD_write_data(temp|0x30);
	temp=data%10;
	LCD_write_data(temp|0x30);
}

void uart1_displayAll()
{
//	LCD_Hex8ToAscii(LcdLine1,0,OsUart1CmdBuff[Os_uart1_cmd_len-2]);
//	LCD_Hex8ToAscii(LcdLine1,8,OsUart1CmdBuff[Os_uart1_cmd_len-1]);
//	Uart1_display_Uint32(crc16(OsUart1CmdBuff,6));
	uint8 i=0,j=0;
	uint16 crcData;
	if(Os_uart1_cmd_len>5)
	{
		for(i=0;i<5;i++)
		{
			LCD_Hex8ToAsciiWithOut0x(LcdLine1,i*3,OsUart1CmdBuff[i]);
		}
		for(i=5;i<Os_uart1_cmd_len;i++)
		{
			LCD_Hex8ToAsciiWithOut0x(LcdLine2,j++*3,OsUart1CmdBuff[i]);
		}
	}
	else{
		for(i=0;i<Os_uart1_cmd_len;i++)
			{
				LCD_Hex8ToAsciiWithOut0x(LcdLine1,i*3,OsUart1CmdBuff[i]);
			}
	}
	crcData=crc16(OsUart1CmdBuff,Os_uart1_cmd_len-2);
	LCD_Hex8ToAsciiWithOut0x(LcdLine2,9,crcData>>8);
	LCD_Hex8ToAsciiWithOut0x(LcdLine2,12,crcData);

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

uint8 uart1_get_cmd()
{
	uint8 checkCrcErr=0;
	uint16 crcData=0x0000;
	uint8 i=0;
	uint8 j=0;
	if(Os_uart1_cmd_len>UART1CMDLEN)
	{
		checkCrcErr=2;//����ȹ���
	}else{
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
		//CRCУ��
		crcData=crc16(OsUart1CmdBuff,Os_uart1_cmd_len-2);
		if(crcData!=OsUart1CmdBuff[Os_uart1_cmd_len-1]+(OsUart1CmdBuff[Os_uart1_cmd_len-2]<<8))
		{
			checkCrcErr=1;//crcУ�����
		}
		//��ַУ��
		if(OsUart1CmdBuff[0]!=DeviceAdd)
		{
			checkCrcErr=1;
		}
		uart1_tx_buf[0]=DeviceAdd;
	}

	return checkCrcErr;
}


void Uart1_cmd_hander()
{
	//������ִ������
	Uart1_cmd_parser(OsUart1CmdBuff);
	//���ָ��buff
}

void Uart1_cmd_parser(uint8 *cmd)//��ַУ��
{
	switch(*(cmd+1)){
	case 0x01:
		//��ȡled״̬
		if(Os_uart1_cmd_len!=8) //0x05�����Ϊ8�������������˳�
		{
			uart1_tx_buf[1]=*(cmd+1)|0x80;
			uart1_tx_buf[2]=0x01;
			set_uart1_tx_crc(3,3);
			Os_uart1_rsp_len=5;
			break;
		}
		reactOfcmd01((uint8 *)(cmd+2));
		break;
	case 0x05:
		//����LED״̬
		if(Os_uart1_cmd_len!=8) //0x05�����Ϊ8�������������˳�
		{
			uart1_tx_buf[1]=*(cmd+1)|0x80;
			uart1_tx_buf[2]=0x01;
			set_uart1_tx_crc(3,3);
			Os_uart1_rsp_len=5;
			break;
		}
		uart1_tx_buf[1]=0x05;//����������
		reactOfcmd05((uint8 *)(cmd+2));
		break;
	case 0x0F:
		//���ö��LED״̬
		if(Os_uart1_cmd_len!=10) //0x05�����Ϊ8�������������˳�
		{
			uart1_tx_buf[1]=*(cmd+1)|0x80;
			uart1_tx_buf[2]=0x01;
			set_uart1_tx_crc(3,3);
			Os_uart1_rsp_len=5;
			break;
		}
		reactOfcmd0f((uint8 *)(cmd+2));
		break;
	default:
		//�޴˹����뷴��
		uart1_tx_buf[1]=*(cmd+1)|0x80;
		uart1_tx_buf[2]=0x01;
		set_uart1_tx_crc(3,3);
		Os_uart1_rsp_len=5;
		break;
	}
}

void Uart1_cmd_parser_1(uint8 *cmd)//����У��
{
	switch(*cmd){
		case 0x01:
			//��ȡled״̬
			Uart1_SendTxBuff(8);
			break;
		case 0x02:
			//����LED״̬

			break;
		default:
			break;
	}
}

void set_uart1_tx_buff(uint8 temp,uint8 add)
{
	uart1_tx_buf[add]=temp;
}

void set_uart1_tx_crc(uint8 add,uint8 len)
{
	uint16 checkCrc=crc16(uart1_tx_buf,len);
	uart1_tx_buf[add+1]=checkCrc;
	uart1_tx_buf[add]=checkCrc>>8;
}
