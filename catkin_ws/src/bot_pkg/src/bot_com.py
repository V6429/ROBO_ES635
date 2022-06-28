import serial
import time

ser = serial.Serial( port='/dev/ttyUSB0', # edit it 
                     baudrate = 9600,
                     parity=serial.PARITY_NONE,
                     stopbits=serial.STOPBITS_ONE,
                     bytesize=serial.EIGHTBITS,
                     timeout=None
                    )

while True:
    ser.write("e\n".encode("utf-8"))
    ser.write("e\n".encode("utf-8"))
    ser.write("5000\n".encode("utf-8"))
    ser.write("e \n".encode("utf-8"))
    data= ser.readline()
    print(int(data))
    time.sleep(1)
