import psh.data
import struct



class Datapoint:
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        raise NotImplementedError
    
    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        raise NotImplementedError
    


class MeasurementData(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, values: psh.data.MeasurementData.Values):
        self.values = values

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 24:
            return False

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
        
        self.values = psh.data.MeasurementData.Values(
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

        return True
    


class LoadControl_Setpoints(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, setpoints: psh.data.LoadControl.Setpoints):
        self.setpoints = setpoints

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 8:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            setVoltage,
            setCurrent,
            setPower,
            setResistance
        ) = struct.unpack(">lllL", byteStream)
        
        self.setpoints = psh.data.LoadControl.Setpoints(
			float(setVoltage) / 1000000.0,
			float(setCurrent) / 1000000.0,
			float(setPower) / 1000000.0,
			float(setResistance)
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">lllL",
           	int(self.setpoints.setVoltage * 1000000.0),
            int(self.setpoints.setCurrent * 1000000.0),
            int(self.setpoints.setPower * 1000000.0),
            int(self.setpoints.setResistance)
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    
class LoadControl_Limits(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, limits: psh.data.LoadControl.Limits):
        self.limits = limits

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 8:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageLimit,
            currentLimit,
            powerLimit,
            temperatureLimit
        ) = struct.unpack(">llll", byteStream)
        
        self.limits = psh.data.LoadControl.Limits(
			float(voltageLimit) / 1000000.0,
			float(currentLimit) / 1000000.0,
			float(powerLimit) / 1000000.0,
			float(temperatureLimit) / 1000.0
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">llll",
           	int(self.limits.voltageLimit * 1000000.0),
            int(self.limits.currentLimit * 1000000.0),
            int(self.limits.powerLimit * 1000000.0),
            int(self.limits.temperatureLimit * 1000.0)
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    
class LoadControl_ControlStatus(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, controlStatus: psh.data.LoadControl.ControlStatus):
        self.controlStatus = controlStatus

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 2:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            controlRegister,
            statusRegister
        ) = struct.unpack(">HH", byteStream)
        
        self.controlStatus = psh.data.LoadControl.ControlStatus(
			controlRegister,
   			statusRegister
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">H",
           	self.controlStatus.controlRegister
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    

    
class Calibration_Values(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, values: psh.data.Calibration.Values):
        self.values = values

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 8:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageFactor,
            voltageOffset,
            currentFactor,
            currentOffset
        ) = struct.unpack(">llll", byteStream)
        
        self.values = psh.data.Calibration.Values(
			float(voltageFactor) / 1000000.0,
			float(voltageOffset) / 1000000.0,
			float(currentFactor) / 1000000.0,
			float(currentOffset) / 1000000.0
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">llll",
           	int(self.values.voltageFactor * 1000000.0),
            int(self.values.voltageOffset * 1000000.0),
            int(self.values.currentFactor * 1000000.0),
            int(self.values.currentOffset * 1000000.0)
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    
class Calibration_Control(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, control: psh.data.Calibration.Control):
        self.control = control

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 1:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            controlRegister,
        ) = struct.unpack(">H", byteStream)
        
        self.control = psh.data.Calibration.Control(
			controlRegister
		)     

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">H",
           	self.control.controlRegister
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    


class AutoZero_Values(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, values: psh.data.AutoZero.Values):
        self.values = values

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 4:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageOffset,
            currentOffset
        ) = struct.unpack(">ll", byteStream)
        
        self.values = psh.data.AutoZero.Values(
			float(voltageOffset) / 1000000.0,
			float(currentOffset) / 1000000.0
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">ll",
            int(self.values.voltageOffset * 1000000.0),
            int(self.values.currentOffset * 1000000.0)
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    
class AutoZero_Limits(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, limits: psh.data.AutoZero.Limits):
        self.limits = limits

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 4:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            voltageLimit,
            currentLimit
        ) = struct.unpack(">ll", byteStream)
        
        self.limits = psh.data.AutoZero.Limits(
			float(voltageLimit) / 1000000.0,
			float(currentLimit) / 1000000.0
		)

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">ll",
            int(self.limits.voltageLimit * 1000000.0),
            int(self.limits.currentLimit * 1000000.0)
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    
class AutoZero_Control(Datapoint):
    # --------------------------------------------------------------------------
    #
    def __init__(self, control: psh.data.AutoZero.Control):
        self.control = control

    # --------------------------------------------------------------------------
    #
    def decode(self, registers: list[int]) -> bool:
        if len(registers) != 1:
            return False

        byteStream = bytes()
        for reg in registers:
            byteStream = byteStream + struct.pack(">H", reg)
            
        (
            controlRegister,
        ) = struct.unpack(">H", byteStream)
        
        self.control = psh.data.AutoZero.Control(
			controlRegister
		)     

        return True
    
    # --------------------------------------------------------------------------
    #
    def encode(self) -> list[int]:
        byteStream = struct.pack(">H",
           	self.control.controlRegister
        )
        
        registerList = []
        for ctr in range(0, len(byteStream), 2):
            registerList.append(struct.unpack(">H", byteStream[ctr:ctr+2])[0])
            
        return registerList
    