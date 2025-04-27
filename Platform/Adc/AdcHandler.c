/*
 * AdcHandler.c
 *
 *  Created on: Apr 10, 2025
 *      Author: manue
 */

#include <Adc/AdcHandler.h>
#include <LookupTable/LinearInterpolation.h>
#include <Calibration/Calibration.h>
#include <Timer/TimerMicro.h>

#include <adc.h>
#include <tim.h>

#include <string.h>
#include <math.h>



#define ADCHANDLER_ADC_CHANNEL_COUNT			5
#define ADCHANDLER_ADC_AVERAGING				8
#define ADCHANDLER_ADC_BUFFER_LENGTH			(ADCHANDLER_ADC_CHANNEL_COUNT * ADCHANDLER_ADC_AVERAGING)

#define ADCHANDLER_CHANNELINDEX_VOLTAGE			1
#define ADCHANDLER_CHANNELINDEX_CURRENT			0
#define ADCHANDLER_CHANNELINDEX_TEMPFET			2
#define ADCHANDLER_CHANNELINDEX_TEMPMCU			3
#define ADCHANDLER_CHANNELINDEX_VREF			4

#define ADCHANDLER_ADC_VREF_NOMINAL				3.3f
#define ADCHANDLER_ADC_RESOLUTION				65535
#define ADCHANDLER_ADC_TO_VIN_FACTOR			(ADCHANDLER_ADC_VREF_NOMINAL / (float)ADCHANDLER_ADC_RESOLUTION)

#define ADCHANDLER_ADC_REFINT					((float)VREFINT_CAL_VREF / 1000.0f)
#define ADCHANDLER_ADC_REFCAL					((float)((uint32_t)(*VREFINT_CAL_ADDR) << 4))

#define ADCHANDLER_TSENS_CAL_REF				((float)TEMPSENSOR_CAL_VREFANALOG / 1000.0f)
#define ADCHANDLER_TSENS_CAL_T1					((float)TEMPSENSOR_CAL1_TEMP)
#define ADCHANDLER_TSENS_CAL_T2					((float)TEMPSENSOR_CAL2_TEMP)
#define ADCHANDLER_TSENS_CAL1					((float)((uint32_t)(*TEMPSENSOR_CAL1_ADDR) << 4))
#define ADCHANDLER_TSENS_CAL2					((float)((uint32_t)(*TEMPSENSOR_CAL2_ADDR) << 4))

#define ADCHANDLER_CONVERSIONFACTOR_VOLTAGE		20.0f
#define ADCHANDLER_CONVERSIONFACTOR_CURRENT		2.5f

static float AdcHandler_ConvertTempFet(float adcVoltage);
static float AdcHandler_ConvertTempMcu(float adcSample, float adcVRef);



static Calibration_t gCalibrationVoltage =
{
	.linearGradient = 1.0f,
	.zeroOffset = -10.5e-3f,

	.conversionFactor = ADCHANDLER_CONVERSIONFACTOR_VOLTAGE * ADCHANDLER_ADC_TO_VIN_FACTOR
};

static Calibration_t gCalibrationCurrent =
{
	.linearGradient = 1.0f,
	.zeroOffset = -295.0e-6f,

	.conversionFactor = ADCHANDLER_CONVERSIONFACTOR_CURRENT * ADCHANDLER_ADC_TO_VIN_FACTOR
};

static volatile uint16_t gAdcDmaBuffer[ADCHANDLER_ADC_BUFFER_LENGTH * 2];
static volatile uint16_t *gpAdcDmaBufferReady;
static volatile uint32_t gAdcDmaBufferOverrun;

static TimerMicro_t gAdcTimer;
static volatile uint32_t gAdcInterval;

static TimerMicro_t gAdcProcessingTimer;
static volatile uint32_t gAdcProcessingTime;

static volatile float gVddaCal;

static AdcHandler_Data_t gAdcData;
static bool gAdcDataReady;



