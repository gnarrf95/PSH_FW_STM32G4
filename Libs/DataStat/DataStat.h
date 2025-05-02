/*
 * DataStat.h
 *
 *  Created on: May 1, 2025
 *      Author: manue
 */

#ifndef DATASTAT_DATASTAT_H_
#define DATASTAT_DATASTAT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	float latest;
	float mean;
	float min;
	float max;

	const uint32_t averagingDepth;
	uint32_t averagingCounter;
} DataStat_t;



void DataStat_Process(DataStat_t *pThis, float newValue);
bool DataStat_AveragingValid(DataStat_t *pThis);

void DataStat_Reset(DataStat_t *pThis);



#ifdef __cplusplus
}
#endif

#endif /* DATASTAT_DATASTAT_H_ */
