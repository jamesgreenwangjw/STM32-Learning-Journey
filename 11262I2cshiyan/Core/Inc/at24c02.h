/*
 * at24c02.h
 *
 *  Created on: Nov 27, 2025
 *      Author: wangjunwei
 */

#ifndef INC_AT24C02_H_
#define INC_AT24C02_H_

#include"main.h"

int8_t AT24C02_WriteOneByte(uint8_t WriteAddr , uint8_t DataToWrite);
int8_t AT24C02_ReadOneByte(uint8_t ReadAddr);



#endif /* INC_AT24C02_H_ */
