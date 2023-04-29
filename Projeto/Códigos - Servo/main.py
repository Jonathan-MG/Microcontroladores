import utime
from servo import Servo
 
s1 = Servo(0)       # Servo pin is connected to GP0
 
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
    
    
if __name__ == '__main__':
    while True:
        centro()
        esquerda()
        direita()      
