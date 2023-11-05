from machine import Pin, PWM, UART,ADC
import utime
from servo import Servo

uart = UART(0,9600)
bomba = Pin(16, Pin.OUT)

soil = ADC(Pin(28)) 
uart = UART(0,9600)
min_moisture=44500
max_moisture=65535
readDelay = 0.5 

led = Pin(25, Pin.OUT)
led.value(1)

s1 = Servo(2)

in1_F = Pin(21, Pin.OUT)
in2_F = Pin(20, Pin.OUT)
in3_F = Pin(19, Pin.OUT)
in4_F = Pin(18, Pin.OUT)

def Ligar_Bomba():
    bomba.value(0)
    utime.sleep(2)
    bomba.value(1)

def servo_Map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
 
def servo_Angle(angle):
    if angle < 0:
        angle = 0
    if angle > 180:
        angle = 180
    s1.goto(round(servo_Map(angle,0,180,0,1024))) # Convert range value to angle value
    
def Esquerda():
    # 180 é o lado esquerdo do carrinho! 
    servo_Angle(180)
    
def Centro():
    servo_Angle(90)
    
def Direita():
    # 0 é o lado direito do carrinho!
    servo_Angle(0)

def Parar():
    # Parado
    in1_F.value(0)
    in2_F.value(0)
    in3_F.value(0)
    in4_F.value(0)
    
def Frente():
    # Frente
    in1_F.value(0)
    in2_F.value(1)
    in3_F.value(0)
    in4_F.value(1)
    
def Re():
    # Ré
    in1_F.value(1)
    in2_F.value(0)
    in3_F.value(1)
    in4_F.value(0)
    
def Confere_nivel():
    moisture = int(((max_moisture-soil.read_u16())*100/(max_moisture-min_moisture)))
    if moisture >= 100:
        moisture=100
    nivel_dagua= str(moisture).replace(".","")+"F"
    uart.write(nivel_dagua)
    return moisture


Centro()
Parar()
bomba.value(1)

while True:
    if uart.any():
        command = uart.readline()
        print(command)   # descomente esta linha para ver os dados recebidos
        if command==b'B':
            if Confere_nivel() > 0:
                Ligar_Bomba()
                Confere_nivel()
            else:
                pass
        elif command==b'F':
            Frente()
        elif command==b'P':
            Parar()
        elif command==b'R':
            Re()
        elif command==b'D':
            Direita()
        elif command==b'C':
            Centro()
        elif command==b'E':
            Esquerda()
        elif command==b'N':
            Confere_nivel()
