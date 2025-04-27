/*
 * ModbusManager.h
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#ifndef MODBUSMANAGER_COMMUNICATION_H_
#define MODBUSMANAGER_COMMUNICATION_H_

#include <Serial/SerialHandler.h>

#ifdef __cplusplus
extern "C" {
#endif



void ModbusManager_Init(SerialHandler_Instance_t *pSerial, uint8_t busAddress);
void ModbusManager_Handle(void);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_COMMUNICATION_H_ */
