/*
 * AnalogManager_MeasurementData.c
 *
 *  Created on: May 1, 2025
 *      Author: manue
 */

#include <AnalogManager/AnalogManager.h>
#include <Adc/AdcHandler.h>
#include <stddef.h>




#define ANALOGMANAGER_MEASUREMENTDATA_AVG_VDDA			1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_VOLTAGE		1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_CURRENT		1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_POWER			1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_RESISTANCE	1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_TEMPFET		1000UL
#define ANALOGMANAGER_MEASUREMENTDATA_AVG_TEMPMCU		10000UL



static AnalogManager_MeasurementData_t gAnalogMeasurementData =
{
	.vdda.averagingDepth 		= ANALOGMANAGER_MEASUREMENTDATA_AVG_VDDA,
	.voltage.averagingDepth 	= ANALOGMANAGER_MEASUREMENTDATA_AVG_VOLTAGE,
	.current.averagingDepth 	= ANALOGMANAGER_MEASUREMENTDATA_AVG_CURRENT,
	.power.averagingDepth 		= ANALOGMANAGER_MEASUREMENTDATA_AVG_POWER,
	.resistance.averagingDepth 	= ANALOGMANAGER_MEASUREMENTDATA_AVG_RESISTANCE,
	.tempFet.averagingDepth 	= ANALOGMANAGER_MEASUREMENTDATA_AVG_TEMPFET,
	.tempMcu.averagingDepth 	= ANALOGMANAGER_MEASUREMENTDATA_AVG_TEMPMCU
};



//------------------------------------------------------------------------------
//
ErrorStatus AnalogManager_InitAdc(void)
{
	gAnalogMeasurementData.timestamp = 0;

	AnalogManager_ResetMeasurementStats();
	AnalogManager_ResetChargeCounter();
	AnalogManager_ResetEnergyCounter();

	if (AdcHandler_Init() == ERROR)
	{
		return ERROR;
	}

	return SUCCESS;
}

//------------------------------------------------------------------------------
//
ErrorStatus AnalogManager_HandleAdc(void)
{
	if (AdcHandler_Handle() == true)
	{
		AdcHandler_Data_t adcData;
		AdcHandler_GetData(&adcData);

		gAnalogMeasurementData.timestamp = adcData.timestamp;

		gAnalogMeasurementData.chargeCounter = adcData.chargeCounter;
		gAnalogMeasurementData.energyCounter = adcData.energyCounter;

		DataStat_Process(&gAnalogMeasurementData.vdda, adcData.vdda);
		DataStat_Process(&gAnalogMeasurementData.voltage, adcData.voltage);
		DataStat_Process(&gAnalogMeasurementData.current, adcData.current);
		DataStat_Process(&gAnalogMeasurementData.power, adcData.powerPrecise);
		DataStat_Process(&gAnalogMeasurementData.resistance, adcData.resistance);

		DataStat_Process(&gAnalogMeasurementData.tempFet, adcData.tempFet);
		DataStat_Process(&gAnalogMeasurementData.tempMcu, adcData.tempMcu);
	}

	return SUCCESS;
}

//------------------------------------------------------------------------------
//
void AnalogManager_ResetMeasurementStats(void)
{
	DataStat_Reset(&gAnalogMeasurementData.vdda);
	DataStat_Reset(&gAnalogMeasurementData.voltage);
	DataStat_Reset(&gAnalogMeasurementData.current);
	DataStat_Reset(&gAnalogMeasurementData.power);
	DataStat_Reset(&gAnalogMeasurementData.resistance);
	DataStat_Reset(&gAnalogMeasurementData.tempFet);
	DataStat_Reset(&gAnalogMeasurementData.tempMcu);
}

//------------------------------------------------------------------------------
//
void AnalogManager_ResetChargeCounter(void)
{
	AdcHandler_ResetChargeCounter();
	gAnalogMeasurementData.chargeCounter = 0;
}

//------------------------------------------------------------------------------
//
void AnalogManager_ResetEnergyCounter(void)
{
	AdcHandler_ResetEnergyCounter();
	gAnalogMeasurementData.energyCounter = 0;
}

//------------------------------------------------------------------------------
//
const AnalogManager_MeasurementData_t *AnalogManager_GetMeasurementData(void)
{
	return (const AnalogManager_MeasurementData_t *)&gAnalogMeasurementData;
}