//------------------------------------------------------------------------------
//
ErrorStatus AdcHandler_Init(void)
{
	Calibration_Calculate(&gCalibrationVoltage);
	Calibration_Calculate(&gCalibrationCurrent);

	gpAdcDmaBufferReady = NULL;
	gAdcDmaBufferOverrun = 0;

	gVddaCal = ADCHANDLER_ADC_VREF_NOMINAL;

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		return ERROR;
	}

	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)gAdcDmaBuffer, ADCHANDLER_ADC_BUFFER_LENGTH * 2) != HAL_OK)
	{
		return ERROR;
	}

	TimerMicro_Reset(&gAdcTimer);

	if (HAL_TIM_Base_Start(&htim15) != HAL_OK)
	{
		return ERROR;
	}

	return SUCCESS;
}

//------------------------------------------------------------------------------
//
float AdcHandler_GetVdda(void)
{
	return gVddaCal;
}

//------------------------------------------------------------------------------
//
bool AdcHandler_Handle(void)
{
	if (gpAdcDmaBufferReady == NULL)
	{
		return false;
	}

	TimerMicro_Reset(&gAdcProcessingTimer);



	float vrefAcc = 0;
	float voltageAcc = 0;
	float currentAcc = 0;
	float powerAcc = 0;
	float tempFetAcc = 0;
	float tempMcuAcc = 0;

	float voltage;
	float current;
	float powerCalc;
	float powerAccCalc;
	float resistance;
	float tempFetSense;
	float tempMcuSample;

	int64_t chargeCount;
	int64_t energyCount;

	for (uint32_t ctr = 0; ctr < ADCHANDLER_ADC_AVERAGING; ctr++)
	{
		float voltageCal = Calibration_ProcessRaw(&gCalibrationVoltage, (float)gpAdcDmaBufferReady[ctr * ADCHANDLER_ADC_CHANNEL_COUNT + ADCHANDLER_CHANNELINDEX_VOLTAGE]);
		float currentCal = Calibration_ProcessRaw(&gCalibrationCurrent, (float)gpAdcDmaBufferReady[ctr * ADCHANDLER_ADC_CHANNEL_COUNT + ADCHANDLER_CHANNELINDEX_CURRENT]);

		vrefAcc += (float)gpAdcDmaBufferReady[ctr * ADCHANDLER_ADC_CHANNEL_COUNT + ADCHANDLER_CHANNELINDEX_VREF];
		tempFetAcc += (float)gpAdcDmaBufferReady[ctr * ADCHANDLER_ADC_CHANNEL_COUNT + ADCHANDLER_CHANNELINDEX_TEMPFET];
		tempMcuAcc += (float)gpAdcDmaBufferReady[ctr * ADCHANDLER_ADC_CHANNEL_COUNT + ADCHANDLER_CHANNELINDEX_TEMPMCU];

		voltageAcc += voltageCal;
		currentAcc += currentCal;
		powerAcc += voltageCal * currentCal;
	}

	gVddaCal = (ADCHANDLER_ADC_REFCAL * ADCHANDLER_ADC_REFINT) / (vrefAcc / (float)ADCHANDLER_ADC_AVERAGING);
	voltage = (voltageAcc / (float)ADCHANDLER_ADC_AVERAGING) * (gVddaCal / (float)ADCHANDLER_ADC_RESOLUTION) * ADCHANDLER_CONVERSIONFACTOR_VOLTAGE;
	current = (currentAcc / (float)ADCHANDLER_ADC_AVERAGING) * (gVddaCal / (float)ADCHANDLER_ADC_RESOLUTION) * ADCHANDLER_CONVERSIONFACTOR_CURRENT;
	powerCalc = voltage * current;
	powerAccCalc = (powerAcc / (float)ADCHANDLER_ADC_AVERAGING) * (gVddaCal / (float)ADCHANDLER_ADC_RESOLUTION) * (gVddaCal / (float)ADCHANDLER_ADC_RESOLUTION) * (ADCHANDLER_CONVERSIONFACTOR_VOLTAGE * ADCHANDLER_CONVERSIONFACTOR_CURRENT);
	resistance = fabs(voltage / current);
	tempFetSense = (tempFetAcc / (float)ADCHANDLER_ADC_AVERAGING) * (gVddaCal / (float)ADCHANDLER_ADC_RESOLUTION);
	tempMcuSample = tempMcuAcc / (float)ADCHANDLER_ADC_AVERAGING;

	chargeCount = (int64_t)(current * 1000000.0); // uA * ms
	energyCount = (int64_t)(powerAccCalc * 1000000.0); // uW * ms



	gAdcData.vdda = gVddaCal;
	gAdcData.voltage = voltage;
	gAdcData.current = current;
	gAdcData.powerAverage = powerCalc;
	gAdcData.powerPrecise = powerAccCalc;
	gAdcData.tempFet = AdcHandler_ConvertTempFet(tempFetSense);
	gAdcData.tempMcu = AdcHandler_ConvertTempMcu(tempMcuSample, gVddaCal);
	gAdcData.resistance = resistance;
	gAdcData.chargeCounter += chargeCount;
	gAdcData.energyCounter += energyCount;



	gAdcProcessingTime = TimerMicro_Check(&gAdcProcessingTimer);
	gpAdcDmaBufferReady = NULL;
	gAdcDataReady = true;

	return gAdcDataReady;
}

