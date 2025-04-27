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



typedef enum
{
	ACCESS_NONE = 0,
	ACCESS_READONLY,
	ACCESS_WRITEONLY,
	ACCESS_READWRITE
} ModbusManager_Datapoint_Access_e;

typedef struct
{
	uint16_t startAddress;

	ModbusManager_Datapoint_Access_e access;

	uint8_t *pData;
	uint8_t dataSizeBytes;
} ModbusManager_Datapoint_t;

typedef struct
{
	const ModbusManager_Datapoint_t *pArray;
	const uint32_t arraySize;
} ModbusManager_DatapointArray_t;



modbus_Exception_e ModbusManager_Datapoint_ReadRegister(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress, uint16_t *pRegisterBuffer);
modbus_Exception_e ModbusManager_Datapoint_WriteRegister(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress, uint16_t registerValue);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_DATAPOINT_H_ */
