/*
 * ModbusManager_MeasurementData.c
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#include <ModbusManager/MeasurementData/MeasurementData.h>
#include <ModbusManager/Registers.h>
#include <ModbusEmbedded/modbus_buffer.h>
#include <Timer/TimerMicro.h>
#include <Adc/AdcHandler.h>



static struct
{
	uint32_t timestamp;
	int32_t vdda;
	int32_t voltage;
	int32_t current;
	int32_t power;
	uint32_t resistance;
	int32_t tempFet;
	int32_t tempMcu;
	int64_t charge;
	int64_t energy;
} gMeasurementData_DoubleBuffer;

static const modbus_Buffer_Datapoint_t gDatapoints[] =
{
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_TIMESTAMP,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.timestamp,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_VDDA,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.vdda,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_VOLTAGE,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.voltage,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_CURRENT,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.current,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_POWER,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.power,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_RESISTANCE,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.resistance,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_TEMPFET,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.tempFet,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_TEMPMCU,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.tempMcu,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_CHARGECOUNT,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.charge,
		.dataSizeBytes = 8
	},
	{
		.startAddress = MODBUSMANAGER_MEASUREMENTDATA_ENERGYCOUNT,
		.accessType = MODBUS_BUFFER_ACCESS_READONLY,
		.pDataBuffer = (uint8_t *)&gMeasurementData_DoubleBuffer.energy,
		.dataSizeBytes = 8
	}
};

static const modbus_Buffer_t gDatapointArray =
{
	.pArray = gDatapoints,
	.arraySize = sizeof(gDatapoints) / sizeof(modbus_Buffer_Datapoint_t)
};



static void ModbusManager_MeasurementData_FetchDoubleBuffer(void);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_MeasurementData_ReadHoldingRegisters(modbus_FunctionCode_e /* functionCode */, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	if (registerAddress == MODBUSMANAGER_MEASUREMENTDATA_DOUBLEBUFFER_TRIGGER)
	{
		ModbusManager_MeasurementData_FetchDoubleBuffer();
	}

	return modbus_Buffer_ReadRegister(&gDatapointArray, registerAddress, pRegisterBuffer);
}



//------------------------------------------------------------------------------
//
static void ModbusManager_MeasurementData_FetchDoubleBuffer(void)
{
	gMeasurementData_DoubleBuffer.timestamp = TimerMicro_GetTimestamp();

	AdcHandler_Data_t adcDataBuffer;
	AdcHandler_GetData(&adcDataBuffer);

	gMeasurementData_DoubleBuffer.vdda = (int32_t)(adcDataBuffer.vdda * 1000000.0);
	gMeasurementData_DoubleBuffer.voltage = (int32_t)(adcDataBuffer.voltage * 1000000.0);
	gMeasurementData_DoubleBuffer.current = (int32_t)(adcDataBuffer.current * 1000000.0);
	gMeasurementData_DoubleBuffer.power = (int32_t)(adcDataBuffer.powerPrecise * 1000000.0);
	gMeasurementData_DoubleBuffer.resistance = (uint32_t)adcDataBuffer.resistance;
	gMeasurementData_DoubleBuffer.tempFet = (int32_t)(adcDataBuffer.tempFet * 1000.0);
	gMeasurementData_DoubleBuffer.tempMcu = (int32_t)(adcDataBuffer.tempMcu * 1000.0);
	gMeasurementData_DoubleBuffer.charge = adcDataBuffer.chargeCounter;
	gMeasurementData_DoubleBuffer.energy = adcDataBuffer.energyCounter;
}


