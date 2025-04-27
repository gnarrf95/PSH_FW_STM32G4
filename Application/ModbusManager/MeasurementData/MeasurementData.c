/*
 * ModbusManager_MeasurementData.c
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#include <ModbusManager/MeasurementData/MeasurementData.h>
#include <ModbusManager/Datapoint.h>
#include <Timer/TimerMicro.h>
#include <Adc/AdcHandler.h>



/*
 * Measurement Data Format:
 * timestamp 	u32		us		2R
 * vdda 		s32		uV		2R
 * voltage 		s32		uV		2R
 * current 		s32		uA		2R
 * power 		s32		uW		2R
 * tempFet 		s16		m°C		1R
 * tempMcu 		s16		m°C		1R
 * charge 		s64		uA*ms	4R
 * energy 		s64		uW*ms	4R
 */

#define MODBUSMANAGER_HOLDINGREGISTER_TIMESTAMP			0x0000
#define MODBUSMANAGER_HOLDINGREGISTER_VDDA				0x0002
#define MODBUSMANAGER_HOLDINGREGISTER_VOLTAGE			0x0004
#define MODBUSMANAGER_HOLDINGREGISTER_CURRENT			0x0006
#define MODBUSMANAGER_HOLDINGREGISTER_POWER				0x0008
#define MODBUSMANAGER_HOLDINGREGISTER_TEMPFET			0x000A
#define MODBUSMANAGER_HOLDINGREGISTER_TEMPMCU			0x000B
#define MODBUSMANAGER_HOLDINGREGISTER_CHARGECOUNT		0x000C
#define MODBUSMANAGER_HOLDINGREGISTER_ENERGYCOUNT		0x0010

#define MODBUSMANAGER_DOUBLEBUFFER_TRIGGER				MODBUSMANAGER_HOLDINGREGISTER_TIMESTAMP

static struct
{
	uint32_t timestamp;
	int32_t vdda;
	int32_t voltage;
	int32_t current;
	int32_t power;
	int16_t tempFet;
	int16_t tempMcu;
	int64_t charge;
	int64_t energy;
} gDoubleBuffer;

static const ModbusManager_Datapoint_t DATAPOINTS[] =
{
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_TIMESTAMP,
		.pData = (uint8_t *)&gDoubleBuffer.timestamp,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_VDDA,
		.pData = (uint8_t *)&gDoubleBuffer.vdda,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_VOLTAGE,
		.pData = (uint8_t *)&gDoubleBuffer.voltage,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_CURRENT,
		.pData = (uint8_t *)&gDoubleBuffer.current,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_POWER,
		.pData = (uint8_t *)&gDoubleBuffer.power,
		.dataSizeBytes = 4
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_TEMPFET,
		.pData = (uint8_t *)&gDoubleBuffer.tempFet,
		.dataSizeBytes = 2
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_TEMPMCU,
		.pData = (uint8_t *)&gDoubleBuffer.tempMcu,
		.dataSizeBytes = 2
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_CHARGECOUNT,
		.pData = (uint8_t *)&gDoubleBuffer.charge,
		.dataSizeBytes = 8
	},
	{
		.startAddress = MODBUSMANAGER_HOLDINGREGISTER_ENERGYCOUNT,
		.pData = (uint8_t *)&gDoubleBuffer.energy,
		.dataSizeBytes = 8
	}
};

static const ModbusManager_DatapointArray_t gModbusManager_MeasurementData_Array =
{
	.pArray = DATAPOINTS,
	.arraySize = sizeof(DATAPOINTS) / sizeof(ModbusManager_Datapoint_t)
};



static void ModbusManager_MeasurementData_FetchDoubleBuffer(void);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_MeasurementData_ReadHoldingRegisters(modbus_FunctionCode_e /* functionCode */, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	if (registerAddress == MODBUSMANAGER_DOUBLEBUFFER_TRIGGER)
	{
		ModbusManager_MeasurementData_FetchDoubleBuffer();
	}

	return ModbusManager_Datapoint_ReadRegister(&gModbusManager_MeasurementData_Array, registerAddress, pRegisterBuffer);
}



//------------------------------------------------------------------------------
//
static void ModbusManager_MeasurementData_FetchDoubleBuffer(void)
{
	gDoubleBuffer.timestamp = TimerMicro_GetTimestamp();

	AdcHandler_Data_t adcDataBuffer;
	AdcHandler_GetData(&adcDataBuffer);

	gDoubleBuffer.vdda = (int32_t)(adcDataBuffer.vdda * 1000000.0);
	gDoubleBuffer.voltage = (int32_t)(adcDataBuffer.voltage * 1000000.0);
	gDoubleBuffer.current = (int32_t)(adcDataBuffer.current * 1000000.0);
	gDoubleBuffer.power = (int32_t)(adcDataBuffer.powerPrecise * 1000000.0);
	gDoubleBuffer.tempFet = (int16_t)(adcDataBuffer.tempFet * 1000.0);
	gDoubleBuffer.tempMcu = (int16_t)(adcDataBuffer.tempMcu * 1000.0);
	gDoubleBuffer.charge = adcDataBuffer.chargeCounter;
	gDoubleBuffer.energy = adcDataBuffer.energyCounter;
}


