/*
 * ModbusManager.c
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#include <ModbusEmbedded/modbus.h>
#include <Timer/TimerMicro.h>
#include <Logger/Logger.h>
#include <ModbusManager/Communication.h>
#include <ModbusManager/MeasurementData/MeasurementData.h>
#include <ModbusManager/LoadControl/LoadControl.h>
#include <ModbusManager/Calibration/Calibration.h>
#include <stdbool.h>
#include <stddef.h>



static void ModbusManager_SerialReceiveCallback(uint32_t dataSize, SerialHandler_CallbackReason_t callbackReason);

static void ModbusManager_GenericFunction(modbus_Pdu_t *pPduRequest, modbus_Pdu_t *pPduResponse);

static modbus_Exception_e ModbusManager_ReadHoldingRegisterCallback(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer);
static modbus_Exception_e ModbusManager_WriteRegisterCallback(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t registerValue);



static SerialHandler_Instance_t *gpModbusSerialHandler = NULL;
static uint8_t gModbusBusAddress = 0;

static uint8_t gModbusRxBuffer[512];
static uint8_t gModbusTxBuffer[512];
static uint32_t gModbusTxLength = 0;

#ifdef MODBUS_USE_ASCII
	static uint8_t gModbusStopBytes[] = { '\r', '\n' };
#endif

static modbus_t gModbusInstance =
{
	.busAddress = 0,

	.pGenericFunctionHandler = ModbusManager_GenericFunction,
	.pGenericReadHandler = NULL,
	.pGenericWriteHandler = NULL,

	.pReadCoilHandler = NULL,
	.pReadDiscreteHandler = NULL,
	.pReadInputRegisterHandler = NULL,
	.pReadHoldingRegisterHandler = ModbusManager_ReadHoldingRegisterCallback,

	.pWriteCoilHandler = NULL,
	.pWriteRegisterHandler = ModbusManager_WriteRegisterCallback
};

static bool gModbusRequestReady = false;
static bool gModbusResponseReady = false;

static TimerMicro_t gModbusTimer;
static uint32_t gModbusProcessingTime = 0;



//------------------------------------------------------------------------------
//
void ModbusManager_Init(SerialHandler_Instance_t *pSerial, uint8_t busAddress)
{
	assert_param(pSerial != NULL);
	assert_param(busAddress != 0);

	gpModbusSerialHandler = pSerial;
	gModbusBusAddress = busAddress;

	gpModbusSerialHandler->pBuffer = (uint8_t *)gModbusRxBuffer;
	gpModbusSerialHandler->bufferSize = sizeof(gModbusRxBuffer);

#ifdef MODBUS_USE_ASCII
	gpModbusSerialHandler->triggerOnStopBytes = true;
	gpModbusSerialHandler->pStopBytes = (uint8_t *)gModbusStopBytes;
	gpModbusSerialHandler->stopBytesSize = sizeof(gModbusStopBytes);
#else
	gpModbusSerialHandler->triggerOnTimeout = true;
	gpModbusSerialHandler->timeoutBitsCount = 11;
#endif

	gpModbusSerialHandler->pListenerCallback = ModbusManager_SerialReceiveCallback;

	SerialHandler_Init(gpModbusSerialHandler);
}

//------------------------------------------------------------------------------
//
void ModbusManager_Handle(void)
{
	if (gModbusRequestReady && (gModbusResponseReady == false))
	{
		TimerMicro_Reset(&gModbusTimer);
		modbus_ProcessData(&gModbusInstance);
		gModbusProcessingTime = TimerMicro_Check(&gModbusTimer);

#ifdef MODBUS_USE_ASCII
		gModbusTxLength = modbus_EncodeAscii((char *)gModbusTxBuffer, sizeof(gModbusTxBuffer), &gModbusInstance.pduResponse);
#else
		gModbusTxLength = modbus_EncodeRtu((uint8_t *)gModbusTxBuffer, sizeof(gModbusTxBuffer), &gModbusInstance.pduResponse);
#endif

		if (gModbusTxLength > 0)
		{
			gModbusResponseReady = true;
		}
		gModbusRequestReady = false;
	}

	if (gModbusResponseReady)
	{
		if (SerialHandler_SendData(gpModbusSerialHandler, (uint8_t *)gModbusTxBuffer, gModbusTxLength) == ERROR)
		{
			LOG_ERROR("SerialHandler_SendData() failed.");
		}
		else
		{
			gModbusResponseReady = false;
		}
	}
}



//------------------------------------------------------------------------------
//
static void ModbusManager_SerialReceiveCallback(uint32_t dataSize, SerialHandler_CallbackReason_t callbackReason)
{
	if (gModbusRequestReady)
	{
		LOG_ERROR("New Modbus Request received, before last one was Processed.");
		return;
	}

#ifdef MODBUS_USE_ASCII
	if (callbackReason != SERIALHANDLER_CALLBACK_STOP_BYTES)
	{
		LOG_WARNING("ModbusManager_SerialReceiveCallback() received invalid callbackReason [%d].", (int)callbackReason);
		return;
	}

	if (modbus_DecodeAscii((const char *)gModbusRxBuffer, dataSize, &gModbusInstance.pduRequest) == false)
	{
		LOG_ERROR("modbus_DecodeAscii() failed.");
		return;
	}
#else
	if (callbackReason != SERIALHANDLER_CALLBACK_TIMEOUT)
	{
		LOG_WARNING("ModbusManager_SerialReceiveCallback() received invalid callbackReason [%d].", (int)callbackReason);
		return;
	}

	if (modbus_DecodeRtu((const uint8_t *)gModbusRxBuffer, dataSize, &gModbusInstance.pduRequest) == false)
	{
		LOG_ERROR("modbus_DecodeRtu() failed.");
		return;
	}
#endif

	if (gModbusInstance.pduRequest.busAddress == gModbusBusAddress)
	{
		gModbusRequestReady = true;
	}
}

//------------------------------------------------------------------------------
//
static void ModbusManager_GenericFunction(modbus_Pdu_t *pPduRequest, modbus_Pdu_t *pPduResponse)
{
	modbus_SetExceptionResponse(MODBUS_EXCEPTION_ILLEGALFUNCTION, pPduResponse);
}

//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_ReadHoldingRegisterCallback(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	modbus_Exception_e ret;

	ret = ModbusManager_MeasurementData_ReadHoldingRegisters(functionCode, registerAddress, pRegisterBuffer);
	if (ret != MODBUS_EXCEPTION_ILLEGALDATAADDRESS)
	{
		return ret;
	}

	ret = ModbusManager_LoadControl_ReadHoldingRegisters(functionCode, registerAddress, pRegisterBuffer);
	if (ret != MODBUS_EXCEPTION_ILLEGALDATAADDRESS)
	{
		return ret;
	}

	ret = ModbusManager_Calibration_ReadHoldingRegisters(functionCode, registerAddress, pRegisterBuffer);
	if (ret != MODBUS_EXCEPTION_ILLEGALDATAADDRESS)
	{
		return ret;
	}

	return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
}

//------------------------------------------------------------------------------
//
static modbus_Exception_e ModbusManager_WriteRegisterCallback(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t registerValue)
{
	modbus_Exception_e ret;

	ret = ModbusManager_LoadControl_WriteHoldingRegisters(functionCode, registerAddress, registerValue);
	if (ret != MODBUS_EXCEPTION_ILLEGALDATAADDRESS)
	{
		return ret;
	}

	ret = ModbusManager_Calibration_WriteHoldingRegisters(functionCode, registerAddress, registerValue);
	if (ret != MODBUS_EXCEPTION_ILLEGALDATAADDRESS)
	{
		return ret;
	}

	return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
}


