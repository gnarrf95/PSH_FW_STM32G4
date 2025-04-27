/*
 * LUT.h
 *
 *  Created on: Apr 6, 2025
 *      Author: manue
 */

#ifndef LOOKUPTABLE_LUT_H_
#define LOOKUPTABLE_LUT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	float x;
	float y;
} LUT_Entry_t;

typedef struct
{
	LUT_Entry_t *pLutArray;
	uint32_t arraySize;
} LUT_t;



#ifdef __cplusplus
}
#endif

#endif /* LOOKUPTABLE_LUT_H_ */
