from machine import Pin,UART
uart = UART(0,9600)
led = Pin(25, Pin.OUT)
while True:
    if uart.any():
        command = uart.readline()
        print(command)   # descomente esta linha para ver os dados recebidos
        #if command==b'L':
        #    led.value(1)
        #    print("ON")
        #elif command==b'D':
        #    led.value(0)
        #    print("OFF")
