/*
 * AnalogManager.c
 *
 *  Created on: May 1, 2025
 *      Author: manue
 */

#include <AnalogManager/AnalogManager.h>
#include <stddef.h>



//------------------------------------------------------------------------------
//
ErrorStatus AnalogManager_Init(void)
{
	ErrorStatus ret = SUCCESS;

	if (AnalogManager_InitAdc() == ERROR)
	{
		ret = ERROR;
	}

	return ret;
}

//------------------------------------------------------------------------------
//
ErrorStatus AnalogManager_Handle(void)
{
	ErrorStatus ret = SUCCESS;

	if (AnalogManager_HandleAdc() == ERROR)
	{
		ret = ERROR;
	}

	return ret;
}


