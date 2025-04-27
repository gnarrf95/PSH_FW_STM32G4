import psh.comm

import time
import sys

client = psh.comm.ModbusComm("COM3", 115200)

if client.connect() == False:
    print("Connection failed.")
    sys.exit(1)

values = psh.comm.Calibration_Values(
	1.023,
	12.3e-6,
 	0.9954,
	-1.45e-3
)

control = psh.comm.Calibration_Control(0xBEEF)

if client.write_Calibration_Values(values) == False:
    print("write_Calibration_Values() failed.")
    sys.exit(1)
    
if client.write_Calibration_Control(control) == False:
    print("write_Calibration_Control() failed.")
    sys.exit(1)
