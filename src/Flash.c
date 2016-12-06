/*
 * Flash.c
 *
 *  Created on: 2016年3月12日
 *      Author: ICUI4CU
 */

#include "Flash.h"
#include <msp430f149.h>
#include "lcd1602.h"
#include "type.h"

void FlashInit()
{
	FCTL2=FWKEY+FSSEL_1+FN3+FN4;	//分频为320kHz
}

/***************************************************************************
 * 段擦除
 * adr:要擦除的段内的任一地址
 ***************************************************************************/
void FlashErase(uint16 Adr)
{
	uint8 *p0;
	FCTL1=FWKEY+ERASE;
	FCTL3=FWKEY;
	while(FlashBusy()==1);
	p0=(uint8 *)Adr;
	*p0=0;
	while(FlashBusy()==1);
	FCTL3=FWKEY+LOCK;
}

/***************************************************************************
 * 测试FLASH是否忙
 * 返回值：1：忙；2：不忙
 ***************************************************************************/
uint8 FlashBusy()
{
	if((FCTL3&BUSY)==BUSY)
		return 1;
	else
		return 0;
}

/******************************************************************************
 * 读字节
 * Adr:需要读取位置的地址，注意：不是指针
 ******************************************************************************/
uint8 Flash_ReadByte(uint16 Adr)
{
	return *(uint8 *)Adr;
}


/******************************************************************************
 * 向Flash中写一字节
 * Adr:源数据存储的首地址，注意：不是指针，是地址（数组可传数组名，变量可传变量地址（&val））
 * Data_cnt:需要写入数据的字节数
 * Target_Adr:数据将要写入Flash的地址，尽量写在信息存储区（inf_B:0x1000-0x107f inf_A:0x1080-0x10ff）,
 * 			0x1100-0xffff为主flash区，一般存放程序，最好不要修改
 * 注意：需要写入的数据应全部在同一段中，如果不全在同一段中则会有部分数据写入失败。
 * flash中的地址是采用由高到低的方式计算的，对于一个地址，基址是该段的地址的最大值，变址值地址相对于基址的差值（例如:0x107d的基址就是0x107f,变址就是2）
 ******************************************************************************/
void Flash_WriteByte(uint8 *Adr,uint16 Data_cnt,uint16 Target_Adr)
{
	uint16 Adr_f=0xffff;
	uint16 Adr_r=0x0000;
	uint8 *prt_adr;
	uint8 *src_adr;
	uint16 i;
	prt_adr=(uint8 *)Target_Adr;
	src_adr=Adr;
	if((Target_Adr<0x1100)&&(Target_Adr>0x0fff)){//向信息段插入数据
		//需要计算Adr_f和Adr_r
		Adr_f=0x10ff-((0x10ff-Target_Adr)/128)*128;
		Adr_r=(0x10ff-Target_Adr)%128;
		//调用Flash_PrtUnWB保护不需改变的数据并且使需要改变的数据可修改。
		Flash_PrtUnWB(Adr_f,128,Adr_r,Data_cnt);
	}else if((Target_Adr<=0xffff)&&(Target_Adr>0x10ff)){//向主程序端插入数据，尽量不要（既最好不要向flash 0x1100到0xffff段中修改数据）
		//需要计算Adr_f和Adr_r
		Adr_f=0xffff-((0xffff-Target_Adr)/512)*512;
		Adr_r=(0xffff-Target_Adr)%512;
		//调用Flash_PrtUnWB保护不需改变的数据并且使需要改变的数据可修改。
		Flash_PrtUnWB(Adr_f,512,Adr_r,Data_cnt);
	}else{

	}
	//向flash中写入数据
	FCTL1=FWKEY+WRT;
	FCTL3=FWKEY;
	while(FlashBusy()==1);
	for(i=0;i<Data_cnt;i++){
		*prt_adr--=*src_adr++;
	}
	while(FlashBusy()==1);
	FCTL1=FWKEY;
	FCTL3=FWKEY+LOCK;
}

void Flash_PrtUnWB(uint16 Prt_Adrf,uint16 Prt_cnt,uint16 Prt_Adrr,uint16 DataCnt)// Protect Unwrite byte
{
	uint16 i=0;
	uint8 *Flash_ptrAdr;
	Flash_ptrAdr=(uint8 *)Prt_Adrf;
	//1.保护该字段数据，将全字段数据copy到数组中
	uint8 temp_bk[512];
	for(i=0;i<Prt_cnt;i++){
		temp_bk[i]=Flash_ReadByte(i+Prt_Adrf-Prt_cnt+1);
	}
	//2.擦除改字段
	FlashErase(Prt_Adrf);
	//3.将不需要修改的数据写回flash
	FCTL1=FWKEY+WRT;
	FCTL3=FWKEY;
	while(FlashBusy()==1);
	for(i=0;i<Prt_cnt;i++){
		if(!((i>=Prt_Adrr)&&(i<(Prt_Adrr+DataCnt)))){//if(!((i>=Prt_Adrr)&&(i<(Prt_Adrr+DataCnt))))
			*Flash_ptrAdr=temp_bk[Prt_cnt-i-1];
		}
		Flash_ptrAdr--;
	}
	while(FlashBusy()==1);
	FCTL1=FWKEY;
	FCTL3=FWKEY+LOCK;
}
