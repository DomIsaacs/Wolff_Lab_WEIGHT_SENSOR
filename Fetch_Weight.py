from socket import *
import time
# Writing to an excel 
# sheet using Python
import xlwt
from xlwt import Workbook

import time
import threading

path = 'C:/Users/domis/projects/weight_sensor_test_folder/xlwt_example.xls'

# Workbook is created

wb = Workbook()

# set global variable flag
flag = 1
def normal():
    global flag
    while flag==1:
        time.sleep(2)
        if flag==False:
            print('The while loop is now closing')

def get_input():
    global flag
    keystrk=input('Press a key \n')
    # thread doesn't continue until key is pressed
    print('You pressed: ', keystrk)
    flag=False
    print('flag is now:', flag)

n=threading.Thread(target=normal)
i=threading.Thread(target=get_input)
n.start()
i.start()
 
address_MEGA_1 =('10.90.125.50', 1900) #define server IP and port, this may be a little tricky, each mega is assigned a different IP (but they can all talk to the same port, at least one at a time)
client_socket=socket(AF_INET, SOCK_DGRAM) #Set up the Socket
client_socket.settimeout(1) #Only wait 1 second for a response
 
while(flag):
    
    #we ask for temprature
    data_request = "weight" #Set data request to weight
    client_socket.sendto(str.encode(data_request), address_MEGA_1) #Send the data request
 
    try:
 
        rec_data, addr = client_socket.recvfrom(2048) #Read response from arduino
        weight = float(rec_data) #Convert string rec_data to float temp
        print("Weight:", weight, "g.")# Print the result
 
    except:
        pass
 
     # now we ask for temprature
    time.sleep(2) #delay before sending next command
    data_request = "temperature" #Set data request to temperature
    client_socket.sendto(str.encode(data_request), address_MEGA_1) #Send the data request
 
    try:
 
        rec_data, addr = client_socket.recvfrom(2048) #Read response from arduino
        temperature = float(rec_data) #Convert string rec_data to float temp
        print("Temperature ", temperature , " C.") # Print the result
 
    except:
        pass
 
    time.sleep(2) #delay before sending next command
 
    print("")
    

# add_sheet is used to create sheet.
sheet1 = wb.add_sheet('Sheet 1')
sheet1.write(1, 0, "weight")
sheet1.write(2, 0, weight)
sheet1.write(1, 1, "temp")
sheet1.write(2, 1, temperature)
wb.save(path)
