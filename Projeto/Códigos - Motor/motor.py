from machine import Pin, PWM
import utime

in1_T = Pin(10, Pin.OUT)
in2_T = Pin(11, Pin.OUT)
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
        in3_T.value(1)
        in4_T.value(0)
        
        in1_F.value(1)
        in2_F.value(0)
        in3_F.value(1)
        in4_F.value(0)
        
        utime.sleep(1)
        
        '''
        # Parado
        in1.value(1)
        in2.value(1)
        in3.value(1)
        in4.value(1)
        utime.sleep(1)'''