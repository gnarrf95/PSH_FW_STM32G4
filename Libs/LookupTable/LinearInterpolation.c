/*
 * LinearInterpolation.c
 *
 *  Created on: Apr 6, 2025
 *      Author: manue
 */

#include <LookupTable/LinearInterpolation.h>

#include <stddef.h>
#include <math.h>

#include <main.h>



//------------------------------------------------------------------------------
//
float LUT_Interpolate(const LUT_t *pLut, float input)
{
	assert_param(pLut != NULL);
	assert_param(pLut->pLutArray != NULL);

	if (input >= pLut->pLutArray[pLut->arraySize - 1].x)
	{
		return pLut->pLutArray[pLut->arraySize - 1].y;
	}

	for (uint32_t ctr = 0; ctr < (pLut->arraySize - 1); ctr++)
	{
		if (input == pLut->pLutArray[ctr].x)
		{
			return pLut->pLutArray[ctr].y;
		}

		if (
			(ctr < pLut->arraySize - 1) &&
			(input > pLut->pLutArray[ctr].x) &&
			(input < pLut->pLutArray[ctr + 1].x)
		)
		{
			float x0 = pLut->pLutArray[ctr].x;
			float x1 = pLut->pLutArray[ctr+1].x;
			float y0 = pLut->pLutArray[ctr].y;
			float y1 = pLut->pLutArray[ctr+1].y;

			return (y0 + (input - x0) * ((y1-y0) / (x1-x0)));
		}
	}

	return NAN;
}

//------------------------------------------------------------------------------
//
float LUT_InterpolateReverse(const LUT_t *pLut, float input)
{
	assert_param(pLut != NULL);
	assert_param(pLut->pLutArray != NULL);

	if (input >= pLut->pLutArray[pLut->arraySize - 1].y)
	{
		return pLut->pLutArray[pLut->arraySize - 1].x;
	}

	for (uint32_t ctr = 0; ctr < (pLut->arraySize - 1); ctr++)
	{
		if (input == pLut->pLutArray[ctr].y)
		{
			return pLut->pLutArray[ctr].x;
		}

		if (
			(ctr < pLut->arraySize - 1) &&
			(input > pLut->pLutArray[ctr].y) &&
			(input < pLut->pLutArray[ctr + 1].y)
		)
		{
			float x0 = pLut->pLutArray[ctr].y;
			float x1 = pLut->pLutArray[ctr+1].y;
			float y0 = pLut->pLutArray[ctr].x;
			float y1 = pLut->pLutArray[ctr+1].x;

			return (y0 + (input - x0) * ((y1-y0) / (x1-x0)));
		}
	}

	return NAN;
}


