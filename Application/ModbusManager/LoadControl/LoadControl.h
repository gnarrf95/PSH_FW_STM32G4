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



#define MODBUSMANAGER_LOADCONTROL_SETVOLTAGE				0x0000
#define MODBUSMANAGER_LOADCONTROL_SETCURRENT				0x0002
#define MODBUSMANAGER_LOADCONTROL_SETPOWER					0x0004
#define MODBUSMANAGER_LOADCONTROL_SETRESISTANCE				0x0006

#define MODBUSMANAGER_LOADCONTROL_VOLTAGELIMIT				0x0100
#define MODBUSMANAGER_LOADCONTROL_CURRENTLIMIT				0x0102
#define MODBUSMANAGER_LOADCONTROL_POWERLIMIT				0x0104
#define MODBUSMANAGER_LOADCONTROL_TEMPERATURELIMIT			0x0106

#define MODBUSMANAGER_LOADCONTROL_CONTROLREGISTER			0x0F00
#define MODBUSMANAGER_LOADCONTROL_STATUSREGISTER			0x0F01

#define MODBUSMANAGER_LOADCONTROL_DOUBLEBUFFER_TRIGGER		MODBUSMANAGER_LOADCONTROL_SETVOLTAGE



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
