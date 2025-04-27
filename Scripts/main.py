import psh.comm

import time

client = psh.comm.ModbusComm("COM3", 115200)

if client.connect() == False:
    print("Connection failed.")
    exit(1)

while True:
    data = None
    
    data = client.read_MeasurementData()
    if data is None:
        print("readMeasurements() failed.")
    else:
        print("Timestamp  :", data.timestamp, "s")
        print("VDDA       :", round(data.vdda, 3), "V")
        print("Voltage    :", round(data.voltage, 3), "V")
        print("Current    :", round(data.current, 3), "A")
        print("Power      :", round(data.power, 4), "W")
        print("Resistance :", round(data.resistance, 4), "Ohm")
        print("TempFET    :", round(data.tempFet, 1), "°C")
        print("TempMCU    :", round(data.tempMcu, 1), "°C")
        print("Charge     :", float(data.chargeCount) / 1000000000.0, "As")
        print("Energy     :", float(data.energyCount) / 1000000000.0, "Ws")
        print("")
    
    time.sleep(0.5)
