/*
 * ModbusManager_Datapoints.h
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#ifndef MODBUSMANAGER_DATAPOINT_H_
#define MODBUSMANAGER_DATAPOINT_H_

#include <stdint.h>
#include <modbus.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	uint16_t startAddress;

	uint8_t *pData;
	uint8_t dataSizeBytes;
} ModbusManager_Datapoint_t;

typedef struct
{
	ModbusManager_Datapoint_t *pArray;
	uint32_t arraySize;
} ModbusManager_DatapointArray_t;



modbus_Exception_e ModbusManager_Datapoint_ReadRegister(ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress, uint16_t *pRegisterBuffer);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_DATAPOINT_H_ */
