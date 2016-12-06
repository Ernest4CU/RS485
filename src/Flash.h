#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_
#include "type.h"


void FlashInit();	//flash初始化
void FlashErase(uint16 adr);//擦除Flash的段，只需将段中任意地址传入，即可擦除整段数据，擦除数据即将该段存储区全部置1.
uint8 FlashBusy();	//检忙
uint8 Flash_ReadByte(uint16 Adr); // 读字节
/******************************************************************************
 * 向Flash中写一字节
 * Adr:源数据存储的首地址，注意：不是指针，是地址（数组可传数组名，变量可传变量地址（&val））
 * Data_cnt:需要写入数据的字节数
 * Target_Adr:数据将要写入Flash的地址，尽量写在信息存储区（inf_B:0x1000-0x107f inf_A:0x1080-0x10ff）,
 * 			0x1100-0xffff为主flash区，一般存放程序，最好不要修改
 * 注意：需要写入的数据应全部在同一段中，如果不全在同一段中则会有部分数据写入失败。
 * flash中的地址是采用由高到低的方式计算的，对于一个地址，基址是该段的地址的最大值，变址值地址相对于基址的差值（例如:0x107d的基址就是0x107f,变址就是2）
 * 例如利用此函数将数组tem（包含3个数）向flash中0x107e为首地址连续写入，则tem[0]存放在0x107e,tem[1]存放在0x10ed,tem[2]存放在0x10ec,
 * 	如果连续存的数更多，则依次存储下去，直到本段结束，数据必须保证存储在同一段中，计算方法为Target_Adr和Target_Adr+Data_cnt必须保证在同一段flash中
 ******************************************************************************/
void Flash_WriteByte(uint8 *Adr,uint16 Data_cnt,uint16 Target_Adr);
void Flash_PrtUnWB(uint16 Prt_Adrf,uint16 Prt_cnt,uint16 Prt_Adrr,uint16 DataCnt);// Protect Unwrite byte
#endif /* SRC_FLASH_H_ */
