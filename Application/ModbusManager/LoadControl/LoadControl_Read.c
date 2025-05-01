/*
 * LoadControl_Read.c
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#include <ModbusManager/LoadControl/LoadControl.h>
#include <ModbusManager/Registers.h>
#include <ModbusEmbedded/modbus_buffer.h>



static ModbusManager_LoadControl_Buffer_t gLoadControl_DoubleBuffer;

static const modbus_Buffer_Datapoint_t gDatapoints[] =
{
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETVOLTAGE,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_DoubleBuffer.setVoltage,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETCURRENT,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_DoubleBuffer.setCurrent,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETPOWER,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_DoubleBuffer.setPower,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_LOADCONTROL_SETRESISTANCE,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gLoadControl_DoubleBuffer.setResistance,
		.dataSizeBytes = 4
	}
};

static const modbus_Buffer_t gDatapointArray =
{
	.pArray = gDatapoints,
	.arraySize = sizeof(gDatapoints) / sizeof(modbus_Buffer_Datapoint_t)
};



static void ModbusManager_LoadControl_FetchDoubleBuffer(void);
static modbus_Exception_e ModbusManager_LoadControl_GetControlRegister(uint16_t *pRegisterBuffer);
static modbus_Exception_e ModbusManager_LoadControl_GetStatusRegister(uint16_t *pRegisterBuffer);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_LoadControl_ReadHoldingRegisters(modbus_FunctionCode_e /* functionCode */, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	if (registerAddress == MODBUSMANAGER_LOADCONTROL_DOUBLEBUFFER_TRIGGER)
	{
		ModbusManager_LoadControl_FetchDoubleBuffer();
	}

	if (registerAddress == MODBUSMANAGER_LOADCONTROL_CONTROLREGISTER)
	{
		return ModbusManager_LoadControl_GetControlRegister(pRegisterBuffer);
	}
	else if (registerAddress == MODBUSMANAGER_LOADCONTROL_STATUSREGISTER)
	{
		return ModbusManager_LoadControl_GetStatusRegister(pRegisterBuffer);
	}
	else
	{
		return modbus_Buffer_ReadRegister(&gDatapointArray, registerAddress, pRegisterBuffer);
	}
}



//------------------------------------------------------------------------------
//
static void ModbusManager_LoadControl_FetchDoubleBuffer(void)
{
	gLoadControl_DoubleBuffer.setVoltage = 12000000;
	gLoadControl_DoubleBuffer.setCurrent = 2500000;
	gLoadControl_DoubleBuffer.setPower = 3000000;
	gLoadControl_DoubleBuffer.setResistance = 4700;

	gLoadControl_DoubleBuffer.voltageLimit = 32000000;
	gLoadControl_DoubleBuffer.currentLimit = 5100000;
	gLoadControl_DoubleBuffer.powerLimit = 90000000;
	gLoadControl_DoubleBuffer.temperatureLimit = 120000;
}

//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_LoadControl_GetControlRegister(uint16_t *pRegisterBuffer)
{
	*pRegisterBuffer = 0xDEAD;
	return MODBUS_EXCEPTION_SUCCESS;
}

//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_LoadControl_GetStatusRegister(uint16_t *pRegisterBuffer)
{
	*pRegisterBuffer = 0xBEEF;
	return MODBUS_EXCEPTION_SUCCESS;
}



