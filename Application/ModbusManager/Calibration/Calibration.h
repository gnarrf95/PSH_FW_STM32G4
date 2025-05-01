/*
 * Calibration.h
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#ifndef MODBUSMANAGER_CALIBRATION_CALIBRATION_H_
#define MODBUSMANAGER_CALIBRATION_CALIBRATION_H_

#include <modbus.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	int32_t voltageFactor;
	int32_t voltageOffset;
	int32_t currentFactor;
	int32_t currentOffset;
} ModbusManager_Calibration_Buffer_t;



modbus_Exception_e ModbusManager_Calibration_ReadHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer);
modbus_Exception_e ModbusManager_Calibration_WriteHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t registerValue);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_CALIBRATION_CALIBRATION_H_ */
