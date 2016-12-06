#ifndef _LCD1602_H_
#define _LCD1602_H_
#include "type.h"

#define LcdLine1 0
#define LcdLine2 1

//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#define LCDDATSEL P4SEL
#define LCDDAT P4OUT
#define LCDDATDIR P4DIR

#define LCDCOMSEL P5SEL
#define LCDCOM P5OUT
#define LCDCOMDIR P5DIR

//12864/1602Һ�����ƹܽ�
#define RS_CLR	        LCDCOM &= ~BIT5           //RS�õ�
#define RS_SET	        LCDCOM |=  BIT5           //RS�ø�

#define RW_CLR	        LCDCOM &= ~BIT6           //RW�õ�
#define RW_SET	        LCDCOM |=  BIT6           //RW�ø�

#define EN_CLR	        LCDCOM &= ~BIT7           //E�õ�
#define EN_SET	        LCDCOM |=  BIT7           //E�ø�

#define PSB_CLR	        LCDCOM &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET	        LCDCOM |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR	        LCDCOM &= ~BIT1            //RST�õ�
#define RST_SET	        LCDCOM |= BIT1             //RST�ø�

void Lcd_Portinit();//	��ʼ��IO���ӳ���
void LCD_write_com(unsigned char com) ;//	��ʾ������д�뺯��
void LCD_write_data(unsigned char data) ;//	��ʾ������д�뺯��
void LCD_clear(void) ;//	��ʾ�������ʾ
void LCD_write_str(unsigned char row,unsigned char column,unsigned char *s) ;//	��ʾ���ַ���д�뺯��
void LCD_write_char(unsigned char row,unsigned char column,unsigned char data);//	��ʾ�����ַ�д�뺯��
void LCD_init(void);//	��ʾ����ʼ������
void LCD_Desk(void);//Һ����ʾ�����ʼ��
void LCD_Hex8ToAscii(uint8 Hex8_line,uint8 Hex8_adr,uint8 Hex8_val);//��16λ������Lcd��Hex8_adr����ʾ
void LCD_Hex16ToAscii(uint8 Hex16_line,uint8 Hex16_adr,uint16 Hex16_val);//��16λ������Lcd��Hex16_adr����ʾ

#endif
