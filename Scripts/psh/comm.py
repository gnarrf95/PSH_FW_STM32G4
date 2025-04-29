import pymodbus
import pymodbus.client

import struct
import threading
import time
import math

from dataclasses import dataclass



@dataclass
class MeasurementData:
    timestamp: float
    vdda: float
    
    voltage: float
    current: float
    power: float
    resistance: float
    
    tempFet: float
    tempMcu: float
    
    chargeCount: int
    energyCount: int
    
@dataclass
class LoadControl_Setpoints:
    setVoltage: float
    setCurrent: float
    setPower: float
    setResistance: float
    
@dataclass
class LoadControl_Limits:
    voltageLimit: float
    currentLimit: float
    powerLimit: float
    temperatureLimit: float
    
@dataclass
class LoadControl_ControlStatus:
    controlRegister: int
    statusRegister: int
    
@dataclass
class Calibration_Values:
    voltageFactor: float
    voltageOffset: float
    currentFactor: float
    currentOffset: float
    
@dataclass
class Calibration_Control:
    controlRegister: int
    


class ModbusComm:
    ADDRESS_MEASUREMENTDATA = 0x0000
    
    ADDRESS_LOADCONTROL_SETPOINTS = 0x0000
    ADDRESS_LOADCONTROL_LIMITS = 0x0100
    ADDRESS_LOADCONTROL_CONTROL = 0x0F00
    ADDRESS_LOADCONTROL_STATUS = 0x0F01
    
    ADDRESS_CALIBRATION_VALUES = 0x1000
    ADDRESS_CALIBRATION_CONTROL = 0x1F00
    
    ADDRESS_AUTOZERO_VALUES = 0x2000
    ADDRESS_AUTOZERO_LIMITS = 0x2100
    ADDRESS_AUTOZERO_CONTROL = 0x2F00
    
    # --------------------------------------------------------------------------
    #
    def decode_MeasurementData(registers: list[int]) -> MeasurementData:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            timestamp,
            vdda,
            voltage,
            current,
            power,
            resistance,
            tempFet,
            tempMcu,
            charge,
            energy
        ) = struct.unpack(">LllllLllqq", byteStream)
        
        return MeasurementData(
            float(timestamp) / 1000000.0,
            float(vdda) / 1000000.0,
            float(voltage) / 1000000.0,
            float(current) / 1000000.0,
            float(power) / 1000000.0,
            float(resistance),
            float(tempFet) / 1000.0,
            float(tempMcu) / 1000.0,
            charge,
            energy
        )
        
    # --------------------------------------------------------------------------
    #
    def decode_LoadControl_Setpoints(registers: list[int]) -> LoadControl_Setpoints:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            setVoltage,
            setCurrent,
            setPower,
            setResistance
        ) = struct.unpack(">lllL", byteStream)
        
        return LoadControl_Setpoints(
			float(setVoltage) / 1000000.0,
			float(setCurrent) / 1000000.0,
			float(setPower) / 1000000.0,
			float(setResistance)
		)
                
    # --------------------------------------------------------------------------
    #
    def decode_LoadControl_Limits(registers: list[int]) -> LoadControl_Limits:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageLimit,
            currentLimit,
            powerLimit,
            temperatureLimit
        ) = struct.unpack(">llll", byteStream)
        
        return LoadControl_Limits(
			float(voltageLimit) / 1000000.0,
			float(currentLimit) / 1000000.0,
			float(powerLimit) / 1000000.0,
			float(temperatureLimit) / 1000.0
		)
              
    # --------------------------------------------------------------------------
    #
    def decode_LoadControl_ControlStatus(registers: list[int]) -> LoadControl_ControlStatus:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            controlRegister,
            statusRegister
        ) = struct.unpack(">HH", byteStream)
        
        return LoadControl_ControlStatus(
			controlRegister,
   			statusRegister
		)
        
    # --------------------------------------------------------------------------
    #
    def decode_Calibration_Values(registers: list[int]) -> Calibration_Values:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageFactor,
            voltageOffset,
            currentFactor,
            currentOffset
        ) = struct.unpack(">llll", byteStream)
        
        return Calibration_Values(
			float(voltageFactor) / 1000000.0,
			float(voltageOffset) / 1000000.0,
			float(currentFactor) / 1000000.0,
			float(currentOffset) / 1000000.0
		)

    # --------------------------------------------------------------------------
    #
    def decode_Calibration_Control(registers: list[int]) -> Calibration_Control:
        byteStream = bytes()
        
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            controlRegister,
        ) = struct.unpack(">H", byteStream)
        
        return Calibration_Control(
			controlRegister
		)     

        
        
    # --------------------------------------------------------------------------
    #
    def encode_LoadControl_Setpoints(setpoints: LoadControl_Setpoints) -> list[int]:
        byteStream = struct.pack(">lllL",
           	int(setpoints.setVoltage * 1000000.0),
            int(setpoints.setCurrent * 1000000.0),
            int(setpoints.setPower * 1000000.0),
            int(setpoints.setResistance)
        )
        
        registerList = []
        
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
             
    # --------------------------------------------------------------------------
    #
    def encode_LoadControl_Limits(limits: LoadControl_Limits) -> list[int]:
        byteStream = struct.pack(">llll",
           	int(limits.voltageLimit * 1000000.0),
            int(limits.currentLimit * 1000000.0),
            int(limits.powerLimit * 1000000.0),
            int(limits.temperatureLimit * 1000.0)
        )
        
        registerList = []
        
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
             
    # --------------------------------------------------------------------------
    #
    def encode_LoadControl_Control(controlStatus: LoadControl_ControlStatus) -> list[int]:
        byteStream = struct.pack(">H",
           	controlStatus.controlRegister
        )
        
        registerList = []
        
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
               
    # --------------------------------------------------------------------------
    #
    def encode_Calibration_Values(values: Calibration_Values) -> list[int]:
        byteStream = struct.pack(">llll",
           	int(values.voltageFactor * 1000000.0),
            int(values.voltageOffset * 1000000.0),
            int(values.currentFactor * 1000000.0),
            int(values.currentOffset * 1000000.0)
        )
        
        registerList = []
        
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
              
    # --------------------------------------------------------------------------
    #
    def encode_Calibration_Control(control: Calibration_Control) -> list[int]:
        byteStream = struct.pack(">H",
           	control.controlRegister
        )
        
        registerList = []
        
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
          
           
           
    # --------------------------------------------------------------------------
    #
    def __init__(self, port: str, baudrate: int = 19200, deviceAddress: int = 1):
        self.comPort = port
        self.baudrate = baudrate
        self.deviceAddress = deviceAddress
        self.serCom = None
        
    # --------------------------------------------------------------------------
    #
    def connect(self) -> bool:
        if self.serCom is not None:
            return True
        
        try:
            self.serCom = pymodbus.client.ModbusSerialClient(
                port=self.comPort,
                framer=pymodbus.FramerType.RTU,
                baudrate=self.baudrate)
            
        except:
            self.serCom = None
            return False
        
        return True
    
    # --------------------------------------------------------------------------
    #
    def disconnect(self) -> None:
        if self.serCom is not None:
            self.serCom.close()
            self.serCom = None
            
            
            
    # --------------------------------------------------------------------------
    #
    def read_MeasurementData(self) -> MeasurementData | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_input_registers(address=ModbusComm.ADDRESS_MEASUREMENTDATA, count=24, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_MeasurementData(resp.registers)
      
    # --------------------------------------------------------------------------
    #
    def read_LoadControl_Setpoints(self) -> LoadControl_Setpoints | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_holding_registers(address=ModbusComm.ADDRESS_LOADCONTROL_SETPOINTS, count=8, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_LoadControl_Setpoints(resp.registers)
    
    # --------------------------------------------------------------------------
    #
    def read_LoadControl_Limits(self) -> LoadControl_Limits | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_holding_registers(address=ModbusComm.ADDRESS_LOADCONTROL_LIMITS, count=8, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_LoadControl_Limits(resp.registers)
    
    # --------------------------------------------------------------------------
    #
    def read_LoadControl_ControlStatus(self) -> LoadControl_ControlStatus | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_holding_registers(address=ModbusComm.ADDRESS_LOADCONTROL_CONTROL, count=2, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_LoadControl_ControlStatus(resp.registers)
    
    # --------------------------------------------------------------------------
    #
    def read_Calibration_Values(self) -> Calibration_Values | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_holding_registers(address=ModbusComm.ADDRESS_CALIBRATION_VALUES, count=8, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_Calibration_Values(resp.registers)
    
    # --------------------------------------------------------------------------
    #
    def read_Calibration_Control(self) -> Calibration_Control | None:
        if self.serCom is None:
            return None
        
        resp = None
        
        try:
            resp = self.serCom.read_holding_registers(address=ModbusComm.ADDRESS_CALIBRATION_CONTROL, count=1, slave=self.deviceAddress)
        except:
            return None
        
        if resp.isError():
            return None
        
        return ModbusComm.decode_Calibration_Control(resp.registers)
    
    
    
    # --------------------------------------------------------------------------
    #
    def write_LoadControl_Setpoints(self, setpoints: LoadControl_Setpoints) -> bool:
        if self.serCom is None:
            return None
        
        registers = ModbusComm.encode_LoadControl_Setpoints(setpoints)
        
        resp = None
        
        try:
            resp = self.serCom.write_registers(address=ModbusComm.ADDRESS_LOADCONTROL_SETPOINTS, values=registers, slave=self.deviceAddress)
        except:
            return False
        
        if resp.isError():
            return False
        
        return True
    
    # --------------------------------------------------------------------------
    #
    def write_LoadControl_Limits(self, limits: LoadControl_Limits) -> bool:
        if self.serCom is None:
            return None
        
        registers = ModbusComm.encode_LoadControl_Limits(limits)
        
        resp = None
        
        try:
            resp = self.serCom.write_registers(address=ModbusComm.ADDRESS_LOADCONTROL_LIMITS, values=registers, slave=self.deviceAddress)
        except:
            return False
        
        if resp.isError():
            return False
        
        return True
    
    # --------------------------------------------------------------------------
    #
    def write_LoadControl_Control(self, controlStatus: LoadControl_ControlStatus) -> bool:
        if self.serCom is None:
            return None
        
        registers = ModbusComm.encode_LoadControl_Control(controlStatus)
        
        resp = None
        
        try:
            resp = self.serCom.write_registers(address=ModbusComm.ADDRESS_LOADCONTROL_CONTROL, values=registers, slave=self.deviceAddress)
        except:
            return False
        
        if resp.isError():
            return False
        
        return True
    
    # --------------------------------------------------------------------------
    #
    def write_Calibration_Values(self, values: Calibration_Values) -> bool:
        if self.serCom is None:
            return None
        
        registers = ModbusComm.encode_Calibration_Values(values)
        
        resp = None
        
        try:
            resp = self.serCom.write_registers(address=ModbusComm.ADDRESS_CALIBRATION_VALUES, values=registers, slave=self.deviceAddress)
        except:
            return False
        
        if resp.isError():
            return False
        
        return True
    
    # --------------------------------------------------------------------------
    #
    def write_Calibration_Control(self, control: Calibration_Control) -> bool:
        if self.serCom is None:
            return None
        
        registers = ModbusComm.encode_Calibration_Control(control)
        
        resp = None
        
        try:
            resp = self.serCom.write_registers(address=ModbusComm.ADDRESS_CALIBRATION_CONTROL, values=registers, slave=self.deviceAddress)
        except:
            return False
        
        if resp.isError():
            return False
        
        return True
    