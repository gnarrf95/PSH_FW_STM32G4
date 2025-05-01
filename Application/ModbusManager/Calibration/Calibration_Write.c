/*
 * Calibration_Write.c
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#include <ModbusManager/Calibration/Calibration.h>
#include <ModbusManager/Datapoint.h>
#include <ModbusManager/Registers.h>
#include <Logger/Logger.h>



static ModbusManager_Calibration_Buffer_t gCalibration_Buffer;

static const ModbusManager_Datapoint_t gDatapoints[] =
{
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_VOLTAGECAL_FACTOR,
		.access = ACCESS_WRITEONLY,
		.pData = (uint8_t *)&gCalibration_Buffer.voltageFactor,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_VOLTAGECAL_OFFSET,
		.access = ACCESS_WRITEONLY,
		.pData = (uint8_t *)&gCalibration_Buffer.voltageOffset,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_CURRENTCAL_FACTOR,
		.access = ACCESS_WRITEONLY,
		.pData = (uint8_t *)&gCalibration_Buffer.currentFactor,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_CURRENTCAL_OFFSET,
		.access = ACCESS_WRITEONLY,
		.pData = (uint8_t *)&gCalibration_Buffer.currentOffset,
		.dataSizeBytes = 4
	}
};

static const ModbusManager_DatapointArray_t gDatapointArray =
{
	.pArray = gDatapoints,
	.arraySize = sizeof(gDatapoints) / sizeof(ModbusManager_Datapoint_t)
};



static modbus_Exception_e ModbusManager_Calibration_ProcessControlRegister(uint16_t registerValue);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_Calibration_WriteHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t registerValue)
{
	if (registerAddress == MODBUSMANAGER_CALIBRATION_CONTROLREGISTER)
	{
		return ModbusManager_Calibration_ProcessControlRegister(registerValue);
	}
	else
	{
		return ModbusManager_Datapoint_WriteRegister(&gDatapointArray, registerAddress, registerValue);
	}
}



//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_Calibration_ProcessControlRegister(uint16_t registerValue)
{
	float voltageFactor = (float)gCalibration_Buffer.voltageFactor / 1000000.0;
	float voltageOffset = (float)gCalibration_Buffer.voltageOffset / 1000000.0;
	float currentFactor = (float)gCalibration_Buffer.currentFactor / 1000000.0;
	float currentOffset = (float)gCalibration_Buffer.currentOffset / 1000000.0;

	LOG_INFO("New Control Word [ %04X ].", registerValue);
	LOG_INFO("Voltage Factor [ %.4f ].", voltageFactor);
	LOG_INFO("Voltage Offset [ %.6f V ].", voltageOffset);
	LOG_INFO("Current Factor [ %.4f ].", currentFactor);
	LOG_INFO("Current Offset [ %.6f A ].", currentOffset);

	return MODBUS_EXCEPTION_SUCCESS;
}


