#ifndef _LCD1602_H_
#define _LCD1602_H_
#include "type.h"

#define LcdLine1 0
#define LcdLine2 1

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#define LCDDATSEL P4SEL
#define LCDDAT P4OUT
#define LCDDATDIR P4DIR

#define LCDCOMSEL P5SEL
#define LCDCOM P5OUT
#define LCDCOMDIR P5DIR

//12864/1602液晶控制管脚
#define RS_CLR	        LCDCOM &= ~BIT5           //RS置低
#define RS_SET	        LCDCOM |=  BIT5           //RS置高

#define RW_CLR	        LCDCOM &= ~BIT6           //RW置低
#define RW_SET	        LCDCOM |=  BIT6           //RW置高

#define EN_CLR	        LCDCOM &= ~BIT7           //E置低
#define EN_SET	        LCDCOM |=  BIT7           //E置高

#define PSB_CLR	        LCDCOM &= ~BIT0            //PSB置低，串口方式
#define PSB_SET	        LCDCOM |=  BIT0            //PSB置高，并口方式

#define RST_CLR	        LCDCOM &= ~BIT1            //RST置低
#define RST_SET	        LCDCOM |= BIT1             //RST置高

void Lcd_Portinit();//	初始化IO口子程序
void LCD_write_com(unsigned char com) ;//	显示屏命令写入函数
void LCD_write_data(unsigned char data) ;//	显示屏数据写入函数
void LCD_clear(void) ;//	显示屏清空显示
void LCD_write_str(unsigned char row,unsigned char column,unsigned char *s) ;//	显示屏字符串写入函数
void LCD_write_char(unsigned char row,unsigned char column,unsigned char data);//	显示屏单字符写入函数
void LCD_init(void);//	显示屏初始化函数
void LCD_Desk(void);//液晶显示界面初始化
void LCD_Hex8ToAscii(uint8 Hex8_line,uint8 Hex8_adr,uint8 Hex8_val);//将16位数据在Lcd的Hex8_adr上显示
void LCD_Hex16ToAscii(uint8 Hex16_line,uint8 Hex16_adr,uint16 Hex16_val);//将16位数据在Lcd的Hex16_adr上显示

#endif
