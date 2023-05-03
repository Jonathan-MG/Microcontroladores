from machine import Pin, PWM
import utime
from servo import Servo

def servo_Map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
 
def servo_Angle(angle):
    if angle < 0:
        angle = 0
    if angle > 180:
        angle = 180
    s1.goto(round(servo_Map(angle,0,180,0,1024))) # Convert range value to angle value
    
def esquerda():
    # 180 é o lado esquerdo do carrinho!
    for i in range(100,180,1): 
        servo_Angle(i)
        utime.sleep(0.0005)
    utime.sleep(1)
    for i in range(180,100,-1):
            servo_Angle(i)
            utime.sleep(0.0005)
    utime.sleep(1)
    
def centro():
    servo_Angle(100)
    utime.sleep(1)
    
def direita():
    # 0 é o lado direito do carrinho!
    for i in range(100,0,-1):
        servo_Angle(i)
        utime.sleep(0.0005)
    utime.sleep(1)
    for i in range(0,100,1):
        servo_Angle(i)
        utime.sleep(0.0005)
    utime.sleep(1)
    
s1 = Servo(0)

in1_T = Pin(11, Pin.OUT)
in2_T = Pin(10, Pin.OUT)
in3_T = Pin(12, Pin.OUT)
in4_T = Pin(13, Pin.OUT)

in1_F = Pin(21, Pin.OUT)
in2_F = Pin(20, Pin.OUT)
in3_F = Pin(19, Pin.OUT)
in4_F = Pin(18, Pin.OUT)

while True:

        # Parado
        in1_T.value(0)
        in2_T.value(0)
        in3_T.value(0)
        in4_T.value(0)
        
        in1_F.value(0)
        in2_F.value(0)
        in3_F.value(0)
        in4_F.value(0)
        
        centro()
        
        utime.sleep(1)
        
        '''
        # Atrás
        in1.value(0)
        in2.value(1)
        in3.value(1)
        in4.value(0)
        utime.sleep(1)
        '''
        # Frente
        in1_T.value(1)
        in2_T.value(0)
        in3_T.value(0)
        in4_T.value(1)
        
        in1_F.value(0)
        in2_F.value(1)
        in3_F.value(0)
        in4_F.value(1)
        
        centro()
        esquerda()
        direita()
        
        utime.sleep(1)
        
        '''
        # Parado
        in1.value(1)
        in2.value(1)
        in3.value(1)
        in4.value(1)
        utime.sleep(1)'''



