import serial
import time

# list_of_commands = ["$100=250.00"
#                     ,"$101=250.00"
#                     ,"$102=250.00"
#                     ,"G21G91G0X1Y1Z-1F100"
#                     ,"G21G91G0X1F100"
#                     ,"G21G91G0Y-1F100"
#                     ,"G21G91G0X-1F100"
#                     ,"G21G91G0Z1F100"
#                     ,"G21G91G0Z-1F100"]

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 112500
# ser.write(str.encode("\r\n\r\n"))
# time.sleep(2)
# ser.flushInput()
# while True:
#     if len(list_of_commands) != 0:
#         ser.write(str.encode(list_of_commands.pop(0)+"\n"))
#     grbl_out = ser.readline().strip()
#     print ('REC:',grbl_out)
# print ("End of commands lol")
