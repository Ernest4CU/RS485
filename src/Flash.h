#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_
#include "type.h"


void FlashInit();	//flash��ʼ��
void FlashErase(uint16 adr);//����Flash�ĶΣ�ֻ�轫���������ַ���룬���ɲ����������ݣ��������ݼ����öδ洢��ȫ����1.
uint8 FlashBusy();	//��æ
uint8 Flash_ReadByte(uint16 Adr); // ���ֽ�
/******************************************************************************
 * ��Flash��дһ�ֽ�
 * Adr:Դ���ݴ洢���׵�ַ��ע�⣺����ָ�룬�ǵ�ַ������ɴ��������������ɴ�������ַ��&val����
 * Data_cnt:��Ҫд�����ݵ��ֽ���
 * Target_Adr:���ݽ�Ҫд��Flash�ĵ�ַ������д����Ϣ�洢����inf_B:0x1000-0x107f inf_A:0x1080-0x10ff��,
 * 			0x1100-0xffffΪ��flash����һ���ų�����ò�Ҫ�޸�
 * ע�⣺��Ҫд�������Ӧȫ����ͬһ���У������ȫ��ͬһ��������в�������д��ʧ�ܡ�
 * flash�еĵ�ַ�ǲ����ɸߵ��͵ķ�ʽ����ģ�����һ����ַ����ַ�Ǹöεĵ�ַ�����ֵ����ֵַ��ַ����ڻ�ַ�Ĳ�ֵ������:0x107d�Ļ�ַ����0x107f,��ַ����2��
 * �������ô˺���������tem������3��������flash��0x107eΪ�׵�ַ����д�룬��tem[0]�����0x107e,tem[1]�����0x10ed,tem[2]�����0x10ec,
 * 	���������������࣬�����δ洢��ȥ��ֱ�����ν��������ݱ��뱣֤�洢��ͬһ���У����㷽��ΪTarget_Adr��Target_Adr+Data_cnt���뱣֤��ͬһ��flash��
 ******************************************************************************/
void Flash_WriteByte(uint8 *Adr,uint16 Data_cnt,uint16 Target_Adr);
void Flash_PrtUnWB(uint16 Prt_Adrf,uint16 Prt_cnt,uint16 Prt_Adrr,uint16 DataCnt);// Protect Unwrite byte
#endif /* SRC_FLASH_H_ */
