import serial
from time import sleep
import pygame

controlBuffer = [64, 64, 64, 64, 64, 64, 0, 0]
gain = [1, 1, -1, 1, 1, 1, 1, 1]
joy = []
ser = serial.Serial('COM6', 9600)

def main():  
     
    pygame.joystick.init()  
    pygame.display.init()  

     
    if not pygame.joystick.get_count():  
        print "\nPlease connect a joystick and run again.\n"  
        quit()  
    print "\n%d joystick(s) detected." % pygame.joystick.get_count()  
    for i in range(pygame.joystick.get_count()):  
        myjoy = pygame.joystick.Joystick(i)  
        myjoy.init()  
        joy.append(myjoy)  
        print "Joystick %d: " % (i) + joy[i].get_name()  
    print "Depress trigger (button 0) to quit.\n"  
  
    mainLoop()  
 
def mainLoop() :
    while True:
    
        pygame.event.get()
       
        
        for x in range(0, 4):
            pos = joy[0].get_axis(x)
            pos = pos*63*gain[x] + 64
            controlBuffer[x] = int(pos)
       
        
        #print controlBuffer[0]
       
        string = '<'
                
        for key in controlBuffer:
            ch = chr(key)
            string += ch
        string += '>'
        
        ser.write(str(string))
                
        #print string
        
        print ser.readline()
        
        sleep(.01)
                
        
# allow use as a module or standalone script  
if __name__ == "__main__":  
    main()  
