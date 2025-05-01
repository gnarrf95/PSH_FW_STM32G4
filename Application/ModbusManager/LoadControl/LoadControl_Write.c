/*
 * LoadControl.c
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#include <ModbusManager/LoadControl/LoadControl.h>
#include <ModbusManager/Registers.h>
#include <ModbusEmbedded/modbus_buffer.h>
#include <Logger/Logger.h>



static ModbusManager_LoadControl_Buffer_t gLoadControl_Buffer;

static const modbus_Buffer_Datapoint_t DATAPOINTS[] =
{
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETVOLTAGE,
		.accessType = MODBUS_BUFFER_ACCESS_WRITEONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_Buffer.setVoltage,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETCURRENT,
		.accessType = MODBUS_BUFFER_ACCESS_WRITEONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_Buffer.setCurrent,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETPOWER,
		.accessType = MODBUS_BUFFER_ACCESS_WRITEONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_Buffer.setPower,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETRESISTANCE,
		.accessType = MODBUS_BUFFER_ACCESS_WRITEONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_Buffer.setResistance,
		.dataSizeBytes = 4
	}
};

static const modbus_Buffer_t gDatapointArray =
{
	.pArray = DATAPOINTS,
	.arraySize = sizeof(DATAPOINTS) / sizeof(modbus_Buffer_Datapoint_t)
};



static modbus_Exception_e ModbusManager_LoadControl_ProcessControlRegister(uint16_t registerValue);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_LoadControl_WriteHoldingRegisters(modbus_FunctionCode_e /* functionCode */, uint16_t registerAddress, uint16_t registerValue)
{
	if (registerAddress == MODBUSMANAGER_LOADCONTROL_CONTROLREGISTER)
	{
		return ModbusManager_LoadControl_ProcessControlRegister(registerValue);
	}
	else
	{
		return modbus_Buffer_WriteRegister(&gDatapointArray, registerAddress, registerValue);
	}
}



//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_LoadControl_ProcessControlRegister(uint16_t registerValue)
{
	float setVoltage = (float)gLoadControl_Buffer.setVoltage / 100.0;
	float setCurrent = (float)gLoadControl_Buffer.setCurrent / 100.0;
	float setPower = (float)gLoadControl_Buffer.setPower / 100.0;
	float setResistance = (float)gLoadControl_Buffer.setResistance * 10.0;

	float voltageLimit = (float)gLoadControl_Buffer.voltageLimit;
	float currentLimit = (float)gLoadControl_Buffer.currentLimit;
	float powerLimit = (float)gLoadControl_Buffer.powerLimit;
	float temperatureLimit = (float)gLoadControl_Buffer.temperatureLimit;

	LOG_INFO("New Control Word [ %04X ].", registerValue);
	LOG_INFO("Set Voltage [ %.1f V ].", setVoltage);
	LOG_INFO("Set Current [ %.1f A ].", setCurrent);
	LOG_INFO("Set Power [ %.1f W ].", setPower);
	LOG_INFO("Set Resistance [ %.1f R ].", setResistance);
	LOG_INFO("Voltage Limit [ %.1f V ].", voltageLimit);
	LOG_INFO("Current Limit [ %.1f A ].", currentLimit);
	LOG_INFO("Power Limit [ %.1f W ].", powerLimit);
	LOG_INFO("Temperature Limit [ %.1f °C ].", temperatureLimit);

	return MODBUS_EXCEPTION_SUCCESS;
}


