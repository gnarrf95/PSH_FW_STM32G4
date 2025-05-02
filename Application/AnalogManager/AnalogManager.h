/*
 * AnalogManager.h
 *
 *  Created on: May 1, 2025
 *      Author: manue
 */

#ifndef ANALOGMANAGER_ANALOGMANAGER_H_
#define ANALOGMANAGER_ANALOGMANAGER_H_

#include <DataStat/DataStat.h>
#include <stdint.h>
#include <stdbool.h>
#include <main.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	uint32_t timestamp;

	DataStat_t vdda;
	DataStat_t voltage;
	DataStat_t current;
	DataStat_t power;
	DataStat_t resistance;

	DataStat_t tempFet;
	DataStat_t tempMcu;

	int64_t chargeCounter;
	int64_t energyCounter;
} AnalogManager_MeasurementData_t;



ErrorStatus AnalogManager_Init(void);
ErrorStatus AnalogManager_Handle(void);

ErrorStatus AnalogManager_InitAdc(void);
ErrorStatus AnalogManager_HandleAdc(void);

void AnalogManager_ResetMeasurementStats(void);
void AnalogManager_ResetChargeCounter(void);
void AnalogManager_ResetEnergyCounter(void);

const AnalogManager_MeasurementData_t *AnalogManager_GetMeasurementData(void);



#ifdef __cplusplus
}
#endif

#endif /* ANALOGMANAGER_ANALOGMANAGER_H_ */
