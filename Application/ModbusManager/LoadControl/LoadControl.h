/*
 * LoadControl.h
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#ifndef MODBUSMANAGER_LOADCONTROL_LOADCONTROL_H_
#define MODBUSMANAGER_LOADCONTROL_LOADCONTROL_H_

#include <modbus.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	int32_t setVoltage;
	int32_t setCurrent;
	int32_t setPower;
	uint32_t setResistance;

	int32_t voltageLimit;
	int32_t currentLimit;
	int32_t powerLimit;
	int32_t temperatureLimit;
} ModbusManager_LoadControl_Buffer_t;



modbus_Exception_e ModbusManager_LoadControl_ReadHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer);
modbus_Exception_e ModbusManager_LoadControl_WriteHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t registerValue);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_LOADCONTROL_LOADCONTROL_H_ */
