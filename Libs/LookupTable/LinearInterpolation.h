/*
 * LinearInterpolation.h
 *
 *  Created on: Apr 6, 2025
 *      Author: manue
 */

#ifndef LOOKUPTABLE_LINEARINTERPOLATION_H_
#define LOOKUPTABLE_LINEARINTERPOLATION_H_

#include <LookupTable/LUT.h>

#ifdef __cplusplus
extern "C" {
#endif



float LUT_Interpolate(const LUT_t *pLut, float input);
float LUT_InterpolateReverse(const LUT_t *pLut, float input);



#ifdef __cplusplus
}
#endif

#endif /* LOOKUPTABLE_LINEARINTERPOLATION_H_ */
