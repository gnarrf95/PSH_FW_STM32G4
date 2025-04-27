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



#define MODBUSMANAGER_CALIBRATON_VOLTAGECAL_FACTOR			0x1000
#define MODBUSMANAGER_CALIBRATON_VOLTAGECAL_OFFSET			0x1002
#define MODBUSMANAGER_CALIBRATON_CURRENTCAL_FACTOR			0x1004
#define MODBUSMANAGER_CALIBRATON_CURRENTCAL_OFFSET			0x1006

#define MODBUSMANAGER_CALIBRATION_CONTROLREGISTER			0x1F00

#define MODBUSMANAGER_CALIBRATION_DOUBLEBUFFER_TRIGGER		MODBUSMANAGER_CALIBRATON_VOLTAGECAL_FACTOR



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
