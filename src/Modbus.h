/*
 * Modbus.h
 *
 *  Created on: 2016Äê12ÔÂ6ÈÕ
 *      Author: 58255
 */

#ifndef SRC_MODBUS_H_
#define SRC_MODBUS_H_

#include "type.h"
uint16 crc16(uint8 *puchMsg, uint16 usDataLen);
void reactOfcmd01(uint8 *cmd);
void reactOfcmd05(uint8 *cmd);
void reactOfcmd0f(uint8 *cmd);
#endif /* SRC_MODBUS_H_ */
