from dataclasses import dataclass



class MeasurementData:
    @dataclass
    class Values:
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



class LoadControl:
    @dataclass
    class Setpoints:
        setVoltage: float
        setCurrent: float
        setPower: float
        setResistance: float
        
    @dataclass
    class Limits:
        voltageLimit: float
        currentLimit: float
        powerLimit: float
        temperatureLimit: float
        
    @dataclass
    class ControlStatus:
        controlRegister: int
        statusRegister: int
    


class Calibration:
    @dataclass
    class Values:
        voltageFactor: float
        voltageOffset: float
        currentFactor: float
        currentOffset: float
        
    @dataclass
    class Control:
        controlRegister: int



class AutoZero:
    @dataclass
    class Values:
        voltageOffset: float
        currentOffset: float

    @dataclass
    class Limits:
        voltageLimit: float
        currentLimit: float
        
    @dataclass
    class Control:
        controlRegister: int