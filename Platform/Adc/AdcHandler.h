/*
 * AdcHandler.h
 *
 *  Created on: Apr 10, 2025
 *      Author: manue
 */

#ifndef ADC_ADCHANDLER_H_
#define ADC_ADCHANDLER_H_

#include <stdint.h>
#include <stdbool.h>

#include <main.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	float vdda;
	float voltage;
	float current;
	float powerAverage;
	float powerPrecise;
	float resistance;
	float tempFet;
	float tempMcu;

	int64_t chargeCounter;
	int64_t energyCounter;
} AdcHandler_Data_t;



ErrorStatus AdcHandler_Init(void);
float AdcHandler_GetVdda(void);

bool AdcHandler_Handle(void);
bool AdcHandler_NewData(void);
bool AdcHandler_GetData(AdcHandler_Data_t *pDataBuffer);



#ifdef __cplusplus
}
#endif

#endif /* ADC_ADCHANDLER_H_ */
