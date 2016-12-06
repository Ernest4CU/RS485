#include "lcd1602.h"
#include "Sys_Config.h"
#include <msp430x14x.h>



//*************************************************************************
//			��ʼ��IO���ӳ���
//*************************************************************************
//void delay_ms(unsigned int x)
//{
//	volatile unsigned int i;
//	while(x--)
//		for(i=800;i>0;i--);
//}
void Lcd_Portinit()
{
	LCDDATSEL = 0x00;
	LCDDATDIR = 0xFF;                   //���ݿ����ģʽ
	LCDCOMSEL = 0x00;
	LCDCOMDIR|= BIT5 + BIT6 + BIT7;     //���ƿ�����Ϊ���ģʽ
}

//***********************************************************************
//	��ʾ������д�뺯��
//***********************************************************************
void LCD_write_com(unsigned char com) 
{	
	RS_CLR;
	RW_CLR;
	EN_SET;
	LCDDAT = com;                 //����д��˿�
	delay_ms(5);
	EN_CLR;
}

//***********************************************************************
//	��ʾ������д�뺯��
//***********************************************************************
void LCD_write_data(unsigned char data) 
{
	RS_SET;
	RW_CLR;
	EN_SET;
	LCDDAT = data;                //����д��˿�
	delay_ms(5);
	EN_CLR;
}

//***********************************************************************
//	��ʾ�������ʾ
//***********************************************************************
void LCD_clear(void) 
{
	LCD_write_com(0x01);            //����Ļ��ʾ
	delay_ms(5);
}

//***********************************************************************
//	��ʾ���ַ���д�뺯��
//***********************************************************************
void LCD_write_str(unsigned char row,unsigned char column,unsigned char *s)
{
	
    if (row == 0)
    {
    	LCD_write_com(0x80 + column);        //��һ����ʾ
    }
    else 
    {
    	LCD_write_com(0xC0 + column);        //�ڶ�����ʾ
    }
    
    while (*s) 
    {
    	LCD_write_data( *s);
    	s ++;
    }
}

//***********************************************************************
//	��ʾ�����ַ�д�뺯��
//***********************************************************************
void LCD_write_char(unsigned char row,unsigned char column,unsigned char data)
{
	
    if (row == 0)
    {
    	LCD_write_com(0x80 + column);        //��һ����ʾ
    }
    else 
    {
    	LCD_write_com(0xC0 + column);        //�ڶ�����ʾ
    }
    
    LCD_write_data(data);
}

//***********************************************************************
//	��ʾ����ʼ������
//***********************************************************************
void LCD_init(void) 
{
	Lcd_Portinit();
    LCD_write_com(0x38);		//��ʾģʽ����  
    delay_ms(5);
    LCD_write_com(0x08);		//��ʾ�ر�
    delay_ms(5);
    LCD_write_com(0x01);		//��ʾ����
    delay_ms(5);
    LCD_write_com(0x06);		//��ʾ����ƶ�����
    delay_ms(5);
    LCD_write_com(0x0C);		//��ʾ�����������
    delay_ms(5);
    LCD_clear();
}

//***********************************************************************
//      Һ����ʾ�����ʼ��
//***********************************************************************
void LCD_Desk(void)
{    
  LCD_clear();
  LCD_write_str(0,0,"The Key Number:");
  delay_ms(250);
}

/*************************************************************************
 * ��8λ������Lcd����ʾ
 *************************************************************************/
void LCD_Hex8ToAscii(uint8 Hex8_line,uint8 Hex8_adr,uint8 Hex8_val)
{
	uint8 temp=0x00;
	LCD_write_char(Hex8_line,Hex8_adr,0x30);
	LCD_write_char(Hex8_line,Hex8_adr+1,'x');
	temp = (Hex8_val >> 4) & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex8_line,Hex8_adr+2,(temp | 0x30));
	} else {
		LCD_write_char(Hex8_line,Hex8_adr+2,(temp + 55));
	}
	temp = Hex8_val  & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex8_line,Hex8_adr+3,(temp | 0x30));
	} else {
		LCD_write_char(Hex8_line,Hex8_adr+3,(temp + 55));
	}
}
/*************************************************************************
 * ��16λ������Lcd����ʾ
 *************************************************************************/
void LCD_Hex16ToAscii(uint8 Hex16_line,uint8 Hex16_adr,uint16 Hex16_val)
{
	uint8 temp=0x00;
	LCD_write_char(Hex16_line,Hex16_adr,0x30);
	LCD_write_char(Hex16_line,Hex16_adr+1,'x');
	temp = (Hex16_val >> 12) & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex16_line,Hex16_adr+2,(temp | 0x30));
	} else {
		LCD_write_char(Hex16_line,Hex16_adr+2,(temp + 55));
	}
	temp = (Hex16_val >> 8) & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex16_line,Hex16_adr+3,(temp | 0x30));
	} else {
		LCD_write_char(Hex16_line,Hex16_adr+3,(temp + 55));
	}
	temp = (Hex16_val >> 4) & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex16_line,Hex16_adr+4,(temp | 0x30));
	} else {
		LCD_write_char(Hex16_line,Hex16_adr+4,(temp + 55));
	}
	temp = Hex16_val  & 0x0f;
	if (temp < 10) {
		LCD_write_char(Hex16_line,Hex16_adr+5,(temp | 0x30));
	} else {
		LCD_write_char(Hex16_line,Hex16_adr+5,(temp + 55));
	}
}
