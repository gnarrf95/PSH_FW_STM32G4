/*
 * Calibration.h
 *
 *  Created on: Apr 9, 2025
 *      Author: manue
 */

#ifndef CALIBRATION_CALIBRATION_H_
#define CALIBRATION_CALIBRATION_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	float linearGradient;
	float zeroOffset;

	float conversionFactor;

	float zeroOffsetRaw;
} Calibration_t;



void Calibration_Calculate(Calibration_t *pThis);
float Calibration_Process(Calibration_t *pThis, float value);
float Calibration_ProcessRaw(Calibration_t *pThis, float rawValue);



#ifdef __cplusplus
}
#endif

#endif /* CALIBRATION_CALIBRATION_H_ */
