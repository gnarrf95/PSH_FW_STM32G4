/*
 * Calibration.c
 *
 *  Created on: Apr 10, 2025
 *      Author: manue
 */

#include <Calibration/Calibration.h>

#include <stddef.h>



//------------------------------------------------------------------------------
//
void Calibration_Calculate(Calibration_t *pThis)
{
	if (pThis == NULL)
	{
		return;
	}

	pThis->zeroOffsetRaw = pThis->zeroOffset / pThis->conversionFactor;
}

//------------------------------------------------------------------------------
//
float Calibration_Process(Calibration_t *pThis, float value)
{
	if (pThis == NULL)
	{
		return value;
	}

	return (value + pThis->zeroOffset) * pThis->linearGradient;
}

//------------------------------------------------------------------------------
//
float Calibration_ProcessRaw(Calibration_t *pThis, float rawValue)
{
	if (pThis == NULL)
	{
		return rawValue;
	}

	return (rawValue + pThis->zeroOffsetRaw) * pThis->linearGradient;
}


