import sys
import serial as ser
import time

def main():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # after the timeout the program continues
    enableTX = True
    enableRX = False
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    line = b'9'
    while (1):
        # RX
        while (s.in_waiting > 0 or enableRX):  # while the input buffer isn't empty
            line = s.read(size=1)  # read 1 char from the input buffer
            if (line == b'8'):
                menu = s.read_until(expected=b'|')  # read  from the buffer until the terminator is received,
                print(menu.decode("ascii"))
            if (s.in_waiting == 0):
                enableTX = True  # enable transmission to echo the received data
        # TX
        while (s.out_waiting > 0 or enableTX):  # while the output buffer isn't empty
            if (line == b'4'):
                inChar = input("Enter delay value (X) between 0-9999: \n")
                bytesChar = bytes(inChar + '\n', "ascii")
                s.write(bytesChar)
                time.sleep(0.25)  # delay for accurate read/write operations on both ends
                line = b'9'
                enableTX = True
            elif (line == b'9' or line == b'\x00' or line == b'8'):
                inChar = input("Enter number between 1-8: \n")
                bytesChar = bytes(inChar, 'ascii')
                s.write(bytesChar)
                time.sleep(0.25)  # delay for accurate read/write operations on both ends
            if (s.out_waiting == 0):
                enableTX = False

if __name__ == '__main__':
    main()



