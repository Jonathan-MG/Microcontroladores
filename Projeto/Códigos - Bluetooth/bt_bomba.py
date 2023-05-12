from machine import Pin,UART
uart = UART(0,9600)
bomba = Pin(16, Pin.OUT)
while True:
    if uart.any():
        command = uart.readline()
        print(command)   # descomente esta linha para ver os dados recebidos
        if command==b'L':
            bomba.value(0)
            print("ON")
        elif command==b'D':
            bomba.value(1)
            print("OFF")
