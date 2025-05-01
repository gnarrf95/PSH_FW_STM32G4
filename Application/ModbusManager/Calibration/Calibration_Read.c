/*
 * Calibration_Read.c
 *
 *  Created on: Apr 27, 2025
 *      Author: manue
 */

#include <ModbusManager/Calibration/Calibration.h>
#include <ModbusManager/Registers.h>
#include <ModbusEmbedded/modbus_buffer.h>



static ModbusManager_Calibration_Buffer_t gCalibration_DoubleBuffer;

static const modbus_Buffer_Datapoint_t gDatapoints[] =
{
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_VOLTAGECAL_FACTOR,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gCalibration_DoubleBuffer.voltageFactor,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_VOLTAGECAL_OFFSET,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gCalibration_DoubleBuffer.voltageOffset,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_CURRENTCAL_FACTOR,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gCalibration_DoubleBuffer.currentFactor,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_CALIBRATION_CURRENTCAL_OFFSET,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gCalibration_DoubleBuffer.currentOffset,
		.dataSizeBytes = 4
	}
};

static const modbus_Buffer_t gDatapointArray =
{
	.pArray = gDatapoints,
	.arraySize = sizeof(gDatapoints) / sizeof(modbus_Buffer_Datapoint_t)
};



static void ModbusManager_Calibration_FetchDoubleBuffer(void);
static modbus_Exception_e ModbusManager_Calibration_GetControlRegister(uint16_t *pRegisterBuffer);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_Calibration_ReadHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	if (registerAddress == MODBUSMANAGER_CALIBRATION_DOUBLEBUFFER_TRIGGER)
	{
		ModbusManager_Calibration_FetchDoubleBuffer();
	}

	if (registerAddress == MODBUSMANAGER_CALIBRATION_CONTROLREGISTER)
	{
		return ModbusManager_Calibration_GetControlRegister(pRegisterBuffer);
	}
	else
	{
		return modbus_Buffer_ReadRegister(&gDatapointArray, registerAddress, pRegisterBuffer);
	}
}



//------------------------------------------------------------------------------
//
static void ModbusManager_Calibration_FetchDoubleBuffer(void)
{

}

//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_Calibration_GetControlRegister(uint16_t *pRegisterBuffer)
{
	*pRegisterBuffer = 0xDEAD;
	return MODBUS_EXCEPTION_SUCCESS;
}


