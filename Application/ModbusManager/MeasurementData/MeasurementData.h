/*
 * ModbusManager_MeasurementData.h
 *
 *  Created on: Apr 26, 2025
 *      Author: manue
 */

#ifndef MODBUSMANAGER_MEASUREMENTDATA_H_
#define MODBUSMANAGER_MEASUREMENTDATA_H_

#include <modbus.h>

#ifdef __cplusplus
extern "C" {
#endif



modbus_Exception_e ModbusManager_MeasurementData_ReadHoldingRegisters(modbus_FunctionCode_e functionCode, uint16_t registerAddress, uint16_t *pRegisterBuffer);



#ifdef __cplusplus
}
#endif

#endif /* MODBUSMANAGER_MEASUREMENTDATA_H_ */