//------------------------------------------------------------------------------
//
bool AdcHandler_NewData(void)
{
	return gAdcDataReady;
}

//------------------------------------------------------------------------------
//
bool AdcHandler_GetData(AdcHandler_Data_t *pDataBuffer)
{
	if (pDataBuffer != NULL)
	{
		memcpy(pDataBuffer, &gAdcData, sizeof(AdcHandler_Data_t));

		if (AdcHandler_NewData())
		{
			gAdcDataReady = false;
			return true;
		}
	}

	return false;
}



//------------------------------------------------------------------------------
//
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc != &hadc1)
	{
		return;
	}

	gAdcInterval = TimerMicro_Reset(&gAdcTimer);

	if (gpAdcDmaBufferReady != NULL)
	{
		gAdcDmaBufferOverrun++;
	}

	// First Half of Buffer
	gpAdcDmaBufferReady = (uint16_t *)&gAdcDmaBuffer[0];
}

//------------------------------------------------------------------------------
//
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc != &hadc1)
	{
		return;
	}

	gAdcInterval = TimerMicro_Reset(&gAdcTimer);

	if (gpAdcDmaBufferReady != NULL)
	{
		gAdcDmaBufferOverrun++;
	}

	// Second Half of Buffer
	gpAdcDmaBufferReady = (uint16_t *)&gAdcDmaBuffer[ADCHANDLER_ADC_BUFFER_LENGTH];
}





static const LUT_Entry_t gNtcLutArray[] =
{
	{ 0.02, 230.7138 },
	{ 0.05, 117.6858 },
	{ 0.1, 143.8753 },
	{ 0.25, 104.7292 },
	{ 0.5, 77.18701 },
	{ 1.5, 25.4705 },
	{ 1.75, 13.01543 },
	{ 2.0, -4.22708 },
	{ 2.2, -42.9656 }
};

static const LUT_t gNtcLut =
{
	.pLutArray = (LUT_Entry_t *)gNtcLutArray,
	.arraySize = sizeof(gNtcLutArray) / sizeof(LUT_Entry_t)
};

//------------------------------------------------------------------------------
//
static float AdcHandler_ConvertTempFet(float adcVoltage)
{
	return LUT_Interpolate(&gNtcLut, adcVoltage);
}

//------------------------------------------------------------------------------
//
static float AdcHandler_ConvertTempMcu(float adcSample, float adcVRef)
{
	return ((adcSample * adcVRef) / ADCHANDLER_TSENS_CAL_REF - ADCHANDLER_TSENS_CAL1) * ((ADCHANDLER_TSENS_CAL_T2 - ADCHANDLER_TSENS_CAL_T1) / (ADCHANDLER_TSENS_CAL2 - ADCHANDLER_TSENS_CAL1)) + ADCHANDLER_TSENS_CAL_T1;
}


