# import the libraries
import serial
import time
import RPi.GPIO as GPIO

#Define the GPIO pins of the raspberry pi.
Signal_R1_StorageB = 18
Signal_R1_StorageO = 23
Signal_R2 = 24
Signal_CON = 5
Signal_R3_ContainerB = 12
Signal_R3_ContainerO = 19

#Setting up the GPIO pins on raspberry pi as output pins
GPIO.setmode(GPIO.BCM)
GPIO.setup(Signal_R1_StorageB, GPIO.OUT)
GPIO.setup(Signal_R1_StorageO, GPIO.OUT)
GPIO.setup(Signal_R2, GPIO.OUT)
#GPIO.setup(25, GPIO.OUT) stand by
GPIO.setup(Signal_CON, GPIO.OUT)
#GPIO.setup(6, GPIO.OUT) stand by
GPIO.setup(Signal_R3_ContainerB, GPIO.OUT)
GPIO.setup(Signal_R3_ContainerO, GPIO.OUT)

GPIO.setwarnings(False)

StorageBalancer = 0 # variable to balance inventory usage
ContainerBalancer = 0 # variable to balance inventory Feed

def CNC():

    #This function prints the Gcode lines (from a file) in the arduino serial monitor
    s = serial.Serial('/dev/ttyUSB0', 115200)
    f= open('grbl.gcode', 'r');

    s.write(bytes("\r\n\r\n", 'utf-8'))
    time.sleep(2)
    s.flushInput()

    for line in f:
        l=line.strip()
        print('Sending: ' + l)
        s.write(bytes(l + '\n', 'utf-8'))
        grbl_out = s.readline()
        print (' : ' , grbl_out.strip())
        
    f.close()
    s.close()
    
    
if __name__ == "__main__":

    while True:
        
        #Send signal to robotic arm 1 to pickup from blue storage 
        if StorageBalancer == 0 :
          GPIO.output(Signal_R1_StorageB, GPIO.HIGH)
          time.sleep(1)
          GPIO.output(Signal_R1_StorageB, GPIO.LOW)
          StorageBalancer = StorageBalancer + 1

        #Send signal to robotic arm 1 to pick up from orange storage  
        elif StorageBalancer == 1 :
          GPIO.output(Signal_R1_StorageO, GPIO.HIGH)
          time.sleep(1)
          GPIO.output(Signal_R1_StorageO, GPIO.LOW)
          StorageBalancer = StorageBalancer - 1
          
        print("Robotic arm 1 is moving the product!")
        time.sleep(10);
        print("AVG is moving the product!")
        time.sleep(10);

        #send signal to R2 to start

        GPIO.output(Signal_R2, GPIO.HIGH)
        time.sleep(1)
        GPIO.output(Signal_R2, GPIO.LOW)
        
        print("Robotic arm 2 is moving the product!")
        time.sleep(11)
        
        #send signal to conveyor to start

        GPIO.output(Signal_CON, GPIO.HIGH)
        time.sleep(1)
        GPIO.output(Signal_CON, GPIO.LOW)
        
        print("Conveyor is moving the product!")
     
        #Call the CNC function to start the CNC drawing 
        CNC()
        
        print("Drawing is done!")
        time.sleep(4)

        # sending signal to R3 to pickup from conveyor and move to blue storage  
        if ContainerBalancer == 0 :
          GPIO.output(Signal_R3_ContainerB, GPIO.HIGH)
          time.sleep(1)
          GPIO.output(Signal_R3_ContainerB, GPIO.LOW)
          ContainerBalancer = ContainerBalancer + 1

        # sending signal to R3 to pickup from conveyor and move to orange storage  
        elif ContainerBalancer == 1 :
          GPIO.output(Signal_R3_ContainerO, GPIO.HIGH)
          time.sleep(1)
          GPIO.output(Signal_R3_ContainerO, GPIO.LOW)
          ContainerBalancer = ContainerBalancer - 1
          
        print("Robotic arm 3 is moving the product!")
        time.sleep(13);
        
        
        print("All Set, take the product form inventory!")
        
        Continue = input("Do you want to continue? Yes/No :  ")

        if Continue == "No":
            
            print("Oh no, sorry to see you go!")
            break
        
        elif Continue == "Yes":
            
            print("Great, let's go!")
            continue


