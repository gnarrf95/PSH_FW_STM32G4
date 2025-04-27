/*
 * ModbusManager_Datapoint.c
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#include <ModbusManager/Datapoint.h>
#include <stddef.h>



static const ModbusManager_Datapoint_t *ModbusManager_Datapoint_GetDatapoint(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress);



//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_Datapoint_ReadRegister(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress, uint16_t *pRegisterBuffer)
{
	if (pDatapointArray == NULL)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}
	if (pDatapointArray->pArray == NULL)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}
	if (pRegisterBuffer == NULL)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}

	const ModbusManager_Datapoint_t *pDatapoint = ModbusManager_Datapoint_GetDatapoint(pDatapointArray, registerAddress);
	if (pDatapoint == NULL)
	{
		return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
	}
	if (pDatapoint->access == ACCESS_NONE)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}
	if(pDatapoint->access == ACCESS_WRITEONLY)
	{
		return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
	}

	uint8_t byteOffset = (pDatapoint->dataSizeBytes - 2) - ((registerAddress - pDatapoint->startAddress) * 2);

	*pRegisterBuffer = ((uint16_t)pDatapoint->pData[byteOffset+1] << 8) | (uint16_t)pDatapoint->pData[byteOffset];

	return MODBUS_EXCEPTION_SUCCESS;
}

//------------------------------------------------------------------------------
//
modbus_Exception_e ModbusManager_Datapoint_WriteRegister(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress, uint16_t registerValue)
{
	if (pDatapointArray == NULL)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}
	if (pDatapointArray->pArray == NULL)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}

	const ModbusManager_Datapoint_t *pDatapoint = ModbusManager_Datapoint_GetDatapoint(pDatapointArray, registerAddress);
	if (pDatapoint == NULL)
	{
		return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
	}
	if (pDatapoint->access == ACCESS_NONE)
	{
		return MODBUS_EXCEPTION_SLAVEDEVICEFAILURE;
	}
	if(pDatapoint->access == ACCESS_READONLY)
	{
		return MODBUS_EXCEPTION_ILLEGALDATAADDRESS;
	}

	uint8_t byteOffset = (pDatapoint->dataSizeBytes - 2) - ((registerAddress - pDatapoint->startAddress) * 2);

	pDatapoint->pData[byteOffset] = (uint8_t)(registerValue & 0x00FF);
	pDatapoint->pData[byteOffset+1] = (uint8_t)((registerValue & 0xFF00) >> 8);

	return MODBUS_EXCEPTION_SUCCESS;
}



//------------------------------------------------------------------------------
//
static const ModbusManager_Datapoint_t *ModbusManager_Datapoint_GetDatapoint(const ModbusManager_DatapointArray_t *pDatapointArray, uint16_t registerAddress)
{
	if (pDatapointArray == NULL)
	{
		return NULL;
	}
	if (pDatapointArray->pArray == NULL)
	{
		return NULL;
	}

	for (uint32_t ctr = 0; ctr < pDatapointArray->arraySize; ctr++)
	{
		uint16_t startAddress = pDatapointArray->pArray[ctr].startAddress;
		uint16_t endAddress = startAddress + (pDatapointArray->pArray[ctr].dataSizeBytes / 2) - 1;

		if ((registerAddress >= startAddress) && (registerAddress <= endAddress))
		{
			return &pDatapointArray->pArray[ctr];
		}
	}

	return NULL;
}


