/*
 * DataStat.c
 *
 *  Created on: May 1, 2025
 *      Author: manue
 */

#include <DataStat/DataStat.h>
#include <stddef.h>
#include <math.h>



//------------------------------------------------------------------------------
//
void DataStat_Process(DataStat_t *pThis, float newValue)
{
	if (pThis == NULL)
	{
		return;
	}

	pThis->latest = newValue;

	if (newValue > pThis->max)
	{
		pThis->max = newValue;
	}
	if (newValue < pThis->min)
	{
		pThis->min = newValue;
	}

	if (pThis->averagingDepth != 0)
	{
		if (pThis->averagingCounter == 0)
		{
			pThis->mean = newValue;
		}
		else
		{
			const float averagingFactor = 1.0f / (float)pThis->averagingDepth;
			pThis->mean = (pThis->mean * (1.0f - averagingFactor)) + (newValue * averagingFactor);
		}

		if (pThis->averagingCounter < pThis->averagingDepth)
		{
			pThis->averagingCounter++;
		}
	}
}

//------------------------------------------------------------------------------
//
bool DataStat_AveragingValid(DataStat_t *pThis)
{
	if (pThis == NULL)
	{
		return false;
	}

	return (pThis->averagingCounter >= pThis->averagingDepth);
}

//------------------------------------------------------------------------------
//
void DataStat_Reset(DataStat_t *pThis)
{
	if (pThis == NULL)
	{
		return;
	}

	pThis->latest = 0.0f;
	pThis->mean = 0.0f;
	pThis->min = INFINITY;
	pThis->max = -INFINITY;

	pThis->averagingCounter = 0;
}


